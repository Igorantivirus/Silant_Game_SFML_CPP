#pragma once

#include<iostream>

//#include<SFML/>

#define toFloat(a) static_cast<float>(a)
#define toInt(a) static_cast<int>(a)
#define toUInt(a) static_cast<unsigned int>(a)
#define toUInt8(a) static_cast<unsigned char>(a)

template<typename type>
std::ostream& operator<<(std::ostream& out, const sf::Rect<type> val)
{
	return out << "x = " << val.left << " y = " << val.top << " width = " << val.width << " height = " << val.height;
}
template<typename type>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<type> val)
{
	return out << "x = " << val.x << " y = " << val.y;
}

template<typename type>
sf::Rect<type> operator+(const sf::Rect<type>& left, const sf::Vector2<type> right)
{
	sf::Rect<type> res = left;
	res.left += right.x;
	res.top += right.y;
	return res;
}
template<typename type>
sf::Rect<type> operator-(const sf::Rect<type>& left, const sf::Vector2<type> right)
{
	sf::Rect<type> res = left;
	res.left -= right.x;
	res.top -= right.y;
	return res;
}