#include "precomp.h"
#include "Object.h"

Object::Object(int2 coordinates, int2 dimensions)
{
	m_dimensions = dimensions;
	m_coord = coordinates;
	m_center.x = m_coord.x + m_dimensions.x / 2;
	m_center.y = m_coord.y + m_dimensions.y / 2;
}


int2 Object::GetCoordinates() const
{
	return m_coord;
}
void Object::SetCoordinates(int2 coordinates, const int2 moveMap)
{
	m_coord = coordinates - moveMap;
	m_center.x = m_coord.x + m_dimensions.x / 2;
	m_center.y = m_coord.y + m_dimensions.y / 2;
}


int2 Object::GetDimensions() const
{
	return m_dimensions;
}
void Object::SetDimensions(int2 dimensions)
{
	m_dimensions = dimensions;
}



bool Object::Collides(const Object& object, int2 offset) const
{
	return m_coord.x + offset.x < object.GetCoordinates().x + object.GetDimensions().x
		&& m_coord.x + offset.x + m_dimensions.x > object.GetCoordinates().x
		&& m_coord.y + offset.y < object.GetCoordinates().y + object.GetDimensions().y
		&& m_coord.y + offset.y + m_dimensions.y > object.GetCoordinates().y;

	//Old version, a lot more cumbersome
	/*return (((m_coord.x + offset.x >= object.GetCoordinates().x && m_coord.x + offset.x <= object.GetCoordinates().x + object.GetDimensions().x) ||
		(m_coord.x + offset.x + m_dimensions.x >= object.GetCoordinates().x && m_coord.x + offset.x + m_dimensions.x <= object.GetCoordinates().x + object.GetDimensions().x)) &&
		((m_coord.y + offset.y >= object.GetCoordinates().y && m_coord.y + offset.y <= object.GetCoordinates().y + object.GetDimensions().y) ||
			(m_coord.y + offset.y + m_dimensions.y >= object.GetCoordinates().y && m_coord.y + offset.y + m_dimensions.y <= object.GetCoordinates().y + object.GetDimensions().y))) ||
		(((m_coord.x + offset.x <= object.GetCoordinates().x && m_coord.x + offset.x + m_dimensions.x >= object.GetCoordinates().x) ||
			(m_coord.x + offset.x <= object.GetCoordinates().x + object.GetDimensions().x && m_coord.x + offset.x + m_dimensions.x >= object.GetCoordinates().x + object.GetDimensions().x)) &&
			((m_coord.y + offset.y <= object.GetCoordinates().y && m_coord.y + offset.y + m_dimensions.y >= object.GetCoordinates().y) ||
			(m_coord.y + offset.y <= object.GetCoordinates().y + object.GetDimensions().y && m_coord.y + offset.y + m_dimensions.y >= object.GetCoordinates().y + object.GetDimensions().y)));*/

	/*((m_coord.x <= object.GetCoordinates().x && m_coord.x + m_dimensions.x >= object.GetCoordinates().x + object.GetDimensions().x) &&
		((m_coord.y <= object.GetCoordinates().y && m_coord.y + m_dimensions.y >= object.GetCoordinates().y) ||
			(m_coord.y <= object.GetCoordinates().y + object.GetDimensions().y && m_coord.y + m_dimensions.y >= object.GetCoordinates().y + object.GetDimensions().y)));*/
}

void Object::DrawObject(Tmpl8::Surface* screen, Tmpl8::Sprite* sprite, const int2* mapOffset)
{
	sprite->Draw(screen, m_coord.x, m_coord.y);
}