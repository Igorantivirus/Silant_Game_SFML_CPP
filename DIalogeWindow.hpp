#pragma once

#include<SFML\Graphics.hpp>

#include"ResourceMeneger.hpp"
#include"Render.hpp"
#include"KeyBoard.hpp"
#include"Player.hpp"

class DialogeWindow
{
public:
	enum class Result : char
	{
		False,
		True,
		None
	};
public:
	DialogeWindow(const ResourceMeneger& meneger) :
		sprite{meneger.GetDiaogeWindowSprite()}
	{
		FillText(textLabel, meneger.GetMainFont(), sf::Color::Black, 0.25f, 0.25f);
		point.setRadius(1);
		point.setFillColor(sf::Color::Black);
	
	}

	void Run(const sf::String& txt)
	{
		isActive = true;
		waitNext = false;
		isAsk = false;
		isDialoge = true;
		str = txt;
		ind = 0;
		textLabel.setString("");
	}

	void Ask2(const sf::String& txtA, const sf::String& txtB)
	{
		lastRes = Result::None;
		isActive = true;
		waitNext = false;
		isAsk = true;
		isDialoge = false;
		caret = 0;
		ind = 0;
		str = "          " + txtA + "\n\n          " + txtB;
		textLabel.setString("");
	}

	void Ask(const sf::String& txt)
	{
		lastRes = Result::None;
		isActive = true;
		waitNext = false;
		isAsk = true;
		isDialoge = true;
		caret = 0;
		ind = 0;
		str = txt;
		textLabel.setString("");
	}

	void Stop()
	{
		isActive = waitNext = isAsk = false;
	}

	#pragma region Bool methods

	bool IsAvtive() const
	{
		return isActive;
	}
	bool IsFullEnter() const
	{
		return ind >= str.getSize();
	}
	bool IsWaiting() const
	{
		return waitNext;
	}

	#pragma endregion

	#pragma region Update

	void Update()
	{
		if (waitNext)
			return;
		if (ind < str.getSize())
		{
			auto pr = str[ind++];
			if (pr == '\t')
				waitNext = true;
			else
				textLabel.setString(textLabel.getString() + pr);
		}

		if (textLabel.getGlobalBounds().width >= textRect.width)
		{
			ShiftLeft();
		}
		if (textLabel.getGlobalBounds().height >= textRect.height)
		{
			waitNext = true;
			ShiftDown();
		}
		
	}
	void NextSlide()
	{
		waitNext = false;
		textLabel.setString("");
	}

	void FinalizeDialoge()
	{
		while (ind < str.getSize() && !waitNext)
			Update();
	}

	void SetPositionAtWindow(sf::Vector2f posCenter)
	{
		posCenter.x -= sprite.getTextureRect().width / 2.f;
		posCenter.y -= sprite.getTextureRect().height + 10;

		sprite.setPosition(posCenter);
		textLabel.setPosition(sprite.getPosition() + sf::Vector2f{10, 10});

		SetPointPosAtCerat();
	}

	#pragma endregion

	void GetEvent(const KeyBoard& keyBoard)
	{
		if (!isActive)
			return;
		if (keyBoard.IsBackClick())
			FinalizeDialoge();
		if (isDialoge)
		{
			if (!keyBoard.IsNextClick())
				return;
			if (IsFullEnter())
			{
				if (isAsk)
					Ask2(L"���", L"��");
				else
					isActive = false;
			}
			else if (IsWaiting())
				NextSlide();
		}
		else if (isAsk)
		{
			if (keyBoard.IsUpClick() || keyBoard.IsDownClick())
				caret = !caret;
			if (IsFullEnter() && keyBoard.IsNextClick())
			{
				lastRes = static_cast<Result>(caret);
				isActive = false;
			}
		}
	}
	short GetKaretResult() const
	{
		return caret;
	}

	Result GetLastResult() const
	{
		return lastRes;
	}

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
		window.draw(textLabel);
		if (isAsk && !isDialoge && IsFullEnter())
			window.draw(point);
	}

private:
	sf::Sprite sprite;

	sf::FloatRect textRect{0, 0, 175, 50};

	sf::CircleShape point;

	sf::Text textLabel;
	sf::String str;
	size_t ind = 0;

	bool waitNext = false;
	bool isActive = false;

	bool isDialoge = false;
	bool isAsk = false;
	bool caret = 0;

	Result lastRes = Result::None;

private://methods

	void ShiftLeft()
	{
		auto pr = textLabel.getString();
		bool find = false;

		for (size_t i = pr.getSize(); i > 0; --i)
		{
			if (pr[i - 1] == ' ')
			{
				pr[i - 1] = '\n';
				find = true;
				break;
			}
			else if (pr[i - 1] == ',' || pr[i - 1] == '.' || pr[i - 1] == '!' || pr[i - 1] == '?')
			{
				pr.insert(i, '\n');
				find = true;
				break;
			}
		}
		if (!find)
			pr.insert(pr.getSize() - 2, '\n');
		
		textLabel.setString(pr);
	}

	void ShiftDown()
	{
		auto pr = textLabel.getString();
		bool find = false;
		auto oldInd = ind;

		for (size_t i = pr.getSize(); i > 0; --i, --ind)
		{
			if (pr[i - 1] == ' ')
			{
				pr.erase(i - 1, pr.getSize() - i + 1);
				find = true;
				break;
			}
			else if (pr[i - 1] == ',' || pr[i - 1] == '.' || pr[i - 1] == '!' || pr[i - 1] == '?')
			{
				if(i != pr.getSize())
					pr.erase(i, pr.getSize() - i + 1);
				find = true;
				break;
			}
		}
		if (!find)
		{
			ind = oldInd - 1;
			pr.erase(pr.getSize() - 1);
		}
		textLabel.setString(pr);
	}

	void SetPointPosAtCerat()
	{
		point.setPosition(textLabel.getPosition() + sf::Vector2f{10.f, 5 + caret * 20.f});
	}
};
