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

	const sf::Keyboard::Key Up() const
	{
		return up;
	}
	const sf::Keyboard::Key Down() const
	{
		return down;
	}
	const sf::Keyboard::Key Left() const
	{
		return left;
	}
	const sf::Keyboard::Key Right() const
	{
		return right;
	}
	const sf::Keyboard::Key Pause() const
	{
		return pause;
	}
	const sf::Keyboard::Key Inventory() const
	{
		return inventory;
	}
	const sf::Keyboard::Key Next() const
	{
		return next;
	}
	const sf::Keyboard::Key Back() const
	{
		return back;
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
	bool SetNext(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || next == key)
			return false;
		next = key;
		return true;
	}
	bool SetBack(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || back == key)
			return false;
		back = key;
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
	bool IsNext()
	{
		return sf::Keyboard::isKeyPressed(next);
	}
	bool IsBack()
	{
		return sf::Keyboard::isKeyPressed(back);
	}

	#pragma endregion

private:
	sf::Keyboard::Key up = sf::Keyboard::W;
	sf::Keyboard::Key down = sf::Keyboard::S;
	sf::Keyboard::Key left = sf::Keyboard::A;
	sf::Keyboard::Key right = sf::Keyboard::D;

	sf::Keyboard::Key next = sf::Keyboard::Enter;
	sf::Keyboard::Key back = sf::Keyboard::RShift;

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