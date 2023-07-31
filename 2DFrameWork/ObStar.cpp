#include "framework.h"
using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Buffer> ObStar::fillVertexBuffer = nullptr;
ComPtr<ID3D11Buffer> ObStar::vertexBuffer = nullptr;

void ObStar::CreateStaticMember()
{
    StaticVertexCount::Trianglestrip() = 15;
    StaticVertexCount::Linestrip() = 6;

    shared_ptr<VertexPC[]> Vertex{ new VertexPC[StaticVertexCount::Trianglestrip()] };

    /*vertex[0] = Vector2(0.5f * cosf(0), 0.5f * sinf(0));
    vertex[1] = Vector2(0.5f * cosf(72 * ToRadian), 0.5f * sinf(72 * ToRadian));
    vertex[2] = Vector2(0.5f * cosf(144 * ToRadian), 0.5f * sinf(144 * ToRadian));
    vertex[3] = Vector2(0.5f * cosf(216 * ToRadian), 0.5f * sinf(216 * ToRadian));
    vertex[4] = Vector2(0.5f * cosf(288 * ToRadian), 0.5f * sinf(288 * ToRadian));*/

    for (UINT i = 0; i < 5; i++)
    {
        Vertex[i * 3].position.x = 0.0f;
        Vertex[i * 3].position.y = 0.0f;
        Vertex[i * 3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

        Vertex[i * 3 + 1].position.x = cosf(i * 144 * ToRadian) * 0.5f;
        Vertex[i * 3 + 1].position.y = sinf(i * 144 * ToRadian) * 0.5f;
        Vertex[i * 3 + 1].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

        Vertex[i * 3 + 2].position.x = cosf((i + 1) * 144 * ToRadian) * 0.5f;
        Vertex[i * 3 + 2].position.y = sinf((i + 1) * 144 * ToRadian) * 0.5f;
        Vertex[i * 3 + 2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

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
        const HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, fillVertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    
    Vertex.reset(new VertexPC[StaticVertexCount::Linestrip()]);

    Vertex[0].position.x = -0.5f;
    Vertex[0].position.y = 0.2f;
    Vertex[0].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[1].position.x = 0.5f;
    Vertex[1].position.y = 0.2f;
    Vertex[1].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[2].position.x = -0.3f;
    Vertex[2].position.y = -0.4f;
    Vertex[2].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[3].position.x = 0.0f;
    Vertex[3].position.y = 0.6f;
    Vertex[3].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[4].position.x = 0.3f;
    Vertex[4].position.y = -0.4f;
    Vertex[4].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[5].position.x = -0.5f;
    Vertex[5].position.y = 0.2f;
    Vertex[5].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

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
        const HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}

ObStar::ObStar()
{
    collider = Collider::star;
}

void ObStar::Render()
{
    if (!isVisible)return;
    GameObject::Render();

    basicShader->Set();

    constexpr UINT stride = sizeof(VertexPC);
    constexpr UINT offset = 0;

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
