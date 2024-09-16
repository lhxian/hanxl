#include<Windows.h>
#define DEBUG
#ifdef DEBUG
#include<stdio.h>
#define MY_Debug(a,...) printf(a"\n",__VA_ARGS__);

#else
#define MY_Debug(a,...) ;
#endif
HINSTANCE g_instance;
LRESULT CALLBACK winproc(HWND,UINT,WPARAM,LPARAM);
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE pre,LPSTR cmdLine ,int cmdshow){
  #ifdef DEBUG
  if(AllocConsole()){
    freopen("CONOUT$","r",stdin);
    freopen("CONOUT$","w",stdout);
    freopen("CONOUT$","w",stderr);
    fputs("debug\n",stderr);
  }
  #endif
  g_instance= hinstance;
  WNDCLASS winclass={0};
  winclass.hInstance=g_instance;
  winclass.hCursor=(HCURSOR)LoadCursor(NULL,IDC_ARROW);
  winclass.lpszClassName="TEST";
  winclass.lpfnWndProc=winproc;
  winclass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);

  RegisterClass(&winclass);

  HWND win=CreateWindow("TEST","test",WS_OVERLAPPEDWINDOW,100,100,400,400,NULL,NULL,g_instance,NULL);
  ShowWindow(win,cmdshow);
  MSG msg={0};
  while(GetMessage(&msg,NULL,0,0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);

  }
  FreeConsole();
  return (int)msg.wParam;
}
LRESULT CALLBACK winproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
  switch(msg){
    case WM_CREATE:{
      SetTimer(hwnd,(UINT_PTR)200,2000,NULL);
    }break;
    case WM_TIMER:{
      MY_Debug("wm_timer")
    }break;

    case WM_DESTROY:{
      PostQuitMessage(0);
    }break;

    default:
    return DefWindowProc(hwnd,msg,wparam,lparam);
  }
  return 0;
}