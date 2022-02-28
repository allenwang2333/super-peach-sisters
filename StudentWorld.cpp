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
: GameWorld(assetPath), m_nexLevel(false), m_reachedMario(false) 
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    // initializes whether reached mario or flag
    m_nexLevel = false;
    m_reachedMario = false;
    
    // initialize ostream file name
    ostringstream oss;
    oss.fill('0');
    oss << setw(2) << getLevel();
    
    // converts ostrigstream to string
    string s = oss.str();

    // load level file
    Level lev(assetPath());
    
    // load level file, referenced from spec
    string level_file = "level" + s + ".txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find" << level_file <<" data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr <<  level_file << "is improperly formatted" << endl; 
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
        //referenced from spec
        Level::GridEntry ge;
        // renders level
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
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0)); // 0 means no goodie
                    break;
                case Level::star_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 1)); // 1 means starGoodie
                    break;
                case Level::flower_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 2)); // 2 means flower goodie 
                    break;
                case Level::mushroom_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 3)); // 3 means mushroom goodie
                    break;
                case Level::pipe:
                    m_actors.push_back(new Pipe(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::flag:
                    m_actors.push_back(new Flag(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::goomba:
                    m_actors.push_back(new Goomba(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 180 * randInt(0, 1))); // direction is random number of 0 or 180
                    break;
                case Level::koopa:
                    m_actors.push_back(new Koopa(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 180 * randInt(0, 1)));
                    break;
                case Level::piranha:
                    m_actors.push_back(new Piranha(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 180 * randInt(0, 1)));
                    break;
                case Level::mario:
                    m_actors.push_back(new Mario(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
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
    /*
        for given x, y
        if x y is in the range of a blocking oject in vector of actors 
        if is blocking (blocking objects can never die)
        return true
    */
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++) {
        if((*p)->isBlocking() && (x >= (*p)->getX() && x < (*p)->getX() + SPRITE_WIDTH) && (y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT)) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::isOverlap(double x, double y) {
    return (isOverlapHelper(x, y) || isOverlapHelper(x, y+SPRITE_HEIGHT-1) ||
    isOverlapHelper(x+SPRITE_WIDTH-1, y) || isOverlapHelper(x+SPRITE_WIDTH-1, y+SPRITE_HEIGHT-1));
}

bool StudentWorld::isOverlapHelper(double x, double y) {
    /*
        for given x, y
        if x y is in the range of a blocking oject in vector of actors 
        if is damageable and is alive
        return true
    */
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++) {
        if((*p)->isDamageable() && (*p)->isAlive() && (x >= (*p)->getX() && x < (*p)->getX() + SPRITE_WIDTH) && (y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT)) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::isOverlapPeach(double x, double y) {
    return (isOverlapPeachHelper(x, y) || isOverlapPeachHelper(x+SPRITE_WIDTH-1, y) ||
        isOverlapPeachHelper(x, y+SPRITE_HEIGHT-1) || isOverlapPeachHelper(x+SPRITE_WIDTH-1, y+SPRITE_HEIGHT-1));
}

bool StudentWorld::isOverlapPeachHelper(double x, double y) {
    // return true if overlaps peach
    double peachX = m_peach->getX();
    double peachY = m_peach->getY();
    if (x < peachX+SPRITE_WIDTH && x >= peachX && y < peachY+SPRITE_HEIGHT && y>= peachY) {
        return true;
    }
    return false;
}

void StudentWorld::moveToNext() {
    m_nexLevel = true;
}

void StudentWorld::reachedMario() {
    m_reachedMario = true;
}

int StudentWorld::move()
{
    // initializes level info
    ostringstream oss;
    oss.fill('0');
    oss << setw(2) << getLevel();
    string s = oss.str();

    // initializes score
    ostringstream oscore;
    oscore.fill('0');
    oscore << setw(6) << getScore();
    string f = oscore.str();

    if (m_peach->isAlive()){
        m_peach->doSomething(); 
    }
    // every actor doSomething
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++) {
        if ((*p)->isAlive()){
            (*p)->doSomething();
        }
    }
    // if die
    if (!m_peach->isAlive()){
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    // if reach to next level
    if (m_nexLevel) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    // if reached mario 
    if (m_reachedMario) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    // delete dead actor and erase from vector
    deleteActor();
    // initializes info string
    string lives = intToString(getLives());
    string level = s;
    string score = f;
    string shootPower = "ShootPower! ";
    string jumpPower = "JumpPower! ";
    string starPower = "StarPower! ";
    // put them together
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
    setGameStatText(text); // set text
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    
    //return GWSTATUS_PLAYER_DIED;
    return GWSTATUS_CONTINUE_GAME;
}

string StudentWorld::intToString(int input) {
    // my int to string converter
    ostringstream ss;
    ss << input;
    return ss.str();
}

bool StudentWorld::bonk(double x, double y) {
    // traversing through vector bonk target object
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++){
        if((x >= (*p)->getX() && x < (*p)->getX() + SPRITE_WIDTH) && (y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT) ||
            (x + 4 >= (*p)->getX() && x + 4 < (*p)->getX() + SPRITE_WIDTH && y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT)) {
            if ((*p)->isAlive()){
                (*p)->isBonked();
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::damage(double x, double y) {
    // traversing through vector return true if is dameged 
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++){
        if((*p)->isDamageable() && ((x >= (*p)->getX() && x < (*p)->getX() + SPRITE_WIDTH) && (y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT) ||
            (x + 4 >= (*p)->getX() && x + 4 < (*p)->getX() + SPRITE_WIDTH && y >= (*p)->getY() && y < (*p)->getY() + SPRITE_HEIGHT))) {
            if ((*p)->isAlive()){
                (*p)->isDamaged();
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::newFireball(double x, double y, int dir, int type) {
    // kind: 0 --> peachFireball
    // kind: 1 --> piranhaFireball
    if (type == 0) {
        m_actors.push_back(new PeachFireball(this, x, y, dir));
    }
    if (type == 1) {
        m_actors.push_back(new PiranhaFireball(this, x, y, dir));
    }
    return;
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

void StudentWorld::newShell(double x, double y, int dir) {
    m_actors.push_back(new Shell(this, x, y, dir));
}

void StudentWorld::deleteActor() {
    // delete dead actor and remove it from vector
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
    // delete all actors and pop the vector
    delete m_peach;
    while (!m_actors.empty()){
        delete m_actors[m_actors.size()-1];
        m_actors.pop_back();
    }
}

bool StudentWorld::getPeachPower(int type) {
    /*
            starGoodie = 1 --> starPower
            flowerGoodie = 2 --> shootPower
            mushroomGoodie = 3 --> jumpPower
            hit point = 4 --> hit point
    */
    switch (type)
    {
    case 1:
        return (m_peach->getStarPower());
        break;
    case 2:
        return (m_peach->getShootPower());
        break;
    case 3:
        return (m_peach->getJumpPower());
        break;    
    default:
        return false;
        break;
    }
}

void StudentWorld::gainPeachPower(int type, int num) {
    /*
            starGoodie = 1 --> starPower
            flowerGoodie = 2 --> shootPower
            mushroomGoodie = 3 --> jumpPower
            hit point = 4 --> hit point
        */
    switch (type)
    {
    case 1:
        return (m_peach->gainPower(1));
        break;
    case 2:
        return (m_peach->gainPower(2));
        break;
    case 3:
        return (m_peach->gainPower(3));
        break;
    case 4:
        m_peach->setHitPoint(num);
        break;    
    default:
        break;
    }
}

void StudentWorld::bonkPeach() {
    m_peach->isBonked();
}

void StudentWorld::damagePeach() {
    m_peach->isDamaged();
}

double StudentWorld::getPeachX() {
    return m_peach->getX();
}

double StudentWorld::getPeachY() {
    return m_peach->getY();
}
