#include<iostream>
#include<string>
#include<vector>
#include <algorithm>
using namespace std;

struct Process {
	int  s;//所需的内存
	int id;//进程名
	bool  isIn = false;//是否装载进内存
	int  addr;
}*p;

struct Store {//内存说明表
	//int id;//快号
	int len;//快长
	int addr;//起始地址
	bool isAva;//是否分配
};

void Print(vector<Store> &s);
void Distribute(int &id, Process *p, vector<Store> &storeV);
void Recycle(int &id, Process *p, vector<Store> &storeV);
bool comp(const Store &a, const Store &b);
void Best(int &id, Process *p, vector<Store> &storeV, vector <Store> &spare);
bool compDec(const Store &a, const Store &b);
void Worst(int &id, Process *p, vector <Store> &storeV, vector<Store> &spare);
/*
能够输入给定的内存大小，进程的个数，每个进程所需内存空间的大小等;
选择分配 / 回收操作;
能够显示进程在内存的存储地址、大小等;
显示每次完成内存分配或回收后内存空间的使用情况;
*/
int main() {
	vector<Store> storeV;//分区说明表

	float totalSize;
	int  num;
	cout << "请输入内存大小(MB)";
	//cin >> totalSize;
	totalSize = 1024;
	Store s;
	s.addr = 0;
	s.isAva = true;
	s.len = totalSize;
	storeV.push_back(s);
	vector <Store> spare;
	for (int i = 0; i < storeV.size(); i++) {
		Store stemp;
		if (storeV[i].isAva) {
			stemp.len = storeV[i].len;
			stemp.addr = storeV[i].addr;
			stemp.isAva = true;
			spare.push_back(stemp);
		}
	}
	cout << "进程个数:";
	//cin >> num;
	num = 3;
	p = new Process[num];
	cout << "请输入进程所需内存(MB)" << endl;
	for (int i = 0; i < num; i++) {
		p[i].id = i;
		//cin >> p[i].s;
		p[i].s = 100 * i;
	}
	int  select, sel;
	int id;
	while (true) {
		cout << "输入（1:分配 进程编号；2:回收 进程编号；0:退出）";
		cin >> select;
		if (select == 0) {
			break;
		}
		cout << "请输入要操作的进程编号：" << endl;
		cin >> id;//操作的进程序号
	
		if (select == 1) {
			cout << "请选择使用的分配算法(1-最先适应算法，2-最佳适应算法，3-最坏适应算法：" << endl;
			//cin >> sel;
			sel = 3;
			switch (sel) {
			case 1:
				Distribute(id, p, storeV);			
				break;
			case 2:		
			
				Best(id, p, storeV,spare);
				break;
			case 3:
				Worst(id,p,storeV,spare);
				break;
			default:
				cout << "请输入1-3！";
			}
			Print(storeV);
		}
		else if (select == 2) {
			Recycle(id, p, storeV);
			Print(storeV);
		}
		else {
			cout << "请输入0,1,2" << endl;
		}
	}
	system("pause");
	return 0;
}

void Print(vector<Store> &s) {
	cout << "-----------------------------------------------" << endl;
	cout << "区号\t" << "长度\t" << "起始地址\t" << "状态\n";
	cout << "-----------------------------------------------" << endl;
	for (int i = 0; i < s.size(); i++) {
		cout << i + 1 << "\t" << s[i].len << "\t" << s[i].addr << "\t\t" << (s[i].isAva ? "未分配" : "已分配") << endl;
	}
}
/*
采用最先适应算法分配，交换技术进行替换
id 为待分配内存的进程编号
*/
void Distribute(int &id, Process *p, vector<Store> &storeV) {
	Store slice;
	if (p[id].isIn == true) {
		cout <<"该进程已分配内存！" << endl;
		return;
	}
	bool  isEnough = false;
	for (int i = 0; i < storeV.size(); i++) {
		if (storeV[i].isAva) {
			if (p[id].s < storeV[i].len) {
				//碎片
				slice.addr = storeV[i].addr + p[id].s;
				slice.isAva = true;
				slice.len = storeV[i].len - p[id].s;
				vector<Store>::iterator it = storeV.begin() + i + 1;
				storeV.insert(it, slice);
				//刚分配
				p[id].addr = storeV[i].addr;
				storeV[i].len = p[id].s;
				storeV[i].isAva = false;
				isEnough = true;
				break;
			}
			else if (p[id].s == storeV[i].len) {
				isEnough = true;
				storeV[i].isAva = false;
				p[id].addr = storeV[i].addr;
				break;
			}
		}
	}
	if (isEnough) {
		p[id].isIn = true;
	}
	else {
		//交换
		for (int i = 0; i < storeV.size(); i++) {
			float lenAva = storeV[i].len;
			if (lenAva > p[id].s) {
				slice.addr = storeV[i].addr + p[id].s;
				slice.isAva = true;
				slice.len = storeV[i].len - p[id].s;
				vector<Store>::iterator it = storeV.begin() + i + 1;
				storeV.insert(it, slice);
				storeV[i].len = p[id].s;
				p[id].isIn = true;
				break;
			}
			else if (storeV[i].len = p[id].s) {
				p[id].addr = storeV[i].addr;
				p[id].isIn = true;
				break;
			}
		}
	}
	if (p[id].isIn == false) {//交换后还是不够
		cout << "内存空间不足！" << endl;
	}
}
/*
通过进程的物理地址回收内存
*/
void Recycle(int &id, Process *p, vector<Store> &storeV) {
	int index = -1;
	for (int i = 0; i < storeV.size() - 1; i++) {//不需要考虑最后一块
		if (!storeV[i].isAva) {
			if (storeV[i].addr == p[id].addr) {//找到区号
				index = i;
			}
		}
	}
	if (index == -1) {
		cout << "该进程未调入内存！" << endl;
		return;
	}
	//回收
	if (index == 0) {
		if (storeV[index + 1].isAva) {
			storeV[index].isAva = true;
			storeV[index].len += storeV[index + 1].len;
			vector<Store>::iterator it = storeV.begin() + index + 1;
			storeV.erase(it);

		}
		else {
			storeV[index].isAva = true;
		}
	}
	else {
		if (storeV[index + 1].isAva) {//下一区空闲
			if (storeV[index - 1].isAva) {//合并，上一区也空闲
				storeV[index - 1].len += (storeV[index + 1].len + storeV[index].len);
				vector<Store>::iterator it = storeV.begin() + index;
				storeV.erase(it + 1);
				storeV.erase(it);
			}
			else {
				storeV[index].isAva = true;
				storeV[index].len += storeV[index + 1].len;
				vector<Store>::iterator it = storeV.begin() + index + 1;
				storeV.erase(it);
			}
		}
		else {//下一区不空闲
			if (storeV[index - 1].isAva)//上一区空闲
			{
				storeV[index - 1].len += storeV[index].len;
				vector<Store>::iterator it = storeV.begin() + index;
				storeV.erase(it);
			}
			else {//上下区都不空闲
				storeV[index].isAva = true;
			}
		}
	}
	p[id].isIn = false;
}
/*
采用最佳适应法分配内存
按空闲内存块的大小从小到大排列
*/
bool comp(const Store &a, const Store &b) {
	return a.len < b.len;
}
void Best(int &id, Process *p, vector<Store> &storeV,vector <Store> &spare) {
	if (p[id].isIn==true) {
		cout << "该进程已分配！" << endl;
		return;
	}
	Store slice;
	
	sort(spare.begin(), spare.end(), comp);//对storeV按len块大小升序排序
	//遍历空闲说明表，查找适合的块
	bool isAllDis=false;//恰好分配
	for (int i = 0; i < spare.size(); i++) {
		if (spare[i].len > p[id].s) {
			p[id].addr = spare[i].addr;
			spare[i].addr += p[id].s;
			spare[i].len -= p[id].s;
			sort(spare.begin(), spare.end(), comp);
			p[id].isIn = true;
			break;
		}
		else if (spare[i].len == p[id].s) {
			p[id].addr = spare[i].addr;
			vector<Store>::iterator it = storeV.begin() + i ;
			spare.erase(it);
			isAllDis = true;
			p[id].isIn = true;
			break;
		}
	}
	if (!p[id].isIn) {
		cout << "内存不足！" << endl;
		return;
	}
	for (int i = 0; i < storeV.size(); i++) {
		if (storeV.at(i).isAva) {
			if (storeV[i].addr == p[id].addr) {
				if (isAllDis) {
					//更新其他值
					storeV[i].isAva = false;
					break;
				}
				else {
					//碎片
					slice.addr = storeV[i].addr + p[id].s;
					slice.len = storeV[i].len - p[id].s;
					slice.isAva = true;
					vector<Store>::iterator it = storeV.begin() + i + 1;
					storeV.insert(it, slice);
					//内存足够，分配
					storeV[i].isAva = false;
					storeV[i].len = p[id].s;
					break;
				}
			}
		
		}
	}
}
bool compDec(const Store &a, const Store &b) {
	return a.len > b.len;
}
void Worst(int &id, Process *p, vector <Store> &storeV, vector<Store> &spare) {
	if (p[id].isIn == true) {
		cout << "该进程已分配！" << endl;
		return;
	}
	Store slice;

	sort(spare.begin(), spare.end(), compDec);//对storeV按len块大小升序排序
									
	bool isAllDis = false;//恰好分配
	for (int i = 0; i < spare.size(); i++) {
		if (spare[i].len > p[id].s) {
			p[id].addr = spare[i].addr;
			spare[i].addr += p[id].s;
			spare[i].len -= p[id].s;
			sort(spare.begin(), spare.end(), comp);
			p[id].isIn = true;
			break;
		}
		else if (spare[i].len == p[id].s) {
			p[id].addr = spare[i].addr;
			vector<Store>::iterator it = storeV.begin() + i;
			spare.erase(it);
			isAllDis = true;
			p[id].isIn = true;
			break;
		}
	}
	if (!p[id].isIn) {
		cout << "内存不足！" << endl;
		return;
	}
	for (int i = 0; i < storeV.size(); i++) {
		if (storeV.at(i).isAva) {
			if (storeV[i].addr == p[id].addr) {
				if (isAllDis) {
					//更新其他值
					storeV[i].isAva = false;
					break;
				}
				else {
					//碎片
					slice.addr = storeV[i].addr + p[id].s;
					slice.len = storeV[i].len - p[id].s;
					slice.isAva = true;
					vector<Store>::iterator it = storeV.begin() + i + 1;
					storeV.insert(it, slice);
					//内存足够，分配
					storeV[i].isAva = false;
					storeV[i].len = p[id].s;
					break;
				}
			}

		}
	}
}