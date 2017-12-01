#include <iostream>
#include<string>
#include <deque>
#include<queue>
using namespace std;

struct Process {
	string name;
	double arriveTime;//请求时间
	double  runTime;//预计运行时间
	int ind;//顺序
	bool arrived = false;//是否到达
	bool isGet = false;//是否访问标记
};
struct SArray {
	double value;//到达时间
	int   index;
};
//子程序
void  FCFS(Process *p1, int len, double  *achieveT);
void  SJF(Process *p1, int len, double *t);
void HRF(Process *p3, int len, double *t);
void RR(Process *p, int len);
//void RR(Process *p4, queue<Process> q, int len, int *arrT);
void SP(Process * p5, int len, double * t);
//辅助函数
void Sort(SArray *s, int len);
int  FindFirst(Process *p, int len);
double  ComputeResponse(Process p, double preT);
void  Print(Process *p1, double *t, SArray * s, int len);

/*
输入:进程名、请求时间、预计运行时间,可选择多个调度算法依次执行
输出：进程执行顺序、完成时间、周转时间、带权周转时间，平均周转和平均带权周转
*/
int main() {
	//依次输入进程总数，各进程进程名、到达时间(秒)、预计运行时间
	int total = 0;
	cout << "Enter the count of process:";
	cin >> total;
	Process *p = new Process[total];
	double *t = new double[total];
	for (int i = 0; i < total; i++) {
		p[i].ind = i;//初始化下标
	//  cin >> p[i].name >> p[i].arriveTime >> p[i].runTime;
	}
	p[0].name = "1"; p[0].arriveTime = 10; p[0].runTime = 2;
	p[1].name = "2"; p[1].arriveTime = 10.2; p[1].runTime = 1;
	p[2].name = "3"; p[2].arriveTime = 10.4; p[2].runTime = 0.5;
	p[3].name = "4"; p[3].arriveTime = 10.5; p[3].runTime = 0.3;
	cout << "Enter the number (1:FCFS,2:SJF，3：HRF,4:时间片轮转法，5：优先级法(静态|动态)，0：Quit)" << endl;
	int select;

	while (true) {
		cin >> select;
		if (select == 0) {
			break;
		}
		Process *p_bkup = new Process[total];
		for (int i = 0; i < total; i++)
		{
			p_bkup[i] = p[i];
		}
		switch (select) {
		case 1:
			FCFS(p_bkup, total, t);
			break;
		case 2:
			SJF(p_bkup, total, t);
			break;
		case 3:
			HRF(p_bkup, total, t);
			break;
		case 4:
			RR(p_bkup, total);
			//RR(p, q, total, t);
			break;
		case 5:
			SP(p_bkup, total, t);
			break;
		default:
			cout << "Please enter 0~5" << endl;
		}
	}
	system("pause");
	return 0;
}
//先来先服务算法
void  FCFS(Process *p1, int len, double *achieveT) {
	SArray *s = new SArray[len];
	for (int i = 0; i < len; i++) {
		s[i].value = p1[i].arriveTime;
		s[i].index = p1[i].ind;
	}
	Sort(s, len);
	achieveT[0] = p1[s[0].index].runTime + p1[s[0].index].arriveTime;//最早的完成时间
	for (int i = 1; i < len; i++) {
		achieveT[i] = p1[s[i].index].runTime + (p1[s[i].index].arriveTime > achieveT[i - 1] ? p1[s[i].index].arriveTime : achieveT[i - 1]);
	}

	Print(p1, achieveT, s, len);
}
//最短作业优先法,
void  SJF(Process *p1, int len, double *t) {
	int first = FindFirst(p1, len);
	SArray *s = new SArray[len];
	SArray *next = new SArray[len];
	p1[first].isGet = true;
	s[0].index = first;
	s[0].value = p1[first].runTime;
	t[0] = p1[first].arriveTime + p1[first].runTime;
	//确定顺序
	int cnt1 = 0;
	for (int c = 1; c < len; c++) {
		int count = 0;
		for (int i = 0; i < len; i++) {
			if (p1[i].arriveTime <= t[c - 1] && p1[i].isGet != true) {//寻找最短运行时间的作业
				next[count].index = i;
				next[count].value = p1[i].runTime;
				count++;
			}
		}
		if (count > 0) {
			Sort(next, count);
			t[c] = p1[next[0].index].runTime + t[c - 1];
			s[c].index = next[0].index;
			s[c].value = next[0].value;
			p1[next[0].index].isGet = true;
			cnt1++;

		}
		else {//没有请求时间小于上一个进程完成时间的进程,找到未完成中最先到达的进程
			int f, count2 = 0;
			for (int i = 0; i < len; i++) {
				if (p1[i].isGet != true) {
					next[count2].index = i;
					next[count2].value = p1[i].arriveTime;
					count2++;
				}
			}
			Sort(next, count2);
			t[c] = p1[next[0].index].runTime + t[c - 1];
			s[c].index = next[0].index;
			s[c].value = next[0].value;
			p1[next[0].index].isGet = true;
		}
	}
	Print(p1, t, s, len);
	delete[] next;
}
//最高响应比法
void HRF(Process *p3, int len, double *t) {
	SArray *s = new SArray[len];
	SArray *next = new SArray[len];
	int first = FindFirst(p3, len);
	t[0] = p3[first].arriveTime + p3[first].runTime;
	s[0].index = first;
	s[0].value = p3[first].arriveTime;
	p3[first].isGet = true;
	//确定顺序
	for (int i = 1; i < len; i++) {
		int count = 0;
		for (int j = 0; j < len; j++) {
			if (p3[j].isGet != true) {
				if (p3[j].arriveTime <= t[i - 1]) {
					next[count].index = j;
					next[count].value = ComputeResponse(p3[j], t[i - 1]);
					count++;
				}
			}
		}
		if (count > 0) {
			Sort(next, count);//从小到大，应取最大值
			t[i] = t[i - 1] + p3[next[count - 1].index].runTime;
			s[i].index = p3[next[count - 1].index].ind;
			s[i].value = p3[next[count - 1].index].arriveTime;
			p3[next[count - 1].index].isGet = true;
		}
		else {
			int count2 = 0;
			for (int j = 0; j < len; j++) {
				if (p3[j].isGet != true) {
					if (p3[j].arriveTime <= t[i]) {
						next[count2].index = j;
						next[count2].value = p3[j].arriveTime;
						count2++;
					}
				}
			}
			Sort(next, count2);//应取最小值
			t[i] = t[i - 1] + p3[next[0].index].runTime;
			s[i].index = p3[next[0].index].ind;
			s[i].value = p3[next[0].index].arriveTime;
			p3[next[0].index].isGet = true;
		}
	}
	Print(p3, t, s, len);
	delete[] next;
}
//轮转法
void RR(Process *p, int len) {
	double span;
	cout << "输入时间片：";
	cin >> span;
	double  *leftTime = new double[len];
	double  turnAvr = 0;
	double weightAvr = 0;
	for (int i = 0; i < len; i++) {
		leftTime[p[i].ind] = p[i].runTime;
	}
	deque<Process*> q;
	int finished = 0;
	for (double t = 0; finished < len; ) {
		for (int i = 0; i < len; i++) {
			if (p[i].arriveTime <= t && !p[i].arrived) {
				q.push_front(&p[i]);
				p[i].arrived = true;
			}
		}
		if (q.size() == 0) {
			t += span;
			continue;
		}
		Process* cp = q.front();
		q.pop_front();

		//给cp运行
		leftTime[cp->ind] -= span;
		//是否运行完?
		if (leftTime[cp->ind] <= 0) {
			finished++;
			turnAvr += t + span + leftTime[cp->ind];
			weightAvr += (t + span + leftTime[cp->ind] - cp->arriveTime) / (double)cp->runTime;
			cout << "进程:" << cp->name << "\t完成时间：" << t + span + leftTime[cp->ind] << "\t周转时间：" << t + span + leftTime[cp->ind] - cp->arriveTime << "\t带权周转时间：" << (t + span + leftTime[cp->ind] - cp->arriveTime) / (double)cp->runTime << endl;
			t += span + leftTime[cp->ind];
		}
		else {
			q.push_back(cp);
			t += span;
		}
	}
	cout << "平均周转时间为：" << turnAvr / len << "平均带权周转时间为：" << weightAvr / len << endl;
}
void RR(Process *p, queue<Process> q, int len, double *t) {
	cout << "输入时间片：" << endl;
	int span;
	cin >> span;
	SArray *s = new SArray[len];
	SArray *s2 = new SArray[len];
	for (int i = 0; i < len; i++) {
		s[i].index = i;
		s[i].value = p[i].arriveTime;
	}
	Sort(s, len);
	//剩余时间
	int *lastT = new int[len];
	//出队列时间
	int *outT = new int[len];
	for (int i = 0; i < len; i++) {
		lastT[s[i].index] = p[s[i].index].runTime;
		outT[s[i].index] = s[i].value;
	}

	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			if (p[s[j].index].isGet != true) {
				if (outT[s[j].index] <= outT[s[i].index]) {//第一个进程必须执行
					q.push(p[s[j].index]);
					if (lastT[s[j].index] > span) {
						outT[s[j].index] += span;
						lastT[s[j].index] -= span;
						q.pop();
					}
					else {
						outT[s[j].index] += lastT[s[j].index];//完成
						lastT[s[j].index] = 0;
						p[s[j].index].isGet = true;
						cout << "进程" << p[s[j].index].name << "完成时间为：" << outT[s[j].index] << "周转时间：" << endl;
						q.pop();

					}
				}
				else {
					for (int c = 0; c <= i; c++) {
						if (p[s[c].index].isGet != true) {
							q.push(p[s[c].index]);
							if (lastT[s[c].index] > span) {
								outT[s[c].index] += span;
								lastT[s[c].index] -= span;
								q.pop();
							}
							else {
								outT[s[c].index] += lastT[s[c].index];//完成
								lastT[s[c].index] = 0;
								p[s[c].index].isGet = true;
								cout << "进程" << p[s[c].index].name << "完成时间为：" << outT[s[j].index] << "周转时间：" << endl;
								q.pop();
							}
						}
					}
				}
			}
		}
	}

}
//优先级法
void SP(Process * p5, int len, double *t) {
	cout << "请输入优先级：";
	int *p = new int[len];
	SArray *s = new SArray[len];
	SArray *next = new SArray[len];
	for (int i = 0; i < len; i++) {
		cin >> p[i];
	}
	int first = FindFirst(p5, len);
	s[0].index = first;
	s[0].value = p5[first].arriveTime;
	t[0] = p5[first].arriveTime + p5[first].runTime;
	//p5[first].isGet = true;
	for (int c = 1; c < len; c++) {
		int count = 0;
		for (int j = 0; j < len; j++) {
			if (p5[j].arriveTime <= t[c - 1] && p5[j].isGet != true) {
				next[count].index = j;
				next[count].value = p[j];
				count++;
			}
		}
		if (count > 0) {//按照优先级来执行
			Sort(next, count);
			t[c] = p5[next[0].index].runTime + t[c - 1];
			s[c].index = next[0].index;
			s[c].value = p5[next[0].index].arriveTime;
			p5[next[0].index].isGet = true;
		}
		else {//按照其到达时间来执行
			int f, count2 = 0;
			for (int i = 0; i < len; i++) {
				if (p5[i].isGet != true) {
					next[count2].index = i;
					next[count2].value = p5[i].arriveTime;
					count2++;
				}
			}
			Sort(next, count2);//越早到的先运行
			t[c] = p5[next[0].index].runTime + t[c - 1];
			s[c].index = next[0].index;
			s[c].value = next[0].value;
			p5[next[0].index].isGet = true;
		}
	}
	Print(p5, t, s, len);
	delete[] next;
}
//改进的冒泡排序,从小到大
void Sort(SArray *s, int len) {
	double temp;
	int  ind;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len - i - 1; j++) {
			if (s[j].value > s[j + 1].value) {//从小至大
				temp = s[j].value;
				s[j].value = s[j + 1].value;
				s[j + 1].value = temp;
				//交换下标
				ind = s[j].index;
				s[j].index = s[j + 1].index;
				s[j + 1].index = ind;
			}
		}
	}
}
//找到最先请求的进程，返回其下标ind
int  FindFirst(Process *p, int len) {
	Process   first = p[0];
	for (int i = 1; i < len; i++) {
		if (first.arriveTime > p[i].arriveTime) {
			first.arriveTime = p[i].arriveTime;
			first.ind = p[i].ind;
		}
	}
	return first.ind;
}
//输出结果
void  Print(Process *p1, double *t, SArray * s, int len) {
	cout << "进程名\t执行顺序\t开始时间\t完成时间\t周转时间\t带权周转时间\t\n";
	double  turnAvr = 0, weightAvr = 0;
	
	for (int i = 0; i < len; i++) {
		if (s[i].index == 0) {
			cout << p1[0].name << "\t" << s[0].index + 1 << "\t\t" << p1[0].arriveTime << "\t\t" << t[s[0].index] << "\t\t" << (t[s[0].index] - p1[0].arriveTime) << "\t\t" << (t[s[0].index] - p1[0].arriveTime) / (double)p1[0].runTime << "\t\n";
		}
		else {
			cout << p1[i].name << "\t" << s[i].index + 1 << "\t\t" << t[s[i].index - 1] << "\t\t" << t[s[i].index] << "\t\t" << (t[s[i].index] - p1[i].arriveTime) << "\t\t" << (t[s[i].index] - p1[i].arriveTime) / (double)p1[i].runTime << "\t\n";
			turnAvr += (t[s[i].index] - p1[i].arriveTime);
			weightAvr += (t[s[i].index] - p1[i].arriveTime) / p1[i].runTime;
		}
	}
	cout << "平均周转时间：" << turnAvr / len << "平均带权周转时间：" << weightAvr / len << endl;
	delete[] p1;
}
//计算响应比
double  ComputeResponse(Process p, double preT) {
	float res;
	res = (preT - p.arriveTime + p.runTime) / p.runTime;//响应比
	return res;
}
