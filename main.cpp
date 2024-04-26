#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>
using namespace sf;

class Bullet{
    public:
        CircleShape shape;
        Vector2f currVelocity;
        float maxSpd;
        Bullet(float radius = 5.0f)
        :currVelocity(0.0f,0.0f),maxSpd(15.0f){
            this->shape.setRadius(radius);
            this->shape.setFillColor(Color::Red);
        };

};
class Enemy{
    public:
        RectangleShape shape;
        Vector2f currVelocity;
        float maxSpd;
        Enemy(float radius = 5.0f)
        :currVelocity(0.0f,0.0f),maxSpd(15.0f){
            this->shape.setSize(Vector2f(20.f,20.f));
            this->shape.setFillColor(Color::Magenta);
        };

};

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(800, 600), "pew pew!");
    window.setFramerateLimit(60);

    CircleShape player(25.0f);
    player.setFillColor(Color::White);

    Bullet b1(5);
    std::vector<Bullet> bullets;

    Enemy e1(5);
    int spawnCounter = 20;

    std::vector<Enemy> enemies;
  

    Vector2f playerCenter;
    Vector2f mousePosWindow;
    Vector2f aimDir;
    Vector2f aimDirNorm;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //update
        playerCenter = Vector2f(player.getPosition().x + player.getRadius(),player.getPosition().y + player.getRadius());
        mousePosWindow = Vector2f(Mouse::getPosition(window));
        aimDir = mousePosWindow - playerCenter;
        aimDirNorm =  Vector2f(aimDir.x / sqrt(pow(aimDir.x,2) + pow(aimDir.y,2)),aimDir.y / sqrt(pow(aimDir.x,2) + pow(aimDir.y,2)));

        if(Keyboard::isKeyPressed(Keyboard::W)){
            player.move(0,-10.f);
        }
        if(Keyboard::isKeyPressed(Keyboard::A)){
            player.move(-10.0f,0);
        }
        if(Keyboard::isKeyPressed(Keyboard::S)){
            player.move(0,10.0f);
        }
        if(Keyboard::isKeyPressed(Keyboard::D)){
            player.move(10.0f,0);
        }
        std::cout << player.getPosition().x << " " << player.getPosition().y << std::endl;
        if(spawnCounter < 20){
            spawnCounter++;
        }
        if(spawnCounter >= 20 && enemies.size() < 20){
            e1.shape.setPosition(Vector2f(rand()%window.getSize().x,rand()%window.getSize().y));
            enemies.push_back(Enemy(e1));
            // std::cout << enemies.size() << std::endl;
            spawnCounter = 0;
        }
        
        if(Mouse::isButtonPressed(Mouse::Left)){
            
            b1.shape.setPosition(playerCenter);
            b1.currVelocity = aimDirNorm * b1.maxSpd;

            bullets.push_back(Bullet(b1));
        }
        for(size_t i = 0;i < bullets.size();i++){
            bullets[i].shape.move(bullets[i].currVelocity);
            if(bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x 
            || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y){
                bullets.erase(bullets.begin() + i);
            }else{
                for(size_t j = 0; j < enemies.size();j++){
                    if(bullets[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds())){
                        bullets.erase(bullets.begin() + i);
                        enemies.erase(enemies.begin() + j);
                        
                    }
                }
            }
            
        }

        window.clear();
        for(size_t i = 0; i < enemies.size();i++){
            window.draw(enemies[i].shape);
        }
        window.draw(player);
        for(size_t i = 0; i < bullets.size();i++){
            window.draw(bullets[i].shape);

        }
        
        

        
        window.display();
    }

    return 0;
}