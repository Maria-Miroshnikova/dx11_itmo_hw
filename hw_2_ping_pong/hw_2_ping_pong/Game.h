#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>



#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "InputDevice.h"
#include "DisplayWind32.h"
#include "GameComponent.h"
#include <vector>

class Game {

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	std::vector<GameComponent*> Components;

	//dx11 элементы
	ID3D11Texture2D* backBuffer;
	ID3D11DeviceContext* Context;
	int DebugAnnotation; // ??
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	HINSTANCE Instance;
	IDXGISwapChain* SwapChain;

	DisplayWin32* Display;
	InputDevice* InputDev;
//	Viewport ? мб это часть draw

	// ресурсы и вью
	int RenderSRV; // ?? 
	ID3D11RenderTargetView* RenderView;

	// параметры
	LPCWSTR Name;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float StartTime;
	float TotalTime;
	int FrameCount;
	std::chrono::time_point<std::chrono::steady_clock> CurTime;
	float DeltaTime;

	int ScreenResized;
	bool isExitRequested = false;

	//////////////////////////////////////////////////////////////

	Game(LPCWSTR name, int screenWidth, int screenHeight);
	~Game();

protected:
	virtual void DestroyResources();
	virtual void Draw();
	virtual void EndFrame();
	virtual void Initialize();
	virtual void PrepareFrame();
	virtual void PrepareResources();
	virtual void Update();
	virtual void UpdateInternal();

public:
	void Exit();
	void MessageHandler();
	void RestoreTargets();
	void Run();

private:

	void CreateBackBuffer(HRESULT res);
	
	////////////////////////////////////
	// добавочные функции
	void FrameStats();
};