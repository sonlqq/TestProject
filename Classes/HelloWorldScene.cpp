#include "HelloWorldScene.h"
#include <math.h>
USING_NS_CC;

#define BORDER_HEIGHT 10
#define BANNER_HEIGHT 100

enum GameSate
{
	Intro,
	Start,
	GameOver
};

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
void HelloWorld::resetData()
{
	
	this->currentNumber = 1;
	if (gameState != GameOver)
		this->currentLevel = 1;
	
	offset = 20;
	
	labelNumber.clear();
	path.clear();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (visibleSize.width>960) offset = 60;
	this->gameState = Intro;

}

void HelloWorld::drawUI()
{
	Sprite* background = Sprite::create("background4.png");
	background->setAnchorPoint(Vec2(0,0));
	this->addChild(background, 1);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	int widthScreen = visibleSize.width;
	int heightScreen = visibleSize.height;
		 
	
	remainTime = CCLabelBMFont::create("", "fonts/font.fnt");
	remainTime->setAnchorPoint(Vec2(1,1));
	remainTime->setScale(0.4);
	remainTime->setPosition(Vec2(widthScreen - BORDER_HEIGHT, heightScreen - BORDER_HEIGHT));
	this->addChild(remainTime, 1);
	
	Label *remainTimeLabel = Label::createWithTTF("Remain time:", "fonts/Double_Bubble_shadow.ttf",20);
	remainTimeLabel->enableOutline(Color4B::BLUE,1);
	remainTimeLabel->setColor(Color3B(255,0,0));
	remainTimeLabel->setAnchorPoint(Vec2(1,1));
	//remainTimeLabel->setScale(0.4);
	remainTimeLabel->setPosition(Vec2(remainTime->getPositionX() - 3.5*BORDER_HEIGHT, heightScreen - BORDER_HEIGHT));
	this->addChild(remainTimeLabel, 1);

	CCLabelBMFont *findNumberLabel = CCLabelBMFont::create("Find number:", "fonts/futura-48.fnt");
	findNumberLabel->setAnchorPoint(Vec2(0,1));
	findNumberLabel->setScale(0.4);
	findNumberLabel->setPosition(Vec2(BORDER_HEIGHT, heightScreen - BORDER_HEIGHT));
	this->addChild(findNumberLabel, 1);

	findNumber = CCLabelBMFont::create("1", "fonts/font.fnt");
	findNumber->setAnchorPoint(Vec2(0,1));
	findNumber->setScale(0.4);
	findNumber->setPosition(Vec2(findNumberLabel->getPositionX() + findNumberLabel->getBoundingBox().size.width*0.4 + BORDER_HEIGHT/2, heightScreen - BORDER_HEIGHT));
	this->addChild(findNumber, 1);

	this->scheduleUpdate();

}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	resetData();
	drawUI();
	this->setTouchEnabled(true);

		
	/*fuelBarBorder =	 CCSprite::create("progress_bg.png");

	fuelBarBorder->setPosition(ccp(widthScreen/2, heightScreen-50));

	this->addChild(fuelBarBorder,1);*/

	/*progressTimer = CCProgressTimer::create( CCSprite::create("progress.png") );
    if ( progressTimer != NULL )
    {
        progressTimer->setType(kCCProgressTimerTypeBar);
        progressTimer->setMidpoint(ccp(0, 1));
        progressTimer->setBarChangeRate(ccp(1, 0));
        progressTimer->setPercentage(100);
		progressTimer->setPosition(ccp(widthScreen - progressTimer->getBoundingBox().size.width/2, heightScreen - progressTimer->getBoundingBox().size.height));
        this->addChild(progressTimer,-1);
    }*/
	
    return true;
}

void HelloWorld::updateTimeLeft(float dt)
{
	long currentTime = time(NULL);
	if (currentTime - initTime >= 1)
	{
		
		initTime = currentTime;
		remainTime->setString(std::to_string(--timeForOneLevel));
	}

	//progressTimer->setPercentage( progressTimer->getPercentage()-(dt));
   

}

void HelloWorld::update (float dt)
{

	updateTimeLeft(dt);
	if (gameState == Intro)
	{
		gameState = Start;
		Size visibleSize = Director::getInstance()->getVisibleSize();
		int widthScreen = visibleSize.width;
		int heightScreen = visibleSize.height;
		generateData();
		generateNumber(widthScreen, heightScreen - BANNER_HEIGHT);
		drawNumber();
	}

	if (timeForOneLevel <= 0) gameOver();
	else if (path.size() == 0) nextLevel();
	
}



void HelloWorld::drawSquare (int numColumn, int numRow, int widthOfOneSquare, int heightOfOneSquare)
{
	LayerColor* blueSky = LayerColor::create(Color4B(0, 0, 0, 255)); 
	Size visibleSize = Director::getInstance()->getVisibleSize();
	blueSky->setContentSize(visibleSize);
	this->addChild(blueSky,-1);

	int widthScreen = visibleSize.width;
	int heightScreen = visibleSize.height;

	auto draw_node = DrawNode::create();
	Color4F color(1, 1, 0, 0.5);

	//-------------------------------

	for (int j = 0; j < numColumn-1; j++)
		draw_node->drawSegment(Vec2(widthOfOneSquare * (j+1), 1), Vec2(widthOfOneSquare * (j+1), heightScreen-1),0.5, color);  
	for (int i = 0; i < numRow-1; i++)
		draw_node->drawSegment(Vec2(1, heightOfOneSquare * (i+1)), Vec2(widthScreen-1, heightOfOneSquare * (i+1)),1.0, color);  
	//addChild(draw_node, -1);

	//--------------------------------------------

	Color4F color1(1, 0, 0, 1);
	//auto draw_node = DrawNode::create();

	//if( path.size() > 2) {
	  
		for(int i = 0; i < path.size(); i++){
		  
			CCPoint point = path.at(i);
			draw_node->drawDot(point, 5, color1);  
		  
		}
	   addChild(draw_node, -1);
	   
	//}
		path.clear();
}



Color3B HelloWorld::getRandomColor()
{
	Color3B array[] = {Color3B::MAGENTA, Color3B::RED, Color3B::BLUE,Color3B::ORANGE}; 
	int color = randomValueBetween(0,3);
	return array[color];
}

void HelloWorld::drawNumber(Color3B color, bool isRotationNumber)
{
	initTime = time(NULL);
	srand(unsigned(time(NULL)));
	random_shuffle(path.begin(), path.end());
	int temp = currentNumber;
	Label *label;
	
    for(int i = 0; i < path.size(); i++){
		//label = CCLabelBMFont::create("", "fonts/font.fnt");
		label = Label::createWithTTF("Remain time:", "fonts/Double_Bubble_shadow.ttf",25);
		label->setColor(color);
		//label->setScale(0.4);
		
		label->setString(std::to_string(temp));
		label->setPosition(path.at(i));
		if (isRotationNumber)
			label->setRotation(randomValueBetween(-45,45));
		this->addChild(label, 1);
		temp++;
		labelNumber.push_back(label);
	}
	
	//path.clear();

}

float HelloWorld::randomValueBetween(float low, float high) 
{

	 return low + ((high - low) * CCRANDOM_0_1());
}

//float HelloWorld::randomValueBetween(float low, float high) {
//	 return (((float) rand() / 0xFFFFFFFFu) * (high - low)) + low;
//}

void HelloWorld::generateData()
{
	if (currentLevel < 11)
	{
		timeForOneLevel = (currentLevel*0.5)*60;
		numpoint = (currentLevel+1)*10;
	}
	else
	{

	}

	
}
void HelloWorld::generateNumber(/*int numpoint,*/ int width, int height)
{
	//----------------------------
	int m = 0;
	int n = 0;
	
	for (int i=sqrt(numpoint); i>=2; i--)
	{
		if ( numpoint % i == 0) 
		{
			n = i;
			m = numpoint / i;
			break;
		}

	}
	//----------------------------

	int numRow = n;
	int numColumn = m;
	int heightOfOneSquare = height / numRow;
	int widthOfOneSquare = width / numColumn;
	int currentPostion = 0;
	int _x, _y, temp_x, temp_y;
	for (int i = 0; i < numRow; i++)
		for (int j = 0; j < numColumn; j++)
		{
			
			_x = int (randomValueBetween(widthOfOneSquare * j, widthOfOneSquare * (j+1) - 1));
			_y = int (randomValueBetween(heightOfOneSquare * i, heightOfOneSquare * (i+1) -1));

			if (_x < 10) _x = 10;
			else if( _x > width - 10 ) _x = width - 10;
			if (_y < 10) _y = 10;
			else if( _y > height - 10 ) _y = height - 10;

			

			if ( /*(currentPostion -1) % 10 !=0 &&*/ currentPostion >numColumn && currentPostion < numpoint)
			{
				
				temp_x = abs(path.at(currentPostion-numColumn).x - _x);
				temp_y = abs(path.at(currentPostion-numColumn).y -_y);
				if ( temp_x < offset &&  temp_y < offset )
				
				{
					//if (temp_x < 10) _x += 10;
					_y += offset;
				}

			}

			if ( /*(currentPostion -1) % 10 !=0 &&*/ currentPostion >=1 && currentPostion < numpoint)
			{
				
				temp_x = abs(path.at(currentPostion-1).x - _x);
				temp_y = abs(path.at(currentPostion-1).y -_y);
				if ( temp_x < offset &&  temp_y < offset )
				
				{
					 _x += offset;
					//if (temp_y < 10) _y += 10;
				}

			}

			currentPostion++;
			path.push_back(Vec2(_x,_y));

		}

	//drawNumber();
	//drawSquare (numColumn, numRow, widthOfOneSquare, heightOfOneSquare);

}

void HelloWorld::gameOver()
{
	if(gameState == GameOver) return;
	gameState = GameOver;
	Size winSize = Director::getInstance()->getWinSize();

	Sprite* gameOverSprite = Sprite::create("game_over.png");
	gameOverSprite->setAnchorPoint(Vec2(0,0));
	this->addChild(gameOverSprite,1);
	// Tạo 1 nút reset game là 1 label
	LabelBMFont * restartLabel = LabelBMFont::create("Restart", "fonts/futura-48.fnt");
	restartLabel->setScale(0.3);
	MenuItemLabel *restartItem = MenuItemLabel::create(restartLabel,CC_CALLBACK_1(HelloWorld::resetGame,this));

	restartItem->setScale(0.1);
	restartItem->setPosition( Point(winSize.width/2, winSize.height*0.4));

	Menu *menu = Menu::create(restartItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,1);

	restartItem->runAction(ScaleTo::create(0.5, 1.0));
	this->unscheduleUpdate(); // dừng update Scene

}

void HelloWorld::resetGame(Ref* pSender) {		

	/*auto scene = HelloWorld::createScene();

	Director::getInstance()->replaceScene(TransitionZoomFlipY::create(0.5, scene));	 */
	this->removeAllChildren();
	resetData();
	this->currentLevel = currentLevel;
	drawUI();


}

void HelloWorld::nextLevel()
{
	if(gameState == GameOver) return;
	gameState = GameOver;
	Size winSize = Director::getInstance()->getWinSize();

	Sprite* nextLevelSprite = Sprite::create("next_level.png");
	nextLevelSprite->setAnchorPoint(Vec2(0,0));
	this->addChild(nextLevelSprite,1);
	// Tạo 1 nút reset game là 1 label
	LabelBMFont * nextLevelLabel = LabelBMFont::create("Next Level", "fonts/futura-48.fnt");
	nextLevelLabel->setScale(0.3);
	MenuItemLabel *nextLevelItem = MenuItemLabel::create(nextLevelLabel,CC_CALLBACK_1(HelloWorld::goToNextLevel,this));

	nextLevelItem->setScale(0.1);
	nextLevelItem->setPosition( Point(winSize.width/2, winSize.height*0.4));

	Menu *menu = Menu::create(nextLevelItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,1);

	nextLevelItem->runAction(ScaleTo::create(0.5, 1.0));
	this->unscheduleUpdate(); // dừng update Scene
}
void HelloWorld:: goToNextLevel (Ref* pSender)
{
	this->removeAllChildren();
	resetData();
	this->currentLevel ++;
	drawUI();
	/*Size visibleSize = Director::getInstance()->getVisibleSize();
	int widthScreen = visibleSize.width;
	int heightScreen = visibleSize.height;

	generateNumber(widthScreen, heightScreen - BANNER_HEIGHT);
	drawNumber();*/

}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (gameState == Intro) return;
	if (gameState == GameOver) return; 

	CCPoint touchPoint;

	for( auto& touch : touches)
	{
		touchPoint = touch->getLocation();
	}

	touchPoint = this->convertToNodeSpace(touchPoint);

	CCPoint currentPoint = path.at(0);
	Label* label = labelNumber.at(0);
	CCParticleExplosion* firework;
	if( touchPoint.x >= currentPoint.x-offset && touchPoint.x <= currentPoint.x+offset && touchPoint.y >= currentPoint.y-offset && touchPoint.y <= currentPoint.y+offset)
	{
		if(path.size() !=1)
		{
			currentNumber +=1;
			findNumber->setString(std::to_string(currentNumber));
		}
		else
		{
			findNumber->setString("--");
		}
		//label->setString("");
		label->setCascadeOpacityEnabled(true);
		CCFadeOut *fade = CCFadeOut::create(0.4);
		label->runAction(fade);
		labelNumber.erase(labelNumber.begin());
		path.erase(path.begin());
		//firework = CCParticleExplosion::create();
		//firework->setPosition(touchPoint);
		//firework->setScale(0.5);
		//firework->setSpeed(234);
		//firework->setSpeedVar(1);
		//firework->setStartSize(2);
		////firework->setStartSizeVar(0);
		//firework->setEndSize(10);
		////firework->setEndSizeVar(0);
		//firework->setLife(0);
		//firework->setLifeVar(0.708);

		ParticleExplosion* m_emitter =  ParticleExplosion::create();
		// Radius Mode
		m_emitter->setEmitterMode( kCCParticleModeRadius);

		// Radius Mode: startRadius
		m_emitter->setStartRadius(0);
		m_emitter->setStartRadiusVar(0);//ccp(0,0);
		m_emitter->setScale(0.5);
		//m_emitter->setSpeed(234);
		//m_emitter->setSpeedVar(1);
		m_emitter->setStartSize(20);
		//m_emitter->setStartSizeVar(0);
		m_emitter->setEndSize(100);
		//m_emitter->setEndSizeVar(0);
		m_emitter->setLife(0);
		m_emitter->setLifeVar(0.708);

		m_emitter->setAutoRemoveOnFinish(true);
		m_emitter->setPosition(touchPoint);
		
		
		this->addChild(m_emitter,1);
	}
}
