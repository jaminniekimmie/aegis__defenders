#pragma once
#include "gameNode.h"

class GUI : public gameNode
{
private:
	vector<tagImage> _vHeartRed;
	vector<tagImage> _vHeartYellow;
	tagImage _blueFlower;
	int _blueFlowerCount;
	tagImage _mineral;
	int _mineralCount;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	vector<tagImage> getHeartRed() { return _vHeartRed; }
	vector<tagImage> getHeartYellow() { return _vHeartYellow; }

	GUI() {}
	~GUI() {}
};

