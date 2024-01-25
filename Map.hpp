#pragma once

#include<vector>
#include<fstream>
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

		LoadColision(read);
		LoadDoors(read);
		LoadObjects(read);
		LoadItemObjects(read);

		read.close();
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
	bool HaveIntersectionWithIObjs(const sf::Vector2f pos, sf::String& str) const
	{
		for (const auto& i : iobjs)
			if (i.Contain(pos))
			{
				str = i.GetText();
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
		if (i >= doors.size())
			return DoorObj();
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
		for (float i = 0, x, y, w, h; i < count; ++i)
		{
			read >> x >> y >> w >> h;
			prRect.left = x * PIXELS_IN_BLOCK;
			prRect.top = y * PIXELS_IN_BLOCK;
			prRect.width = w * PIXELS_IN_BLOCK;
			prRect.height = h * PIXELS_IN_BLOCK;
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

	void LoadObjects(std::ifstream& read)
	{
		int count;
		read >> count;
		objs.clear();
		objs.reserve(count);
		std::string pr;
		for (int i = 0, j = 0, gd; i < count; ++i)
		{
			read >> gd;
			if (gd != 0 && ReadWrite::GameDataReader(gd) != 0)
			{
				std::getline(read, pr);
				continue;
			}
			objs.push_back({meneger});
			read >> objs[j++];
			std::getline(read, pr);
		}
	}

	void LoadItemObjects(std::ifstream& read)
	{
		iobjs.clear();
		if (!read.is_open() || read.eof())
			return;
		int count, pr;
		read >> count;
		iobjs.reserve(count);
		std::string prs;
		for (int i = 0; i < count; ++i)
		{
			read >> pr;
			if (!ItemObj::IsNullInData(pr))
				continue;
			iobjs.push_back({meneger});
			read >> iobjs[i];
			std::getline(read, prs);
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