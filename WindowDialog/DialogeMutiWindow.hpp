#pragma once

#include"DIalogeWindow.hpp"
#include"InventoryWindow.hpp"

#define FONT_FILE "WindowDialog\\constan.ttf"
#define TEXTURE_PATH "WindowDialog\\PlayerSprites.png"

class MultiDialogeWindow
{
public:
	MultiDialogeWindow(PersonInfo& info) : invent{ info }, dialoge{}
	{
		texture.loadFromFile(TEXTURE_PATH);
		font.loadFromFile(FONT_FILE);

		invent.Init(texture, font);
		dialoge.Init(texture, font);
	}

	void RunDialoge(const sf::String& txt)
	{
		invent.Stop();
		dialoge.Run(txt);
	}
	void RunAsk(const sf::String& txtA, const sf::String& txtB)
	{
		invent.Stop();
		dialoge.Ask(txtA, txtB);
	}
	void RunInventory()
	{
		invent.Run();
		dialoge.Stop();
	}

	short GetCaretResult() const
	{
		return dialoge.GetKaretResult();
	}

	bool IsActive() const
	{
		return dialoge.IsAvtive() || invent.IsActive();
	}
	bool IsActiveDialoge() const
	{
		return dialoge.IsAvtive();
	}
	bool IsActiveInventory() const
	{
		return invent.IsActive();
	}

	void SetPosAtCenter(const sf::Vector2f& pos, const sf::Vector2u& size)
	{
		if (dialoge.IsAvtive())
			dialoge.SetPositionAtWindow(pos + sf::Vector2f{0, size.y / 2.f});
		else if (invent.IsActive())
			invent.SetPositionAtWindow(pos);
	}

	void Update()
	{
		if (dialoge.IsAvtive())
			dialoge.Update();
		else if (invent.IsWait())
		{
			dialoge.Run(invent.GetWaitStr());
			invent.Stop();
		}
	}

	void GetEvent(KeyBoard& keyboard)
	{
		if (dialoge.IsAvtive())
			dialoge.GetEvent(keyboard);
		else if (invent.IsActive())
			invent.GetEvent(keyboard);
	}

	void Draw(sf::RenderWindow& window) const
	{
		if (dialoge.IsAvtive())
			dialoge.Draw(window);
		else if (invent.IsActive())
			invent.Draw(window);
	}

private:
	sf::Texture texture;

	sf::Font font;

	DialogeWindow dialoge;
	InventoryWindow invent;

};