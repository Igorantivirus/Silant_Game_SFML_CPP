#pragma once

#include<SFML\Graphics.hpp>

#include"ResourceMeneger.hpp"
#include"Reader.hpp"
#include"Helper.hpp"
#include"Converter.hpp"
#include"Enums.hpp"

class DoorObj
{
public:
	DoorObj() {}
	DoorObj(const Package::DoorP& package)
	{
		SetPackage(package);
	}
	DoorObj(const sf::Texture& texture, const sf::IntRect& irect) : sprite{ texture, irect } {}

	bool Intersection(const sf::FloatRect& nrect) const
	{
		return rect.intersects(nrect);
	}
	bool Contain(const sf::Vector2f& pos) const
	{
		return rect.contains(pos);
	}

	bool IsLock() const
	{
		return lock;
	}
	void Close() {}
	void Open() {}

	#pragma region Get Set

	sf::Vector2f GetNextPos() const
	{
		return newxPos;
	}
	void SetNextPos(const sf::Vector2f& pos)
	{
		newxPos = pos;
	}

	sf::FloatRect GetBarierRect() const
	{
		return rect;
	}
	void SetBarierRect(const sf::FloatRect& nr)
	{
		rect = nr;
	}

	TypeID GetNextNomer() const
	{
		return nextNomer;
	}
	void SetNextNomer(const TypeID n)
	{
		nextNomer = 0;
	}

	#pragma endregion

	void Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

	void SetPackage(const Package::DoorP& pac)
	{
		newxPos = pac.nextPos;
		nextNomer = pac.nextRoom;
		rect = pac.trigerBox;
	}

private:
	sf::Sprite sprite;

	bool lock = false;
	sf::FloatRect rect{};

	TypeID nextNomer{};
	sf::Vector2f newxPos{};

};