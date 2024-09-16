#include<windows.h>
#include<stdio.h>
void sub_function(int a){

  for(int i=0;i<a;++i){
    printf("hello world %d\n",i);
    Sleep(1000);
  }
  
}
DWORD WINAPI Sub(LPVOID parameter){
  int* a=(int*)parameter;
  for(int i=0;i<*a;++i){
    printf("hello world %d\n",i);
    Sleep(1000);
    printf("sub %d\n",i);
  }
  return (DWORD)0;
}
int main(){
  int a;
  scanf_s("%d",&a);
  HANDLE sub=CreateThread(NULL,0,Sub,(LPVOID)&a,0,NULL);
  for(int i=0;i<a;++i){
    printf("main %d\n",i);
    Sleep(900);
  }
  //Sleep(1000);
  if(!CloseHandle(sub)) printf("close handle fail\n");
  else printf("close thread susseed\n");
  printf("close thread\n");

  printf("sleep\n");
  Sleep(4000);
  
}