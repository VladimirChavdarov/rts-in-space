#pragma once

class Object
{
public:
	Object(int2 coordinates, int2 dimensions);
	virtual int2 GetCoordinates() const;
	virtual void SetCoordinates(int2 coordinates, const int2 moveMap);

	int2 GetDimensions() const;
	void SetDimensions(int2 coordinates);

	bool Collides(const Object& object, const int2 offset = {0, 0}) const;
	virtual void DrawObject(Tmpl8::Surface* screen, Tmpl8::Sprite* sprite, const int2* mapOffset);

	int2 m_center;

protected:
	int2 m_coord;
	int2 m_dimensions;
	
	//vec m_vector;
};
