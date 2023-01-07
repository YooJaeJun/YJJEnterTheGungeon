#include "framework.h"

ID3D11Buffer* ObLine::vertexBuffer = nullptr;

void ObLine::CreateStaticMember()
{
    StaticVertexCount::Linestrip() = 2;

    VertexPC* Vertex = new VertexPC[StaticVertexCount::Linestrip()];

    Vertex[0].position.x = -0.5f;
    Vertex[0].position.y = 0.0f;
    Vertex[0].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    Vertex[1].position.x = 0.5f;
    Vertex[1].position.y = 0.0f;
    Vertex[1].color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//버퍼를 읽고 쓰는 방법
        desc.ByteWidth = sizeof(VertexPC) * StaticVertexCount::Linestrip(); //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Vertex;
        //초기화 데이터의 포인터.

        //버퍼 만들기
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        assert(SUCCEEDED(hr));
    }

    delete[] Vertex;
}

void ObLine::DeleteStaticMember()
{
    vertexBuffer->Release();
}


void ObLine::Render()
{
    if (!isVisible)return;
    GameObject::Render();

    basicShader->Set();

    UINT stride = sizeof(VertexPC);
    UINT offset = 0;
    //정점의 도형서술
    D3D->GetDC()->IASetPrimitiveTopology
        //(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        (D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    D3D->GetDC()->IASetVertexBuffers(0,//입력슬롯(16~32개까지 설정가능)
        1,//입력슬롯에 붙이고자 하는 버퍼의 갯수
        &vertexBuffer,
        &stride,//정점버퍼의 한 원소의 바이트단위 크기
        &offset);

    D3D->GetDC()->Draw(StaticVertexCount::Linestrip(), 0);
}


ObLine::ObLine()
    : v(ObNode(0.0f, 0.0f)), w(ObNode(0.0f, 0.0f))
{
    pivot = OFFSET_L;
}

ObLine::ObLine(const Vector2& v1, const Vector2& v2)
    : index(0), v(v1), w(v2)
{
    Init(v1, v2);
}

ObLine::ObLine(const int index, const Vector2& v1, const Vector2& v2)
    : index(index), v(v1), w(v2)
{
    Init(v1, v2);
}

ObLine::ObLine(const ObNode& v1, const ObNode& v2)
    : index(0), v(v1), w(v2)
{
    Init();
}

ObLine::ObLine(const int index, const ObNode& v1, const ObNode& v2)
    : index(index), v(v1), w(v2)
{
    Init();
}

void ObLine::Init()
{
    Vector2 vv = Vector2(v.x, v.y);
    Vector2 vw = Vector2(w.x, w.y);
    Init(vv, vw);
}

void ObLine::Init(const Vector2& v1, const Vector2& v2)
{
    pivot = OFFSET_L;
    SetWorldPos(v1);
    rotation = Utility::DirToRadian(v2 - v1);
    scale.x = v1.Distance(v1, v2);
}

void ObLine::SetV(const ObNode& other)
{
    v = other;
    Init();
}

void ObLine::SetW(const ObNode& other)
{
    w = other;
    Init();
}

void ObLine::SetVIdx(const int other)
{
    v.index = other;
}

void ObLine::SetWIdx(const int other)
{
    w.index = other;
}

bool ObLine::operator<(const ObLine& other) const
{
    return ObNode(v) < ObNode(other.v) ||
        (!(ObNode(other.v) < ObNode(v)) && ObNode(w) < ObNode(other.w));
    // return (v.x < other.v.x || (!(other.v.x < v.x) && v.y < other.v.y)) ||
    //     (w.x < other.w.x || (!(other.w.x < w.x) && w.y < other.w.y));
}

bool ObLine::operator>(const ObLine& other) const
{
    return scale.x > other.scale.x;
}

ObLine& ObLine::operator=(const ObLine& other)
{
    index = other.index;
    v.x = other.v.x;
    v.y = other.v.y;
    w.x = other.w.x;
    w.y = other.w.y;
    Vector2 vv = Vector2(v.x, v.y);
    Vector2 vw = Vector2(w.x, w.y);
    SetWorldPos(vv);
    rotation = Utility::DirToRadian(vw - vv);
    scale.x = vv.Distance(vv, vw);
    return *this;
}

bool ObLine::operator==(const ObLine& ohter) const
{
    return (v == ohter.v && w == ohter.w) ||
        (v == ohter.w && w == ohter.v);
}

bool ObLine::almostEqualLine(const ObLine& other)
{
    return	(v.almostEqualNode(other.v) && w.almostEqualNode(other.w)) ||
        (v.almostEqualNode(other.w) && w.almostEqualNode(other.v));
}