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

	Location GetNextLock() const
	{
		return nextLock;
	}
	void SetNextLock(Location loc)
	{
		nextLock = loc;
	}

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

	int GetNextNomer() const
	{
		return nextNomer;
	}
	void SetNextNomer(int n)
	{
		nextNomer = 0;
	}

#pragma endregion

	void Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

	friend std::ifstream& operator>>(std::ifstream& fin, DoorObj& dobj)
	{
		float x, y;
		fin >> x >> y;
		dobj.sprite.setPosition(x * PIXELS_IN_BLOCK, y * PIXELS_IN_BLOCK);
		dobj.rect.left = x * PIXELS_IN_BLOCK;
		dobj.rect.top = y * PIXELS_IN_BLOCK;

		fin >> x >> y;
		dobj.rect.width = x * PIXELS_IN_BLOCK;
		dobj.rect.height = y * PIXELS_IN_BLOCK;
		int pr;
		fin >> pr;
		dobj.nextLock = static_cast<Location>(pr);
		fin >> pr;
		dobj.nextNomer = pr;
		fin >> x >> y;
		dobj.newxPos = { x * PIXELS_IN_BLOCK, y * PIXELS_IN_BLOCK };
		return fin;
	}

private:
	sf::Sprite sprite;

	bool lock = false;
	sf::FloatRect rect{};

	Location nextLock = Location::Default;
	int nextNomer{};
	sf::Vector2f newxPos{};

};