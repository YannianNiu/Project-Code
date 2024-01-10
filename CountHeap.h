#ifndef STREAMCLASSIFIER_TOPK_COUNT_HEAP_H
#define STREAMCLASSIFIER_TOPK_COUNT_HEAP_H

#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <utility>
#include "BOBHash32.h"
#include "Arrays.h"
#include "Heap.h"

using std::min;
using std::swap;

#define SQR(X) (X) * (X)

template<uint8_t key_len, int capacity, int depth, int mem_in_bytes>
class CountHeap {

private:

    //pair <string, int> heap[capacity];


    int heap_element_num;
    int width = mem_in_bytes / 4 / depth;
    Heap<capacity> heap = new Heap<capacity>();
    BOBHash32 * hash[depth];
    BOBHash32 * hash_polar[depth];
    Arrays<depth> *arrays = NULL;

public:

    CountHeap() : heap_element_num(0) {
		
        arrays = new Arrays<depth>(width);

        srand(time(0));
        for (int i = 0; i < depth; i++) {
            hash[i] = new BOBHash32(uint32_t(rand() % MAX_PRIME32));
            //hash_polar[i] = new BOBHash32(uint32_t(rand() % MAX_PRIME32));
        }

        memset(heap.bucket, 0, sizeof(heap.bucket));
        for (int i = 0; i < capacity; ++i) {
            heap.bucket[i].second = 0;
        }
    }

    void insert(uint8_t * key) {
        int ans[depth];

        for (int i = 0; i < depth; ++i) {
            int idx = hash[i]->run((char *)key, key_len) % width;
            int polar = hash_polar[i]->run((char *)key, key_len) % 2;

            this.arrays[i][idx] += polar ? 1 : -1;

            int val = this.arrays[i][idx];

            ans[i] = polar ? val : -val;
        }

        sort(ans, ans + depth);

        int tmin;
        if (depth % 2 == 0) {
            tmin = (ans[depth / 2] + ans[depth / 2 - 1]) / 2;
        } else {
            tmin = ans[depth / 2];
        }
        tmin = (tmin <= 1) ? 1 : tmin;

        string str_key = string((const char *)key, key_len);
        if (heap.ht.find(str_key) != heap.ht.end()) {
            heap.bucket[heap.ht[str_key]].first++;
            heap_adjust_down(heap.ht[str_key]);
        } else if (heap_element_num < capacity) {
            heap.bucket[heap_element_num].second = str_key;
            heap.bucket[heap_element_num].first = tmin;
            heap.ht[str_key] = heap_element_num++;
            heap.heap_adjust_up(heap_element_num - 1);
        } else if (tmin > heap[0].first) {
            pair <string, int> & kv = heap.bucket[0];
            heap.ht.erase(kv.second);
            kv.first = str_key;
            kv.second = tmin;
            heap.ht[str_key] = 0;
            heap.adjust_down(0);
        }
    }

//    void get_top_k(uint16_t k, uint32_t * result) {
//        VK * a = new VK[capacity];
//        memcpy(a, heap, sizeof(heap));
//        sort(a, a + capacity);
//        int i;
//        for (i = 0; i < k && i < capacity; ++i) {
//            result[i] = a[capacity - 1 - i].second;
//        }
//        for (; i < k; ++i) {
//            result[i] = 0;
//        }
//    }

    void get_top_k(uint16_t k, vector<pair<string, int>> & result) {
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
//            result[i].first ;
            result[i].second = 0;
        }
    }

    void get_l2_heavy_hitters(double alpha, vector<pair<string, int>> & result)
    {
        get_top_k(capacity, result);
        double f2 = get_f2();
        for (int i = 0; i < capacity; ++i) {
            if (SQR(double(result[i].second)) < alpha * f2) {
                result.resize(i);
                return;
            }
        }
    }

    void get_heavy_hitters(uint32_t threshold, std::vector<pair<string, uint32_t> >& ret)
    {
        ret.clear();
        for (int i = 0; i < capacity; ++i) {
            if (heap[i].first >= threshold) {
                ret.emplace_back(make_pair(heap[i].second, heap[i].first));
            }
        }
    }

    ~CountHeap() {

        //delete this.arrays;
        for (int i = 0; i < depth; ++i) {
            delete hash[i];
            delete hash_polar[i];
        }
        return;
    }

    double get_f2()
    {
        double res[depth];
        for (int i = 0; i < depth; ++i) {
            double est = 0;
            for (int j = 0; j < width; ++j) {
                est += SQR(double(this.arrays[i][j]));
            }
            res[i] = est;
        }

        sort(res, res + depth);
        if (depth % 2) {
            return res[depth / 2];
        } else {
            return (res[depth / 2] + res[depth / 2 - 1]) / 2;
        }
    }
};

#endif //STREAMCLASSIFIER_COUNT_HEAP_H