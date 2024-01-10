#include <cstdio>
#include <cstring>
#include <random>
#include <vector>
#include <unordered_set>
using namespace std;

template <int depth>
class Arrays
{
public:
    int* counters[depth] = {NULL};

    Arrays();

	Arrays(int width){
        for(int i = 0; i < depth; i++){
            this->counters[i] = new int[width];
            memset(this->counters[i], 0, sizeof(this->counters[0]));
        }
    };
	~Arrays(){
        for(int i = 0; i < depth; i++){
            delete[] this->counters[i];
        }
    }; 
};

