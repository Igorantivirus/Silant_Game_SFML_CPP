#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include<string>
#include<locale>
#include<codecvt>
#include<cassert>

#include<SFML\Graphics.hpp>

#define toFloat(a)	static_cast<float>(a)
#define toInt(a)	static_cast<int>(a)
#define toUInt(a)	static_cast<unsigned int>(a)
#define toUInt8(a)	static_cast<unsigned char>(a)
#define toUInt32(a)	static_cast<unsigned int>(a)
#define toWChar(a)	static_cast<wchar_t>(a)
#define toChar(a)	static_cast<char>(a)

#define toIntRect(rect) sf::IntRect{ toInt(rect.left), toInt(rect.top), toInt(rect.width), toInt(rect.height) }
#define toFloatRect(rect) sf::FloatRect{ toFloat(rect.left), toFloat(rect.top), toFloat(rect.width), toFloat(rect.height) }

class Converter
{
public:
	template<std::integral type>
	static sf::String UTF8ToUnicode(const std::basic_string<type>& str)
	{
		sf::String res;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (str[i] == 208)
				res += sf::Uint32(str[++i] + 896);
			else if (str[i] == 209)
				res += sf::Uint32(str[++i] + 960);
			else
				res += sf::Uint32(str[i]);
		}
		return res;
	}
	static sf::String UTF8ToUnicode(const sf::String& str)
	{
		sf::String res;
		for (size_t i = 0; i < str.getSize(); ++i)
		{
			if (str[i] == 208)
				res += sf::Uint32(str[++i] + 896);
			else if (str[i] == 209)
				res += sf::Uint32(str[++i] + 960);
			else
				res += sf::Uint32(str[i]);
		}
		return res;
	}

	static std::string UnicodeToUNT8(const sf::String& str)
	{
		std::wstring_convert<std::codecvt_utf8<sf::Uint32>, sf::Uint32> converter;
		return converter.to_bytes(str.toUtf32());
	}

	static sf::String Win1251ToUnocide(const sf::String& str)
	{
		sf::String res = "";
		for (size_t i = 0; i < str.getSize(); ++i)
		{
			if (str[i] >= 192 && str[i] <= 255)
				res += static_cast<sf::Uint32>(str[i] + 848u);
			else if (str[i] == 168)
				res += 1025u;
			else if (str[i] == 184)
				res += 1105u;
			else
				res += str[i];
		}
		return res;
	}

	static sf::String KeyToString(const sf::Keyboard::Key key)
	{
		if (key >= 0 && key <= 25)
			return std::string(1, 'A' + static_cast<char>(key));
		if (key >= 26 && key <= 35)
			return "Num" + std::string(1, static_cast<char>(key) - 26 + '0');
		if (key >= 75 && key <= 84)
			return "Numpad" + std::string(1, static_cast<char>(key) - 75 + '0');
		if (key >= 85 && key <= 99)
		{
			if (key <= 94)
				return "F" + std::string(1, static_cast<char>(key) - 85 + '0');
			return "F1" + std::string(1, static_cast<char>(key) - 94 + '0');
		}
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

};