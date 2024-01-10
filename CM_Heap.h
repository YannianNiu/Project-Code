#ifndef STREAMCLASSIFIER_CM_HEAP_H
#define STREAMCLASSIFIER_CM_HEAP_H

#include <unordered_map>
#include <algorithm>
#include <sstream>
#include "BOBHash32.h"
#include "Heap.h"
#include "Arrays.h"
//#include "../common/cuckoo_hashing.h"

using std::min;
using std::swap;

template<int key_len, int capacity, int depth, int mem_in_bytes>
class CMHeap {

private:

    int heap_element_num;
    int width = mem_in_bytes / 4 / depth;
    Arrays<depth> *arrays = NULL;
    //int *cm_sketch[depth];
    Heap<capacity> heap = new Heap<capacity>();
    BOBHash32 *hash[depth];



public:
    CMHeap() : heap_element_num(0) {
        width = mem_in_bytes / 4 / depth;
        arrays = new Arrays<depth>(width);
        random_device rd;
        for (int i = 0; i < depth; i++) {
            hash[i] = new BOBHash32(uint32_t(rd() % MAX_PRIME32));
        }

		memset(heap.bucket, 0, sizeof(heap.bucket));
        for (int i = 0; i < capacity; ++i) {
            heap.bucket[i].second = 0;
        }
        
    }
    ~CMHeap(){
        clear();
    }

    void insert(uint8_t *key) {
        int tmin = 1 << 30, ans = tmin;

        for (int i = 0; i < depth; ++i) {
            int idx = hash[i]->run((char *) &key, key_len) % width;
            arrays.counters[i][idx]++;
            int val = arrays.counters[i][idx];

            ans = std::min(val, ans);
        }

        string str_key = string((const char *) &key, key_len);
        if (heap.ht.find(str_key) != heap.ht.end()){
            heap.bucket[heap.ht[str_key]].second++;
            heap.adjust_down(heap.ht[str_key]);
        } else if (heap_element_num < capacity) {
            heap.bucket[heap_element_num].first = str_key;
            heap.bucket[heap_element_num].second = ans;
            heap.ht[str_key] = heap_element_num++;
            heap.adjust_up(heap_element_num - 1);
        } else if (tmin > heap.bucket[0].second) {
            pair <string, int> & kv = heap.bucket[0];
            heap.ht.erase(kv.first);
            kv.first = str_key;
            kv.second = ans;
            heap.ht[str_key] = 0;
            heap.adjust_down(0);
        }
    }

    int query(uint8_t * key) {
        int tmin = 1 << 30, ans = tmin;
        for(int i = 0; i < depth; ++i){
            int idx = hash[i]->run((char*)key, key_len) % width;
            int val = arrays->counters[i][idx];
            ans = std::min(val, ans);
        }
        return ans;
    }

    void get_heavy_hitters(uint32_t threshold, std::vector<pair<string, uint32_t> >& ret)
    {
        ret.clear();
        for (int i = 0; i < capacity; ++i) {
            if (heap.bucket[i].second >= threshold) {
                ret.emplace_back(make_pair(string((const char *)heap.bucket[i].first, key_len), heap.bucket[i].second));
            }
        }
    }

    void clear() {
        delete arrays;
        for (int i = 0; i < depth; ++i) {
            delete hash[i];
        }
    }

};

#endif //STREAMCLASSIFIER_COUNT_HEAP_H