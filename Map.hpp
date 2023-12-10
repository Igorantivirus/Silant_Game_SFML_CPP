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

	bool Contain(const sf::Vector2f& pos)
	{
		return Contain(toInt(pos.x), toInt(pos.y));
	}
	bool Contain(const sf::Vector2i& pos)
	{
		return Contain(pos.x, pos.y);
	}
	bool Contain(int x, int y)
	{
		return Contain(toFloat(x), toFloat(y));
	}
	bool Contain(float x, float y)
	{
		for (const auto& i : cols)
			if (i.contains(x, y))
				return true;
		return false;
	}

	bool Intersection(const sf::FloatRect& rect)
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
		read >> count;
		sf::FloatRect prRect;

		cols.Clear();

		for (int i = 0, x1, x2, y1, y2; i < count; ++i)
		{
			read >> x1 >> y1 >> x2 >> y2;
			prRect.left = x1 * PIXELS_IN_BLOCK;
			prRect.top = y1 * PIXELS_IN_BLOCK;
			prRect.width = (x2 - x1) * PIXELS_IN_BLOCK;
			prRect.height = (y2 - y1) * PIXELS_IN_BLOCK;
			cols.Add(prRect);
		}

		read >> count;
		
		for(int i = 0; i < count; ++i){}

		read >> count;

		for(int i = 0; i < count; ++i){}

		read.close();
	}

	bool InBocks(const sf::Vector2f& pos)
	{
		return cols.Contain(pos);
	}
	bool InBocks(const sf::Vector2i& pos)
	{
		return cols.Contain(pos);
	}
	bool InBocks(float x, float y)
	{
		return cols.Contain(x, y);
	}
	bool InBocks(int x, int y)
	{
		return cols.Contain(x, y);
	}

	bool InBocks(const sf::FloatRect& rect)
	{
		return cols.Intersection(rect);
	}

	void Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

private:
	std::vector<Object> objs;
	CollisionMap cols;

	sf::Texture texture;
	sf::Sprite sprite;

	void LoadTexture(Location loc, const std::string& name)
	{
		std::string file = "Textures\\" + name;
		texture.loadFromFile(file);
		sprite.setTexture(texture);
	}

};