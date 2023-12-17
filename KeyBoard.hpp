#pragma once

#include<fstream>

#include<SFML\Window\Keyboard.hpp>

#define SETTINGS_FILE "settings.txt"

class Key
{
public:
	Key() {}
	Key(const sf::Keyboard::Key& k) : key{ k } {}

	Key& operator=(const Key& key)
	{
		if (this == &key)
			return *this;
		this->key = key.key;
		wasPressed = key.wasPressed;
		return *this;
	}
	Key& operator=(const sf::Keyboard::Key& keyb)
	{
		key = keyb;
		wasPressed = false;
		return *this;
	}

	bool operator==(const Key& key) const
	{
		return key.key == this->key;
	}
	bool operator==(const sf::Keyboard::Key& keyb) const
	{
		return key == keyb;
	}

	friend std::istream& operator>>(std::istream& fin, Key& k)
	{
		int pr;
		fin >> pr;
		k.key = static_cast<sf::Keyboard::Key>(pr);
		std::string prs;
		std::getline(fin, prs);
		return fin;
	}
	friend std::ostream& operator<<(std::ostream& fout, const Key& k)
	{
		fout << static_cast<int>(k.key);
		return fout;
	}

	bool IsClicked() const
	{
		if (sf::Keyboard::isKeyPressed(key))
			return (wasPressed = true), false;
		else
			return wasPressed ? (wasPressed = false), true : (wasPressed = false);
	}

	bool IsPressed() const
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	sf::Keyboard::Key GetKey() const
	{
		return key;
	}
	void SetKey(const sf::Keyboard::Key& k)
	{
		key = k;
	}

private:
	sf::Keyboard::Key key = sf::Keyboard::Num0;
	mutable bool wasPressed = false;
};

class KeyBoard
{
public:
	KeyBoard()
	{
		LoadFromFile();
	}
	~KeyBoard()
	{
		SaveSettings();
	}
	
	void LoadFromFile()
	{
		std::ifstream read(SETTINGS_FILE);

		read			>>
			up			>>
			down		>>
			left		>>
			right		>>
			next		>>
			back		>>
			inventory	>>
			pause		>>
			fullScreen;
		read.close();
	}

	void SaveSettings() const
	{
		std::ofstream write(SETTINGS_FILE);
		write <<
			up			<< " up\n"			<<
			down		<< " down\n"		<<
			left		<< " left\n"		<<
			right		<< " right\n"		<<
			next		<< " next\n"		<<
			back		<< " back\n"		<<
			inventory	<< " inventory\n"	<<
			pause		<< " pause\n"		<<
			fullScreen	<< " fullscreen\n";
		write.close();
	}

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

	const Key& Up() const
	{
		return up;
	}
	const Key& Down() const
	{
		return down;
	}
	const Key& Left() const
	{
		return left;
	}
	const Key& Right() const
	{
		return right;
	}

	const Key& Pause() const
	{
		return pause;
	}
	const Key& Inventory() const
	{
		return inventory;
	}
	const Key& FullScreen() const
	{
		return fullScreen;
	}

	const Key& Next() const
	{
		return next;
	}
	const Key& Back() const
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
	bool SetFullScreen(const sf::Keyboard::Key key)
	{
		if (HaveDuplecate(key) || fullScreen == key)
			return false;
		fullScreen = key;
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

	#pragma region ButtonsPressed

	bool IsPressed(const sf::Keyboard::Key key) const
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	bool IsUp() const
	{
		return up.IsPressed();
	}
	bool IsDown() const
	{
		return down.IsPressed();
	}
	bool IsLeft() const
	{
		return left.IsPressed();
	}
	bool IsRight() const
	{
		return right.IsPressed();
	}

	bool IsPause() const
	{
		return pause.IsPressed();
	}
	bool IsInventory() const
	{
		return inventory.IsPressed();
	}
	bool IsFullScreen() const
	{
		return fullScreen.IsPressed();
	}

	bool IsNext() const
	{
		return next.IsPressed();
	}
	bool IsBack() const
	{
		return back.IsPressed();
	}

	#pragma endregion

	#pragma region ButtonsClicked

	bool IsUpClick() const
	{
		return up.IsClicked();
	}
	bool IsDownClick() const
	{
		return down.IsClicked();
	}
	bool IsLeftClick() const
	{
		return left.IsClicked();
	}
	bool IsRightClick() const
	{
		return right.IsClicked();
	}

	bool IsNextClick() const
	{
		return next.IsClicked();
	}
	bool IsBackClick() const
	{
		return back.IsClicked();
	}

	bool IsInventoryClick() const
	{
		return inventory.IsClicked();
	}
	bool IsFullScreenClick() const
	{
		return fullScreen.IsClicked();
	}
	bool IsPauseClick() const
	{
		return pause.IsClicked();
	}

	#pragma endregion

private:
	Key up = sf::Keyboard::W;
	Key down = sf::Keyboard::S;
	Key left = sf::Keyboard::A;
	Key right = sf::Keyboard::D;
	
	Key next = sf::Keyboard::Enter;
	Key back = sf::Keyboard::RShift;
	
	Key inventory = sf::Keyboard::RControl;
	Key pause = sf::Keyboard::Escape;
	Key fullScreen = sf::Keyboard::F11;

	bool HaveDuplecate(const sf::Keyboard::Key key) const
	{
		if (up == key || down == key || right == key || left == key)
			return true;
		if (pause == key || inventory == key || fullScreen == key)
			return true;
		if (next == key || back == key)
			return true;
		return false;
	}

};