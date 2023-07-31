#include "framework.h"
using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Buffer> ObTriangle::fillVertexBuffer = nullptr;
ComPtr<ID3D11Buffer> ObTriangle::vertexBuffer = nullptr;

void ObTriangle::CreateStaticMember()
{
    StaticVertexCount::Trianglestrip() = 3;
    StaticVertexCount::Linestrip() = 3;

    shared_ptr<VertexPC[]> Vertex{ new VertexPC[StaticVertexCount::Trianglestrip()] };
    
    Vertex[0].position.x = 0.0f;
    Vertex[0].position.y = 0.5f;
    Vertex[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Vertex[1].position.x = -0.5f;
    Vertex[1].position.y = -0.4f;
    Vertex[1].color = Color(0.3f, 0.3f, 0.3f, 1.0f);

    Vertex[2].position.x = 0.5f;
    Vertex[2].position.y = -0.4f;
    Vertex[2].color = Color(0.6f, 0.6f, 0.6f, 1.0f);


    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//���۸� �а� ���� ���
        desc.ByteWidth = sizeof(VertexPC) * StaticVertexCount::Trianglestrip(); //���� ũ�� (����Ʈ)�Դϴ�.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���۰� ������ ���ο� ���ε��Ǵ� ����� �ĺ��Ͻʽÿ�

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //���� ���ҽ��� �ʱ�ȭ�ϱ����� �����͸� �����մϴ�.
        data.pSysMem = Vertex.get();
        //�ʱ�ȭ �������� ������.

        //���� �����
        const HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, fillVertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    Vertex.reset(new VertexPC[StaticVertexCount::Linestrip()]);
    
    Vertex[0].position.x = 0.0f;
    Vertex[0].position.y = 0.5f;
    Vertex[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Vertex[1].position.x = -0.5f;
    Vertex[1].position.y = -0.4f;
    Vertex[1].color = Color(0.3f, 0.3f, 0.3f, 1.0f);

    Vertex[2].position.x = 0.5f;
    Vertex[2].position.y = -0.4f;
    Vertex[2].color = Color(0.6f, 0.6f, 0.6f, 1.0f);

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
        HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}

ObTriangle::ObTriangle()
{
    collider = Collider::triangle;
}

void ObTriangle::Render()
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
        D3D.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);   // �׸� ���
        D3D.GetDC()->Draw(StaticVertexCount::Trianglestrip(), 0);
    }
    else
    {
        D3D.GetDC()->IASetVertexBuffers(0,//�Է½���(16~32������ ��������)
            1,//�Է½��Կ� ���̰��� �ϴ� ������ ����
            vertexBuffer.GetAddressOf(),
            &stride,//���������� �� ������ ����Ʈ���� ũ��
            &offset);
        D3D.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        D3D.GetDC()->Draw(StaticVertexCount::Linestrip(), 0);
    }
}


ObTriangle::ObTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3)
    : a(v1), b(v2), c(v3), isBad(false)
{}

ObTriangle::ObTriangle(const Vec2f& n1, const Vec2f& n2, const Vec2f& n3)
    : a(Vector2(n1.x, n1.y)), b(Vector2(n2.x, n2.y)), c(Vector2(n3.x, n3.y)), isBad(false)
{
}

bool ObTriangle::operator==(const ObTriangle& t) const
{
    return	(this->a == t.a || this->a == t.b || this->a == t.c) &&
        (this->b == t.a || this->b == t.b || this->b == t.c) &&
        (this->c == t.a || this->c == t.b || this->c == t.c);
}

bool ObTriangle::ContainsVertex(const Vec2f& v)
{
    return a.NearlyEqualVector2Member(v) ||
        b.NearlyEqualVector2Member(v) ||
        c.NearlyEqualVector2Member(v);
}

bool ObTriangle::CircumCircleContains(const Vec2f& n) const
{
	const Vector2 va = Vector2(a.x, a.y);
    const Vector2 vb = Vector2(b.x, b.y);
    const Vector2 vc = Vector2(c.x, c.y);
    const long double dA = va.LengthSquared();
    const long double dB = vb.LengthSquared();
    const long double dC = vc.LengthSquared();

    const long double ax = a.x;
    const long double ay = a.y;
    const long double bx = b.x;
    const long double by = b.y;
    const long double cx = c.x;
    const long double cy = c.y;

    const long double circum_x = 
        (dA * (cy - by) + dB * (ay - cy) + dC * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
    const long double circum_y = 
        (dA * (cx - bx) + dB * (ax - cx) + dC * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

	const Vector2 circum(static_cast<float>(circum_x / 2), static_cast<float>(circum_y / 2));
	const float circum_radius = Vector2::Distance(va, circum);
	const Vector2 v = Vector2(n.x, n.y);
	const float dist = Vector2::Distance(v, circum);

    return dist <= circum_radius;
}

bool ObTriangle::AlmostEqualTriangle(const ObTriangle& other)
{
    return	(a.NearlyEqualVector2Member(other.a) || a.NearlyEqualVector2Member(other.b) || a.NearlyEqualVector2Member(other.c)) &&
        (b.NearlyEqualVector2Member(other.a) || b.NearlyEqualVector2Member(other.b) || b.NearlyEqualVector2Member(other.c)) &&
        (c.NearlyEqualVector2Member(other.a) || c.NearlyEqualVector2Member(other.b) || c.NearlyEqualVector2Member(other.c));
}
