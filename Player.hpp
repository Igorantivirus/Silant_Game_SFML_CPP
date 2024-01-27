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

	#pragma region Inventory

	bool AddItem(const unsigned int iID)
	{
		Item it;
		it.LoadAtID(iID);
		return info.inventory.Add(it);
	}

	#pragma endregion

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
		aspiratPos = {0.f,0.f};
		barierBox.left = x - barierBox.width / 2.f;
		barierBox.top = y - barierBox.height / 2.f;
	}
	void SetFootCenterPosition(const sf::Vector2f& pos)
	{
		SetFootCenterPosition(pos.x, pos.y);
	}

	const sf::Vector2f& GetAspirPos() const
	{
		return aspiratPos;
	}
	void SetAspirPos(const sf::Vector2f& pos)
	{
		aspiratPos = pos;
	}

	const float GetNextX() const
	{
		return aspiratPos.x;
	}
	const float GetNextY() const
	{
		return aspiratPos.y;
	}
	void SetAspirX(const float x)
	{
		aspiratPos.x = x;
	}
	void SetAspirY(const float y)
	{
		aspiratPos.y = y;
	}

	#pragma endregion

	sf::FloatRect GetBarierBox() const
	{
		return barierBox;
	}
	sf::FloatRect GetBarierBoxAspir() const
	{
		return barierBox + aspiratPos;
	}

	const sf::Sprite& GetSprite() const
	{
		return sprite;
	}

	#pragma endregion

	#pragma region Update

	void UpdatePosition()
	{
		sprite.move(aspiratPos *= sliding);

		barierBox.left = sprite.getPosition().x + sprite.getTextureRect().width / 2.f - barierBox.width / 2.f;
		barierBox.top  = sprite.getPosition().y + sprite.getTextureRect().height - barierBox.height / 2.f;
	}

	void StopRun()
	{
		aspiratPos = { 0.f, 0.f };
		//aspiratPos = sprite.getPosition();
	}

	void UpdateAnum()
	{
		if (aspiratPos.x == 0.f && aspiratPos.y == 0.f)
			pos = pos > 3 ? 4 : 1;
		else if (aspiratPos.x < 0 && pos > 3)
			pos = 1;
		else if (aspiratPos.x > 0 && pos < 4)
			pos = 4;
		/*if (aspiratPos.x - sprite.getPosition().x == 0.f && aspiratPos.y - sprite.getPosition().y == 0.f)
			pos = pos > 3 ? 4 : 1;
		else if ((aspiratPos.x - sprite.getPosition().x) < 0 && pos > 3)
			pos = 1;
		else if ((aspiratPos.x - sprite.getPosition().x) > 0 && pos < 4)
			pos = 4;*/
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


	float speed = .8f;
	float sliding = 0.2f;

	Rotation rotation = Rotation::Up;
	short pos = 1;

	void SetPose(short pose)
	{
		pos = pose;
		sprite.setTextureRect({ 16 * (pose - 1), 0, 16, 34 });
	}

};