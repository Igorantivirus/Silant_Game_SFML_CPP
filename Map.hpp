#pragma once

#include<vector>
#include<string>

#include<SFML\Graphics.hpp>

#include"ResourceMeneger.hpp"
#include"Reader.hpp"
#include"Helper.hpp"
#include"Enums.hpp"
#include"Object.hpp"
#include"Door.hpp"
#include"CollisionMap.hpp"

class GameMap
{
public:
	GameMap(ResourceMeneger& meneger) :
		meneger{meneger}
	{}

	void LoadFromFile(const TypeID num)
	{
		auto pac = RoomReader::Read(num);
		SetPackage(pac);
		waitItemIndex = -1;
	}

	#pragma region bool methods

	bool InBocks(const sf::Vector2f& pos) const
	{
		return cols.Contain(pos) || InLockDoors(pos) || InObjs( pos) || InIObjs(pos);
	}

	bool InBocks(const sf::FloatRect& rect) const
	{
		return cols.Intersection(rect) || InLockDoors(rect) || InObjs(rect) || InIObjs(rect);
	}

	bool HaveIntersectionWithObjs(const sf::Vector2f pos, sf::String& str) const
	{
		for (const auto& i : objs)
			if (i.Contains(pos))
			{
				str = i.GetText();
				return true;
			}
		return false;
	}
	bool HaveIntersectionWithIObjs(const sf::Vector2f pos, sf::String& str, int& ind) const
	{
		ind = -1;
		for (const auto& i : iobjs)
			if (i.Contain(pos))
			{
				str = i.GetText();
				ind = toInt(&i - &iobjs[0]);
				return true;
			}
		return false;
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

	#pragma endregion

	const DoorObj& GetDoorAt(size_t i)
	{
		return doors[i];
	}

	const std::vector<Object>& GetObjs() const
	{
		return objs;
	}
	const std::vector<ItemObj>& GetIObjs() const
	{
		return iobjs;
	}

	void SetWaitItemIndex(const int index)
	{
		if (index >= iobjs.size())
			waitItemIndex = -1;
		else
			waitItemIndex = index;
	}
	const int GetWaitItemInd() const
	{
		return waitItemIndex;
	}
	void DeleteIObjAtInd()
	{
		if (waitItemIndex >= 0 && waitItemIndex < iobjs.size())
			iobjs.erase(iobjs.begin() + waitItemIndex, iobjs.begin() + waitItemIndex + 1);
		waitItemIndex = -1;
	}

	void SetPackage(const Package::MapP& pac)
	{
		if (pac.backgroundFile.empty())
			return;
		objs.clear();
		iobjs.clear();
		doors.clear();
		cols.Clear();

		sprite = meneger.OpenNewBackground(pac.backgroundFile);

		for (const auto& i : pac.collisionP)
			cols.Add(i);
		for (const auto& i : pac.doorsP)
			doors.push_back(i);
		for (const auto& i : pac.objectsP)
			objs.push_back({meneger, i});
		for (const auto& i : pac.objectsItemP)
			iobjs.push_back({meneger, i});
	}	 
	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

private:
	ResourceMeneger& meneger;

	std::vector<Object> objs;
	std::vector<ItemObj> iobjs;
	std::vector<DoorObj> doors;
	CollisionMap cols;

	sf::Sprite sprite;

	int waitItemIndex = -1;

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

	bool InObjs(const sf::FloatRect& rect) const
	{
		for (const auto& i : objs)
			if (i.Intersecting(rect))
				return true;
		return false;
	}
	bool InObjs(const sf::Vector2f& pos) const
	{
		for (const auto& i : objs)
			if (i.Contains(pos))
				return true;
		return false;
	}

	bool InIObjs(const sf::FloatRect rect) const
	{
		for (const auto& i : iobjs)
			if (i.Intersecting(rect))
				return true;
		return false;
	}
	bool InIObjs(const sf::Vector2f& pos) const
	{
		for (const auto& i : iobjs)
			if (i.Contain(pos))
				return true;
		return false;
	}
};