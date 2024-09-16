#include<iostream>
#include<windows.h>
using namespace std;
void fun(int a){
  static int* p=new int[a];
  Sleep(500);
}
int main(){
  int cnt;
  cin>>cnt;
  for(int i=0;i<cnt;++i)  fun(5000);
}