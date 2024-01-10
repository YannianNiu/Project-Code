#ifndef CUOURS_H
#define CUOURS_H


#include <cstdint>
#include <unordered_map>

#include <string.h>
#include "Arrays.h"
#include "BOBHash32.h"

template<int key_len, int depth, int mem_in_bytes>
class CocoSketch{


private:
    int width = mem_in_bytes / 4 / depth;
    BOBHash32* hash[depth] = {NULL};
    Arrays<depth> *arrays = NULL;
    unordered_map<uint8_t*, int> map;

public:

    CocoSketch(){

        arrays = new Arrays<depth>(width);
        for(int i = 0; i < depth; i++){
            hash[i] = new BOBHash32(i + 750);
        }
    }

    ~CocoSketch(){
        for(uint32_t i = 0;i < depth;++i){
            delete hash[i];
        }
        delete arrays;
    }

    void Insert(uint8_t *item){
        int minimum = 0x7fffffff;
        uint32_t minPos, minHash;

        for(uint32_t i = 0;i < depth;++i){
            int position = (hash[i]->run((const char*)item, key_len)) % width;
            if(arrays.counter[i][position].ID == item){
                arrays.counter[i][position].count += 1;
                return;
            }
            if(arrays.counter[i][position].count < minimum){
                minPos = position;
                minHash = i;
                minimum = arrays.counter[i][position].count;
            }
        }

        arrays.counter[minHash][minPos].count += 1;
        if(randomGenerator() % arrays.counter[minHash][minPos].count == 0){
            arrays.counter[minHash][minPos].ID = item;
        }
    }

    void AllQuery(){
        for(uint32_t i = 0;i < depth;++i){
            for(uint32_t j = 0;j < width;++j){
                map[arrays.counter[i][j].ID] = arrays.counter[i][j].count;
            }
        }

    }

};

#endif
