#pragma once

#include<SFML/Graphics.hpp>

#include"Enums.hpp"
#include"Helper.hpp"

class Player
{
public:
	Player(){}
	Player(const sf::String& fileName, const sf::IntRect& rect)
	{
		animTexture.loadFromFile(fileName);
		sprite.setTexture(animTexture);
		sprite.setTextureRect(rect);
	}

	#pragma region SetGet

	bool SetTexture(const sf::String file, const sf::IntRect& rect)
	{
		if (!animTexture.loadFromFile(file))
			return false;
		sprite.setTexture(animTexture);
		sprite.setTextureRect(rect);
		return true;
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

	Rotation GetRotation() const
	{
		return rotation;
	}
	void SetRotation(Rotation val)
	{
		rotation = val;
	}

	#pragma region Position

	#pragma region RealPos

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

	void Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

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

	constexpr static float sliding = .1f;

private:
	sf::Texture animTexture;
	sf::Sprite sprite;

	sf::Vector2f aspiratPos{};

	sf::FloatRect barierBox = {0, 0, 16, 6};

	float speed = 1.f;

	Rotation rotation = Rotation::Up;
	short pos = 1;

	void SetPose(short pose)
	{
		pos = pose;
		sprite.setTextureRect({ 16 * (pose - 1), 0, 16, 34 });
	}

};