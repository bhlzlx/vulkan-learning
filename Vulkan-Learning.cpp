// Vulkan-Learning.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Vulkan-Learning.h"
#include <iostream>
#include <cassert>
#include <thread>
#include <vknpp/vknpp.h>
#include <vknpp/vkTypes.h>
#include <vknpp/vkDylib.h>

struct vkapp
{
	vknpp::Dylib vklib;
	vknpp::Instance instance;
	vknpp::Surface surface;
	vknpp::PhysicalDevice physicalDevice;
	vknpp::LogicalDevice logicalDevice;
	//
	VkSemaphore imageAvail;
	VkSemaphore drawFinished;
	//
	bool canRender = false;

	bool Resize(uint32_t _width, uint32_t _height)
	{
		bool rst = false;
		if (canRender)
		{
			canRender = false;
			rst = logicalDevice.CreateSwapchain(surface); assert(rst && "operation failed!");
			rst = logicalDevice.swapchain.CreateCommandBuffers(logicalDevice); assert(rst && "operation failed!");
			rst = logicalDevice.swapchain.CreateImages(logicalDevice); assert(rst && "operation failed!");
			rst = logicalDevice.swapchain.RecordCommandBuffers(logicalDevice); assert(rst && "operation failed!");
			canRender = true;
		}
		return rst;
	}

	bool Draw()
	{
		uint32_t image_index;
		VkResult result = vkAcquireNextImageKHR( logicalDevice, logicalDevice.swapchain, UINT64_MAX, this->imageAvail, VK_NULL_HANDLE, &image_index);
		switch (result) 
		{
		case VK_SUCCESS:
		case VK_SUBOPTIMAL_KHR:
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			return Resize(0, 0);
		default:
			std::cout << "Problem occurred during swap chain image acquisition!" << std::endl;
			return false;
		}

		VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		VkSubmitInfo submit_info = 
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
			nullptr,                                      // const void                  *pNext
			1,                                            // uint32_t                     waitSemaphoreCount
			&imageAvail,              // const VkSemaphore           *pWaitSemaphores
			&wait_dst_stage_mask,                         // const VkPipelineStageFlags  *pWaitDstStageMask;
			1,                                            // uint32_t                     commandBufferCount
			&logicalDevice.swapchain.commandBuffers[image_index],  // const VkCommandBuffer       *pCommandBuffers
			1,                                            // uint32_t                     signalSemaphoreCount
			&drawFinished            // const VkSemaphore           *pSignalSemaphores
		};

		if (vkQueueSubmit( logicalDevice.presentQueue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
			return false;
		}

		VkPresentInfoKHR present_info = {
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
			nullptr,                                      // const void                  *pNext
			1,                                            // uint32_t                     waitSemaphoreCount
			&drawFinished,           // const VkSemaphore           *pWaitSemaphores
			1,                                            // uint32_t                     swapchainCount
			(VkSwapchainKHR*)&logicalDevice.swapchain,                            // const VkSwapchainKHR        *pSwapchains
			&image_index,                                 // const uint32_t              *pImageIndices
			nullptr                                       // VkResult                    *pResults
		};
		result = vkQueuePresentKHR(logicalDevice.presentQueue, &present_info);

		switch (result) {
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
		case VK_SUBOPTIMAL_KHR:
			return Resize(0, 0);
		default:
			std::cout << "Problem occurred during image presentation!" << std::endl;
			return false;
		}
		return true;
	}
}vk;


#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
HWND hwnd;

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
    LoadStringW(hInstance, IDC_VULKANLEARNING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VULKANLEARNING));

    MSG msg;
	bool rst = false;
	// ���� vulkan-1.dll
	rst = vk.vklib.Load(); assert(rst && "operation failed!");
	rst = vk.vklib.LoadGlobalApi(); assert(rst && "operation failed!");
	rst = vknpp::Instance::Create(vknpp::defInstCreateInfo, vk.instance); assert(rst && "operation failed!");
	rst = vk.vklib.LoadInstanceApi(vk.instance); assert(rst && "operation failed!");
	rst = vknpp::Surface::CreateWin32Surface(vk.instance, hwnd, hInstance, vk.surface); assert(rst && "operation failed!");
	std::vector<vknpp::PhysicalDevice> vecPhyDvc; assert(rst && "operation failed!");
	rst = vknpp::PhysicalDevice::EnumPhysicalDevices(vk.instance, vecPhyDvc); assert(rst && "operation failed!");
	rst = false;
	for (auto & _dvc : vecPhyDvc)
	{
		auto prop = _dvc.GetProperties();
		if (_dvc.SelectQueueFamilies(vk.surface))
		{
			printf("Graphics card \'%s\' selected!", prop.deviceName);
			vk.physicalDevice = _dvc;
			rst = true;
			break;
		}
		else
		{
			printf("Graphics card \'%s\' unsupported!", prop.deviceName);
		}
	}
	assert(rst && "operation failed!");
	//
	rst = vknpp::LogicalDevice::Create(vk.physicalDevice, vk.logicalDevice); assert(rst && "operation failed!");
	rst = vk.vklib.LoadDeviceApi(vk.logicalDevice); assert(rst && "operation failed!");
	//
	rst = vk.logicalDevice.CreateSwapchain(vk.surface); assert(rst && "operation failed!");

	rst = vk.logicalDevice.swapchain.CreateImages(vk.logicalDevice); assert(rst && "operation failed!");
	rst = vk.logicalDevice.swapchain.CreateCommandBuffers(vk.logicalDevice); assert(rst && "operation failed!");
	rst = vk.logicalDevice.swapchain.RecordCommandBuffers(vk.logicalDevice); assert(rst && "operation failed!");

	rst = vk.logicalDevice.SelectQueues(); assert(rst && "operation failed!");
	//
	rst = vk.logicalDevice.CreateSemaphore(vk.imageAvail); assert(rst && "operation failed!");
	rst = vk.logicalDevice.CreateSemaphore(vk.drawFinished); assert(rst && "operation failed!");

	vk.canRender = true;

	bool bLoop = true;
	bool bResize = false;

	while (bLoop) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			WndProc( hwnd, msg.message, msg.wParam, msg.lParam );
			// Process events
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
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Resize
			if (bResize) 
			{
				bResize = false;
				vk.Resize(320, 320);
			}
			// Draw
			if (vk.canRender)
			{
				vk.Draw();
			}
			else 
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VULKANLEARNING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_VULKANLEARNING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   hwnd = hWnd;

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
