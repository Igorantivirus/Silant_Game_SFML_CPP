#pragma once

#include<vector>

#include<SFML/Graphics.hpp>

#include"ResourceMeneger.hpp"
#include"Enums.hpp"
#include"Helper.hpp"
#include"Inventory.hpp"

struct PersonInfo
{
	Inventory inventory;
	int hp = 100;
	int boroda = 10;
	int money = -1000;
	int defence = 0;
	int armor = 0;

	PersonInfo& operator=(const PersonInfo& other)
	{
		if (this == &other)
			return *this;
		hp = other.hp;
		boroda = other.boroda;
		money = other.money;
		defence = other.defence;
		armor = other.armor;
		inventory = other.inventory;
		return *this;
	}
};

class Player
{
public:
	Player(const ResourceMeneger& meneger)
	{
		sprite.setTexture(meneger.GetPlayerTexture());
		sprite.setTextureRect({ 0, 0, 16, 34 });

		Item itm;
		itm.LoadAtID(1);
		info.inventory.Add(itm);

		itm.LoadAtID(2);
		info.inventory.Add(itm);

		itm.LoadAtID(3);
		info.inventory.Add(itm);

		itm.LoadAtID(4);
		info.inventory.Add(itm);

		itm.LoadAtID(5);
		info.inventory.Add(itm);

	}

	#pragma region SetGet

	const PersonInfo& GetInfo() const
	{
		return info;
	}
	PersonInfo& GetInfoLink()
	{
		return info;
	}

	unsigned GetWidth() const
	{
		return sprite.getTextureRect().width;
	}
	unsigned GetHeight() const
	{
		return sprite.getTextureRect().height;
	}

	float GetSpeed() const
	{
		return speed;
	}
	void SetSpeed(float val)
	{
		if (val > 0)
			speed = val;
	}

	float GetSeeding() const
	{
		return sliding;
	}
	void SetSleeding(float v)
	{
		if (v < 1 || v > 0)
			sliding = v;
	}

	Rotation GetRotation() const
	{
		return rotation;
	}
	void SetRotation(Rotation val)
	{
		rotation = val;
	}
	void OppositeRotation()
	{
		switch (rotation)
		{
		case Rotation::Up:
			rotation = Rotation::Down;
			break;
		case Rotation::Down:
			rotation = Rotation::Up;
			break;
		case Rotation::Right:
			rotation = Rotation::Left;
			break;
		case Rotation::Left:
			rotation = Rotation::Right;
			break;
		default:
			break;
		}
	}

	#pragma region Position

	#pragma region RealPos

	sf::Vector2f GetViewPosition() const
	{
		sf::Vector2f pos = { barierBox.left + barierBox.width / 2.f, barierBox.top + barierBox.height / 2.f };
		switch (rotation)
		{
		case Rotation::Up:
			pos.y -= barierBox.height / 2.f + 1.f;
			break;
		case Rotation::Down:
			pos.y += barierBox.height / 2.f + 1.f;
			break;
		case Rotation::Right:
			pos.x += barierBox.width / 2.f + 1;
			break;
		case Rotation::Left:
			pos.x -= barierBox.width / 2.f + 1;
			break;
		default:
			break;
		}
		return pos;
	}

	sf::Vector2f GetFootCenterPosition() const
	{
		return sprite.getPosition() + sf::Vector2f{sprite.getTextureRect().width / 2.f, toFloat(sprite.getTextureRect().height)};
	}
	sf::Vector2f GetCenterPosition() const
	{
		return sprite.getPosition() + sf::Vector2f{sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f};
	}

	void SetFootCenterPosition(float x, float y)
	{
		sprite.setPosition(
			x - sprite.getTextureRect().width / 2.f,
			y - sprite.getTextureRect().height
		);
		barierBox.left = x - barierBox.width / 2.f;
		barierBox.top = y - barierBox.height / 2.f;
	}
	void SetFootCenterPosition(const sf::Vector2f& pos)
	{
		SetFootCenterPosition(pos.x, pos.y);
	}
	void SetFootCenterPosition(const sf::Vector2i& pos)
	{
		SetFootCenterPosition(toFloat(pos.x), toFloat(pos.y));
	}
	void SetFootCenterPosition(int x, int y)
	{
		SetFootCenterPosition(toFloat(x), toFloat(y));
	}

	void SetCenterPosition(float x, float y)
	{
		sprite.setPosition(
			x - sprite.getTextureRect().width / 2.f,
			y - sprite.getTextureRect().height / 2.f
		);
		barierBox.left = x - barierBox.width / 2.f;
		barierBox.top = y  + sprite.getTextureRect().height / 2.f - barierBox.height / 2.f;
	}
	void SetCenterPosition(int x, int y)
	{
		SetCenterPosition(toFloat(x), toFloat(y));
	}
	void SetCenterPosition(const sf::Vector2f& pos)
	{
		SetCenterPosition(pos.x, pos.y);
	}
	void SetCenterPosition(const sf::Vector2i& pos)
	{
		SetCenterPosition(toFloat(pos.x), toFloat(pos.y));
	}

	#pragma endregion
	
	#pragma region aspirPos
	
	sf::Vector2f GetAspirCenterPos() const
	{
		return aspiratPos + sf::Vector2f{sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f};
	}
	sf::Vector2f GetAspirCenterFootPos() const
	{
		return aspiratPos + sf::Vector2f{sprite.getTextureRect().width / 2.f, toFloat(sprite.getTextureRect().height)};
	}

	void SetAspirCenterPos(float x, float y)
	{
		aspiratPos.x = x - sprite.getTextureRect().width / 2.f;
		aspiratPos.y = y - sprite.getTextureRect().height / 2.f;
	}
	void SetAspirCenterPos(int x, int y)
	{
		SetAspirCenterPos(toFloat(x), toFloat(y));
	}
	void SetAspirCenterPos(const sf::Vector2f& pos)
	{
		SetAspirCenterPos(pos.x, pos.y);
	}
	void SetAspirCenterPos(const sf::Vector2i& pos)
	{
		SetAspirCenterPos(toFloat(pos.x), toFloat(pos.y));

	}

	void SetAspirCenterFootPos(float x, float y)
	{
		aspiratPos.x = x - sprite.getTextureRect().width / 2.f;
		aspiratPos.y = y - sprite.getTextureRect().height;
	}
	void SetAspirCenterFootPos(int x, int y)
	{
		SetAspirCenterFootPos(toFloat(x), toFloat(y));
	}
	void SetAspirCenterFootPos(const sf::Vector2f& pos)
	{
		SetAspirCenterFootPos(pos.x, pos.y);
	}
	void SetAspirCenterFootPos(const sf::Vector2i& pos)
	{
		SetAspirCenterFootPos(toFloat(pos.x), toFloat(pos.y));
	}

	#pragma endregion

	#pragma endregion

	sf::IntRect GetRectSprite() const
	{
		return sprite.getTextureRect();
	}

	sf::FloatRect GetBarierBox() const
	{
		return barierBox;
	}
	sf::FloatRect GetBarierBoxAspir() const
	{
		return barierBox + (aspiratPos - sprite.getPosition());
	}

	const sf::Sprite& GetSprite() const
	{
		return sprite;
	}

	#pragma endregion

	#pragma region Update

	void UpdatePosition()
	{
		sprite.move((aspiratPos - sprite.getPosition()) * sliding);

		barierBox.left = sprite.getPosition().x + sprite.getTextureRect().width / 2.f - barierBox.width / 2.f;
		barierBox.top = sprite.getPosition().y + sprite.getTextureRect().height - barierBox.height / 2.f;

		if (std::abs(aspiratPos.x - sprite.getPosition().x) < sliding)
			aspiratPos.x = sprite.getPosition().x;
		if (std::abs(aspiratPos.y - sprite.getPosition().y) < sliding)
			aspiratPos.y = sprite.getPosition().y;
	}

	void StopRun()
	{
		aspiratPos = sprite.getPosition();
	}

	void UpdateAnum()
	{
		if (aspiratPos.x - sprite.getPosition().x == 0.f && aspiratPos.y - sprite.getPosition().y == 0.f)
			pos = pos > 3 ? 4 : 1;
		else if ((aspiratPos.x - sprite.getPosition().x) < 0 && pos > 3)
			pos = 1;
		else if ((aspiratPos.x - sprite.getPosition().x) > 0 && pos < 4)
			pos = 4;
		else if (pos == 6)
			pos = 4;
		else if (pos > 3)
			pos++;
		else if (pos == 3)
			pos = 1;
		else if (pos < 3)
			pos++;
		SetPose(pos);
	}

	#pragma endregion

	#pragma region Going

	void Up()
	{
		aspiratPos.y -= speed;
		rotation = Rotation::Up;
	}
	void Down()
	{
		aspiratPos.y += speed;
		rotation = Rotation::Down;
	}
	void Left()
	{
		aspiratPos.x -= speed;
		rotation = Rotation::Left;
		if (pos > 3)
			SetPose(1);
	}
	void Right()
	{
		aspiratPos.x += speed;
		rotation = Rotation::Right;
		if (pos < 4)
			SetPose(4);
	}

	#pragma endregion

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

private:
	PersonInfo info;

	sf::Sprite sprite;

	sf::Vector2f aspiratPos{};

	sf::FloatRect barierBox = {0, 0, 15, 6};

	float speed = .2f;
	float sliding = .1f;

	Rotation rotation = Rotation::Up;
	short pos = 1;

	void SetPose(short pose)
	{
		pos = pose;
		sprite.setTextureRect({ 16 * (pose - 1), 0, 16, 34 });
	}

};