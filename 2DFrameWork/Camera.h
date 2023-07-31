#pragma once
class Camera
{
private:
    Camera();

public:
    static Camera& GetInstance()
    {
        static std::unique_ptr<Camera> inst;
        if (!inst)
            inst = std::unique_ptr<Camera>(new Camera());
        return *inst;
    }

public:
    void                Set();
    void                Zoom(const float zDelta);
    void                ResizeScreen();
    const Matrix&       GetP() const { return P; };
    const Matrix&       GetVP() const { return VP; };

private:
    Viewport            viewport;       // 뷰포트
    Matrix              V, P, VP, S;    // View x Proj

public:
    // 카메라위치
    Vector2             position;
    Vector3             zoomFactor;
};