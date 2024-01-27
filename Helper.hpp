#pragma once

#include<iostream>
#include<fstream>

#include<SFML/Graphics.hpp>

#include<SFMLmyplus/out.hpp>
using namespace out;

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

sf::Text& FillText(sf::Text& text, const sf::Font& font, const sf::Color& color, float scaleX = 1.f, float scaleY = 1.f)
{
	text.setFont(font);
	text.setFillColor(color);
	text.setScale(scaleX, scaleY);
	return text;
}

sf::Text& FillText(sf::Text& text, const sf::Font& font, const sf::String& str, const sf::Color& color, const sf::Vector2f& pos = {0.f,0.f}, const sf::Vector2f& scale = { 1.f,1.f }, unsigned size = 40, float outlineThiscknes = 3)
{
	text.setFont(font);
	text.setString(str);
	text.setFillColor(color);
	text.setPosition(pos);
	text.setScale(scale.x, scale.y);
	text.setCharacterSize(size);
	text.setOutlineThickness(outlineThiscknes);
	return text;
}

