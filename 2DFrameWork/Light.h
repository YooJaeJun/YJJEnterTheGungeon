#pragma once
class Light
{
private:
    struct PointLight
    {
        Vector2 screenPos;      //��ũ����ǥ  //8
        float   radius;         //������ũ��  //12
        float   select;         //���� ����    //16
        Color   lightColor;     //���� ��     // 16 + 16
        Color   outColor;       //���� �� ��
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
