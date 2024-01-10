#ifndef STREAMCLASSIFIER_CUHEAP_H
#define STREAMCLASSIFIER_CUHEAP_H

#include <unordered_map>
#include <algorithm>
#include <utility>
#include "BOBHash32.h"
#include "Heap.h"
#include "Arrays.h"

using std::min;
using std::swap;

template<int key_len, int capacity, int mem_in_bytes, int depth>


class CUHeap {

private:

    int heap_element_num;
    int width = mem_in_bytes / 4 / depth;
    Arrays<depth> *arrays = NULL;
    Heap<capacity> heap = new Heap<capacity>();
    BOBHash32 *hash[depth];


public:

    CUHeap() : heap_element_num(0) {
        width = mem_in_bytes / 4 / depth;
        arrays = new Arrays<depth>(width);
        for (int i = 0; i < depth; i++)
            hash[i] = new BOBHash32(i + 750);
    }

    void insert(uint8_t *key) {
        uint32_t tmin = 1 << 30;
        int idxs[depth];
        for (int i = 0; i < depth; ++i) {
            idxs[i] = hash[i]->run((char *) key, 4) % width;
            tmin = min(arrays[i][idxs[i]], tmin);
        }
        for (int i = 0; i < depth; ++i) {
            if (arrays[i][idxs[i]] == tmin)
                ++arrays[i][idxs[i]];
        }

        string str_key = string((const char *)key, key_len);
        if (heap.ht.find(str_key) != heap.ht.end()) {
            heap.bucket[heap.ht[str_key]].first++;
            heap.adjust_down(heap.ht[str_key]);
        } else if (heap_element_num < capacity) {
            heap.bucket[heap_element_num].second = str_key;
            heap.bucket[heap_element_num].first = tmin;
            heap.ht[key] = heap_element_num++;
            heap.adjust_up(heap_element_num - 1);
        } else if (tmin > heap.bucket[0].first) {
            pair<string, int> & kv = heap.bucket[0];
            heap.ht.erase(kv.second);
            kv.first = str_key;
            kv.second = tmin;
            heap.ht[str_key] = 0;
            heap.adjust_down(0);
        }
    }


    void get_top_k(uint16_t k, vector<pair<string, uint32_t>> & result) {
        pair<string, int> * a = new pair<string, int>[capacity];
        for (int i = 0; i < capacity; ++i) {
            a[i] = heap.bucket[i];
        }
        sort(a, a + capacity);
        int i;
        for (i = 0; i < k && i < capacity; ++i) {
            result[i].first = a[capacity - 1 - i].first;
            result[i].second = a[capacity - 1 - i].second;
        }
        for (; i < k; ++i) {
            result[i].first = (string)0;
            result[i].second = 0;
        }
    }

    void build(uint32_t * items, int n)
    {
        for (int i = 0; i < n; ++i) {
            insert(items[i]);
        }
    }

    ~CUHeap() {

        delete arrays;
        for (int i = 0; i < depth; ++i) {
            delete hash[i];
        }
        return;
    }
};

#endif //STREAMCLASSIFIER_CUHEAP_H