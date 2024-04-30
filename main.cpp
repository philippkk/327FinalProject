#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <string>
#define MAPWIDTH 20
#define MAPHEIGHT 12
#define SCREENWIDTH 1280
#define SCREENHEIGHT 768
using namespace sf;
class Player;
class Enemy;

/*
todo: 
    text score and lives
    decide on melee vs projectile attack
    power ups
    

    death screen

*/



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
        this->shape.setFillColor(Color::Cyan);
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
    float spd;
    float drag;
    int dir;
    int health;
    float moveRate;

    
    Enemy(float radius = 5.0f)
        : currVelocity(0.0f, 0.0f), maxSpd(15.0f),dir(0)
    {
        

        this->texture.loadFromFile("fishing_full/global.png");
        this->rect = IntRect(0,0,16,16);
        
        Sprite s(texture,rect);
        this->sprite = s;
        sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height) / 2.f);
        //this->sprite.rotate(-45.f);
        this->sprite.setScale(4,4);
        this->shape.setSize(Vector2f(20.f, 20.f));
        this->shape.setFillColor(Color::Magenta);

        this->spd = 12.05f;
        this->drag = 1.0f;
    };

    void HandleMovement(Player *player){
        //need player dir
        Vector2f playerDir = player->sprite.getPosition() - sprite.getPosition();
        playerDir.x +=  player->sprite.getGlobalBounds().width/2;
        playerDir.y +=  player->sprite.getGlobalBounds().height/2;
        Vector2f playerDirNorm = Vector2f(playerDir.x / sqrt(pow(playerDir.x, 2) + pow(playerDir.y, 2)), playerDir.y / sqrt(pow(playerDir.x, 2) + pow(playerDir.y, 2)));
        //give a burst of speed in that direction
         if (clock.getElapsedTime().asSeconds() > moveRate)
        {
            currVelocity = (playerDirNorm * spd);
            clock.restart();
        }
        if(currVelocity.x > maxSpd){
            currVelocity.x = maxSpd;
        }
        if(currVelocity.x < -maxSpd){
            currVelocity.x = -maxSpd;
        }
        if(currVelocity.y > maxSpd){
            currVelocity.y = maxSpd;
        }
        if(currVelocity.y < -maxSpd){
            currVelocity.y = -maxSpd;
        }
        if(currVelocity.x > drag){currVelocity.x -= drag;}

        if(currVelocity.x < -drag){currVelocity.x += drag;}

        if(currVelocity.y > drag){currVelocity.y -= drag;}

        if(currVelocity.y < -drag){currVelocity.y += drag;}

        if(currVelocity.x < drag && currVelocity.x > -drag){
            currVelocity.x = 0;
        }
        if(currVelocity.y < drag && currVelocity.y > -drag){
            currVelocity.y = 0;
        }
        
        sprite.move(currVelocity);
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

class PowerUp{
    public:
        Texture texture;
        Sprite sprite;
        IntRect rect;
        Clock clock;
        int type;
        //0 is 20/20
        //1 is upshot rate
        //2 is up damage
        //3 is up spd
        PowerUp(Texture *t, IntRect r)
        {
            
            texture = *t;
            rect = r;
            Sprite s(*t, r);
            sprite = s;
            sprite.setScale(4, 4);
        }
        PowerUp(){}
        void animate(int max, int size, float mili)
        {
            if (clock.getElapsedTime().asMilliseconds() > mili)
            {
                if (rect.left >= max)
                {
                    rect.left = max - 16;
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


Tile tiles[MAPWIDTH][MAPHEIGHT];
int tileIndex[MAPWIDTH][MAPHEIGHT];
bool isPlayerMoving = false;
FPS fps;
int dir = 0;

Clock timeCounterClock;
int timeAlive = 0;

void handlePlayerMovement(Player *player);

String toString(Int32 integer)
{
    char numstr[10]; // enough to hold all numbers up to 32-bits
    snprintf(numstr, 10,"%i", integer);
    return numstr;
}

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
    
    Texture global;
    global.loadFromFile("fishing_full/global.png");
    IntRect powerUpRect(480,272,16,16);
    PowerUp power(&global,powerUpRect);
    std::vector<PowerUp> powerUps;
    Clock powerUpClock;

    Player player(25.0f);
player.sprite.setPosition(Vector2f(SCREENWIDTH/2,SCREENHEIGHT/2));
    Bullet b1(10);
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


    Font font;
    font.loadFromFile("fonts/Minecraft.ttf");

    int score = 0;
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setPosition(10,10);
    scoreText.setOutlineThickness(2.f);
    scoreText.setOutlineColor(Color::Black);
    Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(40);
    timeText.setPosition(10,60);
    timeText.setOutlineThickness(2.f);
    timeText.setOutlineColor(Color::Black);
    

    Clock shotTimer;
    float shootDelay = 0.0f;
    int damage = 1;
    int numShots = 15;

    bool playerAlive = true;
    Text deathText;
    deathText.setFont(font);
    deathText.setCharacterSize(40);
    deathText.setPosition(SCREENWIDTH/3,SCREENHEIGHT/3);
    deathText.setOutlineThickness(2.f);
    deathText.setOutlineColor(Color::Red);
    deathText.setString("you're dead lol \n press R to restart i think");
    while (window.isOpen())
    {
        if(powerUps.size() < 3){
            if(powerUpClock.getElapsedTime().asSeconds() > 5){
                //spawn powerup
                int num = rand() % 4;
                if(num == 0){
                    power.type = 0;
                    power.rect.top = 272;
                    power.sprite.setTextureRect(power.rect);
                }else if (num == 1){
                    power.type = 1;
                    power.rect.top = 288;
                    power.sprite.setTextureRect(power.rect);
                }else if(num ==2){
                    power.type = 2;
                    power.rect.top = 304;
                    power.sprite.setTextureRect(power.rect);
                }else if(num == 3){
                    power.type = 3;
                    power.rect.top = 320;
                    power.sprite.setTextureRect(power.rect);
                }
                power.sprite.setPosition(rand()%(SCREENWIDTH-100)+100,rand()%(SCREENHEIGHT-100) + 100);
                powerUps.push_back(power);
                powerUpClock.restart();
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //fps clock
        fps.update();
        std::cout<<fps.getFPS()<<std::endl;
        std::cout<<powerUps.size()<<std::endl;
        if (timeCounterClock.getElapsedTime().asSeconds() > 1)
        {   
            if(playerAlive){
                timeAlive += 1;
                score += 1;
            }
            timeCounterClock.restart();
        }

        // update
        playerCenter = Vector2f(player.sprite.getPosition().x + 40, player.sprite.getPosition().y + 40);
        mousePosWindow = Vector2f(Mouse::getPosition(window));
        aimDir = mousePosWindow - playerCenter;
        aimDirNorm = Vector2f(aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)), aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

        if (window.hasFocus())
        {
            if(playerAlive){
            handlePlayerMovement(&player);

            if (Mouse::isButtonPressed(Mouse::Left))
            {

                b1.shape.setPosition(playerCenter);
                b1.currVelocity = aimDirNorm * b1.maxSpd;

                if (shotTimer.getElapsedTime().asSeconds() > shootDelay){
                    bool up = false;
                    for (int i = 0; i < numShots; i++)
                    {
    
                        bullets.push_back(Bullet(b1));
                    }
                    shotTimer.restart();
                }            
            }

            if (Keyboard::isKeyPressed(Keyboard::I))
            {
                b1.shape.setPosition(playerCenter);
                b1.currVelocity = Vector2f(0.f, -1.f) * b1.maxSpd;
                if (shotTimer.getElapsedTime().asSeconds() > shootDelay)
                {
                    bool up = false;
                    for (int i = 0; i < numShots; i++)
                    {
                        if (!up)
                        {
                            up = true;
                            b1.shape.setPosition(Vector2f(playerCenter.x + 10 + (i * 10), playerCenter.y));
                        }
                        else
                        {
                            up = false;
                            b1.shape.setPosition(Vector2f(playerCenter.x - (i * 10), playerCenter.y));
                        }
                        bullets.push_back(Bullet(b1));
                    }
                    shotTimer.restart();
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::J))
            {
                b1.shape.setPosition(playerCenter);
                b1.currVelocity = Vector2f(-1, 0) * b1.maxSpd;
                if (shotTimer.getElapsedTime().asSeconds() > shootDelay)
                {
                    bool up = false;
                    for (int i = 0; i < numShots; i++)
                    {
                        if (!up)
                        {
                            up = true;
                            b1.shape.setPosition(Vector2f(playerCenter.x, playerCenter.y + 10 + (i * 10)));
                        }
                        else
                        {
                            up = false;
                            b1.shape.setPosition(Vector2f(playerCenter.x, playerCenter.y - (i * 10)));
                        }
                        bullets.push_back(Bullet(b1));
                    }
                    shotTimer.restart();
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::K))
            {
                b1.shape.setPosition(playerCenter);
                b1.currVelocity = Vector2f(0, 1) * b1.maxSpd;
                if (shotTimer.getElapsedTime().asSeconds() > shootDelay)
                {
                    bool up = false;
                    for (int i = 0; i < numShots; i++)
                    {
                        if (!up)
                        {
                            up = true;
                            b1.shape.setPosition(Vector2f(playerCenter.x + 10 + (i * 10), playerCenter.y));
                        }
                        else
                        {
                            up = false;
                            b1.shape.setPosition(Vector2f(playerCenter.x - (i * 10), playerCenter.y));
                        }
                        bullets.push_back(Bullet(b1));
                    }
                    shotTimer.restart();
                }
            }
            if(Keyboard::isKeyPressed(Keyboard::L)){
                b1.shape.setPosition(playerCenter);
                b1.currVelocity = Vector2f(1,0) * b1.maxSpd;

                if (shotTimer.getElapsedTime().asSeconds() > shootDelay){
                    bool up = false;
                    for(int i = 0 ;i < numShots;i++){
                        if(!up){
                            up = true;
                            b1.shape.setPosition(Vector2f(playerCenter.x ,playerCenter.y + 10+ (i * 10)));
                        }else{
                            up = false;
                            b1.shape.setPosition(Vector2f(playerCenter.x ,playerCenter.y- (i * 10)));
                        }
                        bullets.push_back(Bullet(b1));
                    }
                    
                    shotTimer.restart();
                }            
            }
            }
            for (size_t i = 0; i < powerUps.size(); i++)
            {
                if (powerUps[i].sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
                {
                    if(powerUps[i].type == 0){
                        numShots++;
                    }else if(powerUps[i].type == 1){
                        if(shootDelay > 0.04f)
                            shootDelay -= 0.04f;
                    }else if(powerUps[i].type == 2){
                        damage+=2;
                    }else if(powerUps[i].type == 3){
                        player.maxSpd += 0.25f;
                    }
                    powerUps.erase(powerUps.begin() + i);
                }
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
                            Vector2f playerDir = player.sprite.getPosition() - enemies[j].sprite.getPosition();
                            playerDir.x +=  player.sprite.getGlobalBounds().width/2;
                            playerDir.y +=  player.sprite.getGlobalBounds().height/2;
                            Vector2f playerDirNorm = Vector2f(playerDir.x / sqrt(pow(playerDir.x, 2) + pow(playerDir.y, 2)), playerDir.y / sqrt(pow(playerDir.x, 2) + pow(playerDir.y, 2)));
                            enemies[j].currVelocity += (-1.f * playerDirNorm) * 10.f;
                            enemies[j].health -= damage;
                            if(enemies[j].health < 0){
                                enemies.erase(enemies.begin() + j);
                                score += 25;

                            }
                            break;
                        }
                    }
                }
            }
        }
         for (size_t i = 0; i < enemies.size(); i++){
            FloatRect prect(player.sprite.getGlobalBounds().left + 20,player.sprite.getGlobalBounds().top  + 20,player.sprite.getGlobalBounds().width - 40, player.sprite.getGlobalBounds().height-40);
             if (enemies[i].sprite.getGlobalBounds().intersects(prect))
             {
                playerAlive = false;
             }
         }
        // std::cout << player.shape.getPosition().x << " " << player.shape.getPosition().y << std::endl;


        if(!playerAlive){
            if(Keyboard::isKeyPressed(Keyboard::R)){
                score = 0;
                timeAlive = 0;
                numShots = 1;
                damage = 1;
                player.maxSpd = 8;
                powerUps.clear();
                enemies.clear();
                playerAlive = true;
                shootDelay = 0.25f;
            }
        }
        if (spawnCounter < 20)
        {
            spawnCounter++;
        }
        if (spawnCounter >= 20 && enemies.size() < round(timeAlive * 0.2))
        {
            int possiblePos[10][2] = {
            {800,0},{816,0},{832,0},{880,0},{832,16},
            {752,16},{864,32},{800,48},{816,48},{848,48}
            };
            int num = rand() % 10;
            e1.rect.left = possiblePos[num][0];
            e1.rect.top = possiblePos[num][1];
            e1.sprite.setTextureRect(e1.rect);

            int sideOrTop = rand()%100;
            int x,y;
            if(sideOrTop > 50){
                //spawn on side
                int leftOrRight = rand()%100;
                if(leftOrRight > 50){
                    x = 0;
                } else{
                    x = SCREENWIDTH;
                }
                y = rand()%SCREENHEIGHT;
            }else{
                //spawn on top/bottom
                int topOrBottom = rand()%100;
                if(topOrBottom > 50){
                    y = 0;
                } else{
                    y = SCREENHEIGHT;
                }
                x = rand()%SCREENWIDTH;
            }
            e1.moveRate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.75f)) + 0.25f;

            e1.health = 5 + round((timeAlive/2));
            e1.sprite.setPosition(Vector2f(x, y));
            
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
                 //enemies[i].sprite.rotate(90.f);
            }else if(enemies[i].sprite.getPosition().x > player.sprite.getPosition().x && enemies[i].dir == 1){
                enemies[i].dir = 0;
                enemies[i].sprite.scale(-1.f,1.f);
                 //enemies[i].sprite.rotate(-90.f);

            }   
            enemies[i].HandleMovement(&player);
            window.draw(enemies[i].sprite);
        }


        

        window.draw(player.sprite);
        for (size_t i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i].shape);
        }
        for (size_t i = 0; i < powerUps.size(); i++)
        {   
            
            powerUps[i].animate(496,16,500);
            window.draw(powerUps[i].sprite);
        }
        scoreText.setString("score: " + toString(score));
        timeText.setString("Time alive: "+ toString(timeAlive));	 
        window.draw(scoreText);
        window.draw(timeText);

        if(!playerAlive){
            window.draw(deathText);
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
    if (!isAnyKeyPressed() || (!Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::D)))
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