#include "framework.h"
using Microsoft::WRL::ComPtr;

unique_ptr<ObLine> GameObject::axisObject = nullptr;
ComPtr<ID3D11Buffer> GameObject::WVPBuffer = nullptr;
ComPtr<ID3D11Buffer> GameObject::colorBuffer = nullptr;
unique_ptr<Shader> GameObject::basicShader = nullptr;
unique_ptr<Shader> GameObject::imageShader = nullptr;
unique_ptr<Shader> GameObject::tileMapShader = nullptr;

void GameObject::CreateStaticMember()
{
	//CreateConstantBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Matrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, NULL, WVPBuffer.GetAddressOf());	// WVPBuffer �������
		assert(SUCCEEDED(hr));
	}
	D3D.GetDC()->VSSetConstantBuffers(0, 1, WVPBuffer.GetAddressOf());

	//CreateConstantBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Color);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //�������
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, NULL, colorBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	D3D.GetDC()->VSSetConstantBuffers(1, 1, colorBuffer.GetAddressOf());

	basicShader = make_unique<Shader>(L"1.Basic");
	imageShader = make_unique<Shader>(L"2.Image");
	tileMapShader = make_unique<Shader>(L"3.TileMap");
	axisObject = make_unique<ObLine>();
}


GameObject::GameObject()
{
	P = nullptr;
	position.x = 0;
	position.y = 0;
	isVisible = true;
	isFilled = true;
	scale.x = 1.0f;
	scale.y = 1.0f;
	rotation = 0;
	rotationX = 0;
	rotationY = 0;
	rotationRelative = 0;
	color = Color(0.5, 0.5, 0.5, 0.5);
	isAxis = false;
	pivot = OFFSET_N;
	space = Space::world;
	colOnOff = true;
}

void GameObject::Update()
{
	Pi = Matrix::CreateTranslation(pivot.x, pivot.y, 0.0f);
	S = Matrix::CreateScale(scale.x, scale.y, 1.0f);
	R = Matrix::CreateRotationZ(rotation);
	// RX = Matrix::CreateRotationX(rotationX);
	// RY = Matrix::CreateRotationY(rotationY);
	T = Matrix::CreateTranslation(position.x, position.y, 0.0f);
	R2 = Matrix::CreateRotationZ(rotationRelative);

	// ����ȭ �̽� ������ ������ reverseLR�� ��ü
	// RT = R * RX * RY * T * R2;
	RT = R * T * R2;

	//P�� �ּҰ� ������
	if (P)
		RT *= *P;

	W = Pi * S * RT;
}

void GameObject::Update(const bool notRotation)
{
	Pi = Matrix::CreateTranslation(pivot.x, pivot.y, 0.0f);
	S = Matrix::CreateScale(scale.x, scale.y, 1.0f);
	T = Matrix::CreateTranslation(position.x, position.y, 0.0f);

	RT = T;

	W = Pi * S * RT;
}

void GameObject::Render()
{
	if (isAxis)
	{
		//right
		axisObject->SetWorldPos(GetWorldPivot());	// GetWorldPivot() ����. ������� W���� ������. RT���� �������� �� GetWorldPos()
		axisObject->rotation = Utility::DirToRadian(GetRight());
		axisObject->scale.x = scale.x * 2.0f;
		axisObject->color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		axisObject->Update();
		axisObject->Render();
		//up
		axisObject->rotation = Utility::DirToRadian(GetUp());
		axisObject->scale.x = scale.y * 2.0f;
		axisObject->color = Color(0.0f, 1.0f, 0.0f, 1.0f);
		axisObject->Update();
		axisObject->Render();
	}

	switch (space)
	{
	case Space::world:
		WVP = W * CAM.GetVP();		// V: ī�޶� ������ �޴´�.
		break;
	case Space::screen:
		WVP = W * CAM.GetP();
		break;
	}

	WVP = WVP.Transpose();	// ��ġ���: �� �켱���� �� �켱���� ��������. GPU ������ �� �켱�� �� ����
	{
		// subresource�� map �Լ��� �̿�. CPU�� GPU�� �ߺ� ������ ����
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D.GetDC()->Map(WVPBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Matrix), &WVP, sizeof(Matrix));
		D3D.GetDC()->Unmap(WVPBuffer.Get(), 0);
	}
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D.GetDC()->Map(colorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(Color), &color, sizeof(Color));
		D3D.GetDC()->Unmap(colorBuffer.Get(), 0);
	}
}

ColPos GameObject::Intersect(Vector2 coord)
{
	if (not colOnOff) 
		return ColPos::none;
	
	if (collider == Collider::rect)
	{
		if (GetRight() == RIGHT) //ȸ��X			// 1�̳� 0�̳� 0.5�� �ε��Ҽ��� ���� ��� == �� ����
		{
			Utility::RECT rc(GetWorldPivot(), scale);
			return IntersectRectCoord(rc, coord);
		}
		else //ȸ��
		{
			Vector2 rcpivot = Vector2::Transform(pivot, S);		// �ǹ� �ٽ� ���� ��ġ
			Utility::RECT rc1(rcpivot, scale);

			Matrix rcInverse = RT.Invert();
			coord = Vector2::Transform(coord, rcInverse);		// ���콺�� �簢���� ü���. SRT * (1/RT). scale�� ����

			return IntersectRectCoord(rc1, coord);
		}
	
	}
	else if (collider == Collider::circle)
	{
		Utility::CIRCLE cc(GetWorldPivot(), scale);
		return Utility::IntersectCircleCoord(cc, coord);
	}
	return ColPos::none;
}

ColPos GameObject::Intersect(shared_ptr<GameObject> ob)
{
	if (colOnOff == false or ob->colOnOff == false) 
		return ColPos::none;

	if (collider == Collider::line)
	{
		Utility::LINE l(GetWorldPos(), Vector2(GetWorldPos().x + cos(rotation) * scale.x, GetWorldPos().y + sin(rotation) * scale.x));
		
		// �� ��
		if (ob->collider == Collider::line)
		{
			Utility::LINE l2(ob->GetWorldPos(), 
				Vector2(ob->GetWorldPos().x + cos(ob->rotation) * ob->scale.x,
					ob->GetWorldPos().y + sin(ob->rotation) * ob->scale.x));
			return Utility::IntersectLineLine(l, l2);
		}
		// �� �簢��
		else if (ob->collider == Collider::rect)
		{
			Utility::RECT rc(ob->GetWorldPivot(), ob->scale);
			return Utility::IntersectRectLine(rc, l);
		}
		// �� ��
		else if (ob->collider == Collider::circle)
		{
			Utility::CIRCLE cc(ob->GetWorldPivot(), ob->scale);
			return Utility::IntersectCircleLine(cc, l);
		}
	}
	else if (collider == Collider::rect)
	{
		// �簢�� ��
		if (ob->collider == Collider::line)
		{
			Utility::RECT rc(GetWorldPivot(), scale);
			Utility::LINE l(ob->GetWorldPos(),
				Vector2(ob->GetWorldPos().x + cos(ob->rotation) * ob->scale.x,
					ob->GetWorldPos().y + sin(ob->rotation) * ob->scale.x));
			return Utility::IntersectRectLine(rc, l);
		}
		// �簢�� �簢��
		else if (ob->collider == Collider::rect)
		{
			if (GetRight() == RIGHT && ob->GetRight() == RIGHT)
			{
				Utility::RECT rc1(GetWorldPivot(), scale);
				Utility::RECT rc2(ob->GetWorldPivot(), ob->scale);
				return Utility::IntersectRectRect(rc1, rc2);
			}
			else
			{
				return Utility::IntersectRectRect(make_shared<GameObject>(*this), ob);
			}

		}
		// �簢�� ��
		else if (ob->collider == Collider::circle)
		{
			if (GetRight() == RIGHT)
			{
				Utility::RECT rc1(GetWorldPivot(), scale);
				Utility::CIRCLE cc2(ob->GetWorldPivot(), ob->scale);
				return Utility::IntersectRectCircle(rc1, cc2);
			}
			else
			{
				Vector2 rc1pivot = Vector2::Transform(pivot, S);
				Utility::RECT rc1(rc1pivot, scale);
				Matrix rcInverse = RT.Invert();
				Vector2 cc2pivot = ob->GetWorldPivot();
				cc2pivot = Vector2::Transform(cc2pivot, rcInverse);
				Utility::CIRCLE cc2(cc2pivot, ob->scale);

				return IntersectRectCircle(rc1, cc2);
			}
		}
	}
	else if (collider == Collider::circle)
	{
		// �� ��
		if (ob->collider == Collider::line)
		{
			Utility::CIRCLE cc(GetWorldPivot(), scale);
			Utility::LINE l(ob->GetWorldPos(),
				Vector2(ob->GetWorldPos().x + cos(ob->rotation) * ob->scale.x,
					ob->GetWorldPos().y + sin(ob->rotation) * ob->scale.x));
			return Utility::IntersectCircleLine(cc, l);
		}
		// �� �簢��
		else if (ob->collider == Collider::rect)
		{
			if (GetRight() == RIGHT)
			{
				Utility::RECT rc1(ob->GetWorldPivot(), ob->scale);
				Utility::CIRCLE cc2(GetWorldPivot(), scale);
				return Utility::IntersectRectCircle(rc1, cc2);
			}
			else
			{
				Vector2 rc1pivot = Vector2::Transform(ob->pivot, ob->S);
				Utility::RECT rc1(rc1pivot, ob->scale);
				Matrix rcInverse = ob->RT.Invert();
				Vector2 cc2pivot = GetWorldPivot();
				cc2pivot = Vector2::Transform(cc2pivot, rcInverse);
				Utility::CIRCLE cc2(cc2pivot, scale);

				return IntersectRectCircle(rc1, cc2);
			}
		}
		// �� ��
		else if (ob->collider == Collider::circle)
		{
			Utility::CIRCLE cc1(GetWorldPivot(), scale);
			Utility::CIRCLE cc2(ob->GetWorldPivot(), ob->scale);
			return Utility::IntersectCircleCircle(cc1, cc2);
		}
	}
	return ColPos::none;
}

ColPos GameObject::IntersectScreenMouse(Vector2 coord)
{
	coord.y = app.GetHalfHeight() - coord.y;
	coord.x = coord.x - app.GetHalfWidth();
	return Intersect(coord);
}

void GameObject::SetWorldPos(Vector2 worldPos)
{
	if (!P)
		position = worldPos;
	else
	{
		Vector2 location = Vector2::Transform(worldPos, P->Invert());
		position = location;
	}
}

void GameObject::SetWorldPosX(float worldPosX)
{
	if (!P)
		position.x = worldPosX;
	else
	{
		Vector2 location = Vector2::Transform(Vector2(worldPosX, 0), P->Invert());
		position.x = location.x;
	}
}
void GameObject::SetWorldPosY(float worldPosY)
{
	if (!P)
		position.y = worldPosY;
	else
	{
		Vector2 location = Vector2::Transform(Vector2(0, worldPosY), P->Invert());
		position.y = location.y;
	}
}
void GameObject::MoveWorldPos(Vector2 velocity)
{
	if (!P)
		position += velocity;
	else
	{
		Vector2 locVelocity = Vector2::TransformNormal(velocity, P->Invert());
		position += locVelocity;
	}
}
