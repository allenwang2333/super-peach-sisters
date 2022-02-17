#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;

void Actor::isBonked() {
    return;
}

void BlockingObjects::isBonked() {
    cerr << "a pipe at " << getX() << ", " << getY() << " is bonked" << endl; 
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

    if (m_currentInvincibility > 0) {
        m_currentInvincibility--;
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
    double peachX = getStudentWorld()->getPeach()->getX();
    double peachY = getStudentWorld()->getPeach()->getY();
    if (getX() < peachX+8 && getX()+8 > peachX && getY() < peachY+8 && getY()+8 > peachY) {
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
    double peachX = getStudentWorld()->getPeach()->getX();
    double peachY = getStudentWorld()->getPeach()->getY();
    if (getX() < peachX+8 && getX()+8 > peachX && getY() < peachY+8 && getY()+8 > peachY) {
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
    double peachX = getStudentWorld()->getPeach()->getX();
    double peachY = getStudentWorld()->getPeach()->getY();
    if (getX() < peachX+8 && getX()+8 > peachX && getY() < peachY+8 && getY()+8 > peachY) {
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
    if (getStudentWorld()->isOverlap(getX(), getY())){
        getStudentWorld()->bonk(getX(), getY());
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
