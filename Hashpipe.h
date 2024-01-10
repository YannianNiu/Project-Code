#ifndef _hashpipe_H
#define _hashpipe_H

// #include <bits/stdc++.h>
#include <map>
#include "BOBHash32.h"
#include "Arrays.h"
using namespace std;

template<int key_len, int depth, int memory_in_bytes>
class HashPipe
{
    private:
        string key[depth][3000005];
        int width;
        BOBHash32 * BOB[depth+5];
        Arrays<depth> *arrays = NULL;
    
    public:
        HashPipe () {
            width = memory_in_bytes / depth / 4;
            arrays = new Arrays<depth>(width);
            for (int i = 0; i < depth; i++){
                BOB[i]=new BOBHash32(1000 + i);
            }
        }

        void work(string x, int y)
        {
            for (int i=1; i<depth; i++)
            {
                int L = BOB[i]->run(x.c_str(), x.size()) % width;
                if (key[i][L] == x) {
                    arrays[i][L] += y;
                    return;
                }
                if (!arrays[i][L]) {
                    key[i][L] = x; 
                    arrays[i][L] = y;
                    return;
                }
                if (arrays[i][L] < y) {
                    swap(x, key[i][L]);
                    swap(y, arrays[i][L]);
                }
            }
        }

        void insert(uint8_t * _x)
        {
            string x = string((const char *)_x, key_len);
            int L = BOB[0]->run(x.c_str(), x.size()) % width;
            if (key[0][L] == x)
                arrays[0][L] ++;
            else if (!arrays[0][L]) {
                key[0][L] = x;
                arrays[0][L] = 1;
            }
            else {
                work(key[0][L], arrays[0][L]);
                key[0][L] = x;
                arrays[0][L] = 1;
            }
        }
        
        void get_heavy_hitters(int threshold, vector <pair<uint32_t, int>> &v)
        {
            map<string, int> mp;
            mp.clear();
            for (int i = 0; i < depth; i++) {
                for (int j = 0; j < width; j++){
                    mp[key[i][j]] += arrays[i][j];
                }
            }
            for (map<string, int>:: iterator sit = mp.begin(); sit!=mp.end(); sit++){
                if (sit->second >= threshold){
                    v.push_back(make_pair(*(uint32_t *)(sit->first.c_str)(),sit->second));
                }
            }
        }
};
#endif
