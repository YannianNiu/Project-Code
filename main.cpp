#include "sketch.h"
#include <ostream>
#include <random>

double g_sum(double (*g)(double))
{
        
    int coe = 12;
    int p = coe * g(double(3));
    return p;
}

double get_cardinality()
{
    return g_sum([](double x) { return 1.0; });
}


int main(){
    Sketch<3, 4> *sketch = NULL;
    sketch = new Sketch<3, 4>();
    //sketch->check();

    double sum = g_sum([](double x) { return 1.0; });
    //cout << sum <<endl;


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> distrib(0, 200);
    for (uint32_t i = 0; i < 10; i++)
    {
        std::cout << distrib(gen) << std::endl;
    }
}