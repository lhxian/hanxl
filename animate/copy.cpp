#include<Windows.h>
#include<gdiplus.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"gdiplus.lib")
#define W_W 400
#define W_H 400
#define DEBUG
#ifdef DEBUG
#include<stdio.h>
#define MY_Debug(a,...) printf(a,__VA_ARGS__);

#else
#define MY_Debug(a,...) ;
#endif
//global variables
HINSTANCE g_instance;
Gdiplus::Image* g_image;
Gdiplus::Graphics* g_graphics;
Gdiplus::Rect* des_rect;
HBITMAP g_bitmap;
HDC g_mem_dc;

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
  Gdiplus::GdiplusStartupInput startupinput;
  ULONG_PTR gditoken;
  Gdiplus::GdiplusStartup(&gditoken,&startupinput,NULL);

  HWND win=CreateWindow("TEST","test",WS_OVERLAPPEDWINDOW,100,100,W_W,W_H,NULL,NULL,g_instance,NULL);
  ShowWindow(win,cmdshow);
  MSG msg={0};
  while(GetMessage(&msg,NULL,0,0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);

  }
  FreeConsole();
  Gdiplus::GdiplusShutdown(gditoken);
  return (int)msg.wParam;
}
LRESULT CALLBACK winproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
  static INT x=0;
  switch(msg){
    case WM_CREATE:{
      CreateWindow("button","pause",WS_CHILD | WS_VISIBLE,W_W-100,W_H-100,80,30,hwnd,(HMENU)100,g_instance,0);
      SetTimer(hwnd,(UINT_PTR)200,50,NULL);
      HDC hdc=::GetDC(hwnd);
      g_mem_dc=CreateCompatibleDC(hdc);
      RECT rect;
      GetClientRect(hwnd,&rect);
      g_bitmap=CreateCompatibleBitmap(hdc,rect.right-rect.left,rect.bottom-rect.top);
      SelectObject(g_mem_dc,g_bitmap);
      g_graphics= new Gdiplus::Graphics(g_mem_dc);
      g_image=new Gdiplus::Image(L"./res/img/horse/horse-golden.png");
      if(g_image->GetLastStatus() != Gdiplus::Ok){
        MY_Debug("load image file fail")
      }
      des_rect= new Gdiplus::Rect(0,0,128,128);
      //src_rect=new Gdiplus::Rect(0,0,128,128);
      //src_rect=new Gdiplus::Rect(0,0,128,128);


      ::ReleaseDC(hwnd,hdc);
      MY_Debug("width: %d  height: %d\n",g_image->GetWidth(),g_image->GetHeight())
    }break;
    case WM_DESTROY:{
      PostQuitMessage(0);
      DeleteObject(g_bitmap);
      DeleteDC(g_mem_dc);

      delete g_image;
      delete g_graphics;
      delete des_rect;
      //delete src_rect;
      //delete src_rect;
      
    }break;

    case WM_TIMER:{
      static WPARAM index=0;
      SendMessage(hwnd,WM_PRINTCLIENT,0,index);
      x+= 128;
      if(x>=512)  x=0;
      index++;
      
    }break;

    case WM_COMMAND:{
      switch(LOWORD(wparam)){
        case 100:{
          KillTimer(hwnd,(UINT_PTR)200);
        }break;

        default:break;
      }
    }break;

    case WM_PRINTCLIENT:{
      ::PAINTSTRUCT paintstruct; 
      HDC hdc=GetDC(hwnd);
      //HDC hdc=BeginPaint(hwnd,&paintstruct);
      ::HGDIOBJ brush=SelectObject(g_mem_dc,(HBRUSH)GetStockObject(GRAY_BRUSH));
      ::Rectangle(g_mem_dc,0,0,200,200);
      HBRUSH tem_brush=CreateSolidBrush(0xff);
      SelectObject(g_mem_dc,tem_brush);
      Rectangle(g_mem_dc,0,0,128,128);
      if(g_graphics->DrawImage(g_image,*des_rect,x,0,128,128,Gdiplus::UnitPixel) != Gdiplus::Ok){
        MY_Debug("draw image fail")
      }
      //g_graphics->DrawImage(g_image,0,0,300,300);
      //g_graphics->DrawImage(g_image,des_rect,src_rect,Gdiplus::UnitPixel);
      // g_graphics->DrawImage(g_image,*des_rect);
      DeleteObject(tem_brush);
      BitBlt(hdc,0,0,100,100,g_mem_dc,0,0,SRCCOPY);
      ::SelectObject(g_mem_dc,brush);
      ValidateRect(hwnd,NULL);
      ReleaseDC(hwnd,hdc);
      //::EndPaint(hwnd,&paintstruct);

    }break;
    default:
    return DefWindowProc(hwnd,msg,wparam,lparam);
  }
  return (LRESULT)0;
}