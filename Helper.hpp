#pragma once

#include<iostream>
#include<fstream>

#include<SFML/Graphics.hpp>

#define toFloat(a) static_cast<float>(a)
#define toInt(a) static_cast<int>(a)
#define toUInt(a) static_cast<unsigned int>(a)
#define toUInt8(a) static_cast<unsigned char>(a)

sf::Text& FillText(sf::Text& text, const sf::Font& font, const sf::Color& color, float scaleX, float scaleY)
{
	text.setFont(font);
	text.setFillColor(color);
	text.setScale(scaleX, scaleY);
	return text;
}

sf::Uint32 ToUInt32(int c)
{
	if (c >= 144 && c <= 191)
		return c + 896;
	if (c >= 128 && c <= 143)
		return c + 960;
	//if (c == 129)
	//	return 1105;
	//if (c == 145)
	//	return 1025;
	return c;
}

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

void getlineToSpecialSymbol(std::ifstream& read, sf::String& str)
{
	str.clear();
	if (!read.is_open())
		return;
	int pr;
	do
	{
		pr = read.get();
		if (pr == 208 || pr == 209)
			continue;
		str += ToUInt32(pr);
	} while (pr != '|' && pr != '\n' && !read.eof());
	str.erase(str.getSize() - 1);
}