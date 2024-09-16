#include<windows.h>
#include<gdiplus.h>
#include<mmsystem.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"winmm.lib")

#define M_W 800
#define M_H 600
#define S_W 400
#define S_H 400

#define BTN_1 100
#define BTN_2 102
#define TIMER_ID 200
#define S_WINDOW 300
#define ELAPSE 40

//message custom
#define WM_M_PAUSE (WM_USER+1)
#define WM_M_CONTINUE (WM_USER+2)

typedef LRESULT (CALLBACK *Winpr)(HWND,UINT,WPARAM,LPARAM);
//global functions and variables;
static LRESULT CALLBACK Main_proc(HWND,UINT,WPARAM,LPARAM);
static LRESULT CALLBACK Sub_proc(HWND,UINT,WPARAM,LPARAM);

static HINSTANCE G_instance;
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE pre,LPSTR cmdline,int cmdshow){
  WNDCLASS Main_window={0},Sub={0};
  Main_window.lpszClassName="MAIN";
  Main_window.hInstance=hinstance;
  Main_window.lpfnWndProc=Main_proc;
  Main_window.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);
  Main_window.hCursor=LoadCursor(NULL,IDC_ARROW);


  Sub.lpszClassName="SUB";
  Sub.hInstance=hinstance;
  Sub.hCursor=LoadCursor(NULL,IDC_ARROW);
  Sub.lpfnWndProc=Sub_proc;
  RegisterClass(&Main_window);
  RegisterClass(&Sub);

  ULONG_PTR gditoken;
  Gdiplus::GdiplusStartupInput gdiInput;
  Gdiplus::GdiplusStartup(&gditoken,&gdiInput,nullptr);

  HWND win=CreateWindow("MAIN","main",WS_OVERLAPPEDWINDOW,200,200,800,600,NULL,NULL,hinstance,0);
  ShowWindow(win,cmdshow);

  MSG msg={0};

  while(GetMessage(&msg,NULL,0,0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  Gdiplus::GdiplusShutdown(gditoken);
  UnregisterClass("SUB",hinstance);UnregisterClass("MAIN",hinstance);
  return (int)msg.wParam;
}

LRESULT CALLBACK Main_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
  static HWND S=NULL;
  static int status=1;
  switch(msg){
    case WM_CREATE:{
      S=CreateWindow("SUB","sub",WS_CHILD | WS_VISIBLE,0,0,S_W,S_H,hwnd,(HMENU)S_WINDOW,G_instance,0);
      CreateWindow("button","pause",WS_CHILD | WS_VISIBLE,100,500,80,30,hwnd,(HMENU)BTN_1,G_instance,0);
      CreateWindow("button","continue",WS_CHILD | WS_VISIBLE,250,500,80,30,hwnd,(HMENU)BTN_2,G_instance,0);
      //HDC hdc=GetDC(hwnd);


      //ReleaseDC(hwnd,hdc);

    }break;

    case WM_COMMAND:{

      switch(LOWORD(wparam)){
        case BTN_1:{

          if(status){
            status=0;
            SendMessage(S,WM_M_PAUSE,0,1);
          }
        }break;

        case BTN_2:{
          if(!status){
            status=1;
            SendMessage(S,WM_M_CONTINUE,0,1);
          }

        }break;

        default:break;
      }
    }break;

    case WM_DESTROY:{
      PostQuitMessage(0);

    }break;


    default:
    return DefWindowProc(hwnd,msg,wparam,lparam);

  }


  return (LRESULT)0;
}

LRESULT CALLBACK Sub_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
  static Gdiplus::Image* S_image=nullptr;
  static bool timer_on=true;
  static HDC S_memdc;
  static HBITMAP S_buf;
  static Gdiplus::Graphics*  S_graphics=nullptr;
  static INT x,y;
  static HBRUSH S_brush;
  static Gdiplus::Rect des_rect(0,0,128,128);
  switch(msg){
    case WM_CREATE:{
      x=0;
      y=128;
      HDC hdc =GetDC(hwnd);
      S_memdc=::CreateCompatibleDC(hdc);
      S_buf=CreateCompatibleBitmap(hdc,S_W,S_H);
      S_brush=CreateSolidBrush(0xffffff);
      SelectObject(S_memdc,S_brush);
      SelectObject(S_memdc,S_buf);
      S_graphics= new Gdiplus::Graphics(S_memdc);
      S_image= new Gdiplus::Image(L"./res/img/horse/horse-golden.png");


      ReleaseDC(hwnd,hdc);
      SetTimer(hwnd,(UINT_PTR)TIMER_ID,ELAPSE,nullptr);
    }break;

    case WM_DESTROY:{
      delete S_image;
      delete S_graphics;
      DeleteObject(S_brush);
      DeleteObject(S_buf);
      DeleteDC(S_memdc);

    }break;

    case WM_PAINT:{
      //MessageBox(NULL,"paint","infor",MB_OK);

      HDC hdc=GetDC(hwnd);
      if(wparam == 1){
        Rectangle(S_memdc,0,0,S_W,S_H);
        S_graphics->DrawImage(S_image,des_rect,x,128,128,128,Gdiplus::UnitPixel);
        BitBlt(hdc,0,0,S_W,S_H,S_memdc,0,0,SRCCOPY);
      }
      ValidateRect(hwnd,nullptr);
      ReleaseDC(hwnd,hdc);
    }break;

    case WM_TIMER:{
      SendMessage(hwnd,WM_PAINT,1,0);
      x+= 128;
      x &=511;
    }break;
    
    case WM_M_PAUSE:{
      KillTimer(hwnd,(UINT_PTR)TIMER_ID);
    }break;

    case WM_M_CONTINUE:{
      SetTimer(hwnd,(UINT_PTR)TIMER_ID,ELAPSE,nullptr);
    }break;

    default:
    return DefWindowProc(hwnd,msg,wparam,lparam);
  }

  return (LRESULT)0;
}