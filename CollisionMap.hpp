#pragma once

#include<SFML\Graphics.hpp>

#include"ResourceMeneger.hpp"
#include"Reader.hpp"
#include"Helper.hpp"
#include"Converter.hpp"
#include"Enums.hpp"

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
