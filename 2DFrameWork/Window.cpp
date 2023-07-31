#include "framework.h"

Application	 app;
std::shared_ptr<Scene> Window::main = nullptr;

Window::Window()
{
}

WPARAM Window::Run(std::shared_ptr<Scene> main)
{
	Window::main = main;
	//Window->D3D.main
	Create();
	D3D.Create();
	GameObject::CreateStaticMember();
	ObLine::CreateStaticMember();
	ObTriangle::CreateStaticMember();
	ObRect::CreateStaticMember();
	ObLine::CreateStaticMember();
	ObCircle::CreateStaticMember();
	ObStar::CreateStaticMember();
	ObStarPointed::CreateStaticMember();
	ObImage::CreateStaticMember();
	main->Init();
	MSG msg = { nullptr };
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			TIMER.Chronometry(app.fixFrame);
			INPUT.Update();
			SOUND.Update();
			GUI.Update();
			TIMER.Update();
			MAPINFO.Update();
			main->Update();
			main->LateUpdate();
			CAM.Set();
			LIGHT.Set();
			D3D.SetRenderTarget();
			D3D.Clear(app.background);
			DWRITE.GetDC()->BeginDraw();
			{
				MAPINFO.Render();
				main->Render();
				GUI.Render();
			}
			DWRITE.GetDC()->EndDraw();
			D3D.Present();
		}
	}
	Save();

	Destroy();

	return msg.wParam;
}

void Window::Create()
{
	WNDCLASSEXW wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = app.instance;
	wndClass.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	wndClass.lpszClassName = app.appName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassExW(&wndClass);
	assert(wHr != 0);

	if (app.fullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = static_cast<DWORD>(app.GetWidth());
		devMode.dmPelsHeight = static_cast<DWORD>(app.GetHeight());
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}
	app.handle = CreateWindowExW
	(
		WS_EX_APPWINDOW
		, app.appName.c_str()
		, app.appName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, nullptr
		, (HMENU)nullptr
		, app.instance
		, nullptr
	);
	assert(app.handle != NULL);

	ShowWindow(app.handle, SW_SHOWNORMAL);
	SetForegroundWindow(app.handle);
	SetFocus(app.handle);

	ShowCursor(true);
	Load();
}


void Window::Load()
{
	ifstream fin;
	string file = "window.ini";
	fin.open(file.c_str(), ios::in);
	string temp;
	if (fin.is_open())
	{
		string temp;
		// fin >> temp >> app.width >> app.height;
		fin >> temp >> app.x >> app.y;
		fin.close();
	}

	RECT rect{ 0, 0, static_cast<LONG>(app.GetWidth()), static_cast<LONG>(app.GetHeight()) };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		app.handle,
		static_cast<LONG>(app.x), 
		static_cast<LONG>(app.y), 
		rect.right - rect.left, 
		rect.bottom - rect.top,
		TRUE
	);
}

void Window::Save()
{
	RECT rc;
	GetWindowRect(app.handle, &rc);
	app.x = static_cast<float>(rc.left);
	app.y = static_cast<float>(rc.top);
	ofstream fout;
	const string file = "window.ini";
	fout.open(file.c_str(), ios::out);
	if (fout.is_open())
	{
		// fout << "Size " << app.width << " " << app.height << endl;
		fout << "Pos " << app.x << " " << app.y << endl;
		fout.close();
	}
}



void Window::Destroy()
{
	if (app.fullScreen == true)
		ChangeDisplaySettings(nullptr, 0);

	DestroyWindow(app.handle);

	UnregisterClass(app.appName.c_str(), app.instance);
}


LRESULT Window::WndProc(const HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	if (Gui::MsgProc(handle, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_MOUSEMOVE:
	{
		INPUT.mouseScreenPos.x = static_cast<float>(LOWORD(lParam));
		INPUT.mouseScreenPos.y = static_cast<float>(HIWORD(lParam));
		break;
	}
	case WM_MOUSEWHEEL:
	{
		CAM.Zoom(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	}
	case WM_SIZE:
	{
		if (D3D.GetCreated())
		{
			const float width = LOWORD(lParam);
			const float height = HIWORD(lParam);
			D3D.ResizeScreen(width, height);
			CAM.ResizeScreen();
			GUI.ResizeScreen();
			if (main)
				main->ResizeScreen();
		}
		break;
	}
	case WM_CLOSE:
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;
	}//case
	default: 
		break;
	}//swith

	return DefWindowProc(handle, message, wParam, lParam);
}
