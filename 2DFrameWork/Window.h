#pragma once
class Window
{
	//static
private:
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
	Window();

public:
	static Window& GetInstance()
	{
		static std::unique_ptr<Window> inst;
		if (!inst)
			inst = std::unique_ptr<Window>(new Window());
		return *inst;
	}

	//member
private:
	static std::shared_ptr<Scene> main;

public:
	void	Create();
	void	Destroy();
	WPARAM	Run(std::shared_ptr<Scene> main);
	void	Load();
	void	Save();
};

