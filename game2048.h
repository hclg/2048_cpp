#pragma once
#include <string>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <stack>
#include <queue>
#include <ctime>
#include <iostream>
#include <curses.h>
using namespace std;
#define N 4 // 格子数
#define SI 5 //每个格子的字符长度
 // 游戏状态
#define S_FAIL   0
#define S_WIN    1
#define S_run 2
#define S_QUIT   3
class game2048 {
	public:
		const int offset = 12;
		game2048(): status(S_run) {
			restart();
		}
        ~game2048() {}
		int getstatus() {
			return status;
		}
		void processINPUT() {
			char c = getch();
			if (c >= 'a' && c <= 'z')
				c -= 32;
			if (status == S_run) {
				if (c == 'W') up();
				else if (c == 'S') down();
				else if (c == 'A') left();
				else if (c == 'D') right();
				nowstatus();
			}
			if (c == 'Q') status = S_QUIT;
			else if (c == 'R') restart();
		}
		void man() {
			clear(); //清空
			for (int i = 0; i <= N; ++i) {
				for (int j = 0; j <= N; ++j) {
					draw(2*i, offset+j*SI, '+');
					if (i != N) {
						draw(2*i+1, offset+j*SI, '|');
						if (j != N)
							drawnum(2*i+1, offset+(j+1)*SI-1, data[i][j]);
					}
					if (j != N) {
						for (int k = 1; k <= N; ++k)
							draw(2*i, offset+j*SI+k, '-');
					}

				}
			}
			mvprintw(2*N+2, -1/2, "W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");
			mvprintw(2*N+3, SI, "HCLG");
			if (status == S_FAIL)
				mvprintw(3*N, SI, " YOU LOSE,PRESS R TO CONTINUE ");
			if (status == S_WIN)
				mvprintw(3*N, SI, " YOU WIN,PRESS R TO CONTINUE ");
		}
		void setTestData() {
			for (int i = 0; i < N; ++i)
				for (int j = 0; j < N; ++j)
					data[i][j] = 16>>i>>j;//调试用的测试数据
		}

	private:

	void nowstatus() {
		bool f = true;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (data[i][j] == 0 || (j != N-1 && data[i][j] == data[i][j+1]) || (i != N-1 && data[i][j] == data[i+1][j])) f = false;
				if (data[i][j] == 2048) {
					status = S_WIN;
					return;
				}
			}
		}
		if (f) status = S_FAIL;
		return;
	}
	void left() {
		queue<int>q;
		for (int i = 0; i < N; ++i) {
			for (auto &j : data[i]) {
				if (j) {
					q.push(j);
					j = 0;
				}
			}
			int j = 0;
			while (!q.empty()) {
				data[i][j] = q.front();
				q.pop();
				if (!q.empty() && q.front() == data[i][j]) {
					q.pop();
					data[i][j]*=2;
				}
				++j;
			}
		}
		randNew();randNew();
	}
	void right() {
		stack<int>q;
		for (int i = 0; i < N; ++i) {
			for (auto &j : data[i]) {
				if (j) {
					q.push(j);
					j = 0;
				}
			}
			int j = N-1;
			while (!q.empty()) {
				data[i][j] = q.top();
				q.pop();
				if (!q.empty() && q.top() == data[i][j]) {
					q.pop();
					data[i][j]*=2;
				}
				--j;
			}
		}
		randNew();randNew();
	}
	void up() {
		queue<int>q;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (data[j][i]) {
					q.push(data[j][i]);
					data[j][i] = 0;
				}
			}
			int j = 0;
			while (!q.empty()) {
				data[j][i] = q.front();
				q.pop();
				if (!q.empty() && q.front() == data[j][i]) {
					q.pop();
					data[j][i]*=2;
				}
				++j;
			}
		}
		randNew();randNew();
	}
	void down() {
		stack<int>q;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (data[j][i]) {
					q.push(data[j][i]);
					data[j][i] = 0;
				}
			}
			int j = N-1;
			while (!q.empty()) {
				data[j][i] = q.top();
				q.pop();
				if (!q.empty() && q.top() == data[j][i]) {
					q.pop();
					data[j][i]*=2;
				}
				--j;
			}
		}
		randNew();randNew();
	}
	void draw(int row, int col, char c) {
		move(row, col);
		addch(c);
	}
	void drawnum(int row, int col, int num) {
		while (num > 0) {
			draw(row, col, num%10+'0');
			num/=10;
			--col;
		}
	}
	void restart() {
		for (auto &i : data)
			for (auto &j : i)
				j = 0;
		randNew();
		randNew();
		status = S_run;
	}
	bool randNew() {
		vector<int> emp;
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				if (data[i][j] == 0)
					emp.push_back(i*N+j);
		if (emp.size() == 0)
			return false;
		int va = emp[rand()%emp.size()];
		//10%的概率产生4
		data[va/N][va%N] = rand()%10 == 1 ? 4 : 2;
		return true;
	}
	int data[N][N];
	int status;
};

