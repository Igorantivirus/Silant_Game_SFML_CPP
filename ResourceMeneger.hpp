#pragma once

#include<SFML/Graphics.hpp>

#define MAIN_GAME_FONT "Settings/constan.ttf"
#define MONO_GAME_FONT "Settings/consola.ttf"

#define DIALOGE_TEXTURE_PATH "Textures/PlayerSprites.png"
#define PLAYER_TEXTURE_PATH "Textures/Player.png"

#define OBJECTS_TEXTUR_FILE "Textures\\Objects.png"

class ResourceMeneger
{
public:
	ResourceMeneger()
	{
		gameFont.loadFromFile(MAIN_GAME_FONT);
		settingsFont.loadFromFile(MONO_GAME_FONT);

		dialogesTexture.loadFromFile(DIALOGE_TEXTURE_PATH);
		playerTexture.loadFromFile(PLAYER_TEXTURE_PATH);
		objectsTexture.loadFromFile(OBJECTS_TEXTUR_FILE);
	}

	const sf::Font& GetMainFont() const
	{
		return gameFont;
	}
	const sf::Font& GetMainMonoFont() const
	{
		return settingsFont;
	}

	sf::Sprite GetInventorySprite() const
	{
		sf::Sprite res;
		res.setTexture(dialogesTexture);
		res.setTextureRect({ 0,0,300,300 });
		res.setScale(0.5f, 0.5f);
		return res;
	}
	sf::Sprite GetDiaogeWindowSprite() const
	{
		sf::Sprite res;
		res.setTexture(dialogesTexture);
		res.setTextureRect({ 0, 300, 195, 70 });
		return res;
	}

	const sf::Texture& GetPlayerTexture() const
	{
		return playerTexture;
	}
	const sf::Texture& GetObjectsTexture() const
	{
		return objectsTexture;
	}

private:
	sf::Font gameFont;
	sf::Font settingsFont;

	sf::Texture dialogesTexture;
	sf::Texture playerTexture;
	sf::Texture objectsTexture;

};