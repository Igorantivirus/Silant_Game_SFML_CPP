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
		textLabel.setFont(gameFont);
		textLabel.setFillColor(sf::Color::Black);
		textLabel.setCharacterSize(10);
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
			textLabel.setString(textLabel.getString() + str[ind++]);



		if (textLabel.getGlobalBounds().width >= textRect.width)
		{
			auto pr = textLabel.getString();

			for(size_t i = pr.getSize(); i > 0; --i)
				if (pr[i - 1] == ' ')
				{
					pr[i - 1] = '\n';
					break;
				}
			textLabel.setString(pr);
		}
		if (textLabel.getGlobalBounds().height >= textRect.height)
		{
			waitNext = true;
			auto pr2 = textLabel.getString();
			for (size_t i = pr2.getSize(); i > 0; --i, --ind)
				if (pr2[i - 1] == ' ')
				{
					pr2.erase(i - 1, pr2.getSize() - i + 1);
					break;
				}
			textLabel.setString(pr2);
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

};
