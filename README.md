# 2048 简单小游戏
#### 游戏规则：
> 用W,S,A,D控制上下左右操作 Q退出，R重新开始 
> 当某个位置得到2048时获胜
> 不能再有新的操作时失败
> 移动： 
> 每次移动时，有两个相同的撞在一起会叠加，每个数字一次移动只能叠加一次，从边上开始往后叠加
> 如向左移动：空 2 2 2  左移动 -> 空 空 2 4,  2 2 2 2 左移动 -> 空 空 4 4 
>
> 每次操作后都会随机选择两个空位进行添加2或4，新出4的概率10% 
> 如果没有空位就不添加，只有一个添加一个
> 重新开始随机两个位置添加，其余清空

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200313105552533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Nsb25I,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200313105745634.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Nsb25I,size_16,color_FFFFFF,t_70)

依赖库
linux: 

```bash
sudo apt-get install libncurses5-dev 
```
编译：
```bash
Linux/Mac: g++ 2048.cpp -l ncurses -o 2048 
Windows: g++ 2048.cpp C:\pdcurs36\wincon\pdcurses.a -I C:\pdcurs36\ -o 2048 
```
为了尝试一下curses库，做了这个小游戏，代码有点丑有待改进
源代码：

```cpp
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
		void setTestData() {//调试用的测试数据
			for (int i = 0; i < N; ++i)
				for (int j = 0; j < N; ++j)
					data[i][j] = 16>>i>>j;
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


void initialize() {
	//ncurses初始化
    initscr();
    //按键不需要输入回车直接交互
    cbreak();
    //按键不显示
    noecho();
    //隐藏光标
    curs_set(0);
    //随机数
    srand(time(NULL));
}

void shutdown() {
    endwin();//ncurses清理
}

int main() {
    initialize();
    game2048 game;
    do {
		game.man();
        //move(5,5); //移动光标位置
	// addch(ch); //当前位置画字符ch
	// mvprintw(2, 2, "HCL");//显示在2,2的位置
       	game.processINPUT();
   }while (game.getstatus() != S_QUIT);
   shutdown();
   return 0;
}
```