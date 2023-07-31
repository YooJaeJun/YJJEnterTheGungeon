#pragma once
class Gui
{
public:
	static LRESULT MsgProc(const HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam);

private:
	Gui();

public:
	static Gui& GetInstance()
	{
		static std::unique_ptr<Gui> inst;
		if (!inst)
			inst = std::unique_ptr<Gui>(new Gui());
		return *inst;
	}

private:
	UINT fileID;
public:
	~Gui();
	static void ResizeScreen();

	void Update();
	static void Render();
	bool FileImGui(const string button, const string name, const string fiter, const string minPath);
};