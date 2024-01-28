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

#define OBJECTTEXTURS_INFO "InfoFiles/objectsInfo.xml"

#define ITEMS_INFO_FILE "InfoFiles/items.xml"

#define SAVES_PATH "Saves"

namespace Package
{
	#pragma region

	struct ObjP
	{
		TypeID ID{};
		sf::IntRect spriteRect;
		sf::FloatRect barierBox;
	};

	struct ObjectP
	{
		TypeID ID{};
		sf::String text;
		sf::IntRect spriteRect;
		sf::FloatRect rectBlock;
		sf::Vector2f spritePos;
		bool ghostly{};
	};
	struct ObjectItemP
	{
		TypeID ID{};
		TypeID itemID{};
		sf::String text;
		sf::IntRect spriteRect;
		sf::FloatRect rectBlock;
		sf::Vector2f spritePos;
		bool ghostly{};
	};
	struct DoorP
	{
		sf::FloatRect trigerBox;
		TypeID nextRoom{};
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

	#pragma endregion

	#pragma region Player

	struct ItemP
	{
		TypeID ID{};
		unsigned short type{};
		int HP{};
		int Boroda{};
		int Armor{};
		int Damage{};
		sf::String name;
		sf::String info;
		sf::String useInfo;
		sf::String breakInfo;
	};

	struct InventoryP
	{
		TypeID armor;
		TypeID weapon;

		std::vector<TypeID> items;
	};

	struct PlayerP
	{
		int HP;
		int boroda;

		InventoryP invent;

		TypeID room;
		sf::Vector2f pos;
		
	};

	#pragma endregion

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

	static Package::ObjP ReadObjectInfo(const TypeID ID)
	{
		Package::ObjP res;

		pugi::xml_document doc;
		pugi::xml_parse_result openRes = doc.load_file(OBJECTTEXTURS_INFO);
		if (!openRes)
		{
			std::cout << "Object info file is not open. Error: " << openRes.description() << '\n';
			return res;
		}

		std::string elementName = "object" + std::to_string(ID);
		pugi::xml_node itemNode = doc.child("objects").child(elementName.c_str());

		if (itemNode.empty()) {
			std::cout << "Object with ID " << ID << " not found.\n";
			return res;
		}

		res.ID = ID;
		res.barierBox = ReadRectFromXMLNode(itemNode.child("hitbox"));
		auto pr = ReadRectFromXMLNode(itemNode.child("texture"));
		res.spriteRect.left = toInt(pr.left);
		res.spriteRect.top = toInt(pr.top);
		res.spriteRect.width = toInt(pr.width);
		res.spriteRect.height = toInt(pr.height);

		return res;
	}

private:
	static sf::FloatRect ReadRectFromXMLNode(const pugi::xml_node& nodeRect)
	{
		sf::FloatRect res;

		res.left	= nodeRect.attribute("x").as_int();
		res.top		= nodeRect.attribute("y").as_int();
		res.width	= nodeRect.attribute("width").as_int();
		res.height	= nodeRect.attribute("height").as_int();

		return res;
	}
};

class RoomReader
{
public:

	static Package::MapP Read(const TypeID roomNumber)
	{
		Package::MapP res;
		std::string fileName = "Rooms/room" + std::to_string(roomNumber) + ".xml";

		pugi::xml_document doc;
		pugi::xml_parse_result openRes = doc.load_file(fileName.c_str());
		if (!openRes)
		{
			std::cout << "room is not open. Error: " << openRes.description() << '\n';
			return res;
		}

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
			element.text = Converter::Win1251ToUnocide(elementNode.attribute("text").as_string());
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
			element.text = Converter::Win1251ToUnocide(elementNode.attribute("text").as_string());
			elements.push_back(element);
		}
	}
};

class ItemReader
{
public:
	static Package::ItemP ReadItem(const TypeID ID)
	{
		Package::ItemP res;
		if (ID == 0)
			return res;

		pugi::xml_document doc;
		pugi::xml_parse_result openRes = doc.load_file(ITEMS_INFO_FILE);
		if (!openRes)
		{
			std::cout << "item bd is not open. Error: " << openRes.description() << '\n';
			return res;
		}

		std::string elementName = "item" + std::to_string(ID);
		pugi::xml_node itemNode = doc.child("items").child(elementName.c_str());
		
		if (itemNode.empty()) {
			std::cout << "Item with ID " << ID << " not found.\n";
			return res;
		}

		res.name = itemNode.name();
		res.ID = ID;
		res.type = itemNode.attribute("type").as_uint();

		res.name		= Converter::Win1251ToUnocide(itemNode.attribute("name").value());
		res.info		= Converter::Win1251ToUnocide(itemNode.child("info").text().get());
		res.useInfo		= Converter::Win1251ToUnocide(itemNode.child("useInfo").text().get());
		res.breakInfo	= Converter::Win1251ToUnocide(itemNode.child("breakInfo").text().get());
		res.HP			= itemNode.child("HP").text().as_int();
		res.Boroda		= itemNode.child("boroda").text().as_int();
		res.Armor		= itemNode.child("armor").text().as_int();
		res.Damage		= itemNode.child("damage").text().as_int();

		return res;
	}
};

class SaveWriter
{
public:
	
	static void SavePlayer(const std::string& saveName, const Package::PlayerP& pPac)
	{
		pugi::xml_document playerDoc;
		pugi::xml_node playerNode = playerDoc.append_child("player");
		WritePlayer(playerNode, pPac);

		pugi::xml_document doc;
		pugi::xml_parse_result openRes = doc.load_file(saveName.c_str());
		if (!openRes)
			std::ofstream(saveName);
		pugi::xml_node gameSaveNode = doc.child("gameSave");
		if (!gameSaveNode)
			gameSaveNode = doc.append_child("gameSave");

		gameSaveNode.remove_child("player");

		gameSaveNode.append_copy(playerNode);

		doc.save_file(saveName.c_str());
	}
	static Package::PlayerP LoadPlayer(const std::string& fName)
	{
		Package::PlayerP res;

		pugi::xml_document doc;
		pugi::xml_parse_result openRes = doc.load_file(fName.c_str());
		if (!openRes)
		{
			std::cout << "Fail open file " << fName << " Error : " << openRes.description() << '\n';
			return res;
		}
		pugi::xml_node playerNode = doc.child("gameSave").child("player");
		ReadPlayer(playerNode, res);

		return res;
	}

private:
	static void WriteInventory(pugi::xml_node& node, const Package::InventoryP& inventory) {
		pugi::xml_node armorNode = node.append_child("armor");
		armorNode.append_attribute("ID").set_value(inventory.armor);
		pugi::xml_node weaponNode = node.append_child("weapon");
		weaponNode.append_attribute("ID").set_value(inventory.weapon);
		pugi::xml_node itemNode;
		for (const TypeID& item : inventory.items)
		{
			itemNode = node.append_child("item");
			itemNode.append_attribute("ID").set_value(item);
		}
	}
	static void WritePlayer(pugi::xml_node& node, const Package::PlayerP& pac)
	{
		pugi::xml_node hpNode = node.append_child("HP");
		hpNode.text().set(pac.HP);
		pugi::xml_node borodaNode = node.append_child("boroda");
		borodaNode.text().set(pac.boroda);
		pugi::xml_node roomNode = node.append_child("room");
		roomNode.text().set(pac.room);
		pugi::xml_node positionNode = node.append_child("position");
		positionNode.append_attribute("x").set_value(pac.pos.x);
		positionNode.append_attribute("y").set_value(pac.pos.y);
		pugi::xml_node inventoryNode = node.append_child("inventory");
		WriteInventory(inventoryNode, pac.invent);
	}

	static void ReadInventory(const pugi::xml_node& node, Package::InventoryP& inventory)
	{
		inventory.armor = node.child("armor").attribute("ID").as_uint();
		inventory.weapon = node.child("weapon").attribute("ID").as_uint();
		for (auto item : node.children("item"))
			inventory.items.push_back(item.attribute("ID").as_uint());
	}
	static void ReadPlayer(const pugi::xml_node& node, Package::PlayerP& pac)
	{
		pac.HP = node.child("HP").text().as_uint();
		pac.boroda = node.child("boroda").text().as_uint();
		pac.room = node.child("room").text().as_uint();

		pugi::xml_node posNode = node.child("position");
		pac.pos = { posNode.attribute("x").as_float(), posNode.attribute("y").as_float() };

		pugi::xml_node inventNode = node.child("inventory");
		ReadInventory(inventNode, pac.invent);
	}

};