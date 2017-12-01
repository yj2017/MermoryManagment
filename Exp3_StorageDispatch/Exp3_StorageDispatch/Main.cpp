#include<iostream>
#include <algorithm> 
using namespace std;

#define  INF 100000
/*
输入：磁头位置、移动的方向、磁道访问序列
输出：磁盘调度结果（顺序）、磁头移动的总磁数
*/
struct Road{
	int pos;
//	float arrt;
	//float runt;
	int id;
	bool isOut = { false };
};
int Increase(Road *rLow, int recI[], int &countL, int prePos);
int Decrease(Road *rHigh, int recD[], int &countH, int prePos);
bool Compare(int a, int b);
void   FCFS(Road* r, int cnt, int prePos);
void  Print(Road *r, int total ,int rlen, int prePos);
void SD(Road* r, int cnt, int prePos);
void Lift(Road* r, int cnt, int prePos);
void Select(Road* r, int cnt, int prePos, int* arr,int &rlen);
void Move(Road* r, Road* rLow, Road* rHigh, int pre, int cnt,int &countL, int  &countH);
int main() {
	int totalN;
	cout << "please enter the number:";
	cin >> totalN;
	cout << "磁头当前位置：";
	int prePos;
	cin >> prePos;
	Road* r = new Road[totalN];
	r[0].pos = 98;
	r[1].pos = 183;
	r[2].pos = 37;
	r[3].pos = 122;
	r[4].pos = 14;
	r[5].pos = 124;
	r[6].pos = 65;
	r[7].pos = 67;
	/*98 183 37 122 14 124 65 67
	*/
	cout << "please enter the array" << endl;
	for (int i = 0; i < totalN; i++) {
		cin >> r[i].pos;
	}
	
	while (true) {
		cout << "please enter the select （FCFS：1，Scan：2，SDF:3)";
		int select;
		cin >> select;
		Road* r_bkp = r;
		if (select==0) {
			break;
		}
		switch (select)
		{
		case 1:
			FCFS(r_bkp, totalN, prePos);
			break;
		case 2:
			Lift(r, totalN, prePos);
			break;
		case 3:
			SD(r_bkp, totalN, prePos);
			break;
		default:
			cout << "please enter 0~3";
			break;
		}
		
	}
	system("pause");
	return 0;
}
void   FCFS(Road* r, int cnt,int prePos) {
	int len = abs(r[0].pos-prePos);
	Road* r_modi = r;
	for (int i = 0; i < cnt-1; i++)
	{
		len += abs(r[i + 1].pos- r[i].pos);
	}
	Print(r_modi, cnt,len,prePos);
}
void  Print(Road *r,int total,int rlen, int prePos) {
	cout << "顺序:" << prePos<<"  ";
	for (int i = 0; i < total; i++) {
		cout << r[i].pos << "  ";
	}
	cout << endl;
	cout << "总的磁道长度为：" << rlen<<endl;
}
void Select(Road* r, int cnt, int prePos,int* arr,int &rlen) {
	arr[0] = prePos;
	int nextId = -1;
	for (int i = 0; i <cnt; i++) {
		int min = INF;//无穷大	
		for (int c = 0; c < cnt; c++) {
			if (!r[c].isOut) {
				int temp = abs(r[c].pos - arr[i]);
				if (min > temp) {
					min = temp;
					arr[i + 1] = r[c].pos;
					nextId = c;
				}
			}
		}
		r[nextId].isOut = true;
		rlen += min;
	}
}
void SD(Road* r, int cnt, int prePos) {
	Road* rb = r;
	int* arr = new int[cnt+1];
	int rlen = 0;
	Select(r, cnt, prePos, arr,rlen);
/*
输出
*/
	cout << "顺序为：" << "  ";
	for (int c = 0; c < cnt+1; c++) {
		cout << arr[c] << "  ";
	}
	cout << endl;
	cout << "总的磁道长度为：" << rlen << endl;
}
void Lift(Road* r, int cnt, int prePos) {
	cout << "Please enter the direction(0:to lower firstly,1:to higher firstly)" << endl;
	int d;
	cin >> d;
	Road* rLow = new Road[cnt];
	Road* rHigh = new Road[cnt];
	int cL , cH ;
	Move(r, rLow, rHigh, prePos, cnt,cL,cH);
	int recI[100];
	int recD[100];
	int len1, len2;
	cout << "顺序为：" << "  ";
	switch (d)
	{
	case 0:
		len1 = Decrease(rLow, recD, cL, prePos);
		len2 = Increase(rHigh, recI, cH, recD[cL - 1]);
		break;
	case 1:
		len2 = Increase(rHigh, recI, cH, prePos);
		len1 = Decrease(rLow, recD, cL, recI[cH-1]);
		break;
	default:
		cout << "please enter (0~1)";
		break;
	}
	cout << "总的磁道长度为：" << len1+len2 << endl;
}
/*
先从小到大移
*/
void Move(Road* r,Road* rLow, Road* rHigh,int pre,int cnt, int &countL ,int  &countH ) {
	countL = 0;
	countH = 0;
	for (int i = 0; i < cnt; i++) {
		if (r[i].pos <=pre) {
			rLow[countL].pos = r[i].pos;
			rLow[countL].id = i;
			countL++;
		}
		else {
			rHigh[countH].pos = r[i].pos;
			rHigh[countH].id = i;
			countH++;
		}
	}
}
/*
从小到大
return： 小方向磁道总数
*/
int Increase(Road *rLow, int recI[], int &countL,int prePos) {
	int len = 0;
	for (int i = 0; i < countL; i++) {
		recI[i] = rLow[i].pos;
	}
	sort(recI, recI+countL);
	len = abs(recI[0] - prePos);
	for (int i =0; i<countL-1; i++) {
		cout << recI[i] << "  ";
		len += abs(recI[i +1] - recI[i]);
	}
	cout << recI[countL - 1]<<" ";
	return len;
}
/*
从大到小
return： 高方向磁道总数
*/
int Decrease(Road *rHigh, int recD[], int &countH, int prePos) {
	int len=0;
	for (int i = 0; i < countH; i++) {
		recD[i] = rHigh[i].pos;
	}
	sort(recD, recD + countH, Compare);
	len += abs(prePos - recD[countH - 1]);
	for (int i = countH-1; i >=1 ; i--) {
		cout << recD[i] << "  ";
		len += abs(recD[i ]- recD[i-1]);
	}
	cout << recD[0]<<" ";
	return len;
}
bool Compare(int a, int b) {
	return a < b;
}

