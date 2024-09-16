#include<windows.h>
#include<gdiplus.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"gdiplus.lib")
LRESULT CALLBACK winproc(HWND,UINT,WPARAM,LPARAM);
Gdiplus::Graphics* G_graphics;
Gdiplus::Image* G_image;
HDC G_memdc;
HBITMAP G_buf;
HBRUSH G_brush;
HINSTANCE G_instance;
bool timer_seted;
#define BNT_1 100
#define BNT_2 101
#define TIMER_ID 200
#define TIMER_ELAPSE 100
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE pre,LPSTR cmdLine,int cmdshow){
  WNDCLASS winclass={0};
  winclass.hInstance=G_instance=hinstance;
  winclass.lpfnWndProc=winproc;
  winclass.lpszClassName="WIN";
  winclass.hCursor=LoadCursor(NULL,IDC_ARROW);
  winclass.hbrBackground=G_brush=(HBRUSH)GetStockObject(GRAY_BRUSH);
  winclass.hIcon=(HICON)LoadImage(NULL,"./fold.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_LOADFROMFILE);
  RegisterClass(&winclass);
  ULONG_PTR gditoken;
  Gdiplus::GdiplusStartupInput gdistartupinput;
  Gdiplus::GdiplusStartup(&gditoken,&gdistartupinput,NULL);
  
  HWND win=CreateWindow("WIN","test",WS_OVERLAPPEDWINDOW,300,300,400,400,NULL,NULL,hinstance,0);
  

  ShowWindow(win,cmdshow);
  MSG msg={0};
  while(GetMessage(&msg,0,0,0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  Gdiplus::GdiplusShutdown(gditoken);
  return (int)msg.wParam;
}

LRESULT CALLBACK winproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
  static Gdiplus::Rect des_rect(0,0,128,128);
  static INT x=0;
  switch(msg){
    case WM_CREATE:{
      SetTimer(hwnd,(UINT_PTR)TIMER_ID,TIMER_ELAPSE,NULL);
      timer_seted=true;
      CreateWindow("Button","pause",WS_CHILD | WS_VISIBLE,100,300,80,30,hwnd,(HMENU)BNT_1,G_instance,0);
      CreateWindow("Button","continue",WS_CHILD | WS_VISIBLE,200,300,80,30,hwnd,(HMENU)BNT_2,G_instance,0);
      HDC hdc=GetDC(hwnd);
      G_memdc=CreateCompatibleDC(hdc);
      G_buf=CreateCompatibleBitmap(hdc,200,200);
      SelectObject(G_memdc,G_brush);
      SelectObject(G_memdc,G_buf);
      ReleaseDC(hwnd,hdc);

      G_graphics= new Gdiplus::Graphics(G_memdc);
      G_image= new Gdiplus::Image(L"./res/img/horse/horse-black.png");
    }break;

    case WM_PRINTCLIENT:{
      HDC hdc=GetDC(hwnd);
      ::Rectangle(G_memdc,0,0,200,200);
      G_graphics->DrawImage(G_image,des_rect,x,0,128,128,Gdiplus::UnitPixel);
      BitBlt(hdc,0,0,200,200,G_memdc,0,0,SRCCOPY);
      ValidateRect(hwnd,NULL);
      ReleaseDC(hwnd,hdc);
    }break;

    case WM_TIMER:{
      SendMessage(hwnd,WM_PRINTCLIENT,0,0);
      x+= 128;
      if(x>= 512) x=0;
    }break;

    case WM_DESTROY:{
      delete G_image;
      delete G_graphics;
      DeleteObject(G_buf);
      DeleteDC(G_memdc);
      PostQuitMessage(0);
    }break;

    case WM_COMMAND:{
      switch(LOWORD(wparam)){
        case BNT_1:{
          if(timer_seted){
            KillTimer(hwnd,(UINT_PTR)TIMER_ID);
            timer_seted=false;
          }
        }break;

        case BNT_2:{
          if(!timer_seted){
            SetTimer(hwnd,(UINT_PTR)TIMER_ID,TIMER_ELAPSE,NULL);
            timer_seted=true;
          }
        }break;

        default:break;
      }
    }break;

    case WM_SIZE:{
      HDC hdc=GetDC(hwnd);
      RECT rect;
      GetClientRect(hwnd,&rect);
      DeleteObject(G_buf);
      G_buf=CreateCompatibleBitmap(hdc,rect.right-rect.left,rect.bottom-rect.top);
      SelectObject(G_memdc,G_buf);
      ReleaseDC(hwnd,hdc);
    }break;

    default:
    return DefWindowProc(hwnd,msg,wparam,lparam);
  }
  return (LRESULT)0;
}