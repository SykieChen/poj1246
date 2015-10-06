#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>

using namespace std;

struct idxh {
	int n, x1, x2, y;
	idxh(int n, int x1, int x2, int y) :
		n(n), x1(x1), x2(x2), y(y) {}
};
struct idxv {
	int n, y1, y2, x;
	idxv(int n, int y1, int y2, int x) :
		n(n), y1(y1), y2(y2), x(x) {}
};

//声明原型
int crossh(idxh idx, int p2n, int p2n1);
int crossv(idxv idx, int p2n, int p2n1);

//建立缓存，加速计算
map<idxh, int> libh;
map<idxv, int> libv;

//重载 < 使 map 库识别构造体
bool operator <(const idxh& left, const idxh& right) {
	if (left.n < right.n) return 1;
	if (left.n > right.n) return 0;
	if (left.x1 < right.x1) return 1;
	if (left.x1 > right.x1) return 0;
	if (left.x2 < right.x2) return 1;
	if (left.x2 > right.x2) return 0;
	return left.y < right.y;
}
bool operator <(const idxv& left, const idxv& right) {
	if (left.n < right.n) return 1;
	if (left.n > right.n) return 0;
	if (left.y1 < right.y1) return 1;
	if (left.y1 > right.y1) return 0;
	if (left.y2 < right.y2) return 1;
	if (left.y2 > right.y2) return 0;
	return left.x < right.x;
}


int crossh(idxh idx, int p2n,int p2n1) {
	//预处理越界
	if (idx.x1 < 0) idx.x1 = 0;
	if (idx.x2 > p2n) idx.x2 = p2n;
	if (idx.x2 <= idx.x1) return 0;
	//查找缓存
	if (libh.find(idx) != libh.end()) return libh[idx];
	//递归
	if (idx.n == 1) {
		if (idx.y == 1) {
			if (idx.x1 == 1) return 1;
			else {
				if (idx.x2 == 1) return 1;
				else return 2;
			}
		}
		else return 0;
	}
	else {
		if (idx.y == p2n1) {
			if (idx.x1 == 0) {
				if (idx.x2 == p2n) return 2;
				else return 1;
			}
			else {
				if (idx.x2 == p2n) return 1;
				else return 0;
			}
		}
		else if (idx.y <= p2n1) {
			int r = 0;
			r += crossh(idxh( idx.n - 1, idx.x1, idx.x2, idx.y ), p2n1, p2n1 >> 1);
			r += crossh(idxh( idx.n - 1, idx.x1 - p2n1, idx.x2 - p2n1 , idx.y ), p2n1, p2n1 >> 1);
			return libh[idx] = r;
		}
		else {
			int r = 0;
			r += crossv(idxv( idx.n - 1, p2n1 - idx.x2, p2n1 - idx.x1, idx.y - p2n1 ), p2n1, p2n1 >> 1);
			r += crossv(idxv( idx.n - 1, idx.x1 - p2n1, idx.x2 - p2n1, p2n - idx.y ), p2n1, p2n1 >> 1);
			return libh[idx] = r;
		}
	}
}

int crossv(idxv idx, int p2n, int p2n1) {
	//预处理越界
	if (idx.y1 < 0) idx.y1 = 0;
	if (idx.y2 > p2n) idx.y2 = p2n;
	if (idx.y2 <= idx.y1) return 0;
	//查找缓存
	if (libv.find(idx) != libv.end()) return libv[idx];
	//递归
	if (idx.n == 1) {
		if (idx.x == 1 && idx.y1 == 0) return 1;
		else return 0;
	}
	else {
		if (idx.x == p2n1) {
			if (idx.y1 <= p2n1 - 1 && idx.y2 >= p2n1) return 1;
			else return 0;
		}
		else if (idx.x < p2n1) {
			int r = 0;
			r += crossh(idxh( idx.n - 1, idx.y1 - p2n1, idx.y2 - p2n1, p2n1 - idx.x ), p2n1, p2n1 >> 1);
			r += crossv(idxv( idx.n - 1, idx.y1, idx.y2, idx.x ), p2n1, p2n1 >> 1);
			return libv[idx] = r;
		}
		else {
			int r = 0;
			r += crossh(idxh( idx.n - 1, p2n - idx.y2, p2n - idx.y1, idx.x - p2n1 ), p2n1, p2n1 >> 1);
			r += crossv(idxv( idx.n - 1, idx.y1, idx.y2, idx.x - p2n1 ), p2n1, p2n1 >> 1);
			return libv[idx] = r;
		}
	}
}

//封装为最外层函数
inline int calc(int n, int x1, int x2, int y) {
	return crossh(idxh( n, x1, x2, y ), 1 << n, 1 << (n - 1));
}


int main() {
	int n[101], x1[100], x2[100], y[100], i = 0;
	for (int i = 0; i < 101;i++){
		cin >> n[i];
		if (n[i] == 0) break;
		cin >> x1[i] >> x2[i] >> y[i];
	}
	for (int i = 0; i < 101; i++) {
		if (n[i] == 0) break;
		cout << calc(n[i], x1[i], x2[i], y[i]) << '\n';
	}
	return 0;
}