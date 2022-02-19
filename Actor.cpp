#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
using namespace std;

void BlockingObjects::isBonked() {
}

void Block::doSomething() {
}

void Block::isBonked() {
    if (m_goodie <= 0) {
        getStudentWorld()->playSound(SOUND_PLAYER_BONK);   
    }
    else {
        getStudentWorld()->playSound(SOUND_POWERUP_APPEARS);
        switch (m_goodie) {
        case 1:
            getStudentWorld()->newGoodie(getX(), getY()+8, 1);
            break;
        case 2:
            getStudentWorld()->newGoodie(getX(), getY()+8, 2);
            break;
        case 3:
            getStudentWorld()->newGoodie(getX(), getY()+8, 3);
            break;
        default:
            break;
        }
        m_goodie = 0;
    }
}

void Peach::isBonked() {
    if (m_starPower > 0 && m_temporaryInvincibility > 0) {
        
    }
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

void Peach::isDamaged() {
    if (m_starPower > 0 && m_temporaryInvincibility > 0) {
        
    }
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

void Peach::setHitPoint(int point) {
    m_hitPoint = point;
}

void Peach::gainPower(int type) {
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
    
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY())) {
        isBonked();
    }

    if (m_rJumpDistance > 0) {
        double x = getX();
        double y = getY()+4;

        if (getStudentWorld()->isBlockingObjectAt(x, getY()+9) || getStudentWorld()->isBlockingObjectAt(x+4, getY()+9)) {
            getStudentWorld()->bonk(x, getY()+12);
            m_rJumpDistance = 0;
            return;
        }
        this->moveTo(x, y);
        m_rJumpDistance--;
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
        return;
    }
    
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()) ||
    getStudentWorld()->isBlockingObjectAt(getX(), getY()-3) ||
    getStudentWorld()->isBlockingObjectAt(getX()+4, getY()) ||
    getStudentWorld()->isBlockingObjectAt(getX()+4, getY()-3))
    {
        
    }
    else {
        moveTo(getX(), getY()-4);
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

void FlowerGoodie::doSomething() {
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->increaseScore(50);
        getStudentWorld()->getPeach()->gainPower(2);
        getStudentWorld()->getPeach()->setHitPoint(2);
        setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()-1) || 
    getStudentWorld()->isBlockingObjectAt(getX()+4, getY()-1)) {
    }
    else {
        moveTo(getX(), getY()-2);
        //return;
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

void MushroomGoodie::doSomething() {
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->increaseScore(75);
        getStudentWorld()->getPeach()->gainPower(3);
        getStudentWorld()->getPeach()->setHitPoint(2);
        setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()-1) || 
    getStudentWorld()->isBlockingObjectAt(getX()+4, getY()-1)) {
    }
    else {
        moveTo(getX(), getY()-2);
        //return;
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

void StarGoodie::doSomething() {
    if ((getStudentWorld()->isOverlapPeach(getX(), getY()))) {
        getStudentWorld()->increaseScore(150);
        getStudentWorld()->getPeach()->gainPower(1);
        setDead();
        getStudentWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
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

void PeachFireball::doSomething() {
    if (getStudentWorld()->isOverlap(getX(), getY()) || 
    getStudentWorld()->isOverlap(getX()+4, getY()))
    {
        getStudentWorld()->damage(getX(), getY());
        setDead();
        return;
    }
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()-1) || 
    getStudentWorld()->isBlockingObjectAt(getX()+7, getY()-1))
    
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
}

void PiranhaFireball::doSomething() {
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->getPeach()->isDamaged();
        setDead();
    }
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()-2))
     {
    }
    else {
        moveTo(getX(), getY()-2);
    } 
    double x = getX();
    double y = getY();
    getPositionInThisDirection(getDirection(), 2, x, y);

    if (getStudentWorld()->isBlockingObjectAt(x, y) ||  
    getStudentWorld()->isBlockingObjectAt(x,y+7))

    {
        setDead();

        return;
    }
    moveForward(2);
    increaseAnimationNumber();
}

void Shell::doSomething() {
    if (getStudentWorld()->isOverlap(getX(), getY()) ||
    getStudentWorld()->isOverlap(getX()+4,getY())) {
        getStudentWorld()->damage(getX(), getY());
        setDead();
        return;
    }
    if (getStudentWorld()->isBlockingObjectAt(getX(), getY()-2) ||
        getStudentWorld()->isBlockingObjectAt(getX()+4, getY()-2) ||
        getStudentWorld()->isBlockingObjectAt(getX()+6, getY()-2) ||
        getStudentWorld()->isBlockingObjectAt(getX()+8, getY()-2)
        ){
        
    }
    else {
        moveTo(getX(), getY()-2);
    }
    
    double x = getX();
    double y = getY();
    getPositionInThisDirection(getDirection(), 2, x, y);

    if (getStudentWorld()->isBlockingObjectAt(x, y) || getStudentWorld()->isBlockingObjectAt(x+6, y))
    {
        setDead();
        return;
    }
    moveForward(2);
}

void Goomba::doSomething() {
    if (!isAlive()){
        return;
    }
    if (getStudentWorld()->isOverlapPeach(getX(), getY())){
        getStudentWorld()->getPeach()->isBonked();
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

void Goomba::isBonked() {
    if (getStudentWorld()->getPeach()->isInvincible()){
        getStudentWorld()->playSound(SOUND_PLAYER_KICK);
        getStudentWorld()->increaseScore(100);
        setDead();
    }
}

void Goomba::isDamaged() {
    getStudentWorld()->increaseScore(100);
    setDead();
}

void Koopa::doSomething() {
    if (!isAlive()){
        return;
    }
    if (getStudentWorld()->isOverlapPeach(getX(), getY())){
        getStudentWorld()->getPeach()->isBonked();
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

void Koopa::isBonked() {
    if (getStudentWorld()->getPeach()->isInvincible()){
        getStudentWorld()->playSound(SOUND_PLAYER_KICK);
        getStudentWorld()->increaseScore(100);
        setDead();
        getStudentWorld()->newShell(getX(), getY(), getDirection());
    }
}


void Koopa::isDamaged() {
    getStudentWorld()->increaseScore(100);
    setDead();
    getStudentWorld()->newShell(getX(), getY(), getDirection());
}

void Piranha::doSomething() {
    if (!isAlive()) {
        return;
    }
    increaseAnimationNumber();
    if (getStudentWorld()->isOverlapPeach(getX(), getY())) {
        getStudentWorld()->getPeach()->isBonked();
        isBonked();
        return;
    }
    if (!(abs(getStudentWorld()->getPeach()->getY()-getY()) <= 1.5 * SPRITE_HEIGHT)) {
        return;
    }
    if (getStudentWorld()->getPeach()->getX()-getX() < getX()) {
        setDirection(180);
    }
    else {
        setDirection(0);
    }
    if (m_firingDelay > 0){
        m_firingDelay--;
    }
    double distance = abs(getStudentWorld()->getPeach()->getX() - this->getX());
    if (distance < 8 * SPRITE_WIDTH) {
        if (m_firingDelay <= 0) {
            getStudentWorld()->newFireball(getX(), getY(), getDirection(), 1);
            getStudentWorld()->playSound(SOUND_PIRANHA_FIRE);
            m_firingDelay = 40;
        }
        
    }
    
}

void Piranha::isBonked() {
     if (getStudentWorld()->getPeach()->isInvincible()){
        getStudentWorld()->playSound(SOUND_PLAYER_KICK);
        getStudentWorld()->increaseScore(100);
        setDead();
    }
}

void Piranha::isDamaged() {
    getStudentWorld()->increaseScore(100);
    setDead();
}

void Flag::doSomething() {
    if (!isAlive()){
        return;
    }
    if (getStudentWorld()->isOverlapPeach(getX(), getY())){
        getStudentWorld()->increaseScore(1000);
        setDead();
        getStudentWorld()->moveToNext();
    }
}

void Mario::doSomething() {
        if (!isAlive()){
        return;
    }
    if (getStudentWorld()->isOverlapPeach(getX(), getY())){
        getStudentWorld()->increaseScore(1000);
        setDead();
        getStudentWorld()->reachedMario();
    }
}