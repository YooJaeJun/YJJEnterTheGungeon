#pragma once

struct FontBrushDesc;
struct FontTextDesc;

class DWrite
{
private:
    DWrite();

public:
    static DWrite& GetInstance()
    {
        static std::unique_ptr<DWrite> inst;
        if (!inst)
            inst = std::unique_ptr<DWrite>(new DWrite());
        return *inst;
    }

public:
    ~DWrite() = default;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> GetDC() { return deviceContext; }
    //창사이즈 조절
    void CreateBackBuffer(float width, float height);
    void DeleteBackBuffer();

public:
    //UI안에서 그릴때
    void RenderText
    (
        const wstring text
        , const RECT rect
        , const float size = 20.0f
        , const wstring font = L"돋움체"
        , const Color color = Color(1, 1, 1, 1)
        , const DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL
        , const DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL
        , const DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL
    );

private:
    Microsoft::WRL::ComPtr<ID2D1Device>         device;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext>  deviceContext = nullptr;
    Microsoft::WRL::ComPtr<ID2D1Factory1>       factory;
    Microsoft::WRL::ComPtr<IDWriteFactory>      writeFactory = nullptr;
    Microsoft::WRL::ComPtr<ID2D1Bitmap1>        targetBitmap;

    vector<FontBrushDesc> fontBrush;
    vector<FontTextDesc>  fontText;
};

struct FontBrushDesc
{
    Color color;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush = nullptr;

    //연산자오버라이딩
    bool operator==(const FontBrushDesc& val) const
    {
        return color.x == val.color.x &&
            color.y == val.color.y &&
            color.z == val.color.z &&
            color.w == val.color.w;
    }
};

struct FontTextDesc
{
    wstring font;
    float fontSize;

    DWRITE_FONT_WEIGHT weight;
    DWRITE_FONT_STYLE style;
    DWRITE_FONT_STRETCH stretch;

    Microsoft::WRL::ComPtr<IDWriteTextFormat> format = nullptr;

    bool operator==(const FontTextDesc& val)
    {
        bool b = true;

        b &= font == val.font;
        b &= fontSize == val.fontSize;
        b &= weight == val.weight;
        b &= style == val.style;
        b &= stretch == val.stretch;

        return b;
    }
};
