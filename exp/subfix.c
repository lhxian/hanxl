#include<stdlib.h>
#include<string.h>
#include<stdio.h>
char* expression(const char* str){
  int len=strlen(str)+1;
  char* s=(char*)malloc(sizeof(char) * len),* result=(char*)malloc(sizeof(char) * len);
  int index=-1;
  char* re=result;
  while(*str){
    switch(*str){
      case '+':
      case '-':{
        while(index>=0 && s[index] != '('){
          *re++ = s[index--];
        }
        s[++index]=*str;
      }break;

      case '*':
      case '/':{
        s[++index]=*str;
      }break;

      case '(':{
        s[++index]='(';
      }break;
      case ')':{
        while(s[index] != '('){
          *re++=s[index--];
        }
        --index;

      }break;

      default:
      *re++ = *str;
      break;
    }
    ++str;
  }

  while(index >=0){
    //putchar(s[index]);
    *re++=s[index--];
  }
  *re='\0';

  free(s);
  return result;
}
int main(){
  char ch[200];
  scanf("%s",ch);
  char* exp=expression(ch);
  puts(exp);
  free(exp);
}