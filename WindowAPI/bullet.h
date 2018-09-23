#pragma once
#include "gameNode.h"

//총알 구조체
struct tagBullet
{
	image* bulletImage;
	RECT rc;
	BYTE alpha;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float gravity;
	float radius;
	bool fire;
	int count;
};

//=============================================================
//	## bullet ## (공용총알 - 너희들이 만들면 된다)
//=============================================================
class bullet : public gameNode
{
private:
	//총알 구조체를 담을 벡터, 반복자
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
	vector<tagBullet> _vParticle;
	vector<tagBullet>::iterator _viParticle;
	vector<tagBullet> _vMagazine;
	vector<tagBullet>::iterator _viMagazine;

private:
	const char* _imageName;		//총알 이미지 이름(키값)
	float _range;			//총알 사거리
	int _bulletMax;			//총알 최대갯수
	int a;
	bool _isFrameImg;		//이미지매니져에 들어 있는 이미지가 프레임이 있냐 없냐?

public:
	HRESULT init(const char* imageName, int bulletMax, float range);
	void release(void);
	void update(void);
	void render(void);

	//총알발사
	void fire(float x, float y, float angle, float speed);
	//총알무브
	void move();
	
	void collision();

	//총알삭제
	void removeBullet(int index);

	//총알벡터 가져오기
	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
	vector<tagBullet> getVParticle() { return _vParticle; }
	vector<tagBullet>::iterator getViParticle() { return _viParticle; }
	vector<tagBullet> getVMagazine() { return _vMagazine; }
	vector<tagBullet>::iterator getViMagazine() { return _viMagazine; }

	bullet() {}
	~bullet() {}
};

//=============================================================
//	## missile ## (missile[0] -> 배열처럼 미리 장전해두고 총알발사)
//=============================================================
class triBullet : public gameNode
{
private:
	//총알 구조체를 담을 벡터, 반복자
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
	vector<tagBullet> _vParticle;
	vector<tagBullet>::iterator _viParticle;
	vector<tagBullet> _vMagazine;
	vector<tagBullet>::iterator _viMagazine;



private:
	const char * _imageName;
	float _range;			//총알 사거리
	int _bulletMax;			//총알 최대갯수

public:
	HRESULT init(const char * imageName, int bulletMax, float range);
	void release(void);
	void update(void);
	void render(void);

	//총알발사
	void fire(float x, float y, float angle, float speed);
	//총알무브
	void move();

	void collision();

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
	vector<tagBullet> getVParticle() { return _vParticle; }
	vector<tagBullet>::iterator getViParticle() { return _viParticle; }
	vector<tagBullet> getVMagazine() { return _vMagazine; }
	vector<tagBullet>::iterator getViMagazine() { return _viMagazine; }

	triBullet() {}
	~triBullet() {}
};
