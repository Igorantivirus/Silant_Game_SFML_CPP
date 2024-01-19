#pragma once

#include<iostream>
#include<fstream>

#include<SFML/Graphics.hpp>

#pragma region Convert

#define toFloat(a) static_cast<float>(a)
#define toInt(a) static_cast<int>(a)
#define toUInt(a) static_cast<unsigned int>(a)
#define toUInt8(a) static_cast<unsigned char>(a)

sf::Uint32 ToUInt32(int c)
{
	if (c >= 144 && c <= 191)
		return c + 896;
	if (c >= 128 && c <= 143)
		return c + 960;
	return c;
}

sf::String ToReadlName(const sf::Keyboard::Key key)
{
	if (key >= 0 && key <= 25)
		return std::string(1, 'A' + static_cast<char>(key));
	if (key >= 26 && key <= 35)
		return "Num" + std::string(1, static_cast<char>(key) - 26 + '0');
	if (key >= 75 && key <= 84)
		return "Numpad" + std::string(1, static_cast<char>(key) - 75 + '0');
	if (key >= 85 && key <= 99)
		return "F" + std::string(1, static_cast<char>(key) - 85 + '0');
	switch (key)
	{
		case sf::Keyboard::Key::Escape:		return "Escape";
		case sf::Keyboard::Key::LControl:	return "LControl";
		case sf::Keyboard::Key::LShift:		return "LShift";
		case sf::Keyboard::Key::LAlt:		return "LAlt";
		case sf::Keyboard::Key::LSystem:	return "LSystem";
		case sf::Keyboard::Key::RControl:	return "RControl";
		case sf::Keyboard::Key::RShift:		return "RShift";
		case sf::Keyboard::Key::RAlt:		return "RAlt";
		case sf::Keyboard::Key::RSystem:	return "RSystem";
		case sf::Keyboard::Key::Menu:		return "Menu";
		case sf::Keyboard::Key::LBracket:	return "LBracket";
		case sf::Keyboard::Key::RBracket:	return "RBracket";
		case sf::Keyboard::Key::Semicolon:	return "Semicolon";
		case sf::Keyboard::Key::Comma:		return "Comma";
		case sf::Keyboard::Key::Period:		return "Period";
		case sf::Keyboard::Key::Quote:		return "Quote";
		case sf::Keyboard::Key::Slash:		return "Slash";
		case sf::Keyboard::Key::Backslash:	return "Backslash";
		case sf::Keyboard::Key::Tilde:		return "Tilde";
		case sf::Keyboard::Key::Equal:		return "Equal";
		case sf::Keyboard::Key::Hyphen:		return "Hyphen";
		case sf::Keyboard::Key::Space:		return "Space";
		case sf::Keyboard::Key::Enter:		return "Enter";
		case sf::Keyboard::Key::Backspace:	return "Backspace";
		case sf::Keyboard::Key::Tab:		return "Tab";
		case sf::Keyboard::Key::PageUp:		return "PageUp";
		case sf::Keyboard::Key::PageDown:	return "PageDown";
		case sf::Keyboard::Key::End:		return "End";
		case sf::Keyboard::Key::Home:		return "Home";
		case sf::Keyboard::Key::Insert:		return "Insert";
		case sf::Keyboard::Key::Delete:		return "Delete";
		case sf::Keyboard::Key::Add:		return "Add";
		case sf::Keyboard::Key::Subtract:	return "Subtract";
		case sf::Keyboard::Key::Multiply:	return "Multiply";
		case sf::Keyboard::Key::Divide:		return "Divide";
		case sf::Keyboard::Key::Left:		return "Left";
		case sf::Keyboard::Key::Right:		return "Right";
		case sf::Keyboard::Key::Up:			return "Up";
		case sf::Keyboard::Key::Down:		return "Down";
		default:							return "None";
	}
}

#pragma endregion

#pragma region Initializing

sf::Text& FillText(sf::Text& text, const sf::Font& font, const sf::Color& color, float scaleX, float scaleY)
{
	text.setFont(font);
	text.setFillColor(color);
	text.setScale(scaleX, scaleY);
	return text;
}

sf::Text& FillText(sf::Text& text, const sf::Font& font, const sf::String& str, const sf::Color& color, const sf::Vector2f& pos, const sf::Vector2f& scale, unsigned size, float outlineThiscknes = 3)
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

#pragma endregion



#pragma region InOut

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

std::ifstream& operator>>(std::ifstream& fin, sf::Keyboard::Key& key)
{
	int pr;
	fin >> pr;
	std::string prs;
	std::getline(fin, prs);
	return fin;
}

#pragma endregion