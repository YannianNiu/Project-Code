#ifndef OO_PE_H
#define OO_PE_H

/*
 * On-Off sketch on persistence estimation
 */


#include "../Arrays.h"
#include "../BOBHash32.h"
#include "../BitSet.h"


template<int key_len, int depth, int mem_in_bytes>
class Persistence_estimation{

private:

    int width = mem_in_bytes / 4 / depth;
    Arrays<depth> arrays = NULL;
    BitSet** bitsets = {NULL};
    BOBHash32* hash[depth] = {NULL};
    

public:

    Persistence_estimation(){
        arrays = new Arrays<depth>(width);
        
        for(uint32_t i = 0; i < depth; ++i){
            hash[i] = new BOBHash32(i + 750);
            bitsets[i] = new BitSet(width);
        }
    }

    ~Persistence_estimation(){
        for(uint32_t i = 0; i < depth; ++i){
            delete hash[i];
        }
        delete [] arrays;
        delete [] bitsets;
    }

    void Insert(uint8_t *key, int window){
        for(uint32_t i = 0; i < depth; ++i){
            int index = (hash[i]->run((const char*)key, key_len)) % width;
            arrays.counters[i][index] += (!bitsets[i]->SetNGet(index));
        }
    }

    int Query(uint8_t *key){
        int ret = 1 << 30;
        for(int i = 0; i < depth; ++i){
            int index = (hash[i]->run((const char*)key, key_len)) % width;
            ret = MIN(ret, arrays.counters[i][index]);
        }
        return ret;
    }

    void NewWindow(int window){
        for(int i = 0; i < depth; ++i){
            bitsets[i]->Clear();
        }
    }

/*
private:
    const uint32_t hash_num;
    const uint32_t length;

    BitSet** bitsets;
    COUNT_TYPE** counters;
*/

};

#endif //OO_PE_H