#pragma once

// Singleton + CRTP ���� �̿�. strip Ÿ�Կ� ���� vertex ���� ������ �� �ְ� ��
// 1. �� ���� Ŭ������ �ٸ� ���� ������ ȣ���ϴ� �̸��� ����.
// 2. static����Լ�, �� static����Լ����� ��� ��� �����ϰ�.
template<typename T>
struct StaticVertexCount
{
	static int& Trianglestrip()
	{
		static int vertexCount;
		return vertexCount;
	}
	static int& Linestrip()
	{
		static int vertexCount;
		return vertexCount;
	}
};

enum class ZOrder
{
	none,
	shadow,
	weapon,
	object,
	bullet,
	effect,
	UI,
};

class ObLine;
enum class Space
{
	world,
	screen, // ����� 0,0��
};
enum class Collider
{
	none,
	line,
	triangle,
	rect,
	circle, // ����� 0,0��
	star
};

class GameObject
{
	//static member
private:
	static ID3D11Buffer*		WVPBuffer;
	static ID3D11Buffer*		colorBuffer;
	static unique_ptr<ObLine>	axisObject;

protected:
	static unique_ptr<Shader>	basicShader;
	static unique_ptr<Shader>	imageShader;
	static unique_ptr<Shader>	tileMapShader;

	//static member function
public:
	static void CreateStaticMember();
	static void DeleteStaticMember();

	//member
private:
	Matrix		S, R, T, R2, RT, Pi, WVP;
	Matrix*		P;
	Vector2		position;

protected:
	Matrix		W;

public:
	bool        isVisible;
	bool        isFilled;
	Vector2		pivot;
	Vector2		scale;
	float		rotation;
	float		rotation2;
	Matrix		RX;
	Matrix		RY;
	float		rotationX;
	float		rotationY;
	bool		isAxis;
	Color       color;
	Space		space;
	Collider	collider;
	bool		colOnOff;
	ZOrder		zOrder;

	//member function
public:
	GameObject();
	virtual ~GameObject() {};

	virtual void Update();
	virtual void Update(const bool notRotation);
	virtual void Render();
	ColPos Intersect(Vector2 coord);
	ColPos Intersect(GameObject* ob);
	ColPos IntersectScreenMouse(Vector2 coord);

	//getter setter
public:
	//��ġ �޾ƿ���
	Vector2 GetLocalPos() const	{ return position;}
	Vector2 GetWorldPos() const	{ 
		return Vector2(RT._41, RT._42); 
	}
	Vector2 GetWorldPivot() const { return Vector2(W._41, W._42); }
	//��ġ�� �����Ҷ� =
	void	SetLocalPos(Vector2 location)	{ position = location; }
	void	SetLocalPosX(float locationX)	{ position.x = locationX; }
	void	SetLocalPosY(float locationY)	{ position.y = locationY; }
	void	SetWorldPos(Vector2 worldPos);
	void	SetWorldPosX(float worldPosX);
	void	SetWorldPosY(float worldPosY);
	//�̵���ų�� +=
	void	MoveLocalPos(Vector2 velocity)	{ position += velocity; }
	void	MoveWorldPos(Vector2 velocity);
	//������� ����
	Vector2 GetRight()	{ return Vector2(RT._11, RT._12); }
	Vector2 GetUp()		{ return Vector2(RT._21, RT._22); }
	//�θ���� �����
	void	SetParentRT(GameObject& src)	{ P = &src.RT; }
	void	SetParentT(GameObject& src)	{ P = &src.T; }
	Matrix*	GetParent() { return P; }
};
