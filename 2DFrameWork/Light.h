#pragma once
class Light
{
private:
    struct PointLight
    {
        Vector2 screenPos;      //스크린좌표  //8
        float   radius;         //반지름크기  //12
        float   select;         //조명 선택    //16
        Color   lightColor;     //조명 색     // 16 + 16
        Color   outColor;       //조명 밖 색
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> lightBuffer;

private:
    Light();

public:
    static Light& GetInstance()
    {
        static std::unique_ptr<Light> inst;
        if (!inst)
            inst = std::unique_ptr<Light>(new Light());
        return *inst;
    }

public:
    ~Light() = default;;
    void        Set() const;
    void        SetLightPos(Vector2 WPos);

public:
    PointLight light;
};
