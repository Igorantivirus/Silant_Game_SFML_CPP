#pragma once

#include<fstream>
#include<vector>

#include<pugixml.hpp>

#include"Helper.hpp"
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
		bool ghostly;
	};
	struct ObjectItemP
	{
		unsigned int ID{};
		unsigned int itemID{};
		sf::String text;
		sf::IntRect spriteRect;
		sf::FloatRect rectBlock;
		bool ghostly;
	};
	struct DoorP
	{
		sf::FloatRect barierBox;
		Location loc;
		unsigned nextRoom;
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

class RoomReader
{
public:

	static Package::MapP Read(const unsigned int roomNumber)
	{
		Package::MapP res;
		std::string fileName = "Rooms/room" + std::to_string(roomNumber) + ".txt";

		pugi::xml_document doc;
		if(!doc.load_file(fileName.c_str()))
			return res;



		return res;
	}

};

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

	static Package::MapP ReadMap(const unsigned int nroom)
	{
		decltype(ReadMap(0)) res;

		std::ifstream read("Rooms/room" + std::to_string(nroom) + ".txt");
		std::string prs;
		int pri;
		unsigned int count;

		std::getline(read, res.backgroundFile);
		
		read >> count;
		for (unsigned int i = 0; i < count; ++i)
		{
			res.collisionP.push_back({});
			ReadCollision(read, res.collisionP[i]);
		}

		read >> count;
		for (unsigned int i = 0; i < count; ++i)
		{
			res.doorsP.push_back({});
			ReadDoorP(read, res.doorsP[i]);
		}

		read >> count;
		for (unsigned int i = 0; i < count; ++i)
		{
			res.objectsP.push_back({});
			ReadObject(read, res.objectsP[i]);
		}

		read >> count;
		for (unsigned int i = 0; i < count; ++i)
		{
			res.objectsP.push_back({});
			ReadItemObject(read, res.objectsItemP[i]);
		}

		return res;
	}
private:
	static void ReadCollision(std::ifstream& read, sf::FloatRect& rect)
	{
		read >> rect.left >> rect.top >> rect.width >> rect.height;
	}
	static void ReadDoorP(std::ifstream& read, Package::DoorP& d)
	{
		read >> d.barierBox.left >> d.barierBox.top >> d.barierBox.width >> d.barierBox.height;
		d.barierBox.top *= PIXELS_IN_BLOCK;
		d.barierBox.left *= PIXELS_IN_BLOCK;
		d.barierBox.width *= PIXELS_IN_BLOCK;
		d.barierBox.height *= PIXELS_IN_BLOCK;

		read >> d.nextRoom;
		d.loc = static_cast<Location>(d.nextRoom);
		read >> d.nextRoom;

		read >> d.nextPos.x >> d.nextPos.y;
	}
	static void ReadObject(std::ifstream& read, Package::ObjectP& o)
	{
		read >> o.ID >> o.rectBlock.left >> o.rectBlock.top;
		o.rectBlock.left *= PIXELS_IN_BLOCK;
		o.rectBlock.top *= PIXELS_IN_BLOCK;
		if (o.ID == 0)
		{
			read >> o.rectBlock.width >> o.rectBlock.height;
			o.rectBlock.width *= PIXELS_IN_BLOCK;
			o.rectBlock.height *= PIXELS_IN_BLOCK;
			
			o.ghostly = true;
			o.spriteRect = {};
		}
		else
		{
			o.ghostly = false;
			auto p = ReadObjectInfo(o.ID);
			o.spriteRect = p.spriteRect;
			o.rectBlock.width = p.barierBox.width;
			o.rectBlock.height = p.barierBox.height;
		}
		(void)read.get(); (void)read.get();
		o.text.clear();
		ReadWrite::getlineToStopSymbol(read, o.text, '|');
		o.text = Converter::UTF8ToUnicode(o.text);
	}
	static void ReadItemObject(std::ifstream& read, Package::ObjectItemP& o)
	{
		read >> o.itemID;
		Package::ObjectP pr;
		ReadObject(read, pr);
		o.ghostly = pr.ghostly;
		o.ID = pr.ID;
		o.rectBlock = pr.rectBlock;
		o.spriteRect = pr.spriteRect;
		o.text = pr.text;
	}

};