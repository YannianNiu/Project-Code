#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include "Arrays.h"
#include "BOBHash32.h"

using namespace std;

template<int key_len, int depth, int memory_in_bytes>
class CMSketch
{


private:
    
    int width = memory_in_bytes / 4 / depth;
    BOBHash32* hash[depth] = {NULL};
    Arrays<depth> *arrays = NULL;

public:
    CMSketch(){

        
        width = memory_in_bytes / 4 / depth;
        arrays = new Arrays<depth>(width);
        for(int i = 0; i < depth; i++){
            hash[i] = new BOBHash32(i + 750);
        }
    }

    ~CMSketch(){clear();}

    
    /*
    void check(){
        for(int i = 0; i < depth; i++){
            for(int j = 0; j < width; j++){
                cout << this->arrays->counters[i][j]<<endl;
            }
        }
    }

    */

    void insert(uint8_t * key, int f = 1)
    {
        for (int i = 0; i < depth; i++) {
            int index = (hash[i]->run((const char *)key, key_len)) % width;
            this->arrays->counters[i][index] += f;
        }
    }
	int query(uint8_t * key)
    {
        int ret = 1 << 30;
        for (int i = 0; i < depth; i++) {
        	int index = (hash[i]->run((const char *)key, key_len)) % width;
            int tmp = this->arrays->counters[i][index];
            ret = min(ret, tmp);
        }
        return ret;
    }

    void clear()
	{
		//delete arrays;

		for (int i = 0; i < depth; ++i)
			delete hash[i];
	}

};
