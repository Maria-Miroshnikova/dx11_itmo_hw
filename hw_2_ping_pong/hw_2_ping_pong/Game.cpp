#include "Game.h"

const int WPARAM_KEY_DOWN = 101;
const int WPARAM_KEY_UP = 102;


LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	Game* pThis;

	if (umessage == WM_NCCREATE)
	{
		pThis = static_cast<Game*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		pThis = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
	unsigned int keyCode = static_cast<unsigned int>(wparam);
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		//std::cout << "DOWN Key: " << static_cast<unsigned int>(wparam) << std::endl;

		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);

		wparam = WPARAM_KEY_DOWN;

		//return 0;
		switch (keyCode) {
		case 27:
			PostQuitMessage(0);
			return 0;
			break;
		case 38:
			PostMessage(hwnd, 38, wparam, lparam);
			break;
		case 40:
			PostMessage(hwnd, 40, wparam, lparam);
			break;
		case 87:
			//std::cout << "char key: " << static_cast<unsigned int>(wparam) << std::endl;
			PostMessage(hwnd, 69, wparam, lparam);
			break;
		case 83: {
			//std::cout << "char key: " << static_cast<unsigned int>(wparam) << std::endl;
			PostMessage(hwnd, 68, wparam, lparam);
			break;
		}
		}
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}


Game::Game(LPCWSTR name, int screenWidth, int screenHeight): Name(name) {

	Instance = GetModuleHandle(nullptr);

	Display = new DisplayWin32(name, screenHeight, screenWidth, this);
	InputDev = new InputDevice(this);

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->ClientWidth;
	swapDesc.BufferDesc.Height = Display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	CreateBackBuffer(res);
}

Game::~Game() {
	// это нужно вызывать?
	DestroyResources();

	// это нужно перетащить в дестрой ресурсес?
	for (int i = 0; i < Components.size(); ++i) {
		delete Components[i];
	}

	delete Display;
	delete InputDev;
	Context->Release();
	backBuffer->Release();
	RenderView->Release();
	SwapChain->Release();
}

void Game::CreateBackBuffer(HRESULT res) {

	res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);	// __uuidof(ID3D11Texture2D)
	res = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);
}

void Game::DestroyResources() {
	for (auto c : Components) {
		c->DestroyResources();
	}
}

void Game::Draw() {
	
	// stage 13
	// будет отрисовывать черное окно, не заметно
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
	// будет отрисовывать красное окно, мигает из-за того, что total time
	// превосходит max значение для RGB и берется по модулю
	//{ TotalTime, 0.1f, 0.1f, 1.0f };
	Context->ClearRenderTargetView(RenderView, color);

	for (auto c : Components) {
		c->Draw();
	}
}

// undefined
void Game::EndFrame() {

}

void Game::Exit() {
	DestroyResources();
}

void Game::Initialize() {
	for (auto c : Components) {
		c->Initialize();
	}
}

// как тут использовать inputdevice?
// возможно, это больше про winapp а не про dx
void Game::MessageHandler() {
	MSG msg = {};
	// Handle the windows messages.
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT) {
		isExitRequested = true;
	}

	//std::cout << "msg = " << msg.message << std::endl;
	if (msg.message == 69) {
		/*if (msg.wParam == WPARAM_KEY_DOWN)
			InputDev->AddPressedKey(Keys(87));
		else
			InputDev->RemovePressedKey(Keys(87));*/
		//std::cout << msg.wParam << std::endl;
		//std::cout << "msg = " << msg.message << std::endl;
		InputDev->AddPressedKey(Keys(87));
	}
	if (msg.message == 68) {
		//std::cout << "msg = " << msg.message << std::endl;
		InputDev->AddPressedKey(Keys(83));
	}
	if (msg.message == 40) {
		//std::cout << "msg = " << msg.message << std::endl;
		InputDev->AddPressedKey(Keys(40));
	}
	if (msg.message == 38) {
		//std::cout << "msg = " << msg.message << std::endl;
		InputDev->AddPressedKey(Keys(38));
	}
	//input
}

// undefined
void Game::PrepareFrame() {

}

// undefined
void Game::PrepareResources() {

}

// undefined
void Game::RestoreTargets() {

}

// эта штука запускает игру в бесконечном цикле, пока ее не вырубит пользователь
void Game::Run() {

	Initialize();

	isExitRequested = false;
	PrevTime = std::chrono::steady_clock::now();
	TotalTime = 0;
	FrameCount = 0;

	while (!isExitRequested) {
		// Handle the windows messages.
		MessageHandler();

		FrameStats();

		Context->ClearState();

		Context->OMSetRenderTargets(1, &RenderView, nullptr);

		Update();
		Draw();

		// ?
		Context->OMSetRenderTargets(0, nullptr, nullptr);

		SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	}
}

void Game::Update() {
	for (auto c : Components) {
		c->Update();
	}
}

// undefined
void Game::UpdateInternal() {
}

void Game::FrameStats() {
	CurTime = std::chrono::steady_clock::now();
	DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(CurTime - PrevTime).count() / 1000000.0f;
	PrevTime = CurTime;

	TotalTime += DeltaTime;
	FrameCount++;

	if (TotalTime > 1.0f) {
		float fps = FrameCount / TotalTime;

		TotalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(Display->hWnd, text);

		FrameCount = 0;
	}
}