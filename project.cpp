#include <cstddef>
#include <iostream>
#include <utility>

#include "CM_Sketch.h"
#include "CU_Sketch.h"

#define KEY_LEN 4
#define DEPTH 4
#define MEM_IN_BYTES 1024

int main(){

    CMSketch<KEY_LEN, DEPTH, MEM_IN_BYTES> *cm_sketch = NULL;
    cm_sketch = new CMSketch<KEY_LEN, DEPTH, MEM_IN_BYTES>;

    CUSketch<KEY_LEN, DEPTH, MEM_IN_BYTES> *cu_sketch = NULL;
    cu_sketch = new CUSketch<KEY_LEN, DEPTH, MEM_IN_BYTES>;
}