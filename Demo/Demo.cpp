// Demo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Demo.h"

#pragma comment( lib, "GraphicsVk.lib")
#pragma comment( lib, "VkApiLoader.lib")

#include "DeviceVk.h"
#include "ViewVk.h"
#include "TextureVk.h"
#include "VkApiLoader.h"
#include "clGraphic.h"
#include <vulkan/vulkan.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <io.h>
#include <fcntl.h> //��<sys\types.h>��<sys\stat.h>��<share.h>

clannad::vulkan::Device* device = nullptr;
clannad::vulkan::View* view = nullptr;
clannad::vulkan::ApiLoader apiLoader;
clannad::vulkan::DeviceLoadData;

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

    MSG msg;

    // ����Ϣѭ��: 
	/*
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/
	//
	bool bLoop = true;

	while (bLoop)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//WndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			// Process events
			/*
			switch (msg.message) {
				// Resize
			case WM_SIZE:
				bResize = true;
				vk.canRender = false;
				break;
				// Close
			case WM_DESTROY:
				vk.canRender = false;
				bLoop = false;
				break;
			}
			*/
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		else 
		{

			view->begin();
			view->end();

			/*else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}*/
		}
	}
	return (int)msg.wParam;

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}



static const WORD MAX_CONSOLE_LINES = 500;
void RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// ����һ������̨����  
	AllocConsole();

	// �����㹻�����Ļ������������ǻ����ı�
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	// ���ÿ���̨��Ļ�ĸ߶�
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;

	// ���ÿ���̨��Ļ�����С
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
		coninfo.dwSize);

	// ��ȡ��׼������
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

	//�򿪱�׼�����������ƴ��ļ��ķ�ʽ��fopen,����һ���ļ�������
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	// �Կ�д�ķ�ʽ��
	fp = _fdopen(hConHandle, "w");

	*stdout = *fp;

	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console  
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console  
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog  
	// point to console as well  
	std::ios::sync_with_stdio();
}


//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   RedirectIOToConsole();  

   bool rst = false;
   rst = apiLoader.Initialize();
   //
   VkInstance inst = apiLoader.CreateInstance();
   clannad::vulkan::DeviceLoadData deviceData;
   rst = apiLoader.CreateDevice(inst,0, deviceData);
   //
   view = clannad::vulkan::View::createViewWin32(hWnd);
   //
   device = new clannad::vulkan::Device(deviceData);
   //
   device->attachView(view);
   //
   view->prepareForSwapchain();

   clannad::ClTextureDesc desc;
   desc.format = clannad::ClPixelFormat_RGBA8888_UNORM;
   desc.width = desc.height = 64;
   desc.mipmap = false;
   auto tex = clannad::vulkan::Texture2D::Create(device, desc);
   //
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_SIZE:
	{
		static UINT cx, cy;
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		//
		view->onResize(int(cx), int(cy));
		//
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
