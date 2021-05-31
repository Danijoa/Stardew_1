#include "MainGame.h"
#include "Image.h"
#include "TilemapTool.h"
#include "StartScene.h"
#include "HouseScene.h"
#include "FarmScene.h"
#include "StoreScene.h"
#include "DataManager.h"
#include "InventoryManager.h"
#include "LoadingScene.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();
	DataManager::GetSingleton()->Init();
	InventoryManager::GetSingleton()->Init();

	// ����� �̹���
	int maxWidth, maxHeight;
	maxWidth = max(WINSIZE_X, TILEMAPTOOLSIZE_X);
	maxHeight = max(WINSIZE_Y, TILEMAPTOOLSIZE_Y);
	backBuffer = new Image();
	backBuffer->Init(maxWidth, maxHeight);

	// Ű�� �� ����
	SceneManager::GetSingleton()->AddScene("Ÿ�ϸ���", new TilemapTool());
	SceneManager::GetSingleton()->AddScene("���۾�", new StartScene());
	SceneManager::GetSingleton()->AddScene("�Ͽ콺��", new HouseScene());
	SceneManager::GetSingleton()->AddScene("�����", new FarmScene());
	SceneManager::GetSingleton()->AddScene("������", new StoreScene());
	SceneManager::GetSingleton()->AddLoadingScene("�ε���", new LoadingScene());

	// ���� �� -> �ΰ����� x��� Ű���� �������
	//SceneManager::GetSingleton()->ChangeScene("Ÿ�ϸ���");
	//SceneManager::GetSingleton()->ChangeScene("���۾�");
	//SceneManager::GetSingleton()->ChangeScene("�Ͽ콺��");
	SceneManager::GetSingleton()->ChangeScene("�����");
	//SceneManager::GetSingleton()->ChangeScene("������");

	return S_OK;
}

void MainGame::Release()
{
	KeyManager::GetSingleton()->Release();
	ImageManager::GetSingleton()->Release();
	SceneManager::GetSingleton()->Release();
	DataManager::GetSingleton()->Release();
	InventoryManager::GetSingleton()->Release();

	SAFE_RELEASE(backBuffer);

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();
	DataManager::GetSingleton()->SetCurBackBuffer(backBuffer);
}

void MainGame::Render()
{
	HDC hBackDC = backBuffer->GetMemDC();

	SceneManager::GetSingleton()->Render(hBackDC);


	// ���콺 ��ǥ
	wsprintf(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackDC, 80, 0, szText, strlen(szText));

	// FPS
	TimerManager::GetSingleton()->Render(hBackDC);

	backBuffer->Render(hdc);

	// ��
	//HBRUSH testBrush = CreateSolidBrush(RGB(90, 90, 90));
	//HBRUSH testOldBrush = (HBRUSH)SelectObject(hdc, testBrush);
	//backBuffer->LightRender(hdc, 0, 0);
	//testBrush = (HBRUSH)SelectObject(hdc, testOldBrush);
	//DeleteObject(testBrush);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_MOUSEWHEEL:
		if ((short)HIWORD(wParam) > 0)							//up
			InventoryManager::GetSingleton()->SetScrolledUp(true);
		if ((short)HIWORD(wParam) < 0)							//down
			InventoryManager::GetSingleton()->SetScrolledDown(true);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}
