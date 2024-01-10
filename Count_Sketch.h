#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include "Arrays.h"
#include "BOBHash32.h"

using namespace std;

template<int key_len, int depth, int memory_in_bytes>
class CountSketch
{


private:
    
    int width = memory_in_bytes / 4 / depth;
    Arrays<depth> *arrays = NULL;
    //int* counters[depth] = {NULL};
    BOBHash32* hash[depth] = {NULL};
    BOBHash32* sign_hash[depth] = {NULL};


public:
    CountSketch(){
        arrays = new Arrays<depth>(width);
        for(int i = 0; i < depth; i++){
            hash[i] = new BOBHash32(i + 750);
            sign_hash[i] = new BOBHash32(rand() % 750);
        }
    }

    ~CountSketch(){clear();}
    
    void insert(uint8_t *key, int f = 1)
    {
    	for (int i = 0; i < depth; ++i)
    	{
    		int index = (hash[i]->run((const char*)key, key_len)) % width;
    		int sign = (sign_hash[i]->run((const char*)key, key_len)) % 2;

    		arrays->counters[i][index] += sign ? f : -f;
    	}
    }

    int query(uint8_t *key)
    {
    	int result[depth] = {0};
    	for (int i = 0; i < depth; ++i)
    	{
			int index = (hash[i]->run((const char*)key, key_len)) % width;
    		int sign = (sign_hash[i]->run((const char*)key, key_len)) % 2;

    		result[i] = sign ? arrays.counters[i][index] : -arrays.counters[i][index];
    	}

    	sort(result, result + depth);

    	int mid = depth / 2;
    	int ret;
    	if(depth % 2 == 0)
    		ret = (result[mid] + result[mid - 1]) / 2;
    	else
    		ret = result[mid];

    	return ret;
    }

    void clear()
	{
		for (int i = 0; i < depth; ++i)
		{
			delete arrays;
			delete hash[i];
			delete sign_hash[i];
		}
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
