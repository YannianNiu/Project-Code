

#include "../BitSet.h"
#include "../BOBHash32.h"
#include "../Arrays.h"
#include <cstddef>
#include <cstdint>

#define BITSIZE (1.0/8.0)

template<int key_len, int depth, int mem_in_bytes, int SLOT_NUM>
class Find_persistence_item {




public:

    struct Bucket {
        uint8_t* items[SLOT_NUM];
        int counters[SLOT_NUM];

        inline int Query(uint8_t* item){
            for(uint32_t i = 0;i < SLOT_NUM;++i){
                if(items[i] == item)
                    return counters[i];
            }
            return 0;
        }
    };


    Find_persistence_item(uint64_t memory) :
            length((double)memory / (sizeof(Bucket) + sizeof(int) + (SLOT_NUM + 1) * BITSIZE)){
        buckets = new Bucket[length];
        arrays = new Arrays<1>(length);

        memset(buckets, 0, length * sizeof(Bucket));
        hash = new BOBHash32(750);
        bucketBitsets = new BitSet(SLOT_NUM * length);
        bitsets = new BitSet(length);
    }

    ~Find_persistence_item(){
        delete [] buckets;
        delete arrays;
        delete bucketBitsets;
        delete bitsets;
    }

    void Insert(uint8_t *item, const int window){
        int index = (hash->run((const char*)item, key_len)) % length;
        uint32_t bucketBitPos = index * SLOT_NUM;

        for(uint32_t i = 0;i < SLOT_NUM;++i){
            if(buckets[index].items[i] == item){
                buckets[index].counters[i] += (!bucketBitsets->SetNGet(bucketBitPos + i));
                return;
            }
        }

        if(!bitsets->Get(index)){
            for(uint32_t i = 0;i < SLOT_NUM;++i){
                if(buckets[index].counters[i] == arrays.counters[0][index]){
                    buckets[index].items[i] = item;
                    buckets[index].counters[i] += 1;
                    bucketBitsets->Set(bucketBitPos + i);
                    return;
                }
            }

            arrays.counters[0][index] += 1;
            bitsets->Set(index);
        }
    }

    int Query(uint8_t* item){
        return buckets[this->hash(item) % length].Query(item);
    }

    void NewWindow(const int window){
        bucketBitsets->Clear();
        bitsets->Clear();
    }
private:

    //int width = mem_in_bytes / 4 / depth;
    const uint32_t length;
    Arrays<depth> arrays = NULL;
    //BOBHash32* hash[depth] = {NULL};
    Bucket* buckets;
    BitSet* bitsets = NULL;
    BitSet* bucketBitsets = NULL;
    BOBHash32* hash = NULL;

};
