#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip> 
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    // initialize ostream file name
    ostringstream oss;
    oss.fill('0');
    oss << setw(2) << getLevel();
    
    string s = oss.str();

    // load level file
    Level lev(assetPath());

    string level_file = "level" + s + ".txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find" << s <<" data file" << endl;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "level01.txt is improperly formatted" << endl; 
    }
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
        
        Level::GridEntry ge;

        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                ge = lev.getContentsOf(i, j);
                switch (ge) {
                case Level::empty:
                    break;
                case Level::peach:
                // initialize pointer to Peach
                    m_peach = new Peach(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    break;
                case Level::block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0));
                    break;
                case Level::star_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 1));
                    break;
                case Level::flower_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 2));
                    break;
                case Level::mushroom_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 3));
                    break;
                case Level::pipe:
                    m_actors.push_back(new Pipe(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::flag:
                    m_actors.push_back(new Flag(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                default:
                    break;
                }        
            }
        }
    
    }

    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isBlockingObjectAt(double x, double y) {
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++) {
        if((x >= (*p)->getX() && x < (*p)->getX() + SPRITE_WIDTH) && (y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT) && (*p)->getDepth() == 2) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::isOverlap(double x, double y) {
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++) {
        if((x >= (*p)->getX() && x < (*p)->getX() + SPRITE_WIDTH) && (y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT) && (*p)->getDepth() == 0) {
            return true;
        }
    }
    return false;
}

int StudentWorld::move()
{
    ostringstream oss;
    oss.fill('0');
    oss << setw(2) << getLevel();
    string s = oss.str();

    ostringstream oscore;
    oscore.fill('0');
    oscore << setw(6) << getScore();
    string f = oscore.str();

    m_peach->doSomething();
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++) {
        if ((*p)->isAlive()){
            (*p)->doSomething();
        }
    }
    deleteActor();

    string lives = intToString(getLives());
    string level = s;
    string score = f;
    string shootPower = "ShootPower! ";
    string jumpPower = "JumpPower! ";
    string starPower = "StarPower! ";
    string text = "Lives: " + lives + " Level: " + level + " Points: " + score + " ";
    if (m_peach->getShootPower()){
        text += shootPower;
    }
    if (m_peach->getJumpPower()) {
        text += jumpPower;
    }
    if (m_peach->getStarPower()) {
        text += starPower;
    }
    setGameStatText(text);
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    
    //return GWSTATUS_PLAYER_DIED;
    return GWSTATUS_CONTINUE_GAME;
}

string StudentWorld::intToString(int input) {
    ostringstream ss;
    ss << input;
    return ss.str();
}

void StudentWorld::bonk(double x, double y) {
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++){
        if((x >= (*p)->getX() && x < (*p)->getX() + SPRITE_WIDTH) && (y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT) ||
            (x + 4 >= (*p)->getX() && x + 4 < (*p)->getX() + SPRITE_WIDTH && y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT)) {
            (*p)->isBonked();
            return;
        }
    }
    return;
}

void StudentWorld::newFireball(double x, double y, int dir, int type) {
    // kind: 0 --> peachFireball
    // kind: 1 --> p什么玩意Fireball
    if (type == 0) {
        m_actors.push_back(new PeachFireball(this, x, y, dir));
    }
}

void StudentWorld::newGoodie(double x, double y, int type) {
    /*
            noGoodie = 0
            starGoodie = 1
            flowerGoodie = 2
            mushroomGoodie = 3
        */
    if (type == 1) {
        m_actors.push_back(new StarGoodie(this, x, y));
    }
    if (type == 2) {
         m_actors.push_back(new FlowerGoodie(this, x, y));
    }
    if (type == 3) {
         m_actors.push_back(new MushroomGoodie(this, x, y));
    }
}

void StudentWorld::deleteActor() {
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); ){
        vector<Actor*>::iterator temp = p;
        if(!(*p)->isAlive()){
            delete (*temp);
            p = m_actors.erase(temp);
        }
        else{
            p++;
        }
    }
}

void StudentWorld::cleanUp()
{
    cerr << "start to cleanup" << endl;
    delete m_peach;
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++) {
        delete (*p);
    }
}
