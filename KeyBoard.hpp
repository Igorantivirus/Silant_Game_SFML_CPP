#pragma once

#include<fstream>

#include<SFML\Window\Keyboard.hpp>

class KeyBoard
{
public:
	KeyBoard()
	{
		LoadFromFile();
	}
	
	void LoadFromFile() {}

	void SaveSettings() {}

	KeyBoard& operator=(const KeyBoard& other)
	{
		if (this == &other)
			return *this;
		up		= other.up;
		down	= other.down;
		left	= other.left;
		right	= other.right;
		inventory = other.inventory;
		pause = other.pause;
	}

	#pragma region GetSettings

	const sf::Keyboard::Key Up()
	{
		return up;
	}
	const sf::Keyboard::Key Down()
	{
		return down;
	}
	const sf::Keyboard::Key Left()
	{
		return left;
	}
	const sf::Keyboard::Key Right()
	{
		return right;
	}
	const sf::Keyboard::Key Pause()
	{
		return pause;
	}
	const sf::Keyboard::Key Inventory()
	{
		return inventory;
	}

	#pragma endregion

	#pragma region SetSettings

	bool SetUp(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || up == key)
			return false;
		up = key;
		return true;
	}
	bool SetDown(const sf::Keyboard::Key key)
	{
		if(HaveDuplecate(key) || down == key)
			return false;
		down = key;
		return true;
	}
	bool SetLeft(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || left == key)
			return false;
		left = key;
		return true;
	}
	bool SetRight(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || right == key)
			return false;
		right = key;
		return true;
	}
	bool SetPause(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || pause == key)
			return false;
		pause = key;
		return true;
	}
	bool SetInventory(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || inventory == key)
			return false;
		inventory = key;
		return true;
	}

	#pragma endregion

	bool IsPressed(const sf::Keyboard::Key key) const
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	#pragma region ButtonsPressed

	bool IsUp()
	{
		return sf::Keyboard::isKeyPressed(up);
	}
	bool IsDown()
	{
		return sf::Keyboard::isKeyPressed(down);
	}
	bool IsLeft()
	{
		return sf::Keyboard::isKeyPressed(left);
	}
	bool IsRight()
	{
		return sf::Keyboard::isKeyPressed(right);
	}

	bool IsPause()
	{
		return sf::Keyboard::isKeyPressed(pause);
	}
	bool IsInventory()
	{
		return sf::Keyboard::isKeyPressed(inventory);
	}

	#pragma endregion

private:
	sf::Keyboard::Key up = sf::Keyboard::W;
	sf::Keyboard::Key down = sf::Keyboard::S;
	sf::Keyboard::Key left = sf::Keyboard::A;
	sf::Keyboard::Key right = sf::Keyboard::D;

	sf::Keyboard::Key inventory = sf::Keyboard::I;
	sf::Keyboard::Key pause = sf::Keyboard::Escape;

	bool HaveDuplecate(const sf::Keyboard::Key key) const
	{
		if (key == up || key == down || key == right || key == left)
			return true;
		if (key == pause || key == inventory)
			return true;


		return false;
	}

};