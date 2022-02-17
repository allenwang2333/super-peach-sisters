#ifndef ACTOR_H_
#define ACTOR_H_

#include "GameConstants.h"
#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor :public GraphObject {
    public:
        Actor(StudentWorld* studentWorld, int imageID, int startX, int startY, int dir, int depth, double size) 
        : m_studentWorld(studentWorld), 
        GraphObject(imageID, startX, startY, dir, depth, size), 
        m_alive(true), m_depth(depth) {}

        virtual ~Actor() {}
        virtual void isBonked();
        StudentWorld* getStudentWorld() { return m_studentWorld; }
        bool isAlive() { return m_alive; }
        void setDead() { m_alive = false; }
        virtual void doSomething() = 0;
        int getDepth() { return m_depth; }

    private:
        StudentWorld* m_studentWorld;
        bool m_alive; 
        int m_depth;

};  

class Peach :public Actor {
    public:
        Peach(StudentWorld* studentWorld, int startX, int startY)
        : Actor(studentWorld, IID_PEACH, startX, startY, 0, 0, 1.0),
         m_health(1), m_currentInvincibility(0),m_temporaryInvincibility(0), 
         m_starPower(0), m_shootPower(false), m_jumPower(false), 
         m_timeToRecharge(0), m_rJumpDistance(0), m_hitPoint(0)
          {}

        virtual ~Peach() {}
        virtual void isBonked();
        virtual void gainPower(int type);
        virtual void setHitPoint(int point);
        virtual bool getStarPower() {return m_starPower>0;}
        virtual bool getShootPower() {return m_shootPower;}
        virtual bool getJumpPower() {return m_jumPower;}
        bool isInvincible() { return m_currentInvincibility <= 0; }
        //TODO: finish do something: direction

        
        virtual void doSomething();
    private:
        int m_health;
        int m_currentInvincibility;
        int m_temporaryInvincibility;
        int m_timeToRecharge;
        int m_starPower;
        bool m_shootPower;
        bool m_jumPower;
        int m_rJumpDistance;
        int m_hitPoint;
        /*
            noGoodie = 0
            starGoodie = 1 --> starPower
            flowerGoodie = 2 --> shootPower
            mushroomGoodie = 3 --> jumpPower
        */
};

class Fireball :public Actor {
    public:
        Fireball(StudentWorld* studentWorld, int imageID, int startX, int startY, int dir)
        : Actor(studentWorld, imageID, startX, startY, dir ,1, 1) 
        {}

        virtual ~Fireball() {}
        virtual void isBonked() {}
        virtual void doSomething() {}
    private:
    // kind: 0 --> peachFireball
    // kind: 1 --> p什么玩意Fireball
};

class PeachFireball :public Fireball {
    public:
        PeachFireball(StudentWorld* studentWorld, int startX, int startY, int dir)
        : Fireball(studentWorld, IID_PEACH_FIRE, startX, startY, dir)
        {}

        virtual ~PeachFireball() {}
        virtual void isBonked() {}
        virtual void doSomething();
    private:
};

class PiranhaFireball :public Fireball {
    public:
        PiranhaFireball(StudentWorld* studentWorld, int startX, int startY, int dir)
        : Fireball(studentWorld, IID_PIRANHA_FIRE, startX, startY, dir)
        {}

        virtual ~PiranhaFireball() {}
        virtual void isBonked() {}
        virtual void doSomething() {}
    private:
};

class Flag :public Actor {
    public:
        Flag(StudentWorld* studentWorld, int startX, int startY)
        : Actor(studentWorld, IID_FLAG, startX, startY, 0, 1, 1)
        {}

        virtual ~Flag() {}
        virtual void isBonked() {}
        virtual void doSomething() {}
};

class BlockingObjects: public Actor {
    public:
        BlockingObjects(StudentWorld* studentWorld, int imageID ,int startX, int startY, int dir, int depth, double size) 
        : Actor(studentWorld, imageID, startX, startY, dir, depth, size)
        {}

        virtual ~BlockingObjects() {}
        virtual void isBonked();
        virtual void doSomething() {}
    private:
};

class Block :public BlockingObjects {    
    public:
        Block(StudentWorld* studentWorld, int startX, int startY, int goodie) 
        : BlockingObjects(studentWorld, IID_BLOCK, startX, startY, 0, 2, 1), m_bonked(false),
        m_goodie(goodie) {}

        virtual ~Block() {}
        virtual void isBonked();
        virtual void doSomething();
    private:
        /*
            noGoodie = 0
            starGoodie = 1
            flowerGoodie = 2
            mushroomGoodie = 3
        */
        int m_goodie;
        bool m_bonked;
};

class Pipe :public BlockingObjects {
    public:
        Pipe(StudentWorld* studentWorld, int startX, int startY) 
        : BlockingObjects(studentWorld, IID_PIPE, startX, startY, 0, 2, 1)
        {}

        virtual ~Pipe() {}

    private:

};

class Goodie :public Actor {
    public:
        Goodie(StudentWorld* studentWorld, int imageID, int startX, int startY, int dir, int depth, double size) 
        : Actor(studentWorld, imageID, startX, startY, dir, depth, size)
        {}

        virtual ~Goodie() {}
        virtual void isBonked() {}
        virtual void doSomething() {}
    private:
    /*
            noGoodie = 0
            starGoodie = 1
            flowerGoodie = 2
            mushroomGoodie = 3
        */
};

class FlowerGoodie :public Goodie {
    public:
        FlowerGoodie(StudentWorld* studentWorld, int startX, int startY)
        : Goodie(studentWorld, IID_FLOWER, startX, startY, 0, 1, 1)
        {}
        
        virtual ~FlowerGoodie() {}
        virtual void doSomething();

    private:
};

class MushroomGoodie :public Goodie {
    public:
        MushroomGoodie(StudentWorld* studentWorld, int startX, int startY)
        : Goodie(studentWorld, IID_MUSHROOM, startX, startY, 0, 1, 1)
        {}
        
        virtual ~MushroomGoodie() {}
        virtual void doSomething();

    private:
};

class StarGoodie :public Goodie {
    public:
        StarGoodie(StudentWorld* studentWorld, int startX, int startY)
        : Goodie(studentWorld, IID_STAR, startX, startY, 0, 1, 1)
        {}
        
        virtual ~StarGoodie() {}
        virtual void doSomething();

    private:
};

class Enemies :public Actor {
    public:
        Enemies(StudentWorld* studentWorld, int imageID,int startX, int startY)
        : Actor(studentWorld, imageID, startX, startY, 0, 0, 1.0)
        {}

        virtual void doSomething(){}
        virtual void isBonked() {}
        virtual void isDamaged() {}
    private:
};

#endif // ACTOR_H_
