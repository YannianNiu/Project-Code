
#include <cstdio>
#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>
#include "Arrays.h"
using namespace std;


template<int depth, int width>
class Sketch{

public:
    Arrays<depth, width> *arrays = NULL;
    Sketch(){
        arrays = new Arrays<depth, width>;
    }

    void check(){
        for(int i = 0; i < depth; i++){
            for(int j = 0; j < width; j++){
                cout << this->arrays->counters[i][j]<<endl;
            }
        }
    }

    
};

