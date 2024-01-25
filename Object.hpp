#pragma once

#include<SFML\Graphics.hpp>

#include"Reader.hpp"
#include"ResourceMeneger.hpp"
#include"Helper.hpp"
#include"Converter.hpp"

class Object
{
public:friend class ItemObj;
	Object(const ResourceMeneger& meneger)
	{
		sprite.setTexture(meneger.GetObjectsTexture());
	}
	Object(const Object& other) = default;

	#pragma region Get Set

	const sf::String& GetText() const
	{
		return text;
	}

	unsigned GetID() const
	{
		return ID;
	}

	bool Ghostly() const
	{
		return ghostly;
	}
	void SetGhostly(bool v)
	{
		ghostly = v;
	}

	sf::Vector2f GetPos() const
	{
		return sprite.getPosition();
	}
	sf::Vector2f GetCenterBarierBoxPosition() const
	{
		return sf::Vector2f{rectBlock.left + rectBlock.width / 2.f, rectBlock.top + rectBlock.height / 2.f};
	}

	void SetPos(const sf::Vector2f& npos)
	{
		sprite.setPosition(npos);
	}
	void SetPos(float x, float y)
	{
		sprite.setPosition(x, y);
	}

	#pragma endregion

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

	bool Intersecting(const sf::FloatRect& rect) const
	{
		return rectBlock.intersects(rect);
	}
	bool Contains(const sf::Vector2f& pos) const
	{
		return rectBlock.contains(pos);
	}

	friend std::ifstream& operator>>(std::ifstream& fin, Object& obj)
	{
		fin >> obj.ID >> obj.rectBlock.left >> obj.rectBlock.top;
		obj.rectBlock.left *= PIXELS_IN_BLOCK;
		obj.rectBlock.top *= PIXELS_IN_BLOCK;
		if (obj.ID == 0)
		{
			fin >> obj.rectBlock.width >> obj.rectBlock.height;
			obj.rectBlock.width *= PIXELS_IN_BLOCK;
			obj.rectBlock.height *= PIXELS_IN_BLOCK;
			obj.NullObj();
		}
		else
			obj.FillObjFromFile(obj.ID);
		(void)fin.get(); (void)fin.get();
		int pr = 0;
		obj.text.clear();
		ReadWrite::getlineToStopSymbol(fin, obj.text, '|');
		obj.text = Converter::UTF8ToUnicode(obj.text);
		return fin;
	}

private:
	unsigned ID{};

	sf::Sprite sprite;

	sf::FloatRect rectBlock;

	bool ghostly = false;

	sf::String text;

	void FillObjFromFile(const unsigned newID)
	{
		if (newID == 0)
			return NullObj();

		auto package = ReadWrite::ReadObjectInfo(newID);
		ID = package.ID;
		sprite.setTextureRect(package.spriteRect);
		rectBlock.width = package.barierBox.width;
		rectBlock.height = package.barierBox.height;
		sprite.setPosition(rectBlock.left - package.barierBox.left, rectBlock.top - package.barierBox.top);
	}

	void NullObj()
	{
		sprite.setTextureRect({});
		sprite.setPosition(rectBlock.left, rectBlock.top);
		ghostly = true;
	}
};

class ItemObj
{
public:
	ItemObj(const ResourceMeneger& meneger)
	{
		sprite.setTexture(meneger.GetObjectsTexture());
	}

	#pragma region Get Set

	unsigned GetIDOfItem() const
	{
		return IDItem;
	}
	unsigned GetId() const
	{
		return ID;
	}

	const sf::FloatRect& GetRect() const
	{
		return rectBlock;
	}

	sf::Vector2f GetCenterBarierBoxPosition() const
	{
		return sf::Vector2f{rectBlock.left + rectBlock.width / 2.f, rectBlock.top + rectBlock.height / 2.f};
	}

	const sf::String& GetText() const
	{
		return txt;
	}

	void SetPos(const sf::Vector2f& pos)
	{
		sprite.setPosition(pos);
	}

#pragma endregion

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

	friend std::ifstream& operator>>(std::ifstream& fin, ItemObj& iobj)
	{
		fin >> iobj.IDItem;
		fin >> iobj.ID >> iobj.rectBlock.left >> iobj.rectBlock.top;
		iobj.rectBlock.left *= PIXELS_IN_BLOCK;
		iobj.rectBlock.top *= PIXELS_IN_BLOCK;
		if (iobj.ID == 0)
		{
			fin >> iobj.rectBlock.width >> iobj.rectBlock.height;
			iobj.rectBlock.width *= PIXELS_IN_BLOCK;
			iobj.rectBlock.height *= PIXELS_IN_BLOCK;
			iobj.NullObj();
		}
		else
			iobj.FillObjFromFile(iobj.ID);
		(void)fin.get(); (void)fin.get();
		int pr = 0;
		iobj.txt.clear();
		ReadWrite::getlineToStopSymbol(fin, iobj.txt, '|');
		iobj.txt = Converter::UTF8ToUnicode(iobj.txt);
		return fin;
	}

	static bool IsNullInData(unsigned lineNomer, std::string file = "gamedata.txt")
	{
		std::ifstream read(file);

		for (unsigned i = 0; i < lineNomer; ++i)
			std::getline(read, file);

		int val;
		read >> val;

		read.close();
		return val == 0;
	}

	bool Intersecting(const sf::FloatRect& rect) const
	{
		return rectBlock.intersects(rect);
	}
	bool Contain(const sf::Vector2f& pos) const
	{
		return rectBlock.contains(pos);
	}

private:
	unsigned IDItem = 0;
	unsigned ID = 0;
	sf::Sprite sprite;
	sf::FloatRect rectBlock;

	sf::String txt;

	void FillObjFromFile(const unsigned newID)
	{
		if (newID == 0)
			return NullObj();

		auto package = ReadWrite::ReadObjectInfo(newID);
		ID = package.ID;
		sprite.setTextureRect(package.spriteRect);
		rectBlock.width = package.barierBox.width;
		rectBlock.height = package.barierBox.height;
		sprite.setPosition(rectBlock.left - package.barierBox.left, rectBlock.top - package.barierBox.top);
	}
	void NullObj()
	{
		sprite.setTextureRect({});
		sprite.setPosition(rectBlock.left, rectBlock.top);
	}
};