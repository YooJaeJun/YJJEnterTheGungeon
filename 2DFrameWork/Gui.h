#pragma once
class Gui
{
public:
	static LRESULT MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

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
	void ResizeScreen();

	void Update();
	void Render();
	bool FileImGui(string button, string name, string fiter, string minPath);
};