#pragma once

#include<vector>
#include<fstream>
#include<string>

#include<SFML\Graphics.hpp>

#include"Helper.hpp"
#include"Enums.hpp"

#define PIXELS_IN_BLOCK 16

#define OBJECTTEXTURS_PATH "Sprites\\Objects.png"
#define OBJECTTEXTURS_INFO "Sprites\\objectsinfo.txt"

class Object
{
public:
	Object()
	{
		sprite.setTexture(objTexture);
	}
	Object(const sf::Texture& texture, const sf::IntRect& newRect) :
		sprite{ texture, newRect }, rectBlock{newRect}
	{}
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

	sf::FloatRect GetStopRect()
	{
		return rectBlock;
	}
	void SetStopRect(const sf::FloatRect& rect)
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
	sf::Vector2f GetCenterBarierBoxPosition() const
	{
		return sf::Vector2f{rectBlock.left + rectBlock.width / 2.f, rectBlock.top + rectBlock.height / 2.f};
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

	static void InitializedTexture()
	{
		objTexture.loadFromFile(OBJECTTEXTURS_PATH);
	}

	friend std::ifstream& operator>>(std::ifstream& fin, Object& obj)
	{
		fin >> obj.ID >> obj.rectBlock.left >> obj.rectBlock.top;
		obj.rectBlock.left *= PIXELS_IN_BLOCK;
		obj.rectBlock.top *= PIXELS_IN_BLOCK;
		obj.FillObjFromFile(obj.ID);
		(void)fin.get(); (void)fin.get();
		int pr = 0;
		obj.text.clear();
		std::cout << "new txt\n";
		while ((pr = fin.get()) != '|' && pr != '\n')
		{
			if (pr == 208 || pr == 209)
				continue;
			obj.text += ToUInt32(pr);
		}
		return fin;
	}

private:
	unsigned ID{};

	sf::Sprite sprite;

	sf::FloatRect rectBlock;

	bool ghostly = false;

	sf::String text;

	void FillObjFromFile(unsigned newID)
	{
		std::ifstream read(OBJECTTEXTURS_INFO);

		std::string pr;
		for (unsigned i = 0u; i < newID; ++i)
			std::getline(read, pr);
		sf::IntRect prr;
		read >> ID >> prr.left >> prr.top >> prr.width >> prr.height;
		sprite.setTextureRect(prr);

		read >> rectBlock.width >> rectBlock.height;
		sf::Vector2f prp;
		read >> prp.x >> prp.y;
		sprite.setPosition(rectBlock.left - prp.x, rectBlock.top - prp.y);

		read.close();
	}

private:
	static sf::Texture objTexture;

};
sf::Texture Object::objTexture{};

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
	GameMap()
	{
		Object::InitializedTexture();
	}

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
		LoadObjects(read);

		read.close();
	}

	bool InBocks(const sf::Vector2f& pos) const
	{
		return cols.Contain(pos) || InLockDoors(pos) || InObjs( pos);
	}
	bool InBocks(const sf::Vector2i& pos) const
	{
		return cols.Contain(pos) || InLockDoors({toFloat(pos.x), toFloat(pos.y)}) || InObjs({ toFloat(pos.x),toFloat(pos.y) });
	}
	bool InBocks(float x, float y) const
	{
		return cols.Contain(x, y) || InLockDoors({x, y}) || InObjs({ x, y });
	}
	bool InBocks(int x, int y) const
	{
		return cols.Contain(x, y) || InLockDoors({ toFloat(x), toFloat(y) }) || InObjs({toFloat(x),toFloat(y)});
	}

	bool InBocks(const sf::FloatRect& rect) const
	{
		return cols.Intersection(rect) || InLockDoors(rect) || InObjs(rect);
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
		for (int i = 0; i < count; ++i)
		{
			objs.push_back({});
			read >> objs[i];
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


};