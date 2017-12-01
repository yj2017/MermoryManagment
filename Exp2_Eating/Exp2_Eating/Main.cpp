#include <iostream>
#include<thread>
using namespace std;
void  print2();
void  print1();

int main() {
	thread th1(print1);
	th1.join();
	thread th2(print2);

	th2.join();
	cout << "hello main" << endl;
	system("pause");
	return 0;
}
void  print1() {
	cout << "hello 1" << endl;
}
void  print2() {
	cout << "hello 2" << endl;
}