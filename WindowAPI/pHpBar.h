#pragma once
//=============================================================
//	## hpBar ## (프로그래스바 = 체력바)
//=============================================================

class pHpBar
{
private:
	RECT _rcHp;					//체력바 렉트
	int _x, _y;					//체력바 위치좌표
	float _frontWidth;				//체력바 가로길이
	float _height;				//체력바 세로길이
	float _backWidth;
	float _maxWidth;

	image* _hpBarFront;	//체력바 앞 이미지
	image* _hpBarBack;	//체력바 뒤 이미지

public:
	//체력바 초기화(앞이미지키, 백이미지키, x, y, 가로길이, 세로길이)
	HRESULT init(const char* frontImageKey, const char* backImageKey, int x, int y);
	void release(void);
	void update(void);
	void render(HDC hdc);

	//체력바 게이지 세팅하기
	void setGauge(float currentHp, float maxHp);

	//체력바 위치 좌표 세팅하기
	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }
	void setPosition(int x, int y) { _x = x, _y = y; }
	//체력바 렉트 가져오기
	RECT getRect() { return _rcHp; }

	pHpBar() {}
	~pHpBar() {}
};

