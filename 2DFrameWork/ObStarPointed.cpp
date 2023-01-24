#include "framework.h"
using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Buffer> ObStarPointed::fillVertexBuffer = nullptr;
ComPtr<ID3D11Buffer> ObStarPointed::vertexBuffer = nullptr;

void ObStarPointed::CreateStaticMember()
{
    StaticVertexCount::Trianglestrip() = 31;
    StaticVertexCount::Linestrip() = 6;

    shared_ptr<VertexPC[]> Vertex{ new VertexPC[StaticVertexCount::Trianglestrip()] };

    /*vertex[0] = Vector2(0.5f * cosf(0), 0.5f * sinf(0));
    vertex[1] = Vector2(0.5f * cosf(72 * ToRadian), 0.5f * sinf(72 * ToRadian));
    vertex[2] = Vector2(0.5f * cosf(144 * ToRadian), 0.5f * sinf(144 * ToRadian));
    vertex[3] = Vector2(0.5f * cosf(216 * ToRadian), 0.5f * sinf(216 * ToRadian));
    vertex[4] = Vector2(0.5f * cosf(288 * ToRadian), 0.5f * sinf(288 * ToRadian));*/

    for (int i = 0; i < StaticVertexCount::Trianglestrip(); i += 3)
    {
        Vertex[i].position.x = 0.0f;
        Vertex[i].position.y = 0.0f;
        Vertex[i].color = Color(0.0f, 0.0f, 0.0f, 1.0f);
    }
    //
    Vertex[1].position.x = Vertex[4].position.x = -0.7f;
    Vertex[1].position.y = Vertex[4].position.y = 0.4f;
    Vertex[1].color = Vertex[4].color = Color(0.1f, 0.1f, 0.1f, 1.0f);

    Vertex[2].position.x = -0.1f;
    Vertex[2].position.y = -0.15f;
    Vertex[2].color = Color(0.0f, 0.0f, 0.0f, 0.0f);

    Vertex[5].position.x = 0.1f;
    Vertex[5].position.y = 0.15f;
    Vertex[5].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    //
    Vertex[7].position.x = Vertex[10].position.x = 0.7f;
    Vertex[7].position.y = Vertex[10].position.y = 0.4f;
    Vertex[7].color = Vertex[10].color = Color(0.2f, 0.2f, 0.2f, 1.0f);

    Vertex[8].position.x = -0.1f;
    Vertex[8].position.y = 0.15f;
    Vertex[8].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[11].position.x = 0.1f;
    Vertex[11].position.y = -0.15f;
    Vertex[11].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    //
    Vertex[13].position.x = Vertex[16].position.x = -0.5f;
    Vertex[13].position.y = Vertex[16].position.y = -0.5f;
    Vertex[13].color = Vertex[16].color = Color(0.3f, 0.3f, 0.3f, 1.0f);

    Vertex[14].position.x = -0.1f;
    Vertex[14].position.y = 0.15f;
    Vertex[14].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[17].position.x = 0.1f;
    Vertex[17].position.y = -0.15f;
    Vertex[17].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    //
    Vertex[19].position.x = Vertex[22].position.x = 0.5f;
    Vertex[19].position.y = Vertex[22].position.y = -0.5f;
    Vertex[19].color = Vertex[22].color = Color(0.4f, 0.4f, 0.4f, 1.0f);

    Vertex[20].position.x = 0.1f;
    Vertex[20].position.y = 0.15f;
    Vertex[20].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[23].position.x = -0.1f;
    Vertex[23].position.y = -0.15f;
    Vertex[23].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    //
    Vertex[25].position.x = Vertex[28].position.x = 0.0f;
    Vertex[25].position.y = Vertex[28].position.y = 0.8f;
    Vertex[25].color = Vertex[28].color = Color(0.5f, 0.5f, 0.5f, 1.0f);

    Vertex[26].position.x = -0.15f;
    Vertex[26].position.y = 0.0f;
    Vertex[26].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[29].position.x = 0.15f;
    Vertex[29].position.y = 0.0f;
    Vertex[29].color = Color(0.0f, 0.0f, 0.0f, 1.0f);



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

    Vertex[0].position.x = -0.7f;
    Vertex[0].position.y = 0.4f;
    Vertex[0].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[1].position.x = 0.7f;
    Vertex[1].position.y = 0.4f;
    Vertex[1].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[2].position.x = -0.5f;
    Vertex[2].position.y = -0.5f;
    Vertex[2].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[3].position.x = 0.0f;
    Vertex[3].position.y = 0.8f;
    Vertex[3].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[4].position.x = 0.5f;
    Vertex[4].position.y = -0.5f;
    Vertex[4].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[5].position.x = -0.7f;
    Vertex[5].position.y = 0.4f;
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
        HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}

ObStarPointed::ObStarPointed()
{
    collider = Collider::star;
}

void ObStarPointed::Render()
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

