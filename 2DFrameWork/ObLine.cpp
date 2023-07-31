#include "framework.h"
using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Buffer> ObLine::vertexBuffer = nullptr;

void ObLine::CreateStaticMember()
{
    StaticVertexCount::Linestrip() = 2;

    const shared_ptr<VertexPC[]> Vertex{ new VertexPC[StaticVertexCount::Linestrip()] };

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
        desc.Usage = D3D11_USAGE_DEFAULT;//���۸� �а� ���� ���
        desc.ByteWidth = sizeof(VertexPC) * StaticVertexCount::Linestrip(); //���� ũ�� (����Ʈ)�Դϴ�.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���۰� ������ ���ο� ���ε��Ǵ� ����� �ĺ��Ͻʽÿ�

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //���� ���ҽ��� �ʱ�ȭ�ϱ����� �����͸� �����մϴ�.
        data.pSysMem = Vertex.get();
        //�ʱ�ȭ �������� ������.

        //���� �����
        const HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}


void ObLine::Render()
{
    if (!isVisible)return;
    GameObject::Render();

    basicShader->Set();

    constexpr UINT stride = sizeof(VertexPC);
    constexpr UINT offset = 0;
    //������ ��������
    D3D.GetDC()->IASetPrimitiveTopology
        //(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        (D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    D3D.GetDC()->IASetVertexBuffers(0,//�Է½���(16~32������ ��������)
        1,//�Է½��Կ� ���̰��� �ϴ� ������ ����
        vertexBuffer.GetAddressOf(),
        &stride,//���������� �� ������ ����Ʈ���� ũ��
        &offset);

    D3D.GetDC()->Draw(StaticVertexCount::Linestrip(), 0);
}


ObLine::ObLine()
    : v(Vec2f(0.0f, 0.0f)), w(Vec2f(0.0f, 0.0f))
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

ObLine::ObLine(const Vec2f& v1, const Vec2f& v2)
    : index(0), v(v1), w(v2)
{
    Init();
}

ObLine::ObLine(const int index, const Vec2f& v1, const Vec2f& v2)
    : index(index), v(v1), w(v2)
{
    Init();
}

void ObLine::Init()
{
	const Vector2 vv = Vector2(v.x, v.y);
	const Vector2 vw = Vector2(w.x, w.y);
    Init(vv, vw);
}

void ObLine::Init(const Vector2& v1, const Vector2& v2)
{
    pivot = OFFSET_L;
    SetWorldPos(v1);
    rotation = Utility::DirToRadian(v2 - v1);
    scale.x = v1.Distance(v1, v2);
}

void ObLine::SetV(const Vec2f& other)
{
    v = other;
    Init();
}

void ObLine::SetW(const Vec2f& other)
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
    return Vec2f(v) < Vec2f(other.v) ||
        (!(Vec2f(other.v) < Vec2f(v)) && Vec2f(w) < Vec2f(other.w));
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
    const Vector2 vv = Vector2(v.x, v.y);
    const Vector2 vw = Vector2(w.x, w.y);
    SetWorldPos(vv);
    rotation = Utility::DirToRadian(vw - vv);
    scale.x = vv.Distance(vv, vw);
    return *this;
}

bool ObLine::operator==(const ObLine& other) const
{
    return (v == other.v && w == other.w) ||
        (v == other.w && w == other.v);
}

bool ObLine::NearlyEqualLine(const ObLine& other)
{
    return	(v.NearlyEqualVector2Member(other.v) && w.NearlyEqualVector2Member(other.w)) ||
        (v.NearlyEqualVector2Member(other.w) && w.NearlyEqualVector2Member(other.v));
}