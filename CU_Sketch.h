#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include "Arrays.h"
#include "BOBHash32.h"

using namespace std;

template<int key_len, int depth, int memory_in_bytes>
class CUSketch
{


private:
    string name;
    
    int width = memory_in_bytes / 4 / depth;
    Arrays<depth> *arrays = NULL;
    BOBHash32* hash[depth] = {NULL};


public:
    CUSketch(){
        width = memory_in_bytes / sizeof(int) / depth;
        arrays = new Arrays<depth>(width);
        for(int i = 0; i < depth; i++){
            hash[i] = new BOBHash32(i + 750);
        }
    }

    ~CUSketch(){clear();}


    void insert(uint8_t *key, int f = 1)
    {
        int index[depth] = {0};
        int value[depth] = {0};
    	int min_val = 1 << 30;

    	for (int i = 0; i < depth; ++i)
    	{
    		index[i] = (hash[i]->run((const char*)key, key_len)) % width;
    		value[i] = this->arrays.counters[i][index[i]];
    		min_val = min(min_val, value[i]);
    	}

    	int temp = min_val + f;
    	for (int i = 0; i < depth; ++i)
    		this->arrays.counters[i][index[i]] = max(this->arrays.counters[i][index[i]], temp);
    }
    int query(uint8_t *key)
    {
    	int ret = 1 << 30;
    	for (int i = 0; i < depth; ++i)
    	{
    		int idx = (hash[i]->run((const char*)key, key_len)) % width;
    		ret = min(ret, this->arrays.counters[i][idx]);
    	}
    	return ret;
    }

    void clear()
	{
        delete arrays;

		for(int i = 0; i < depth; ++i)
			delete hash[i];
	}


    /*
    void check(){
        for(int i = 0; i < depth; i++){
            for(int j = 0; j < width; j++){
                cout << this->arrays->counters[i][j]<<endl;
            }
        }
    }
    */
    


};
