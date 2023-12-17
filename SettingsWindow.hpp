#pragma once

#include<SFML\Graphics.hpp>

#include"KeyBoard.hpp"
#include"Render.hpp"

class SettingsMenu
{
public:
	SettingsMenu(KeyBoard& kb, Render& r) : keyboard{ kb }, render{r}
	{
	}

	void Run()
	{
		render.SetDefaultCenterPos();

		while (render.IsOpen())
		{
			DrawAll();
			
			GetEvent();

			TickUpdate();
		}
	}


private:
	KeyBoard& keyboard;
	Render& render;

private://методы

	void DrawAll()
	{
		render.Clear();




		render.FinalizeRender();
	}

	void GetEvent()
	{

	}

	void TickUpdate()
	{

	}
};