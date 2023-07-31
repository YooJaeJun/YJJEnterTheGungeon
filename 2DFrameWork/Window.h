#pragma once
class Window
{
	//static
private:
	static LRESULT CALLBACK WndProc(const HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam);

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
	static void	Destroy();
	WPARAM	Run(std::shared_ptr<Scene> main);
	static void	Load();
	static void	Save();
};

