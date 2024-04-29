#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>

#define MAPWIDTH 20
#define MAPHEIGHT 12
#define SCREENWIDTH 1280
#define SCREENHEIGHT 768
using namespace sf;
class Bullet
{
public:
    CircleShape shape;
    Vector2f currVelocity;
    float maxSpd;
    Bullet(float radius = 5.0f)
        : currVelocity(0.0f, 0.0f), maxSpd(15.0f)
    {
        this->shape.setRadius(radius);
        this->shape.setFillColor(Color::Red);
    };
};
class Enemy
{
public:
    RectangleShape shape;
    Texture texture;
    Sprite sprite;
    IntRect rect;
    Clock clock;
    Vector2f currVelocity;
    float maxSpd;
    int dir;
    Enemy(float radius = 5.0f)
        : currVelocity(0.0f, 0.0f), maxSpd(15.0f),dir(0)
    {
        this->texture.loadFromFile("fishing_full/global.png");
        this->rect = IntRect(0,0,16,16);
        this->rect.left = 768;
        Sprite s(texture,rect);
        this->sprite = s;
        sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height) / 2.f);
        this->sprite.rotate(-45.f);
        this->sprite.setScale(4,4);
        this->shape.setSize(Vector2f(20.f, 20.f));
        this->shape.setFillColor(Color::Magenta);
    };
};
class Player
{
public:
    CircleShape shape;
    Texture texture;
    Sprite sprite;
    IntRect rect;
    Clock clock;
    Vector2f currVelocity;
    float drag;
    float maxSpd;
    float spd;
    Player(float radius = 5.f)
        : currVelocity(0.0f, 0.0f), maxSpd(8.f)
    {
        this->texture.loadFromFile("player.png");
        this->rect = IntRect(0,0,20,20);
        Sprite s(texture,rect);
        this->sprite = s;
        this->sprite.setScale(4,4);
        this->shape.setRadius(radius);
        
        this->shape.setFillColor(Color::White);
        this->drag = 0.55f;
        this->spd = 1.15f;
    }
     void animate(int max, int size, float mili,int dir)
    {
        if(dir == 1){
            rect.top = 21;
        }else{
            rect.top = 0;
        }
        //will need to check direction
        if (clock.getElapsedTime().asMilliseconds() > mili)
        {
            if (rect.left == max)
            {
                rect.left = 0;
            }
            else
            {
                rect.left += size;
            }
            sprite.setTextureRect(rect);
            clock.restart();
        }
    }
};


class Tile
{
public:
    Texture texture;
    Sprite sprite;
    IntRect rect;
    Clock clock;
    Tile(Texture *t, IntRect r)
    {
        texture = *t;
        rect = r;
        Sprite s(*t, r);
        sprite = s;
        sprite.setScale(4, 4);
    }
    Tile()
    {
    }
    void animate(int max, int size, float mili)
    {
        if (clock.getElapsedTime().asMilliseconds() > mili)
        {
            if (rect.left == max)
            {
                rect.left = 0;
            }
            else
            {
                rect.left += size;
            }
            sprite.setTextureRect(rect);
            clock.restart();
        }
    }
};
class FPS
{ 
public:
	/// @brief Constructor with initialization.
	///
	FPS() : mFrame(0), mFps(0) {}

	/// @brief Update the frame count.
	/// 


	/// @brief Get the current FPS count.
	/// @return FPS count.
	const unsigned int getFPS() const { return mFps; }

private:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;

public:
	void update()
	{
		if(mClock.getElapsedTime().asSeconds() >= 1.f)
		{
			mFps = mFrame;
			mFrame = 0;
			mClock.restart();
		}
 
		++mFrame;
	}
};
Tile tiles[MAPWIDTH][MAPHEIGHT];
int tileIndex[MAPWIDTH][MAPHEIGHT];
bool isPlayerMoving = false;
FPS fps;
int dir = 0;


void handlePlayerMovement(Player *player);
int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(SCREENWIDTH, SCREENHEIGHT), "pew pew!");
    window.setFramerateLimit(60);

    // sprites
    Texture texture;
    texture.loadFromFile("fishing_full/Fish_Forage_Items/fish_shadow.png");
    IntRect rectSourceSprite(0, 0, 16, 16);
    Sprite s(texture, rectSourceSprite);
    Tile fish(&texture, rectSourceSprite);
    Texture mapTexture;
    mapTexture.loadFromFile("327map.png");
    Sprite mapSprite(mapTexture);
    mapSprite.setScale(4, 4);

    Player player(25.0f);

    Bullet b1(5);
    std::vector<Bullet> bullets;

    Enemy e1(5);
    int spawnCounter = 20;

    std::vector<Enemy> enemies;

    Vector2f playerCenter;
    Vector2f mousePosWindow;
    Vector2f aimDir;
    Vector2f aimDirNorm;

    for (int i = 0; i < MAPWIDTH; i++)
    {
        for (int j = 0; j < MAPHEIGHT; j++)
        {
            fish.rect.left = 0;
            fish.sprite.setPosition(Vector2f(i * 64, j * 64));
            int num = rand() % 10;
            num *= 16;
            fish.rect.left += num;
            tiles[i][j] = Tile(fish);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //fps clock
        fps.update();
        std::cout<<fps.getFPS()<<std::endl;

        // update
        playerCenter = Vector2f(player.sprite.getPosition().x + 40, player.sprite.getPosition().y + 40);
        mousePosWindow = Vector2f(Mouse::getPosition(window));
        aimDir = mousePosWindow - playerCenter;
        aimDirNorm = Vector2f(aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)), aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

        if (window.hasFocus())
        {
            handlePlayerMovement(&player);

            if (Mouse::isButtonPressed(Mouse::Left))
            {

                b1.shape.setPosition(playerCenter);
                b1.currVelocity = aimDirNorm * b1.maxSpd;

                bullets.push_back(Bullet(b1));
            }
            for (size_t i = 0; i < bullets.size(); i++)
            {
                bullets[i].shape.move(bullets[i].currVelocity);
                if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y)
                {
                    bullets.erase(bullets.begin() + i);
                }
                else
                {
                    for (size_t j = 0; j < enemies.size(); j++)
                    {
                        if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds()))
                        {
                            bullets.erase(bullets.begin() + i);
                            enemies.erase(enemies.begin() + j);
                        }
                    }
                }
            }
        }
        // std::cout << player.shape.getPosition().x << " " << player.shape.getPosition().y << std::endl;

        if (spawnCounter < 20)
        {
            spawnCounter++;
        }
        if (spawnCounter >= 20 && enemies.size() < 20)
        {
            e1.sprite.setPosition(Vector2f(rand() % window.getSize().x, rand() % window.getSize().y));
            
            enemies.push_back(Enemy(e1));
            // std::cout << enemies.size() << std::endl;
            spawnCounter = 0;
        }

        window.clear();
        for (int i = 0; i < MAPWIDTH; i++)
        {
            for (int j = 0; j < MAPHEIGHT; j++)
            {
                tiles[i][j].animate(352, 16, 100);
                window.draw(tiles[i][j].sprite);
            }
        }
        window.draw(mapSprite);
        for (size_t i = 0; i < enemies.size(); i++)
        {
            if(enemies[i].sprite.getPosition().x < player.sprite.getPosition().x && enemies[i].dir == 0){
                enemies[i].sprite.scale(-1.f,1.f);
                enemies[i].dir = 1;
                 enemies[i].sprite.rotate(90.f);
            }else if(enemies[i].sprite.getPosition().x > player.sprite.getPosition().x && enemies[i].dir == 1){
                enemies[i].dir = 0;
                enemies[i].sprite.scale(-1.f,1.f);
                 enemies[i].sprite.rotate(-90.f);

            }   
            window.draw(enemies[i].sprite);
        }


        window.draw(player.sprite);
        for (size_t i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i].shape);
        }
        	 

        window.display();
    }

    return 0;
}

bool isAnyKeyPressed()
{
    for (int k = -1; k < sf::Keyboard::KeyCount; ++k)
    {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
            return true;
    }
    return false;
}

void handlePlayerMovement(Player *player)
{
    // clamp
    if (player->currVelocity.x > player->maxSpd)
    {
        player->currVelocity.x = player->maxSpd;
    }
    else if (player->currVelocity.x < -player->maxSpd)
    {
        player->currVelocity.x = -player->maxSpd;
    }
    if (player->currVelocity.y > player->maxSpd)
    {
        player->currVelocity.y = player->maxSpd;
    }
    else if (player->currVelocity.y < -player->maxSpd)
    {
        player->currVelocity.y = -player->maxSpd;
    }
    // drag
    if (player->currVelocity.x > 0)
    {
        player->currVelocity.x -= player->drag;
    }
    else if (player->currVelocity.x < 0)
    {
        player->currVelocity.x += player->drag;
    }
    if (player->currVelocity.y > 0)
    {
        player->currVelocity.y -= player->drag;
    }
    else if (player->currVelocity.y < 0)
    {
        player->currVelocity.y += player->drag;
    }
    // move
    player->sprite.move(player->currVelocity);
    // low vel check

    if (!isPlayerMoving)
    {
        if ((player->currVelocity.x > -0.5f && player->currVelocity.x < 0) || (player->currVelocity.x < 0.5f && player->currVelocity.x > 0))
        {
            player->currVelocity.x = 0;
        }
        if ((player->currVelocity.y > -0.5f && player->currVelocity.y < 0) || (player->currVelocity.y < 0.5f && player->currVelocity.y > 0))
        {
            player->currVelocity.y = 0;
        }
    }
    if (!isAnyKeyPressed())
    {
        isPlayerMoving = false;
        player->rect.left = 0;
        player->sprite.setTextureRect(player->rect);
    }

    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        player->currVelocity.y -= player->spd;
        isPlayerMoving = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        player->currVelocity.x -= player->spd;
        isPlayerMoving = true;
        dir = 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        player->currVelocity.y += player->spd;
        isPlayerMoving = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        player->currVelocity.x += player->spd;
        isPlayerMoving = true;
        dir = 1;
    }

    if(isPlayerMoving)
        player->animate(20,20,200,dir);
}