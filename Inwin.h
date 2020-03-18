#pragma once
#include <curses.h>
#include <game2048.h>
#define SIZE 20

class Inwin
{
public:
    Inwin(int L, int T):left(L), top(T) {
        down = T+1;
        right = L+SIZE;
    }
    Inwin(int T, int D, int L, int R):left(L), right(R), top(T), down(D) {}
    ~Inwin() {}
    void win(char T, char D) {
        
        for (int i = left+1; )
    }
private:
    int top, down, left, right;


};

