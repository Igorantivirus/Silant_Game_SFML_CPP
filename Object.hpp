#pragma once

#include<SFML\Graphics.hpp>

#include"Reader.hpp"
#include"ResourceMeneger.hpp"
#include"Helper.hpp"
#include"Converter.hpp"

class Object
{
public:
	Object(const ResourceMeneger& meneger, const Package::ObjectP& pac)
	{
		sprite.setTexture(meneger.GetObjectsTexture());
		SetPackage(pac);
	}
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

	TypeID GetID() const
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

	void SetPackage(const Package::ObjectP& pac)
	{
		ghostly = pac.ghostly;
		ID = pac.ID;
		rectBlock = pac.rectBlock;
		sprite.setPosition(pac.spritePos);
		sprite.setTextureRect(pac.spriteRect);
		text = pac.text;
	}

private:
	TypeID ID{};

	sf::Sprite sprite;

	sf::FloatRect rectBlock;

	bool ghostly = false;

	sf::String text;
};

class ItemObj
{
public:
	ItemObj(const ResourceMeneger& meneger)
	{
		sprite.setTexture(meneger.GetObjectsTexture());
	}
	ItemObj(const ResourceMeneger& meneger, const Package::ObjectItemP& pac)
	{
		sprite.setTexture(meneger.GetObjectsTexture());
		SetPackage(pac);
	}

	#pragma region Get Set

	TypeID GetIDOfItem() const
	{
		return IDItem;
	}
	TypeID GetId() const
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

	void SetPackage(const Package::ObjectItemP& pac)
	{
		ID = pac.ID;
		IDItem = pac.itemID;
		rectBlock = pac.rectBlock;
		sprite.setPosition(pac.spritePos);
		sprite.setTextureRect(pac.spriteRect);
		txt = pac.text;
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
	TypeID IDItem = 0;
	TypeID ID = 0;
	sf::Sprite sprite;
	sf::FloatRect rectBlock;

	sf::String txt;
};