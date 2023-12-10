#pragma once

#include<vector>
#include<fstream>
#include<string>

#include<SFML\Graphics.hpp>

#include"Helper.hpp"
#include"Enums.hpp"

#define PIXELS_IN_BLOCK 16

class Object
{
public:
	Object() {}
	Object(const sf::Texture& texture, const sf::IntRect& newRect) :
		sprite{ texture, newRect }, rectBlock{newRect}
	{}
	Object(const Object& other) = default;

	#pragma region Get Set

	void SetTexture(const sf::Texture& texture, const sf::IntRect& newRect)
	{
		sprite.setTexture(texture);
		sprite.setTextureRect(newRect);
	}
	
	sf::IntRect GetRect() const
	{
		return sprite.getTextureRect();
	}
	void SetRect(const sf::IntRect& newRect)
	{
		sprite.setTextureRect(newRect);
	}

	sf::IntRect GetStopRect()
	{
		return rectBlock;
	}
	void SetStopRect(const sf::IntRect& rect)
	{
		rectBlock = rect;
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
	sf::Vector2f GetCenterPos() const
	{
		return sprite.getPosition() + sf::Vector2f{
			toFloat(sprite.getTextureRect().width), toFloat(sprite.getTextureRect().height)
		};
	}

	void SetPos(const sf::Vector2f& npos)
	{
		sprite.setPosition(npos);
	}
	void SetPos(const sf::Vector2i& npos)
	{
		sprite.setPosition(toFloat(npos.x), toFloat(npos.y));
	}
	void SetPos(float x, float y)
	{
		sprite.setPosition(x, y);
	}
	void SetPos(int x, int y)
	{
		sprite.setPosition(toFloat(x), toFloat(y));
	}

	void SetCenterPos(const sf::Vector2f& npos)
	{
		sprite.setPosition(npos.x - sprite.getTextureRect().width, npos.y - sprite.getTextureRect().height);
	}
	void SetCenterPos(const sf::Vector2i& npos)
	{
		sprite.setPosition(
			toFloat(npos.x - sprite.getTextureRect().width), toFloat(npos.y - sprite.getTextureRect().height)
		);
	}
	void SetCenterPos(float x, float y)
	{
		sprite.setPosition(x - sprite.getTextureRect().width, y - sprite.getTextureRect().height);
	}
	void SetCenterPos(int x, int y)
	{
		sprite.setPosition(
			toFloat(x - sprite.getTextureRect().width), toFloat(y - sprite.getTextureRect().height)
		);
	}

	#pragma endregion

	void Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

private:
	sf::Sprite sprite;

	sf::IntRect rectBlock;

	bool ghostly = false;

};

class DoorObj
{
public:
	DoorObj() {}
	DoorObj(const sf::Texture& texture, const sf::IntRect& irect) : sprite{texture, irect} {}

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
		dobj.sprite.setPosition(x * PIXELS_IN_BLOCK,y * PIXELS_IN_BLOCK);
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

class CollisionMap
{
public:
	CollisionMap() {}
	
	#pragma region Set Get

	void Add(const sf::FloatRect& rect)
	{
		cols.push_back(rect);
	}

	sf::FloatRect operator[](size_t i) const
	{
		if (i >= cols.size())
			return sf::FloatRect{};
		return cols[i];
	}

	void SetAt(size_t i, const sf::FloatRect& rect)
	{
		if (i >= cols.size())
			return;
		cols[i] = rect;
	}

	void SetAtOrPush(size_t i, const sf::FloatRect& rect)
	{
		if (i >= cols.size())
			cols.push_back(rect);
		cols[i] = rect;
	}

	auto begin() const
	{
		return cols.begin();
	}
	auto end() const
	{
		return cols.end();
	}

	#pragma endregion

	void Clear()
	{
		cols.clear();
	}

	bool Contain(const sf::Vector2f& pos) const
	{
		return Contain(toInt(pos.x), toInt(pos.y));
	}
	bool Contain(const sf::Vector2i& pos) const
	{
		return Contain(pos.x, pos.y);
	}
	bool Contain(int x, int y) const
	{
		return Contain(toFloat(x), toFloat(y));
	}
	bool Contain(float x, float y) const
	{
		for (const auto& i : cols)
			if (i.contains(x, y))
				return true;
		return false;
	}

	bool Intersection(const sf::FloatRect& rect) const
	{
		for (const auto& i : cols)
			if (i.intersects(rect))
				return true;
		return false;
	}

private:
	std::vector<sf::FloatRect> cols;
};

class GameMap
{
public:
	GameMap() {}

	void LoadFromFile(Location loc, int num)
	{
		std::string filename = NameOfLocSTD(loc) + "\\room" + std::to_string(num) + ".txt";
		std::ifstream read(filename);
		if (!read.is_open())
		{
			std::cout << "Room " << filename << " cnot open!\n";
			return;
		}

		std::string pr;
		getline(read, pr);
		LoadTexture(loc, pr);

		int count;

		LoadColision(read);

		LoadDoors(read);

		read >> count;

		for(int i = 0; i < count; ++i){}

		read.close();
	}

	bool InBocks(const sf::Vector2f& pos) const
	{
		return cols.Contain(pos) || InLockDoors(pos);
	}
	bool InBocks(const sf::Vector2i& pos) const
	{
		return cols.Contain(pos) || InLockDoors({toFloat(pos.x), toFloat(pos.y)});
	}
	bool InBocks(float x, float y) const
	{
		return cols.Contain(x, y) || InLockDoors({x, y});
	}
	bool InBocks(int x, int y) const
	{
		return cols.Contain(x, y) || InLockDoors({toFloat(x), toFloat(y)});
	}

	bool InBocks(const sf::FloatRect& rect) const
	{
		return cols.Intersection(rect) || InLockDoors(rect);
	}

	bool InOpenDoor(const sf::FloatRect& rect, size_t& pos)
	{
		pos = -1;
		for (size_t i = 0; i < doors.size(); ++i)
			if (!doors[i].IsLock() && doors[i].Intersection(rect))
			{
				pos = i;
				return true;
			}
		return false;
	}

	const DoorObj& GetDootAt(size_t i)
	{
		if (i >= doors.size())
			return DoorObj();
		return doors[i];
	}

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

private:
	std::vector<Object> objs;
	std::vector<DoorObj> doors;
	CollisionMap cols;

	sf::Texture texture;
	sf::Sprite sprite;

	void LoadTexture(Location loc, const std::string& name)
	{
		std::string file = "Textures\\" + name;
		texture.loadFromFile(file);
		sf::IntRect pr = { 0, 0, toInt(texture.getSize().x), toInt(texture.getSize().y) };
		sprite.setTextureRect(pr);
		sprite.setTexture(texture);
	}

	void LoadColision(std::ifstream& read)
	{
		int count;
		read >> count;
		cols.Clear();
		sf::FloatRect prRect;
		std::string pr;
		for (float i = 0, x1, x2, y1, y2; i < count; ++i)
		{
			read >> x1 >> y1 >> x2 >> y2;
			prRect.left = x1 * PIXELS_IN_BLOCK;
			prRect.top = y1 * PIXELS_IN_BLOCK;
			prRect.width = (x2 - x1 + 1) * PIXELS_IN_BLOCK;
			prRect.height = (y2 - y1 + 1) * PIXELS_IN_BLOCK;
			cols.Add(prRect);
			getline(read, pr);
		}
	}

	void LoadDoors(std::ifstream& read)
	{
		int count;
		std::string pr;
		read >> count;
		doors.clear();
		for (int i = 0; i < count; ++i)
		{
			doors.push_back({});
			read >> doors[i];
			std::getline(read, pr);
		}
	}

	bool InLockDoors(const sf::FloatRect& rect) const
	{
		for (const auto& i : doors)
			if (i.IsLock() && i.Intersection(rect))
				return true;
			return false;
	}
	bool InLockDoors(const sf::Vector2f& pos) const
	{
		for (const auto& i : doors)
			if (i.IsLock() && i.Contain(pos))
				return true;
		return false;
	}


};