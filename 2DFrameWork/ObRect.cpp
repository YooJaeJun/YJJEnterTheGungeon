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
    //�ð�������� �������
    Vertex[0].position.x = -0.5f;
    Vertex[0].position.y = -0.5f;
    // ������ ä��, ��Į�� ��(�󸶳� ������)
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

ObRect::ObRect()
{
    collider = Collider::rect;
}

void ObRect::Render()
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

Vector2 ObRect::lb() const
{
    return Vector2(GetWorldPos().x - scale.x / 2.0f, GetWorldPos().y - scale.y / 2.0f);
}

Vector2 ObRect::rb() const
{
    return Vector2(GetWorldPos().x + scale.x / 2.0f, GetWorldPos().y - scale.y / 2.0f);
}

Vector2 ObRect::lt() const
{
    return Vector2(GetWorldPos().x - scale.x / 2.0f, GetWorldPos().y + scale.y / 2.0f);
}

Vector2 ObRect::rt() const
{
    return Vector2(GetWorldPos().x + scale.x / 2.0f, GetWorldPos().y + scale.y / 2.0f);
}
