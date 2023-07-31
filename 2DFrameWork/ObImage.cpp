#include "framework.h"
using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Buffer> ObImage::vertexBuffer = nullptr;
ComPtr<ID3D11Buffer> ObImage::uvBuffer = nullptr;

void ObImage::CreateStaticMember()
{
    StaticVertexCount::Trianglestrip() = 4;

    shared_ptr<VertexPT[]> Vertex{ new VertexPT[StaticVertexCount::Trianglestrip()] };

    Vertex[0].position.x = -0.5f;
    Vertex[0].position.y = -0.5f;
    Vertex[0].uv = Vector2(0.0f, 1.0f);

    Vertex[1].position.x = -0.5f;
    Vertex[1].position.y = 0.5f;
    Vertex[1].uv = Vector2(0.0f, 0.0f);

    Vertex[2].position.x = 0.5f;
    Vertex[2].position.y = -0.5f;
    Vertex[2].uv = Vector2(1.0f, 1.0f);

    Vertex[3].position.x = 0.5f;
    Vertex[3].position.y = 0.5f;
    Vertex[3].uv = Vector2(1.0f, 0.0f);

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//버퍼를 읽고 쓰는 방법
        desc.ByteWidth = sizeof(VertexPT) * StaticVertexCount::Trianglestrip(); //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Vertex.get();
        //초기화 데이터의 포인터.

        //버퍼 만들기
        const HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    Vertex.reset(new VertexPT[StaticVertexCount::Linestrip()]);

    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(Vector4);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        const HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, nullptr, uvBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    D3D.GetDC()->VSSetConstantBuffers(2, 1, uvBuffer.GetAddressOf());
}

void ObImage::PlayAnim()
{
    if (maxFrame.x == 1 && maxFrame.y == 1)
        return;

    if (animState != AnimState::stop)
    {
        if (TIMER.GetTick(animTime, animInterval))
        {
            //재생간격이 지났을 때
            if (animXAxis) //가로재생
            {
                if (animState == AnimState::loop)
                {
                    frame.x++;
                    if (frame.x == maxFrame.x)
                        frame.x = 0;
                }
                else if (animState == AnimState::once)
                {
                    frame.x++;
                    if (frame.x == maxFrame.x)
                    {
                        frame.x = maxFrame.x - 1;
                        animState = AnimState::stop;
                    }
                }
                else if (animState == AnimState::reverseLoop)
                {
                    frame.x--;
                    if (frame.x == -1)
                        frame.x = maxFrame.x - 1;
                }
                else //REVERSE_ONCE
                {
                    frame.x--;
                    if (frame.x == -1)
                    {
                        frame.x = 0;
                        animState = AnimState::stop;
                    }
                }
            }
            else //세로재생
            {
                if (animState == AnimState::loop)
                {
                    frame.y++;
                    if (frame.y == maxFrame.y)
                        frame.y = 0;
                }
                else if (animState == AnimState::once)
                {
                    frame.y++;
                    if (frame.y == maxFrame.y)
                    {
                        frame.y = maxFrame.y - 1;
                        animState = AnimState::stop;
                    }
                }
                else if (animState == AnimState::reverseLoop)
                {
                    frame.y--;
                    if (frame.y == -1)
                        frame.y = maxFrame.y - 1;
                }
                else //REVERSE_ONCE
                {
                    frame.y--;
                    if (frame.y == -1)
                    {
                        frame.y = 0;
                        animState = AnimState::stop;
                    }
                }
            }
        }
    }

    if (maxFrame.x != 1)
    {
        uv.x = frame.x / static_cast<float>(maxFrame.x);
        uv.z = (frame.x + 1.0f) / static_cast<float>(maxFrame.x);
    }
    if (maxFrame.y != 1)
    {
        uv.y = frame.y / static_cast<float>(maxFrame.y);
        uv.w = (frame.y + 1.0f) / static_cast<float>(maxFrame.y);
    }
}

ObImage::ObImage(const wstring file): animState()
{
	this->file = file;

	//기본 샘플러 값
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	//하나 이상의 샘플러 만들어 두기
	D3D.GetDevice()->CreateSamplerState(&samplerDesc, sampler.GetAddressOf());

	//텍스쳐 로드
	SRV = TEXTURE.LoadTexture(file);

	//           (최소좌표)   (최대좌표)
	uv = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	animTime = 0.0f;
	animInterval = 0.0f;
	animXAxis = true;
	maxFrame = Vec2i(1, 1);
	frame = Vec2i(0, 0);
	reverseLR = false;
}

void ObImage::Render()
{
    if (!isVisible)return;
    PlayAnim();
    GameObject::Render();
    imageShader->Set();


    D3D11_MAPPED_SUBRESOURCE mappedResource;
    D3D.GetDC()->Map(uvBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (reverseLR)
    {
	    const Vector4 reUv = Vector4(uv.z, uv.y, uv.x, uv.w);
        memcpy_s(mappedResource.pData, sizeof(Vector4), &reUv, sizeof(Vector4));
    }
    else
        memcpy_s(mappedResource.pData, sizeof(Vector4), &uv, sizeof(Vector4));

    D3D.GetDC()->Unmap(uvBuffer.Get(), 0);

    constexpr UINT stride = sizeof(VertexPT);
    constexpr UINT offset = 0;

    D3D.GetDC()->PSSetShaderResources(0, 1, SRV.GetAddressOf());
    D3D.GetDC()->PSSetSamplers(0, 1, sampler.GetAddressOf());
    D3D.GetDC()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    D3D.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    D3D.GetDC()->Draw(StaticVertexCount::Trianglestrip(), 0);
}

void ObImage::ChangeAnim(const AnimState anim, const float interval, const bool xAxis)
{
    animState = anim;
    animInterval = interval;
    animXAxis = xAxis;

    if (animState == AnimState::once)
    {
        if (xAxis)
            frame.x = 0;
        else
            frame.y = 0;
    }
    else if (animState == AnimState::reverseOnce)
    {
        if (xAxis)
            frame.x = maxFrame.x - 1;
        else
            frame.y = maxFrame.y - 1;
    }
    
}

void ObImage::ChangeSampler(const D3D11_FILTER filter, const D3D11_TEXTURE_ADDRESS_MODE addressU, const D3D11_TEXTURE_ADDRESS_MODE addressV)
{
    samplerDesc.Filter = filter;
    samplerDesc.AddressU = addressU;
    samplerDesc.AddressV = addressV;
    D3D.GetDevice()->CreateSamplerState(&samplerDesc, sampler.GetAddressOf());
}
