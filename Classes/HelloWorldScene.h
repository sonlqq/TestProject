#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class HelloWorld : public cocos2d::Layer
{
private:
	int currentNumber;
	int offset;
	std::vector<CCPoint> path;
	std::vector<Label*> labelNumber;

	long initTime;
	int currentLevel;
	int timeForOneLevel;//in second
	int numpoint;
	/*CCProgressTimer* progressTimer;*/
	CCLabelBMFont *remainTime;
	CCLabelBMFont *findNumber;
	int gameState;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void resetData();
	void update(float dt);

	void drawUI();
	Color3B getRandomColor();
	void drawNumber(Color3B color = Color3B::MAGENTA, bool isRotationNumber = false);
	void generateData();
	void drawSquare (int numColumn, int numRow, int widthOfOneSquare, int heightOfOneSquare);
	void updateTimeLeft(float dt);
	void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

	void generateNumber(/*int numpoint,*/ int width, int height);
	float randomValueBetween(float low, float high);
	void gameOver();
	void resetGame(Ref* pSender);
	void nextLevel();
	void goToNextLevel (Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
