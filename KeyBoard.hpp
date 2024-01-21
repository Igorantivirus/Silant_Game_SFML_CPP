#pragma once

#include<fstream>

#include<SFML\Window\Keyboard.hpp>

#include"Reader.hpp"

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
			return wasPressed ? (wasPressed = false), true : false;
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
		auto pr = ReadWrite::ReadKeyBoard();
		for (size_t i = 0; i < CountParams() && i < pr.size(); ++i)
			operator[](i).SetKey(pr[i]);
	}

	void SaveSettings() const
	{
		std::vector<sf::Keyboard::Key> arr;
		for (size_t i = 0; i < CountParams(); ++i)
			arr.push_back(operator[](i).GetKey());
		ReadWrite::WriteKeyBoard(arr);
	}

	bool HaveKey(const sf::Keyboard::Key key) const
	{
		for (size_t i = 0; i < CountParams(); ++i)
			if (operator[](i).GetKey() == key)
				return true;
		return false;
	}

	void DefaultSettings()
	{
		up = sf::Keyboard::W;
		down = sf::Keyboard::S;
		left = sf::Keyboard::A;
		right = sf::Keyboard::D;

		next = sf::Keyboard::Enter;
		back = sf::Keyboard::RShift;

		inventory = sf::Keyboard::RControl;
		pause = sf::Keyboard::Escape;
		fullScreen = sf::Keyboard::F11;

		nullKey = sf::Keyboard::Unknown;
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
		return *this;
	}

	#pragma region All Get

	std::vector<sf::String> GetNames() const
	{
		auto res = ReadWrite::ReadKeyBoardNames();
		for (size_t i = 0; i < CountParams() && i < res.size(); ++i)
			res[i] += " " + Converter::KeyToString(this->operator[](i).GetKey());
		return res;
	}

	size_t CountParams() const
	{
		return 9;
	}

	Key  operator[](size_t ind) const
	{
		if (ind >= CountParams())
			return Key{};
		return *(&up + ind);
	}
	Key& operator[](size_t ind)
	{
		if (ind >= CountParams())
			return nullKey;
		return *(&up + ind);
	}

	#pragma endregion

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
	Key up;
	Key down;
	Key left;
	Key right;
	
	Key next;
	Key back;
	
	Key inventory;
	Key pause;
	Key fullScreen;

	Key nullKey;

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