#pragma once

class Application
{
	friend class Direct3D11;
	friend class Window;
	friend class Gui;

	//getter�� �ΰ� ���ٰ���
private:

	float		x, y;
	float		width;
	float		height;
	HINSTANCE	instance;
	HWND		handle;
	wstring		appName;
	bool		fullScreen;

public:
	bool		vSync;
	Color		background;
	int			fixFrame;
	float		deltaScale;
	float		soundScale;

public:
	Application() :instance(nullptr), handle(nullptr), vSync(false), fullScreen(false)
		, appName(L" "), width(1280.0f), height(720.0f), background(Color(0.7f, 0.7f, 0.7f, 1.0f))
		, fixFrame(200), deltaScale(1.0f), soundScale(1.0f), x(0.0f), y(0.0f) {}
	float	GetWidth() { return width; }
	float	GetHeight() { return height; }
	float	GetHalfWidth() { return width / 2.0f; }
	float	GetHalfHeight() { return height / 2.0f; }
	void	InitWidthHeight(float width, float height) { this->width = width; this->height = height; }

	void	SetInstance(HINSTANCE instance)
	{
		if (!this->instance)
			this->instance = instance;
	}
	void	SetAppName(wstring appName)
	{
		if (this->appName == L" ")
			this->appName = appName;
	}
	//�ػ󵵺����� D3D�� �������� �Լ� �̿�!
};