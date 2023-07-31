#pragma once
//Popup Console
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 

//C
#include <Windows.h>
#include <assert.h>

//C++
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

//DirectX smart pointer

//DirectX d3d 11
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#include <wrl/client.h>

//D2D
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

//Direct Write
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

//DirectXTk 
#include "../Libs/DirectXTK/SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

//DirectXTex
#include "../Libs/DirectXTex/DirectXTex.h"
#pragma comment(lib, "../Libs/DirectXTex/DirectXTex.lib")

//Fmod
#include "../Libs/Fmod/fmod.hpp"
#pragma comment(lib, "../Libs/Fmod/fmodL_vc.lib")

//Imgui
#include "../Libs/ImGui/imgui.h"
#include "../Libs/ImGui/imgui_internal.h"
#include "../Libs/ImGui/imgui_impl_dx11.h"
#include "../Libs/ImGui/imgui_impl_win32.h"
#pragma comment(lib, "../Libs/ImGui/ImGui.lib")

//Dialog
#include "../Libs/ImGui/dirent.h"
#include "../Libs/ImGui/ImGuiFileDialog.h"
#include "../Libs/ImGui/ImGuiFileDialogConfig.h"

//Macro(Pascal)
inline void Check(HRESULT hr) { assert(SUCCEEDED(hr)); }
constexpr float ToRadian = 0.0174532f;

//Macro(WINAPI Style) -> const 변경
constexpr float PI = 3.1415926f;	//180
constexpr float DIV2PI = 1.5707963f;	//90
constexpr float DIV4PI = 0.7853981f;	//45
constexpr float DIV8PI = 0.3926991f;	//22.5
constexpr Vector2 UP    = Vector2(0.0f, 1.0f);
constexpr Vector2 DOWN  = Vector2(0.0f, -1.0f);
constexpr Vector2 LEFT  = Vector2(-1.0f, 0.0f);
constexpr Vector2 RIGHT = Vector2(1.0f, 0.0f);
constexpr Vector2 OFFSET_N      = Vector2(0.0f, 0.0f);
constexpr Vector2 OFFSET_L      = Vector2(0.5f, 0.0f);
constexpr Vector2 OFFSET_R      = Vector2(-0.5f, 0.0f);
constexpr Vector2 OFFSET_B      = Vector2(0.0f, 0.5f);
constexpr Vector2 OFFSET_T      = Vector2(0.0f, -0.5f);
constexpr Vector2 OFFSET_LB     = Vector2(0.5f, 0.5f);
constexpr Vector2 OFFSET_RB     = Vector2(-0.5f, 0.5f);
constexpr Vector2 OFFSET_LT     = Vector2(0.5f, -0.5f);
constexpr Vector2 OFFSET_RT     = Vector2(-0.5f, -0.5f);
constexpr Vector2 DEFAULTSPAWN  = Vector2(5000.0f, 5000.0f);

// enum
enum ColPos
{
    none,
    inter,
    upDown,
    leftRight,
    edge,
    contain
};

// B, L, R, LB, RB, T, LT, RT
enum DirState
{
    dirB,
    dirL,
    dirR,
    dirLB,
    dirRB,
    dirT,
    dirLT,
    dirRT,
    dirNone
};
constexpr int dx[9] = { 0,-1,1,-1,1,0,-1,1,0 };
constexpr int dy[9] = { -1,0,0,-1,-1,1,1,1,0 };


//2DFramework Header
#include "Application.h"
#include "Scene.h"
#include "Window.h"
extern Application app;//Extern Global
#include "Direct3D11.h"
#include "Gui.h"
#include "Input.h"
#include "Timer.h"
#include "Camera.h"
#include "Random.h"
#include "Texture.h"
#include "Sound.h"
#include "Light.h"
#include "SceneManager.h"
#include "DWrite.h"
#include "Numeric.h"
#include "Tile.h"

#include "VertexType.h"
#include "Shader.h"
#include "GameObject.h"
#include "Utility.h"
#include "ObLine.h"
#include "ObTriangle.h"
#include "ObRect.h"
#include "ObStar.h"
#include "ObStarPointed.h"
#include "ObCircle.h"
#include "ObImage.h"
#include "ObTileMap.h"
#include "ObIso.h"
#include "MapInfo.h"

#include "Delaunay.h"
#include "MSTPrim.h"
#include "AStar.h"
#include "Histogram.h"

//Singleton Macro
#define	D3D			Direct3D11::GetInstance()
#define	WIN			Window::GetInstance()
#define	GUI			Gui::GetInstance()
#define CAM         Camera::GetInstance()
#define INPUT		Input::GetInstance()
#define TIMER		Timer::GetInstance()
#define DELTA		Timer::GetInstance().GetDeltaTime()
#define RANDOM		Random::GetInstance()
#define TEXTURE		Texture::GetInstance()
#define SOUND		Sound::GetInstance()
#define LIGHT		Light::GetInstance()
#define SCENE       SceneManager::GetInstance()
#define DWRITE		DWrite::GetInstance()
#define MAPINFO     MapInfo::GetInstance()
#define MSTPRIM     MSTPrim::GetInstance()
#define ASTAR       AStar::GetInstance()
#define HISTOGRAM   Histogram::GetInstance()