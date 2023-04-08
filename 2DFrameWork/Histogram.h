#pragma once

class Histogram
{
private:
	Histogram() {}

public:
	static Histogram& GetInstance()
	{
		static std::unique_ptr<Histogram> inst;
		if (!inst)
			inst = std::unique_ptr<Histogram>(new Histogram());
		return *inst; 
	}

public:
	~Histogram() {}

public:
	void FindMaximalRectangleForStack(const int xStart, const int xEnd, const int yStart, const int yEnd,
		const int xSize, const int ySize, 
		OUT int& xMax, OUT int& yMax, OUT Vec2i& propSour, OUT Vec2i& propDest);
};

