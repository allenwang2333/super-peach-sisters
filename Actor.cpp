#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
using namespace std;

void BlockingObjects::isBonked() {
    /*
        virtual function of isBonked
        play bonk sound when bonked
    */

    getStudentWorld()->playSound(SOUND_PLAYER_BONK);
}

void Block::isBonked() {
    /* 
        if not holding goodie,
        play bonk sound when bonked
        if holding goodie,  release that goodie and play a sound
    */
    
    if (m_goodie <= 0) {
        // no goodie
        getStudentWorld()->playSound(SOUND_PLAYER_BONK);
    }
    else {
        getStudentWorld()->playSound(SOUND_POWERUP_APPEARS);
        switch (m_goodie) {
        case 1: // in actor.h, 1 indicates startGoodie
            getStudentWorld()->newGoodie(getX(), getY()+8, 1);
            break;
        case 2: // 2 indicates flowerGoodie
            getStudentWorld()->newGoodie(getX(), getY()+8, 2);
            break;
        case 3: // 3 indicates mushroomGoodie
            getStudentWorld()->newGoodie(getX(), getY()+8, 3);
            break;
        default:
            break;
        }
        m_goodie = 0; // clear the goodie that the block is holding
    }
}

void Peach::isBonked() {
    if (m_starPower > 0 || m_temporaryInvincibility > 0) {
        // has star power does nothing
    }
    else {
        m_hitPoint--;
        m_temporaryInvincibility = 10;
        m_shootPower = false;
        m_jumPower = false;
        if (m_hitPoint >= 1) {
            getStudentWorld()->playSound(SOUND_PLAYER_HURT);
        }
        if (m_hitPoint <= 0) {
            setDead();
        }
    }
}

void Peach::isDamaged() {
    isBonked(); // does the same lol
}

void Peach::setHitPoint(int point) {
    m_hitPoint = point;
}

void Peach::gainPower(int type) {
/*
    starGoodie = 1 --> starPower
    flowerGoodie = 2 --> shootPower
    mushroomGoodie = 3 --> jumpPower
*/
    switch (type)
    {
    case 1:
        m_starPower = 100;
        break;
    case 2:
        m_shootPower = true;
        break;
    case 3:
        m_jumPower = true;
        break;
    default:
        break;
    }
}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Peach::doSomething() {
    int ch;
    if (!isAlive()) {
        return;
    }

    if (m_starPower > 0) {
        m_starPower--;
    }
    
    if (m_temporaryInvincibility > 0) {
        m_temporaryInvincibility--;
    }

    if (m_timeToRecharge > 0) {
        m_timeToRecharge--;
    }
    
    if (getStudentWorld()->isOverlap(getX(), getY())) {
        if (getStudentWorld()->bonk(getX(), getY()) ||
        getStudentWorld()->bonk(getX()+7, getY())) {
            // left or right
            isBonked();
        }
    }

    if (m_rJumpDistance > 0) {
        double x = getX();
        double y = getY()+4;

        if (getStudentWorld()->isBlockingObjectAt(x, getY()+9) || getStudentWorld()->isBlockingObjectAt(x+4, getY()+9)) {
            // if something above his head
            getStudentWorld()->bonk(x, getY()+12);
            m_rJumpDistance = 0;
            return;
        }
        else {
            this->moveTo(x, y);
            m_rJumpDistance--;
        }
    }
    else {
        if (getStudentWorld()->isBlockingObjectAt(getX(), getY()) ||
        getStudentWorld()->isBlockingObjectAt(getX(), getY()-3) ||
        getStudentWorld()->isBlockingObjectAt(getX()+4, getY()) ||
        getStudentWorld()->isBlockingObjectAt(getX()+4, getY()-3))
        {
            // has something under feet, does nothing
        }
        else {
            moveTo(getX(), getY()-4);
        }
    }

    if (getStudentWorld()->getKey(ch)) {
        double x, y;
        switch (ch)
        {
        case KEY_PRESS_LEFT:
            setDirection(180);
            x = (getX()-4);
            y = getY();
            if (getStudentWorld()->isBlockingObjectAt(x, y)){
                getStudentWorld()->bonk(x,y);
            }
            else {
                moveTo(getX()-4, y);
            }
            break;
        case KEY_PRESS_RIGHT:
            setDirection(0);
            x = (getX()+8);
            y = getY();
            if (getStudentWorld()->isBlockingObjectAt(x, y)){
                getStudentWorld()->bonk(x, y);
            }
            else {
                moveTo(getX()+4, y);
            }
            break;
        case KEY_PRESS_UP:
            if(getStudentWorld()->isBlockingObjectAt(getX(), getY()-1) || getStudentWorld()->isBlockingObjectAt(getX()+4, getY()-1)) {
                if (!m_jumPower){
                    m_rJumpDistance = 8;
                }
                else {
                    m_rJumpDistance = 12;
                }
                getStudentWorld()->playSound(SOUND_PLAYER_JUMP);
            }
            break;
        case KEY_PRESS_SPACE:
            if (!m_shootPower) {
                break;
            }
            if (m_timeToRecharge > 0) {
                break;
            }
            getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
            m_timeToRecharge = 8;
            x = getX();
            y = getY();
            getPositionInThisDirection(getDirection(), 4, x, y);
            getStudentWorld()->newFireball(x, y, getDirection(), 0);
            break;
        default:
            break;
        }
    }
}

void Goodie::helper() {
    // Goodies have same free fall and change direcion
    // so i put them in a helper function in base class of goodies
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()-1) || 
    getStudentWorld()->isBlockingObjectAt(getX()+4, getY()-1)) {
    }
    else {
        moveTo(getX(), getY()-2);
    }
    double x = getX();
    double y = getY();
    getPositionInThisDirection(getDirection(), 2, x, y);

    if (getStudentWorld()->isBlockingObjectAt(x, y) || 
    getStudentWorld()->isBlockingObjectAt(x+6,y) || 
    getStudentWorld()->isBlockingObjectAt(x,y+7) ||
    getStudentWorld()->isBlockingObjectAt(x+6,y+7))
    {
        setDirection(180 - getDirection());
        return;
    }
    moveForward(2);
}

void FlowerGoodie::doSomething() {
    // something that are different in each goodie class
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->increaseScore(50);
        getStudentWorld()->gainPeachPower(2);
        getStudentWorld()->gainPeachPower(4, 2);
        setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    helper();  
}

void MushroomGoodie::doSomething() {
    // something that are different in each goodie class
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->increaseScore(75);
        getStudentWorld()->gainPeachPower(3);
        getStudentWorld()->gainPeachPower(4, 2);
        setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    helper();
}

void StarGoodie::doSomething() {
    // something that are different in each goodie class
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->increaseScore(150);
        getStudentWorld()->gainPeachPower(1);
        setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    helper();
}

void Fireball::helper() {
    // fire ball class has same falling and disapper functionality
    // so i put it in the base class of fireball
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()-2) || 
    getStudentWorld()->isBlockingObjectAt(getX()+7, getY()-2))
    {
    }
    else {
        moveTo(getX(), getY()-2);
    } 
    double x = getX();
    double y = getY();
    getPositionInThisDirection(getDirection(), 2, x, y);

    if (getStudentWorld()->isBlockingObjectAt(x, y) || 
    getStudentWorld()->isBlockingObjectAt(x+6,y) || 
    getStudentWorld()->isBlockingObjectAt(x,y+7) ||
    getStudentWorld()->isBlockingObjectAt(x+6,y+7))
    {
        setDead();
        return;
    }
    moveForward(2);
    increaseAnimationNumber();
}


void PeachFireball::doSomething() {
    // if is overlap kill the enemy
    if (getStudentWorld()->isOverlap(getX(), getY()))
    {
        if(getStudentWorld()->damage(getX(), getY()) || getStudentWorld()->damage(getX()+7, getY())) {
            setDead();
            return;
        }
    }
    helper();
}

void PiranhaFireball::doSomething() {
    // if is overlap with peach, kill peach
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->damagePeach();
        setDead();
    }
    helper();
}

void Shell::doSomething() {
    // kill the enemy it touches
    if (getStudentWorld()->isOverlap(getX(), getY())) {
        if(getStudentWorld()->damage(getX(), getY()) || getStudentWorld()->damage(getX()+7, getY())) {
            setDead();
            return;
        }
    }
    helper();
}

void Enemies::doSomething() {
    // Koopa and Goomba has same doSomething function
    // so i put it in the base class
    if (!isAlive()){
        return;
    }
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->bonkPeach();
        isBonked();
        return;
    }
    double x = getX();
    double y = getY();

    getPositionInThisDirection(getDirection(), 1, x, y);
    if (getStudentWorld()->isBlockingObjectAt(x, y) || 
    getStudentWorld()->isBlockingObjectAt(x+7, y) || 
    !getStudentWorld()->isBlockingObjectAt(x, y-1) ||
    !getStudentWorld()->isBlockingObjectAt(x+7, y-1))
    {
        setDirection(180 - getDirection());
        return;
    }
    moveForward(1);
    increaseAnimationNumber();
}

void Enemies::isBonked() {
    // Goomba and Piranha has same is bonked and is damaged functino
    // so i put them in the function in base class
    if (getStudentWorld()->getPeachPower(1)){
        // 1 is star power
        getStudentWorld()->playSound(SOUND_PLAYER_KICK);
        getStudentWorld()->increaseScore(100);
        setDead();
    }
}

void Enemies::isDamaged() {
    getStudentWorld()->increaseScore(100);
    setDead();
} 

void Koopa::isBonked() {
    if (getStudentWorld()->getPeachPower(1)){
        getStudentWorld()->playSound(SOUND_PLAYER_KICK);
        getStudentWorld()->increaseScore(100);
        setDead();
        getStudentWorld()->newShell(getX(), getY(), getDirection());
    }
}

void Koopa::isDamaged() {
    // generate a new shell when killed
    getStudentWorld()->increaseScore(100);
    setDead();
    getStudentWorld()->newShell(getX(), getY(), getDirection());
}

void Piranha::doSomething() {
    // Piranha has different implementation of doSomething
    // so i overwrite it.
    if (!isAlive()) {
        return;
    }
    increaseAnimationNumber();
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->bonkPeach();
        isBonked();
        return;
    }
    if (!(abs(getStudentWorld()->getPeachY()-getY()) <= 1.5 * SPRITE_HEIGHT)) {
        return;
    }
    if (getStudentWorld()->getPeachX() < getX()) {
        setDirection(180);
    }
    else {
        setDirection(0);
    }
    if (m_firingDelay > 0){
        m_firingDelay--;
        return;
    }
    double distance = abs(getStudentWorld()->getPeachX() - getX());
    // use absolute value from cmath lib
    if (distance < 8 * SPRITE_WIDTH) {
            getStudentWorld()->newFireball(getX(), getY(), getDirection(), 1);
            getStudentWorld()->playSound(SOUND_PIRANHA_FIRE);
            m_firingDelay = 40;
        
    }
}

void Flag::doSomething() {
    if (!isAlive()){
        return;
    }
    if (getStudentWorld()->isOverlapPeach(getX(), getY())){
        getStudentWorld()->increaseScore(1000);
        setDead();
        getStudentWorld()->moveToNext(); // go to next level
    }
}

void Mario::doSomething() {
    if (!isAlive()){
        return;
    }
    if (getStudentWorld()->isOverlapPeach(getX(), getY())){
        getStudentWorld()->increaseScore(1000);
        setDead();
        getStudentWorld()->reachedMario(); // reach mario 
    }
}