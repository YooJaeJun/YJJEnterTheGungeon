#include "framework.h"
using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Buffer> ObRect::fillVertexBuffer = nullptr;
ComPtr<ID3D11Buffer> ObRect::vertexBuffer = nullptr;

void ObRect::CreateStaticMember()
{
    StaticVertexCount::Trianglestrip() = 4;
    StaticVertexCount::Linestrip() = 5;

    shared_ptr<VertexPC[]> Vertex{ new VertexPC[StaticVertexCount::Trianglestrip()] };

    //VertexCount = 4;
    //시계방향으로 정점찍기
    Vertex[0].position.x = -0.5f;
    Vertex[0].position.y = -0.5f;
    // 방향이 채도, 스칼라가 명도(얼마나 밝은지)
    Vertex[0].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[1].position.x = -0.5f;
    Vertex[1].position.y = 0.5f;
    Vertex[1].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[2].position.x = 0.5f;
    Vertex[2].position.y = -0.5f;
    Vertex[2].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[3].position.x = 0.5f;
    Vertex[3].position.y = 0.5f;
    Vertex[3].color = Color(0.0f, 0.0f, 0.0f, 1.0f);


    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//버퍼를 읽고 쓰는 방법
        desc.ByteWidth = sizeof(VertexPC) * StaticVertexCount::Trianglestrip(); //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Vertex.get();
        //초기화 데이터의 포인터.

        //버퍼 만들기
        HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, fillVertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    Vertex.reset(new VertexPC[StaticVertexCount::Linestrip()]);
    
    //VertexCount = 5;
    Vertex[0].position.x = -0.5f;
    Vertex[0].position.y = -0.5f;
    Vertex[0].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[1].position.x = -0.5f;
    Vertex[1].position.y = 0.5f;
    Vertex[1].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[2].position.x = 0.5f;
    Vertex[2].position.y = 0.5f;
    Vertex[2].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[3].position.x = 0.5f;
    Vertex[3].position.y = -0.5f;
    Vertex[3].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[4].position.x = -0.5f;
    Vertex[4].position.y = -0.5f;
    Vertex[4].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//버퍼를 읽고 쓰는 방법
        desc.ByteWidth = sizeof(VertexPC) * StaticVertexCount::Linestrip(); //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Vertex.get();
        //초기화 데이터의 포인터.

        //버퍼 만들기
        HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}

ObRect::ObRect()
{
    collider = Collider::rect;
}

void ObRect::Render()
{
    if (!isVisible)return;
    GameObject::Render();

    basicShader->Set();

    UINT stride = sizeof(VertexPC);
    UINT offset = 0;

    if (isFilled)
    {
        D3D.GetDC()->IASetVertexBuffers(0,
            1,
            fillVertexBuffer.GetAddressOf(),
            &stride,
            &offset);
        D3D.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);   // 그릴 모양
        D3D.GetDC()->Draw(StaticVertexCount::Trianglestrip(), 0);
    }
    else
    {
        D3D.GetDC()->IASetVertexBuffers(0,//입력슬롯(16~32개까지 설정가능)
            1,//입력슬롯에 붙이고자 하는 버퍼의 갯수
            vertexBuffer.GetAddressOf(),
            &stride,//정점버퍼의 한 원소의 바이트단위 크기
            &offset);
        D3D.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        D3D.GetDC()->Draw(StaticVertexCount::Linestrip(), 0);
    }
}

const Vector2 ObRect::lb() const
{
    return Vector2(GetWorldPos().x - scale.x / 2.0f, GetWorldPos().y - scale.y / 2.0f);
}

const Vector2 ObRect::rb() const
{
    return Vector2(GetWorldPos().x + scale.x / 2.0f, GetWorldPos().y - scale.y / 2.0f);
}

const Vector2 ObRect::lt() const
{
    return Vector2(GetWorldPos().x - scale.x / 2.0f, GetWorldPos().y + scale.y / 2.0f);
}

const Vector2 ObRect::rt() const
{
    return Vector2(GetWorldPos().x + scale.x / 2.0f, GetWorldPos().y + scale.y / 2.0f);
}



