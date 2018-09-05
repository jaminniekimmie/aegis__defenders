#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "bullet.h"			//미사일 클래스 사용하기 위해
#include "progressBar.h"	//체력바 클래스 사용하기 위해

//#include "enemyManager.h"
//적매니져 상호참조하기 위해 클래스 전방선언

#define ANGLE getAngle()

class player : public gameNode
{
private:
	PLAYERSTATE _playerState;
	DIRECTION _direction;
	RECT _rc;
	tagImage _player_clu[24];
	float _x, _y;
	float _oldX, _oldY;
	float _gravity;
	float _speed;
	float _angle;
	int _animationSpeed;
	int _index;
	int _count;
	bool _isLeft;
	bool _isJump;
	bool _isFall;
	bool _isBackstep;
	bool _onLand;

	progressBar* _hpBar;		//체력바
	float _maxHp, _currentHp;	//최대체력, 현재체력

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//체력바 피통깍기
	void hitDamage(float damage);

	PLAYERSTATE getState() { return _playerState; }
	RECT getPlayerRc() { return _rc; }
	DIRECTION getDirection() { return _direction; }
	image* getPlayerImage(int playerState) { return _player_clu[playerState].img; }
	int getAnimationSpeed() { return _animationSpeed; }
	int getIndex() { return _index; }
	float getX() { return _x; }
	float getY() { return _y; }
	float getOldX() { return _oldX; }
	float getOldY() { return _oldY; }
	float getGravity() { return _gravity; }
	float getSpeed() { return _speed; }
	float getAngle() { return _angle; }
	bool getIsLeft() { return _isLeft; }
	bool getIsJump() { return _isJump; }
	bool getIsFall() { return _isFall; }
	bool getOnLand() { return _onLand; }
	bool getIsBackstep() { return _isBackstep; }

	void setState(PLAYERSTATE playerState) { _playerState = playerState; }
	void setPlayerRc(RECT rc) { _rc = rc; }
	void setDirection(DIRECTION direction) { _direction = direction; }
	void setAnimationSpeed(int animationSpeed) { _animationSpeed = animationSpeed; }
	void setIndex(int index) { _index = index; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setOldX(float oldX) { _x = oldX; }
	void setOldY(float oldY) { _y = oldY; }
	void setGravity(float gravity) { _gravity = gravity; }
	void setSpeed(float speed) { _speed = speed; }
	void setAngle(float angle) { _angle = angle; }
	void setIsLeft(bool isLeft) { _isLeft = isLeft; }
	void setIsJump(bool isJump) { _isJump = isJump; }
	void setIsFall(bool isFall) { _isFall = isFall; }
	void setOnLand(bool onLand) { _onLand = onLand; }
	void setIsBackstep(bool isBackstep) { _isBackstep = isBackstep; }
	
	void frameChangeLoop();
	void frameChangeOnce();

	player() {}
	~player() {}
};

