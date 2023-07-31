#pragma once

class Scene
{
public:
	virtual ~Scene() = default;;
	//√ ±‚»≠
	virtual void Init() = 0;
	//Loop
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;
	//Window Resize
	virtual void ResizeScreen() = 0;
};