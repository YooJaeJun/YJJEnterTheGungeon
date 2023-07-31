#include "framework.h"
using Microsoft::WRL::ComPtr;

DWrite::DWrite()
{
	HRESULT hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf())
	);
    assert(SUCCEEDED(hr));


    D2D1_FACTORY_OPTIONS option;
    option.debugLevel = D2D1_DEBUG_LEVEL_NONE;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, factory.GetAddressOf());
    assert(SUCCEEDED(hr));

    ComPtr<IDXGIDevice> dxgiDevice;
    hr = D3D.GetDevice()->QueryInterface(dxgiDevice.GetAddressOf());
    assert(SUCCEEDED(hr));

    hr = factory->CreateDevice(dxgiDevice.Get(), device.GetAddressOf());
    assert(SUCCEEDED(hr));

    hr = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, deviceContext.GetAddressOf());
    assert(SUCCEEDED(hr));


    ComPtr<IDXGISurface> dxgiSurface;
    hr = D3D.GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(dxgiSurface.GetAddressOf()));
    assert(SUCCEEDED(hr));


    D2D1_BITMAP_PROPERTIES1 bp;
    bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    bp.dpiX = 96;
    bp.dpiY = 96;
    bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    bp.colorContext = nullptr;

    hr = deviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bp, targetBitmap.GetAddressOf());
    assert(SUCCEEDED(hr));
    deviceContext->SetTarget(targetBitmap.Get());
}

void DWrite::CreateBackBuffer(float width, float height)
{
	HRESULT hr = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
	                                         deviceContext.GetAddressOf());
    assert(SUCCEEDED(hr));

    ComPtr<IDXGISurface> dxgiSurface;
    hr = D3D.GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(dxgiSurface.GetAddressOf()));
    assert(SUCCEEDED(hr));


    D2D1_BITMAP_PROPERTIES1 bp;
    bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    bp.dpiX = 96;
    bp.dpiY = 96;
    bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    bp.colorContext = nullptr;

    hr = deviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bp, targetBitmap.GetAddressOf());
    assert(SUCCEEDED(hr));
    //Safe
    dxgiSurface.Reset();

    deviceContext->SetTarget(targetBitmap.Get());
}

void DWrite::DeleteBackBuffer()
{
    targetBitmap.Reset();
    deviceContext.Reset();
}

void DWrite::RenderText(
	const wstring text, 
    const RECT rect,
    const float size,
    const wstring font,
	const Color color,
	const DWRITE_FONT_WEIGHT weight,
	const DWRITE_FONT_STYLE style,
	const DWRITE_FONT_STRETCH stretch)
{
    FontBrushDesc brushDesc;
    brushDesc.color = color;

    shared_ptr<FontBrushDesc> findBrush = nullptr;

    for (FontBrushDesc& desc : fontBrush)
    {
        if (desc == brushDesc)
        {
            findBrush = make_shared<FontBrushDesc>(desc);
            break;
        }
    }

    //벡터안에 해당 브러시가 없다면
    if (findBrush == nullptr)
    {
	    const D2D1::ColorF colorF = D2D1::ColorF(color.x, color.y, color.z);
        deviceContext->CreateSolidColorBrush(colorF, brushDesc.brush.GetAddressOf());

        //해당 브러시를 벡터에 추가(기존에 존재하지 않기때문에)
        fontBrush.push_back(brushDesc);
        findBrush = make_shared<FontBrushDesc>(brushDesc);
    }

    //매개변수 폰트값 정의
    FontTextDesc textDesc;
    textDesc.font = font;
    textDesc.fontSize = size;
    textDesc.stretch = stretch;
    textDesc.style = style;
    textDesc.weight = weight;

    shared_ptr<FontTextDesc> findText = nullptr;
    //벡터의 사이즈만큼 반복

    for (UINT i = 0; i < fontText.size(); i++)
    {
        //만약 기존 텍스트가 존재 할때
        if (fontText[i] == textDesc)
        {
            //findText = &desc;
            findText = make_shared<FontTextDesc>(fontText[i]);
            break;
        }
    }
    //없다면 추가
    if (findText == nullptr)
    {
        writeFactory->CreateTextFormat
        (
            textDesc.font.c_str(), nullptr, textDesc.weight, textDesc.style, textDesc.stretch, textDesc.fontSize,
            L"ko", textDesc.format.GetAddressOf()
        );

        fontText.push_back(textDesc);
        findText = make_shared<FontTextDesc>(textDesc);
    }

    D2D1_RECT_F rectF;
    rectF.left = static_cast<float>(rect.left);
    rectF.right = static_cast<float>(rect.right);
    rectF.top = static_cast<float>(rect.top);
    rectF.bottom = static_cast<float>(rect.bottom);


    deviceContext->DrawTextW(
        text.c_str(), 
        static_cast<UINT32>(text.size()), 
        findText->format.Get(), 
        rectF, 
        findBrush->brush.Get()
    );
}
