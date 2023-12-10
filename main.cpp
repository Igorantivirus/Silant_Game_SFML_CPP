#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include"Player.hpp"
#include"Map.hpp"

int main()
{
    GameMap map;
    map.LoadFromFile(Location::Default, 4);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), L"Хрень какая-то");
    sf::View view = window.getDefaultView();
    
    view.zoom(0.25);

    Player silant{ "Sprites\\Player.png", sf::IntRect{0, 0, 16, 34} };
    silant.SetSpeed(0.15);

    sf::Event event;
    int tick = 0;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{0.f, silant.GetSpeed()}))
            {
                silant.Up();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{silant.GetSpeed(), 0.f}))
                silant.Left();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{0.f, silant.GetSpeed()}))
                silant.Down();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{silant.GetSpeed(), 0.f}))
                silant.Right();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            silant.StopRun();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            silant.SetAspirCenterFootPos(rand() % 200, rand() % 200);
        silant.UpdatePosition();
        
        window.clear();
        view.setCenter(silant.GetCenterPosition());
        window.setView(view);
        map.Draw(window);
        silant.Draw(window);
        window.display();
        tick++;
        if(tick % 100 == 0)
            silant.UpdateAnum();
        if (tick > 1000000)
            tick = 0;
    }

    return 0;
}






/*
int main()
{
    srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode(800, 600), L"Хрень какаято");

    sf::RectangleShape statS({500,100});

    //sf::CircleShape statS(100.f);
    statS.setFillColor(sf::Color::Blue);
    statS.setPosition(200, 200);

    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition({800,800});

    sf::Vector2f pos = {200, 200};
    sf::Vector2f oldPos;
    sf::Vector2f prPos;

    prPos = shape.getPosition();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            oldPos = pos = prPos = shape.getPosition();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            shape.setPosition(50, 50);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            prPos.y = sf::Mouse::getPosition().y;
            prPos.x = sf::Mouse::getPosition().x;
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            prPos.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            prPos.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            prPos.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            prPos.x += 1;

        oldPos = shape.getPosition();
        pos = oldPos;

        pos.x = prPos.x;

        shape.move((pos - oldPos) * 0.001f);
        if (statS.getGlobalBounds().intersects(shape.getGlobalBounds()))
        {
            shape.setPosition(oldPos);
            prPos.x = oldPos.x;
        }

        oldPos = shape.getPosition();
        pos = oldPos;

        pos.y = prPos.y;

        shape.move((pos - oldPos) * 0.001f);
        if (statS.getGlobalBounds().intersects(shape.getGlobalBounds()))
        {
            shape.setPosition(oldPos);
            prPos.y = oldPos.y;
        }

        window.clear();
        window.draw(shape);
        window.draw(statS);
        window.display();
    }

    return 0;
}*/