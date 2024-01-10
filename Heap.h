
#include <cstdint>
#include <string>
#include <utility>
#include <unordered_map>
#include <algorithm>

using namespace std;

template <int capacity>
class Heap{

public:
    pair <string, int> bucket[capacity];
    int heap_element_num;
    unordered_map<string, int> ht;  //first：key  second：value
    Heap(){}


    void adjust_down(int i) {
        while (i < heap_element_num / 2) {
            int l_child = 2 * i + 1;
            int r_child = 2 * i + 2;
            int larger_one = i;
            if (l_child < heap_element_num && bucket[l_child].second < bucket[larger_one].second) {
                larger_one = l_child;
            }
            if (r_child < heap_element_num && bucket[r_child].second < bucket[larger_one].second) {
                larger_one = r_child;
            }
            if (larger_one != i) {                     //find the largest
                swap(bucket[i], bucket[larger_one]);
                swap(ht[bucket[i].second], ht[bucket[larger_one].second]);
                adjust_down(larger_one);
            } else {
                break;
            }
        }
    }

    void adjust_up(int i) {
        while (i > 1) {
            int parent = (i - 1) / 2;
            if (bucket[parent].second <= bucket[i].second) {
                break;
            }
            swap(bucket[i], bucket[parent]);
            swap(ht[bucket[i].second], ht[bucket[parent].second]);
            i = parent;
        }
    }

    

    void get_top_k(uint16_t k, vector<pair<string, uint32_t>> & result) {
        pair <string, int> * a = new pair <string, int>[capacity];
        memcpy(a, bucket, sizeof(bucket));
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
};