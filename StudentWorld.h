#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Peach;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  virtual void bonk(double x, double y);
  virtual bool damage(double x, double y);
  virtual bool isBlockingObjectAt(double x, double y);
  virtual Peach* getPeach() { return m_peach; }
  virtual void newFireball(double x, double y, int dir, int type);
  virtual void newGoodie(double x, double y, int type);
  virtual void newShell(double x, double y, int dir);
  virtual void deleteActor();
  bool isOverlap(double x, double y);
  bool isOverlapPeach(double x, double y);
  void moveToNext();
  void reachedMario();
  bool getPeachPower(int type);
  void gainPeachPower(int type, int num = 0);
  void bonkPeach();
  void damagePeach();
  double getPeachX();
  double getPeachY();

private:
  std::vector<Actor*> m_actors;
  Peach* m_peach;
  std::string intToString(int input);
  bool m_nexLevel;
  bool m_reachedMario;

     /*
            starGoodie = 1 --> starPower
            flowerGoodie = 2 --> shootPower
            mushroomGoodie = 3 --> jumpPower
            hit point = 4 --> hit point
        */
};

#endif // STUDENTWORLD_H_
