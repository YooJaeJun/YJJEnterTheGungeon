#include "framework.h"

Shader::Shader(wstring file)
{
    Microsoft::WRL::ComPtr<ID3D10Blob> VsBlob;
    Microsoft::WRL::ComPtr<ID3D10Blob> PsBlob;

    const wstring path = L"../Shaders/" + file + L".hlsl";

    constexpr DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0", flags, 0, VsBlob.GetAddressOf(), nullptr);

    D3D.GetDevice()->CreateVertexShader(VsBlob->GetBufferPointer(), VsBlob->GetBufferSize(),
        nullptr, vertexShader.GetAddressOf());

    D3D11_INPUT_ELEMENT_DESC LayoutDesc[2];

    LayoutDesc[0] = { "POSITION",//시멘틱에서 정의한 자료형
    0,//시멘틱에서 정의한 인덱스 0으로 지정했기에 0 사용
    DXGI_FORMAT_R32G32B32_FLOAT,//3개의 32비트 float설정
    0,//0~15사이의 인덱스 지정, 동시에 15개까지 버퍼가 입력슬롯을 가질수있다.
    0,//이 요소가 메모리덩어리 시작지점부터 얼마나 떨어진 위치에 있는가를 알려준다.처음시작0
    D3D11_INPUT_PER_VERTEX_DATA,//넘어가기
    0 };
    // x, y, z 좌표 12바이트만 쓸 거야 라는 뜻

    if (file == L"1.Basic")
    {
        LayoutDesc[1] = 
        { "COLOR",//시멘틱에서 정의한 자료형
        0,//시멘틱에서 정의한 인덱스 0으로 지정했기에 0 사용
        DXGI_FORMAT_R32G32B32_FLOAT,//3개의 32비트 float설정
        0,//0~15사이의 인덱스 지정, 동시에 15개까지 버퍼가 입력슬롯을 가질수있다.
        12,//포지션 R32G32B32 4 ,4,4 ->12 기준 
        D3D11_INPUT_PER_VERTEX_DATA,//넘어가기
        0 };

        constexpr UINT NumElements = 2;
        // 받는 역할은 Input Layout인데, vertext Layout에 담아 쓰기
        D3D.GetDevice()->CreateInputLayout
        (
            LayoutDesc,//정점구조체배열
            NumElements,//배열원소갯수
            VsBlob->GetBufferPointer(),//정점셰이더 포인터
            VsBlob->GetBufferSize(),//셰이더크기
            vertexLayout.GetAddressOf()//입력배치를 포인터를 통해 돌려줌
        );
    }
    // 그 다음 12바이트부터 쓸 거야

    else if (file == L"2.Image")
    {
        LayoutDesc[1] = 
        { "UV",//시멘틱에서 정의한 자료형
        0,//시멘틱에서 정의한 인덱스 0으로 지정했기에 0 사용
        DXGI_FORMAT_R32G32_FLOAT,//2개의 32비트 float설정
        0,//0~15사이의 인덱스 지정, 동시에 15개까지 버퍼가 입력슬롯을 가질수있다.
        12,//포지션 R32G32B32 4 ,4,4 ->12 기준 
        D3D11_INPUT_PER_VERTEX_DATA,//넘어가기
        0 };

        constexpr UINT NumElements = 2;
        // 받는 역할은 Input Layout인데, vertext Layout에 담아 쓰기
        D3D.GetDevice()->CreateInputLayout
        (
            LayoutDesc,//정점구조체배열
            NumElements,//배열원소갯수
            VsBlob->GetBufferPointer(),//정점셰이더 포인터
            VsBlob->GetBufferSize(),//셰이더크기
            vertexLayout.GetAddressOf()//입력배치를 포인터를 통해 돌려줌
        );
    }
    
    else if (file == L"3.TileMap")
    {
        D3D11_INPUT_ELEMENT_DESC LayoutDesc[7];
        LayoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[1] = { "UV", 0,DXGI_FORMAT_R32G32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[2] = { "COLOR", 0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[3] = { "INDICES", 0,DXGI_FORMAT_R32_FLOAT, 0, 36,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[4] = { "STATE", 0,DXGI_FORMAT_R32_FLOAT, 0, 40,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[5] = { "ROOMIDX", 0,DXGI_FORMAT_R32_UINT, 0, 40,D3D11_INPUT_PER_VERTEX_DATA, 0 };
        LayoutDesc[6] = { "DIR", 0,DXGI_FORMAT_R32_UINT, 0, 40,D3D11_INPUT_PER_VERTEX_DATA, 0 };

        constexpr UINT NumElements = 7;
        // 받는 역할은 Input Layout인데, vertext Layout에 담아 쓰기
        D3D.GetDevice()->CreateInputLayout
        (
            LayoutDesc,//정점구조체배열
            NumElements,//배열원소갯수
            VsBlob->GetBufferPointer(),//정점셰이더 포인터
            VsBlob->GetBufferSize(),//셰이더크기
            vertexLayout.GetAddressOf()//입력배치를 포인터를 통해 돌려줌
        );
    }

    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0", flags, 0, PsBlob.GetAddressOf(), nullptr);

    D3D.GetDevice()->CreatePixelShader(PsBlob->GetBufferPointer(), PsBlob->GetBufferSize(),
        nullptr, pixelShader.GetAddressOf());
}

Shader::~Shader()
{
}

void Shader::Set()
{
    //파이프라인에 바인딩
    D3D.GetDC()->VSSetShader(vertexShader.Get(), nullptr, 0);
    D3D.GetDC()->PSSetShader(pixelShader.Get(), nullptr, 0);
    D3D.GetDC()->IASetInputLayout(vertexLayout.Get());
}
