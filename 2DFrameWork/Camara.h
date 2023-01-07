#pragma once
class Camera : public Singleton<Camera>
{
private:
    Viewport            viewport;       //뷰포트
    Matrix              V, P, VP, S;       //View x Proj

public:
    //카메라위치
    Vector2             position;
    Vector3             zoomFactor;

public:
    Camera();
    void          Set();
    void          Zoom(const float zDelta);
    void          ResizeScreen();
    const Matrix& GetP() { return P; };
    const Matrix& GetVP() { return VP; };
};