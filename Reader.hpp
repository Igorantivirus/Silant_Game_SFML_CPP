#pragma once

#include<fstream>
#include<vector>

#include<pugixml.hpp>

#include"Helper.hpp"
#include"Converter.hpp"
#include"ResourceMeneger.hpp"
#include"Enums.hpp"

#define KEYBOARD_SETTINGS_FILE_NAME "Settings/keyboardSettings.txt"

#define GAME_INFO_FILE "InfoFiles/InfoMenu.txt"

#define GAMEDATA_FILE "gamedata.txt"

#define OBJECTTEXTURS_INFO "InfoFiles\\objectsinfo.txt"

namespace Package
{
	struct ObjP
	{
		unsigned int ID{};
		sf::IntRect spriteRect;
		sf::FloatRect barierBox;
	};

	struct ObjectP
	{
		unsigned int ID{};
		sf::String text;
		sf::IntRect spriteRect;
		sf::FloatRect rectBlock;
		sf::Vector2f spritePos;
		bool ghostly;
	};
	struct ObjectItemP
	{
		unsigned int ID{};
		unsigned int itemID{};
		sf::String text;
		sf::IntRect spriteRect;
		sf::FloatRect rectBlock;
		sf::Vector2f spritePos;
		bool ghostly;
	};
	struct DoorP
	{
		sf::FloatRect trigerBox;
		unsigned int nextRoom{};
		sf::Vector2f nextPos;
	};
	struct MapP
	{
		std::string backgroundFile;
		std::vector<sf::FloatRect> collisionP;
		std::vector<DoorP> doorsP;
		std::vector<ObjectP> objectsP;
		std::vector<ObjectItemP> objectsItemP;
	};
}

class ReadWrite
{
public:

	#pragma region ReadWrite

	template<typename type>
	static bool getline(std::basic_ifstream<type, std::char_traits<type>>& read, sf::String& str)
	{
		str.clear();
		if (!read.is_open() || read.eof())
			return false;
		sf::Uint32 pr = read.get();
		while (pr != '\n' && !read.eof())
		{
			str += pr;
			pr = read.get();
		}
		return true;
	}

	template<typename type>
	static bool getlineToStopSymbol(std::basic_ifstream<type, std::char_traits<type>>& read, sf::String& str, const char stop)
	{
		str.clear();
		if (!read.is_open() || read.eof())
			return false;
		sf::Uint32 pr = read.get();
		while (pr != '\n' && !read.eof() && pr != stop)
		{
			str += pr;
			pr = read.get();
		}
		return true;
	}

	template<typename type>
	static bool getlineUTF8(std::basic_ifstream<type, std::char_traits<type>>& read, sf::String& str)
	{
		str.clear();
		if (!read.is_open() || read.eof())
			return false;
		int pr = read.get();
		while (pr != '\n' && !read.eof())
		{
			if (pr == 208)
				str += sf::Uint32(read.get() + 896);
			else if (pr == 209)
				str += sf::Uint32(read.get() + 960);
			else
				str += sf::Uint32(pr);
			pr = read.get();
		}
		return true;
	}

	template<typename type>
	static void write(std::basic_ofstream<type, std::char_traits<type>>& write, const sf::String& str)
	{
		if (write.eof() || !write.is_open())
			return;
		for (const auto& i : str)
			write << static_cast<type>(i);
	}

	#pragma endregion

	#pragma region KeyBoard

	static std::vector<sf::String> ReadKeyBoardNames()
	{
		std::ifstream read(KEYBOARD_SETTINGS_FILE_NAME);

		std::vector<sf::String> res;
		sf::String str;
		int pr;
		while (!read.eof())
		{
			read >> pr;
			getlineUTF8(read, str);
			if (str.getSize() < 15)
				str += std::string(15 - str.getSize(), ' ');
			res.push_back(str);
		}
		return res;
	}

	static std::vector<sf::Keyboard::Key> ReadKeyBoard()
	{
		std::ifstream read(KEYBOARD_SETTINGS_FILE_NAME);
		std::vector<sf::Keyboard::Key> res;
		std::string pr;
		int key;
		for (int i = 0; i < 9; ++i)
		{
			read >> key; res.push_back(static_cast<sf::Keyboard::Key>(key)); std::getline(read, pr);

		}
		read.close();
		return res;
	}
	static void WriteKeyBoard(std::vector<sf::Keyboard::Key> keys)
	{
		std::ifstream inpr(KEYBOARD_SETTINGS_FILE_NAME);
		std::vector<std::string> pr;
		std::string prstr;
		int pri;
		while (!inpr.eof())
		{
			inpr >> pri;
			std::getline(inpr, prstr);
			pr.push_back(prstr);
		}
		inpr.close();

		std::ofstream w(KEYBOARD_SETTINGS_FILE_NAME);

		w << toInt(keys[0]) << pr[0];
		for (size_t i = 1; i < pr.size() && i < keys.size(); ++i)
			w << '\n' << toInt(keys[i]) << pr[i];
		w.close();
	}

	#pragma endregion

	static sf::String ReadInfoGame()
	{
		std::ifstream read(GAME_INFO_FILE);
		sf::String res, pr;
		while (getlineUTF8(read, pr))
			res += pr + '\n';
		return res;
	}
	static int GameDataReader(unsigned lineNumber)
	{
		std::ifstream read(GAMEDATA_FILE);
		std::string pr;

		for (unsigned i = 0; i < lineNumber; ++i)
			std::getline(read, pr);

		int val;
		read >> val;

		read.close();
		return val;
	}

	static Package::ObjP ReadObjectInfo(const unsigned int ID)
	{
		Package::ObjP res;

		std::ifstream read(OBJECTTEXTURS_INFO);
		std::string pr;
		for (unsigned i = 0u; i < ID; ++i)
			std::getline(read, pr);

		read >> 
			res.ID >>
			res.spriteRect.left >> res.spriteRect.top >> res.spriteRect.width >> res.spriteRect.height >>
			res.barierBox.width >> res.barierBox.height >>res.barierBox.left >> res.barierBox.top;

		read.close();
		return res;
	}
private:
};

class RoomReader
{
public:

	static Package::MapP Read(const unsigned int roomNumber)
	{
		Package::MapP res;
		std::string fileName = "RoomsXML/room" + std::to_string(roomNumber) + ".xml";

		pugi::xml_document doc;
		if (!doc.load_file(fileName.c_str()))
			return res;

		res.backgroundFile = doc.child("room").child("background_texture").text().as_string();

		ReadCollisions(doc, res.collisionP);
		ReadDoors(doc, res.doorsP);
		ReadObjects(doc, res.objectsP);
		ReadObjectsItem(doc, res.objectsItemP);
		return res;
	}
private:
	static void ReadCollisions(const pugi::xml_document& doc, std::vector<sf::FloatRect>& elements)
	{
		elements.clear();
		sf::FloatRect element;
		for (auto elementNode : doc.child("room").child("collisions").children("element"))
		{
			element.left	= elementNode.attribute("x").as_float() * PIXELS_IN_BLOCK;
			element.top		= elementNode.attribute("y").as_float() * PIXELS_IN_BLOCK;
			element.width	= elementNode.attribute("width").as_float() * PIXELS_IN_BLOCK;
			element.height	= elementNode.attribute("height").as_float() * PIXELS_IN_BLOCK;
			elements.push_back(element);
		}
	}
	static void ReadDoors(const pugi::xml_document& doc, std::vector<Package::DoorP>& elements)
	{
		elements.clear();
		Package::DoorP element;
		for (auto elementNode : doc.child("room").child("doors").children("element"))
		{
			element.trigerBox.left = elementNode.attribute("x").as_float() * PIXELS_IN_BLOCK;
			element.trigerBox.top = elementNode.attribute("y").as_float() * PIXELS_IN_BLOCK;
			element.trigerBox.width = elementNode.attribute("width").as_float() * PIXELS_IN_BLOCK;
			element.trigerBox.height = elementNode.attribute("height").as_float() * PIXELS_IN_BLOCK;
			element.nextRoom = elementNode.attribute("nextRoom").as_uint();
			element.nextPos.x = elementNode.attribute("newX").as_float() * PIXELS_IN_BLOCK;
			element.nextPos.y = elementNode.attribute("newY").as_float() * PIXELS_IN_BLOCK;
			elements.push_back(element);
		}
	}
	static void ReadObjects(const pugi::xml_document& doc, std::vector<Package::ObjectP>& elements)
	{
		elements.clear();
		Package::ObjectP element;
		int gameDataLine{};
		for (auto elementNode : doc.child("room").child("objects").children("element"))
		{
			gameDataLine = elementNode.attribute("gameDataLine").as_uint();
			element.ID = elementNode.attribute("objectID").as_uint();
			element.rectBlock.left = elementNode.attribute("x").as_float() * PIXELS_IN_BLOCK;
			element.rectBlock.top = elementNode.attribute("y").as_float() * PIXELS_IN_BLOCK;

			if (element.ID == 0)
			{
				element.rectBlock.width		= elementNode.attribute("width").as_float()		* PIXELS_IN_BLOCK;
				element.rectBlock.height	= elementNode.attribute("height").as_float()	* PIXELS_IN_BLOCK;
				element.spriteRect = {};
				element.spritePos = { element.rectBlock.left, element.rectBlock.top };
				element.ghostly = true;
			}
			else
			{
				auto pr = ReadWrite::ReadObjectInfo(element.ID);
				element.spriteRect = pr.spriteRect;
				element.rectBlock.width = pr.barierBox.width;
				element.rectBlock.height = pr.barierBox.height;
				element.spritePos = {
					element.rectBlock.left - pr.barierBox.left, element.rectBlock.top - pr.barierBox.top
				};
				element.ghostly = false;
			}
			element.text = elementNode.attribute("text").as_string();
			elements.push_back(element);
		}
	}
	static void ReadObjectsItem(const pugi::xml_document& doc, std::vector<Package::ObjectItemP>& elements)
	{
		elements.clear();
		Package::ObjectItemP element;
		int gameDataLine{};
		for (auto elementNode : doc.child("room").child("itemObjects").children("element"))
		{
			gameDataLine = elementNode.attribute("gameDataLine").as_uint();
			element.ID = elementNode.attribute("objectID").as_uint();
			element.itemID = elementNode.attribute("itemID").as_uint();
			element.rectBlock.left = elementNode.attribute("x").as_float() * PIXELS_IN_BLOCK;
			element.rectBlock.top = elementNode.attribute("y").as_float() * PIXELS_IN_BLOCK;

			if (element.ID == 0)
			{
				element.rectBlock.width = elementNode.attribute("width").as_float() * PIXELS_IN_BLOCK;
				element.rectBlock.height = elementNode.attribute("height").as_float() * PIXELS_IN_BLOCK;
				element.spriteRect = {};
				element.spritePos = { element.rectBlock.left, element.rectBlock.top };
				element.ghostly = true;
			}
			else
			{
				auto pr = ReadWrite::ReadObjectInfo(element.ID);
				element.spriteRect = pr.spriteRect;
				element.rectBlock.width = pr.barierBox.width;
				element.rectBlock.height = pr.barierBox.height;
				element.spritePos = {
					element.rectBlock.left - pr.barierBox.left, element.rectBlock.top - pr.barierBox.top
				};
				element.ghostly = false;
			}
			element.text = elementNode.attribute("text").as_string();
			elements.push_back(element);
		}
	}
};