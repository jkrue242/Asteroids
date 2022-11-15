//=======================================
// Name: Joseph Krueger
// Date: 11/09/22
// Homework 7 - Asteroids - CIE
// NOTE: UFOs display at intervals of 15 seconds

// Extra Credit Additions:
// 1) added scoring, lives
// 2) added high score, final score
// 3) added end screen displaying scores
// 4) added a "play again" button
// 5) added sounds for bullets and explosions
// 6) added next level button on end screen
// 7) added more levels - more asteroids spawned
// 8) display level, round, score, lives, number of asteroids
//=======================================
#include <SFML/Graphics.hpp>
#include <ctime>
#include <list>
#include <cmath>
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>

//=======================================
using namespace sf;

// max lives for player
const int MAX_LIVES = 5;

// Window size
const int W = 1200;
const int H = 800;

// Conversion factor from degree to radian
float DEGTORAD = 0.017453f;

//=======================================
// Animation class
//=======================================
class Animation
{
public:
    float Frame, speed;
    Sprite sprite;

    // vector of rectangles representing frames of the game
    std::vector<IntRect> frames;

    /*
     * Function: Constructor
     * Inputs: None
     * Outputs: None
     */
    Animation()
    {
        Frame = 0;
        speed = 0;
    }

    /*
     * Function: Constructor
     * Inputs: Texture object, int (5) representing position of texture rectangle and
     * sub-texture count, float (1) speed of animation
     * Outputs: None
     */
    Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
    {
        // set current frame to 0
        Frame = 0;
        // set speed to input speed
        speed = Speed;

        // looping through the number of frames
        for (int i=0;i<count;i++)
        {
            // adding a rectangle with position (x+i*w, y, w, h) to frames vector
            frames.push_back( IntRect(x+i*w, y, w, h)  );
        }

        // setting texture for sprite to input texture
        sprite.setTexture(t);

        // setting origin of frame to center of screen
        sprite.setOrigin(w/2,h/2);

        // set the partial texture to be the first frame
        sprite.setTextureRect(frames[0]);
    }


    /*
     * Function: Animation::update
     * Inputs: None
     * Outputs: None
     */
    void update()
    {
        // incrementing the current frame by the speed
        Frame += speed;
        // set n to store the length of frames vector
        int n = frames.size();

        // if current frame is larger than the count of frames
        if (Frame >= n)
        {
            // reset frame by n (frame count)
            Frame -= n;
        }

        // if the number of frames is greater than 0
        if (n>0)
        {
            // set the current partial texture to the rectangle stored at the index of current
            // frame in frames vector
            sprite.setTextureRect( frames[int(Frame)] );
        }
    }

    /*
     * Function: Animation::isEnd
     * Inputs: None
     * Outputs: bool
     */
    const bool isEnd()
    {
        // returns true if we are at the end frame (end of sub-texture list),
        // else false
        return Frame+speed>=frames.size();
    }
};

//=======================================
// Entity class
//=======================================
class Entity
{
public:
    /*
     * x, y = position of entity
     * dx, dy = speed in that direction
     * R, angle = size, orientation of entity
     */
    float x,y,dx,dy,R,angle;
    bool life;
    std::string name;

    // each entity stores its own animation
    Animation anim;

    /*
     * Function: Constructor
     * Inputs: None
     * Outputs: None
     */
    Entity()
    {
        // add another life
        life=1;
    }

    /*
     * Function: Entity::settings
     * Inputs: Animation object, int (2) position on screen, float (1) angle of orientation,
     * int (1) radius of object
     * Outputs: None
     */
    virtual void settings(Animation &a,float X,float Y,float Angle=0,int radius=1)
    {
        // setting all member variables to the inputs
        anim = a;
        x=X; y=Y;
        angle = Angle;
        R = radius;
    }

    /*
     * Function: Entity::update
     * Inputs: None
     * Outputs: None
     * Note: Function will be overriden in derived classes
     */
    virtual void update(){};

    /*
     * Function: Entity::draw
     * Inputs: RenderWindow object
     * Outputs: None
     */
    virtual void draw(RenderWindow &app)
    {
        // set the animation object's sprite position to (x,y) and rotation to (angle + 90)
        anim.sprite.setPosition(x,y);
        anim.sprite.setRotation(angle+90);
        // draw the sprite
        app.draw(anim.sprite);

        // create a CircleShape object with radius R
        CircleShape circle(R);
        // Color the circle
        circle.setFillColor(Color(255,0,0,170));
        // set circle position to (x,y)
        circle.setPosition(x,y);
        // set the origin of the object to be (R, R)
        circle.setOrigin(R,R);

        // we aren't drawing the circle
        //app.draw(circle);
    }

    /*
     * Function: Destructor
     * Inputs: None
     * Outputs: None
     */
    virtual ~Entity(){};
};

//=======================================
// asteroid class (inherits from Entity class)
//=======================================
class asteroid: public Entity
{
public:
    static int asteroidCount;
    /*
     * Function: Constructor
     * Inputs: None
     * Outputs: None
     */
    asteroid()
    {
        // sets the speed of the asteroid in x and y direction
        // to a random number between 3-7
        dx=rand()%8-4;
        dy=rand()%8-4;

        // set name to 'asteroid'
        name="asteroid";

        // increment asteroidCount
        asteroidCount+=1;
    }

    /*
     * Function: Destructor
     * Inputs: None
     * Outputs: None
     */
    ~asteroid()
    {
        // decrementing asteroidCount
        asteroidCount-=1;
    }

    /*
     * Function: asteroid::update (overrides Entity::update)
     * Inputs: None
     * Outputs: None
     */
    void update()
    {
        // increment x,y positions of asteroid by the speed in x,y directions
        x+=dx;
        y+=dy;

        // if asteroid is off the right side of screen
        if (x>W)
        {
            // set asteroid x position to left side of screen
            x=0;
        }
        // if asteroid is off the left side of screen
        if (x<0)
        {
            // set asteroid x position to right side of screen
            x=W;
        }
        // if asteroid is off the top of screen
        if (y>H)
        {
            // set asteroid y position to bottom of screen
            y=0;
        }
        // if asteroid is off bottom of screen
        if (y<0)
        {
            // set asteroid y position to top of screen
            y=H;
        }
    }

    /*
     * Function: GetAsteroidCount()
     * Inputs: None
     * Outputs: int(1)
     */
    static int GetAsteroidCount() {return asteroidCount;}
};

// initializing asteroid count to 0
int asteroid::asteroidCount = 0;

//=======================================
// bullet class (inherits from Entity class)
//=======================================
class bullet: public Entity
{
public:

    /*
     * Function: Constructor
     * Inputs: None
     * Outputs: None
     */
    bullet()
    {
        // set name to 'bullet'
        name="bullet";
    }

    /*
     * Function: bullet::update (overrides Entity::update)
     * Inputs: None
     * Outputs: None
     */
    void update()
    {
        // set speed in x, y directions to rectangular representation of polar coordinate
        // multiplied by 6
        dx=cos(angle*DEGTORAD)*6;
        dy=sin(angle*DEGTORAD)*6;
//         angle+=rand()%6-3;

        // increment x, y position by speed in x, y directions
        x+=dx;
        y+=dy;

        // if the bullet is off the right, left, top, or bottom of screen, bullet has no
        // "life" -- bullet does not exist anymore
        if (x>W || x<0 || y>H || y<0)
        {
            life=0;
        }
    }
};
//=======================================
// Ufo class (inherits from Entity class)
//=======================================
class Ufo : public Entity
{
public:

    /*
     * Function: Constructor
     * Inputs: None
     * Outputs: None
     */
    Ufo()
    {
        // set name to 'ufo'
        name = "ufo";
    }

    /*
     * Function: Ufo::settings (overrides Entity::settings)
     * Inputs: int (2) location of ufo,
     */
    void settings(Texture &t)
    {
        ufoTex = t;
        R = 20;
        angle = 0;

        // generate random number, if greater than RAND_MAX, randN is set to 1, else 0
        // source: https://stackoverflow.com/questions/23725511/random-number-picker-between-2-numbers
        int randN = (rand() > RAND_MAX/2) ? 1 : 0;

        // if randN is 0
        if (randN == 0)
        {
            // set x position to left side of screen
            x = 0;
            startSide = 0;
        }
            // if randN is 1
        else
        {
            // set x position to right side of screen
            x = W;
            startSide = 1;
        }

        // set y to a random height between 0 and height of screen
        y = rand()%H;

        // make sure we can see ufo (not off top or bottom)
        if (y <= 15)
        {
            y+=15;
        }
        if (H-y<=15)
        {
            y-=15;
        }
    }

    /*
     * Function: Ufo::update (overrides Entity::update)
     * Inputs: None
     * Outputs: None
     */
    void update()
    {
        // constant horizontal speed
        dx = 3;
        // no vertical speed
        dy = 0;

        // update position
        if (startSide == 0)
        {
            x+=dx;
        }
        else
        {
            x-=dx;
        }
        // if ufo goes off the right side of screen or off the left side of screen
        if (x > W || x < 0)
        {
            life = 0;
        }
    }

    /*
     * Function: Ufo::draw (overrides Entity::draw)
     * Inputs: RenderWindow object
     * Outputs: None
     */
    void draw(RenderWindow &app)
    {
        // create ufo sprite
        Sprite ufoObj;
        ufoObj.setPosition(x,y);
        ufoObj.setRotation(angle);
        ufoObj.setTexture(ufoTex);
        // scale the ufo down
        ufoObj.setScale(0.1, 0.1);
        // draw the sprite
        app.draw(ufoObj);
    }

private:
    Texture ufoTex;
    Sprite sprite;
    int startSide;

};
//=======================================
// Game class
//=======================================
class Game
{
public:
    // stores the current score
    static int currentScore;

    // stores high score
    static int highScore;

    // holds the current level
    static int level;

    // current round count in the level
    static int round;

    // stores rounds per level
    static int rounds;
};

// initialize currentScore to 0
int Game::currentScore = 0;

// initializing highScore to 0
int Game::highScore = 0;

// initializing level to 1
int Game::level = 1;

// initializing round to 1
int Game::round = 1;

// initializing rounds to 2
int Game::rounds = 2;

//=======================================
// player class (inherits from Entity class)
//=======================================
class player: public Entity
{
public:
    // stores players lives
    static int lives;

    // bool that stores whether player is accelerating or decelerating
    bool thrust;

    /*
     * Function: Constructor
     * Inputs: None
     * Outputs: None
     */
    player()
    {
        // set name to "player"
        name="player";
    }

    /*
     * Function: player::update (overrides Entity::update)
     */
    void update()
    {
        // if the player is accelerating
        if (thrust)
        {
            // increase the speed in x, y directions
            dx+=cos(angle*DEGTORAD)*0.2;
            dy+=sin(angle*DEGTORAD)*0.2;
        }
            // if the player is decelerating
        else
        {
            // decrease the speed in x, y directions
            dx*=0.99;
            dy*=0.99;
        }

        // setting max speed of player to 5
        int maxSpeed=5;

        // absolute speed takes into account x, y directional speed
        // (pythagorean theorem)
        float speed = sqrt(dx*dx+dy*dy);
        // if speed is faster than max speed (15)
        if (speed>maxSpeed)
        {
            // reduce dx, dy speed by a factor of maxSpeed/speed (should be close to 1)
            dx *= maxSpeed/speed;
            dy *= maxSpeed/speed;
        }

        // increment x, y position by dx, dy speed
        x+=dx;
        y+=dy;

        // if x position of player is off the right side of screen
        if (x>W)
        {
            // set x position to left side of screen
            x=0;
        }
        // if x position of player if off the left side of screen
        if (x<0)
        {
            // set x position to right side of screen
            x=W;
        }
        // if y position of player is off top of screen
        if (y>H)
        {
            // set y position to bottom of screen
            y=0;
        }
        // if y position of player is off bottom of screen
        if (y<0)
        {
            // set y position to top of screen
            y=H;
        }
    }

};

// initialize lives to max lives
int player::lives = MAX_LIVES;

//=======================================
// button class (inherits from Entity
//=======================================
class button
{
public:
    /*
     * Function: Constructor
     * Inputs: std::string (1), int (3), sf::Vector2i (1)
     * Outputs: None
     */
    button(std::string t, float x_pos, float y_pos, sf::Vector2f s):
            text(t), x(x_pos), y(y_pos), size(s)
    {
        // Font: Softers Font by StoryType Studio
        // License: Free for personal use
        // https://www.1001freefonts.com/softers.font
        SetFont("fonts/Softers.otf");

        // set text settings for button
        displayedText.setFont(font);
        displayedText.setString(text);
        displayedText.setFillColor(sf::Color::Green);
        displayedText.setCharacterSize(40);
        displayedText.setOutlineThickness(10);
        displayedText.setPosition(x+15, y+32);
    }

    /*
     * Function: Create
     * Inputs: RenderWindow object
     * Outputs: None
     */
    void Create(RenderWindow &app)
    {
        // set settings of button
        clickButton.setSize(size);
        clickButton.setPosition(x, y);
        clickButton.setOutlineColor(sf::Color::Blue);
        clickButton.setFillColor(sf::Color(0, 0, 255, 175));
        clickButton.setOutlineColor(sf::Color::Black);
        clickButton.setOutlineThickness(10);

        // draw the button
        app.draw(clickButton);
        app.draw(displayedText);
    }

    /*
     * Function: button::SetFont
     * Inputs: std::string
     * Outputs: int
     */
    int SetFont(std::string path)
    {
        // loading font
        if (!font.loadFromFile(path))
        {
            return EXIT_FAILURE;
        }
        return 1;
    }

    /*
     * Function: button::GetX
     * Inputs: None
     * Outputs: int(1)
     */
    int GetX() const {
        return x;
    }

    /*
     * Function: button::GetY
     * Inputs: None
     * Outputs: int(1)
     */
    int GetY() const {
        return y;
    }

private:
    float x;
    float y;
    sf::Vector2f size;
    std::string text;
    sf::Text displayedText;
    sf::RectangleShape clickButton;
    sf::Font font;
};

/*
 * Function: isCollide
 * Inputs: Entity object(2)
 * Outputs: bool
 */
bool isCollide(Entity *a,Entity *b)
{
    // x distance between the entities
    float xDistance = b->x - a->x;
    // y distance between the entities
    float yDistance = b->y - a->y;
    // combined radius of entities
    float combinedRadius = a->R + b->R;
    // use pythagorean theorem to determine if collision occurs
    return pow(xDistance,2)+pow(yDistance,2)<pow(combinedRadius,2);
}

/*
 * Function: CreateAsteroids
 * Inputs: reference to entity pointer list, animation object
 * Outputs: None
 */
void CreateAsteroids(std::list<Entity*> &entities, Animation &anim)
{
    // creating asteroid objects
    for(int i=0;i<15+(2*(Game::level-1));i++)
    {
        // create the asteroid object
        asteroid *a = new asteroid();
        // set asteroid settings - random angle, size 25
        a->settings(anim, rand()%W, rand()%H, rand()%360, 25);
        // add asteroid to Entity list
        entities.push_back(a);
    }
}

//=======================================
/*
 * Function: main (driver code)
 * Inputs: None
 * Outputs: None
 * Purpose: Drives gameplay
 */
int main()
{
    // seed the random number generator
    srand(time(0));
    // init clock
    Clock c;
    // time variable stores current time clock
    int time = 0;

    // Create the window for our game
    RenderWindow app(VideoMode(W, H), "Asteroids!");

    // set the framerate for game
    app.setFramerateLimit(60);

    // load in textures for sprites
    Texture t1,t2,t3,t4,t5,t6,t7,t8;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");

    // UFO texture:
    // "https://www.flaticon.com/free-icons/alien"
    // Alien icons created by Freepik - Flaticon
    t8.loadFromFile("images/ufo.png");


    // UFO Sound:
    // "Alien Alarm Broadcast" by Airborne80
    // License: Attribution 4.0 license
    sf::SoundBuffer alienSoundBuff;
    if (!alienSoundBuff.loadFromFile("sounds/alienSound.ogg"))
    {
        return EXIT_FAILURE;
    }

    // Loading sound buffer
    sf::Sound alienSound;
    alienSound.setBuffer(alienSoundBuff);

    // Bullet Sound:
    // Gunshots by snapssound
    // License: Creative Commons 0
    // https://freesound.org/people/snapssound/sounds/536630/
    sf::SoundBuffer bulletSoundBuff;
    if(!bulletSoundBuff.loadFromFile("sounds/gunshot.ogg"))
    {
        return EXIT_FAILURE;
    }

    // load gunshot sound
    sf::Sound gunshot;
    gunshot.setBuffer(bulletSoundBuff);

    // Explosion Sound:
    // explosion_06 by derplayer
    // License: Creative Commons 0
    // https://freesound.org/people/derplayer/sounds/587196/
    sf::SoundBuffer expSoundBuff;
    if(!expSoundBuff.loadFromFile("sounds/explosion.ogg"))
    {
        return EXIT_FAILURE;
    }

    // load explosion sound
    sf::Sound explosion;
    explosion.setBuffer(expSoundBuff);

    // smooth texture t1,t2 to make pixels less visible
    t1.setSmooth(true);
    t2.setSmooth(true);

    // create background
    Sprite background(t2);

    // setting animations for the textures
    Animation sExplosion(t3, 0,0,256,256, 48, 0.5);
    Animation sRock(t4, 0,0,64,64, 16, 0.2);
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);
    Animation sPlayer(t1, 40,0,40,40, 1, 0);
    Animation sPlayer_go(t1, 40,40,40,40, 1, 0);
    Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);

    // initializing the list of Entity objects
    std::list<Entity*> entities;

    // creating 15 asteroid objects
    CreateAsteroids(entities, sRock);

    // create the player object
    player *p = new player();
    // set player location to 200,200 on the screen, with size 20
    p->settings(sPlayer,200,200,0,20);
    // add player object to Entity list
    entities.push_back(p);

    // Font: Softers Font by StoryType Studio
    // License: Free for personal use
    // https://www.1001freefonts.com/softers.font
    // loading font
    sf::Font font;
    if (!font.loadFromFile("fonts/Softers.otf"))
    {
        std::cout<<"ERROR LOADING FONT"<<std::endl;
    }

    // while app is open
    bool play = true;
    bool win = false;
    bool newLevel = false;
    while (app.isOpen())
    {
        if (play) {
            // checking key press events
            Event event;
            while (app.pollEvent(event)) {
                // if window is closed
                if (event.type == Event::Closed) {
                    // close app
                    app.close();
                }

                // if a key is pressed
                if (event.type == Event::KeyPressed) {
                    // if space button is pressed
                    if (event.key.code == Keyboard::Space) {
                        // create bullet object
                        bullet *b = new bullet();
                        // play gunshot sound
                        gunshot.setVolume(75);
                        gunshot.play();
                        // set bullet location to player location and player orientation, size 10
                        b->settings(sBullet, p->x, p->y, p->angle, 10);
                        // add bullet to Entity list
                        entities.push_back(b);
                    }
                }
            }

            // if right arrow pressed
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                // increment angle by 3 degrees
                p->angle += 3;
            }
            // if left arrow pressed
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                // decrement angle by 3 degrees
                p->angle -= 3;
            }
            // if up arrow pressed
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                // set thrust to true
                p->thrust = true;
            }
                // all other cases
            else {
                // set thrust to false
                p->thrust = false;
            }

            if (newLevel)
            {
                CreateAsteroids(entities, sRock);
                newLevel = false;
            }

            // iterating through each entity
            for (auto a: entities) {
                // iterating through each entity with respect to entity a
                for (auto b: entities) {
                    // if a is asteroid or ufo and b is a bullet
                    if ((a->name == "asteroid" || a->name == "ufo") && b->name == "bullet") {

                        // check for collision between a and b
                        if (isCollide(a, b)) {
                            if (a->name == "ufo")
                            {
                                // 50 pts for hitting ufo
                                Game::currentScore += 50;

                                // stop ufo sound
                                alienSound.stop();
                            }
                            else
                            {
                                // 10 pts for hitting asteroid
                                Game::currentScore += 5;
                            }

                            // 'destroy' a and b
                            a->life = false;
                            b->life = false;

                            // create explosion entity
                            Entity *e = new Entity();
                            // play explosion sound
                            explosion.setVolume(75);
                            explosion.play();
                            // set explosion to the position of entity a
                            e->settings(sExplosion, a->x, a->y);
                            e->name = "explosion";
                            // add explosion to entity list
                            entities.push_back(e);

                            // loop twice (or more if on higher level)
                            for (int i = 0; i < 1+Game::level; i++) {
                                // if size of a is 15 (if a is small asteroid)
                                if (a->R == 15) {
                                    // do nothing
                                    continue;
                                }

                                // create new asteroid entity (split big asteroid into smaller ones)
                                Entity *e = new asteroid();
                                // set new asteroid to have random angle and size 15
                                e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                                // add new asteroid to entity list
                                entities.push_back(e);
                            }

                        }
                    }

                    // if a is player and b is asteroid or ufo
                    if (a->name == "player" && (b->name == "asteroid" || b->name == "ufo")) {
                        // check collision between a and b
                        if (isCollide(a, b)) {
                            // decrement a life from player
                            player::lives -= 1;

                            if (b->name == "ufo")
                            {
                                // stop ufo sound
                                alienSound.stop();
                            }

                            // 'destroy' b
                            b->life = false;

                            // create explosion entity
                            Entity *e = new Entity();
                            // set explosion position to player position
                            e->settings(sExplosion_ship, a->x, a->y);
                            e->name = "explosion";
                            // add explosion to entity list
                            entities.push_back(e);

                            // reset player location
                            p->settings(sPlayer, W / 2, H / 2, 0, 20);
                            // reset player speed
                            p->dx = 0;
                            p->dy = 0;
                        }
                    }
                }

            }
            // if player is accelerating
            if (p->thrust) {
                // set acceleration animation
                p->anim = sPlayer_go;
            }
                // if player is decelerating or stopped
            else {
                // set normal animation
                p->anim = sPlayer;
            }

            // iterating through all entities
            for (auto e: entities) {
                // if we have an explosion at the end of its animation
                if (e->name == "explosion" && e->anim.isEnd()) {
                    // destroy the explosion
                    e->life = 0;
                }
            }

            // timer for ufo sightings
            int elapsedTime = c.getElapsedTime().asSeconds();

            // if a second has passed, increment time
            if (elapsedTime >= 1) {
                time += 1;
                // restart clock
                c.restart();
            }

            // if 20 seconds has elapsed, spawn a ufo
            if (time >= 15) {
                time = 0;
                Ufo *ufo = new Ufo();
                ufo->settings(t8);
                entities.push_back(ufo);
                // start ufo sound
                alienSound.setVolume(2*gunshot.getVolume());
                alienSound.play();
            }

            // iterate through entities
            for (auto i = entities.begin(); i != entities.end();) {
                if (player::lives <= 0) {
                    play = false;
                    win = false;
                    break;
                }
                if (Game::round > Game::rounds)
                {
                    play = false;
                    win = true;
                    break;
                }

                // create entity pointer that points to ith entity in entity list
                Entity *e = *i;

                // update location, animation of entity
                e->update();
                e->anim.update();

                // if the entity has been destroyed
                if (e->life == false) {
                    // remove entity from entity list
                    i = entities.erase(i);
                    // delete the entity pointer and free memory
                    delete e;
                }
                    // increment iterator
                else i++;
            }

            // if there are no asteroids left in the game, create more
            if (asteroid::GetAsteroidCount() <= 0) {
                CreateAsteroids(entities, sRock);
                Game::round+=1;
            }

            // draw background
            app.draw(background);
            // iterate through entities
            for (auto i: entities) {
                // draw each entity
                i->draw(app);
            }

            // display level
            std::string levelText = "Level: "+std::to_string(Game::level);
            sf::Text displayLevel(levelText, font);
            displayLevel.setOutlineThickness(10);
            displayLevel.setPosition(100, 0);
            displayLevel.setFillColor(sf::Color::Cyan);
            app.draw(displayLevel);

            // display round
            std::string rt = "Round: "+std::to_string(Game::round);
            sf::Text displayRound(rt, font);
            displayRound.setOutlineThickness(10);
            displayRound.setPosition(300,0);
            displayRound.setFillColor(sf::Color::Yellow);
            app.draw(displayRound);

            // display score
            std::string scoreText = "Score: "+std::to_string(Game::currentScore);
            sf::Text scoreboard(scoreText, font);
            scoreboard.setOutlineThickness(10);
            scoreboard.setPosition(500, 0);
            scoreboard.setFillColor(sf::Color::Green);
            app.draw(scoreboard);

            // display lives
            std::string lt = "Lives: "+std::to_string(player::lives);
            sf::Text livesText(lt, font);
            livesText.setOutlineThickness(10);
            livesText.setPosition(700, 0);
            livesText.setFillColor(sf::Color::Red);
            app.draw(livesText);

            // display asteroids
            std::string asteroidText = "Asteroids: "+std::to_string(asteroid::asteroidCount);
            sf::Text displayAsteroids(asteroidText, font);
            displayAsteroids.setOutlineThickness(10);
            displayAsteroids.setPosition(900, 0);
            displayAsteroids.setFillColor(sf::Color::Magenta);
            app.draw(displayAsteroids);

        }

            // game over screen
        else
        {
            // update high score
            if (Game::highScore < Game::currentScore)
            {
                Game::highScore = Game::currentScore;
            }

            // clear the screen, prepare end screen
            app.clear();
            app.draw(background);

            // vector storing x, y pos
            sf::Vector2f size(250,150);

            // text for end screen
            std::string endtxt;
            if (win)
            {
                endtxt =
                        " LEVEL COMPLETE\n"
                        "Final Score: " +std::to_string(Game::currentScore)+"\n"
                                                                            "High Score : "+std::to_string(Game::highScore)+"\n";

            }
            else
            {
                endtxt =
                        "  GAME OVER\n"
                        "Final Score: " +std::to_string(Game::currentScore)+"\n"
                                                                            "High Score : "+std::to_string(Game::highScore)+"\n";
            }

            sf::Text endText(endtxt, font);
            endText.setPosition(((W-size.x)/2)+10, 180);
            endText.setFillColor(sf::Color::Green);
            endText.setOutlineThickness(10);
            app.draw(endText);

            // buttons to be created
            button *nextLevelButton = new button("Next Level", (W-size.x)/2, 400, size);
            button *playAgainButton = new button("Play Again", (W-size.x)/2, 400, size);

            // if you won the level
            if (win)
            {
                // create a next level button centered in x and offset in y
                nextLevelButton->Create(app);
            }
                // if you lost
            else
            {
                // create a play again button centered in x and offset in y
                playAgainButton->Create(app);
            }

            // event loop
            Event event;
            while (app.pollEvent(event)) {
                // if window is closed
                if (event.type == Event::Closed) {
                    // close app
                    app.close();
                }

                // if click
                if (event.type == Event::MouseButtonPressed)
                {
                    // check if click is within the bounds of the play again button
                    if (sf::Mouse::getPosition(app).x > playAgainButton->GetX() &&
                        sf::Mouse::getPosition(app).x < playAgainButton->GetX()+size.x &&
                        sf::Mouse::getPosition(app).y > playAgainButton->GetY() &&
                        sf::Mouse::getPosition(app).y < playAgainButton->GetY()+size.y &&
                        !win)
                    {
                        // restart game, reset current score, lives, level
                        play = true;
                        player::lives = MAX_LIVES;
                        Game::currentScore = 0;
                        Game::level = 1;
                        Game::round = 1;
                        newLevel = true;
                    }

                        // check if click is within the bounds of next level button
                    else if (sf::Mouse::getPosition(app).x > nextLevelButton->GetX() &&
                             sf::Mouse::getPosition(app).x < nextLevelButton->GetX()+size.x &&
                             sf::Mouse::getPosition(app).y > nextLevelButton->GetY() &&
                             sf::Mouse::getPosition(app).y < nextLevelButton->GetY()+size.y &&
                             win)
                    {
                        // restart game, increment level, reset lives, KEEP SCORE
                        play = true;
                        Game::level += 1;
                        player::lives = MAX_LIVES;
                        newLevel = true;
                        Game::rounds += 1;
                        Game::round = 1;
                    }
                }
            }
        }

        // display drawings to screen
        app.display();
    }
    return 0;
}