#pragma once

#include<SFML\Graphics.hpp>

#include"Render.hpp"

#define DIALOGE_PATH "Sprites\\Text.png"
#define FONT_PATH "constan.ttf"

class DialogeWindow
{
public:
	DialogeWindow()
	{
		dialogeTexture.loadFromFile(DIALOGE_PATH);
		sprite.setTexture(dialogeTexture);

		sf::Font font;
		font.loadFromFile(FONT_PATH);

		gameFont.loadFromFile(FONT_PATH);
		textLabel.setCharacterSize(10);
		textLabel.setFillColor(sf::Color::Black);
		textLabel.setFont(gameFont);
	}

	void SetText(const sf::String& txt)
	{
		str = txt;
		ind = 0;
		textLabel.setString("");
	}

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

	bool IsFullEnter() const
	{
		return ind >= str.getSize();
	}
	bool IsWaiting() const
	{
		return waitNext;
	}

	void SetPositionAtWindow(sf::Vector2f posCenter)
	{
		posCenter.x -= sprite.getTextureRect().width / 2.f;
		posCenter.y -= sprite.getTextureRect().height + 10;

		sprite.setPosition(posCenter);
		textLabel.setPosition(sprite.getPosition() + sf::Vector2f{10, 10});
	}

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
		window.draw(textLabel);
	}

private:
	sf::Texture dialogeTexture;
	sf::Sprite sprite;

	sf::FloatRect textRect{0, 0, 175, 50};

	sf::Font gameFont;
	sf::Text textLabel;
	sf::String str;
	size_t ind = 0;

	bool waitNext = false;

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

};