#include "framework.h"

Input::Input(): isZoom(false)
{
	//시작주소로부터 어디까지 전부 0으로 초기화
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
}

Input::~Input()
{
}

bool Input::KeyDown(int KeyCode) const
{
    return keyMap[KeyCode] == KEY_INPUT_STATUS_DOWN;
}

bool Input::KeyPress(int KeyCode) const
{
    return keyMap[KeyCode] <= KEY_INPUT_STATUS_PRESS;
}

bool Input::KeyUp(int KeyCode) const
{
    return keyMap[KeyCode] == KEY_INPUT_STATUS_UP;
}

void Input::Update()
{
    // Screen To World
    // mouseWorldPos.x = mouseScreenPos.x - app.GetHalfWidth();
    mouseWorldPos.x = -app.GetHalfWidth() + mouseScreenPos.x;
    // mouseWorldPos.y = app.GetHeight() - mouseScreenPos.y - app.GetHalfHeight();
    // mouseWorldPos.y = app.GetHeight() - app.GetHalfHeight() - mouseScreenPos.y ;
    mouseWorldPos.y = app.GetHalfHeight() - mouseScreenPos.y;

    mouseWorldPos = mouseWorldPos / CAM.zoomFactor;
    mouseWorldPos += CAM.position;


    //메모리복사 keyOldState = keyState 
    memcpy(keyOldState, keyState, sizeof(keyOldState));

    //기존의 배열값 초기화
    ZeroMemory(keyState, sizeof(keyState));

    ZeroMemory(keyMap, sizeof(keyMap));

    //BOOL a;
    //현재 호출상태에서의 키입력값을 가져오는 함수
    if (GetKeyboardState(keyState))
    {
        for (UINT i = 0; i < 256; i++)
        {
	        const unsigned char key = keyState[i] & 0x80;
            //삼항연산자   (bool)? 1{} 0{}
            //키를 누른상태면 1, 아니면 0으로 대입
            keyState[i] = key ? 1 : 0;

	        const int oldState = keyOldState[i];
	        const int state = keyState[i];

            if (oldState == 0 && state == 1)
                keyMap[i] = KEY_INPUT_STATUS_DOWN; //이전 0, 현재 1 - KeyDown
            else if (oldState == 1 && state == 0)
                keyMap[i] = KEY_INPUT_STATUS_UP; //이전 1, 현재 0 - KeyUp
            else if (oldState == 1 && state == 1)
                keyMap[i] = KEY_INPUT_STATUS_PRESS; //이전 1, 현재 1 - KeyPress
            else
                keyMap[i] = KEY_INPUT_STATUS_NONE;
        }
    }
    // 0x80    1000  0000  이전에안눌렀고 지금 누름
    // 0x81    1000  0001  이전에도눌럿고 지금 누름
    // 0x00    0000  0000
    // 0x01    0000  0001

    //&

    // 0x80    1000  0000
}
