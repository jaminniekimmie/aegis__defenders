#include "stdafx.h"
#include "bullet.h"
//=============================================================
//	## bullet ## (공용총알 - 너희들이 만들면 된다)
//=============================================================
HRESULT bullet::init(const char * imageName, int bulletMax, float range)
{
	//총알 이미지 초기화
	_imageName = imageName;
	//총알갯수 및 사거리 초기화
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void bullet::release(void)
{
}

void bullet::update(void)
{
	this->move();
}

void bullet::render(void)
{
	_viParticle = _vParticle.begin();
	for (_viParticle; _viParticle != _vParticle.end(); ++_viParticle)
	{
		if (!_viParticle->fire) continue;
		_viParticle->bulletImage->alphaRender(getMemDC(), _viParticle->rc.left - CAMERAMANAGER->getCamera().left, _viParticle->rc.top - CAMERAMANAGER->getCamera().top, _viParticle->alpha);
	}

	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->bulletImage->render(getMemDC(), _viBullet->rc.left - CAMERAMANAGER->getCamera().left, _viBullet->rc.top - CAMERAMANAGER->getCamera().top);
	}
}

void bullet::fire(float x, float y, float angle, float speed)
{
	//총알 벡터에 담는것을 제한하자
	if (_bulletMax < _vBullet.size() + 1) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.alpha = 255;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getWidth(),
		bullet.bulletImage->getHeight());

	//벡터에 담기
	_vBullet.push_back(bullet);
	

	int randNo = RND->getFromIntTo(10, 15);

	for (int i = 0; i < randNo / 2; i++)
	{
		tagBullet particle;
		ZeroMemory(&particle, sizeof(tagBullet));
		particle.bulletImage = IMAGEMANAGER->findImage("dot_teal");
		particle.speed = speed;
		particle.angle = angle;
		particle.alpha = 255;
		particle.x = particle.fireX = x;
		particle.y = particle.fireY = y;
		if (angle > PI_2)
			particle.x = x - RND->getFloat(_range);
		if (angle < PI_2)
			particle.x = x + RND->getFloat(_range);
		particle.y = y + RND->getFloat(10) - 5;
		particle.fire = false;
		particle.rc = RectMakeCenter(particle.x, particle.y,
			particle.bulletImage->getWidth(),
			particle.bulletImage->getHeight());

		//벡터에 담기
		_vParticle.push_back(particle);
	}
	
	for (int i = 0; i < randNo; i++)
	{
		tagBullet particle;
		ZeroMemory(&particle, sizeof(tagBullet));
		particle.bulletImage = IMAGEMANAGER->findImage("dot_white");
		particle.speed = speed;
		particle.angle = angle;
		particle.alpha = 255;
		particle.x = particle.fireX = x;
		particle.y = particle.fireY = y;
		if (angle > PI_2)
			particle.x = x - RND->getFloat(_range);
		if (angle < PI_2)
			particle.x = x + RND->getFloat(_range);
		particle.y = y + RND->getFloat(10) - 5;
		particle.fire = false;
		particle.rc = RectMakeCenter(particle.x, particle.y,
			particle.bulletImage->getWidth(),
			particle.bulletImage->getHeight());

		//벡터에 담기
		_vParticle.push_back(particle);
	}
}

void bullet::move()
{
	RECT rcTemp;

	_viBullet = _vBullet.begin();
	for (; _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		//총알이 사거리보다 커졌을때
		float distance = getDistance(_viBullet->fireX, _viBullet->fireY,
			_viBullet->x, _viBullet->y);
		if (_range < distance)
		{
			EFFECTMANAGER->play("bulletPuff" + to_string(RND->getFromIntTo(1, 5)), _viBullet->rc.left, _viBullet->rc.top);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else
		{
			++_viBullet;
		}
	}

	for (int i = 0; i < _vParticle.size(); i++)
	{
		if (_vParticle[i].fire)
		{
			_vParticle[i].count++;

			if (_vParticle[i].count > 20)
			{
				_vParticle[i].alpha -= 15;
		
				if (_vParticle[i].alpha <= 0)
					_vParticle.erase(_vParticle.begin() + i);
			}
		}
		else
		{
			_vParticle[i].x = _vParticle[i].fireX + cosf(_vParticle[i].angle) * RND->getFloat(_range) + RND->getFloat(10) - 5;
			_vParticle[i].y = _vParticle[i].fireY - sinf(_vParticle[i].angle) * RND->getFloat(_range) + RND->getFloat(10) - 5;

			_vParticle[i].rc = RectMakeCenter(_vParticle[i].x, _vParticle[i].y,
				_vParticle[i].bulletImage->getWidth(),
				_vParticle[i].bulletImage->getHeight());

			for (int j = 0; j < _vBullet.size(); j++)
			{
				if (IntersectRect(&rcTemp, &_vParticle[i].rc, &_vBullet[j].rc))
					_vParticle[i].fire = true;
			}
		}
	}
}

void bullet::removeBullet(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}


//=============================================================
//	## missile ## (missile[0] -> 배열처럼 미리 장전해두고 총알발사)
//=============================================================
HRESULT triBullet::init(const char * imageName, int bulletMax, float range)
{
	//총알 이미지 초기화
	_imageName = imageName;
	//총알갯수 및 사거리 초기화
	_bulletMax = bulletMax;
	_range = range;
	
	//총알의 갯수만큼 구조체를 초기화 한 후 벡터에 담기
	for (int i = 0; i < bulletMax; i++)
	{
		//총알 구조체 선언
		tagBullet bullet;
		//제로메모리 또는 멤셋
		//구조체의 변수들의 값을 한번에 0으로 초기화 시켜준다
		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
		bullet.fire = false;
	
		//벡터에 담기
		_vBullet.push_back(bullet);
	}

	int randNo = RND->getFromIntTo(8, 15);

	for (int i = 0; i < randNo; i++)
	{
		//총알 구조체 선언
		tagBullet particle;
		//제로메모리 또는 멤셋
		//구조체의 변수들의 값을 한번에 0으로 초기화 시켜준다
		ZeroMemory(&particle, sizeof(tagBullet));
		particle.bulletImage = IMAGEMANAGER->findImage("dot_teal");
		particle.alpha = 255;
		particle.fire = false;

		//벡터에 담기
		_vParticle.push_back(particle);
	}

	for (int i = 0; i < randNo * _bulletMax; i++)
	{
		//총알 구조체 선언
		tagBullet particle;
		//제로메모리 또는 멤셋
		//구조체의 변수들의 값을 한번에 0으로 초기화 시켜준다
		ZeroMemory(&particle, sizeof(tagBullet));
		particle.bulletImage = IMAGEMANAGER->findImage("dot_white");
		particle.alpha = 255;
		particle.fire = false;

		//벡터에 담기
		_vParticle.push_back(particle);
	}

	return S_OK;
}

void triBullet::release(void)
{
}

void triBullet::update(void)
{
	this->move();
}

void triBullet::render(void)
{
	/*
	for (int i = 0; i < _vBullet.size(); i++)
	{
		if (!_vBullet[i].fire) continue;
		_vBullet[i].bulletImage->render(getMemDC(), _vBullet[i].rc.left, _vBullet[i].rc.top);
	}
	*/

	_viParticle = _vParticle.begin();
	for (_viParticle; _viParticle != _vParticle.end(); ++_viParticle)
	{
		if (!_viParticle->fire) continue;
		_viParticle->bulletImage->alphaRender(getMemDC(), _viParticle->rc.left - CAMERAMANAGER->getCamera().left, _viParticle->rc.top - CAMERAMANAGER->getCamera().top, _viParticle->alpha);
	}

	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;
		_viBullet->bulletImage->render(getMemDC(), _viBullet->rc.left - CAMERAMANAGER->getCamera().left, _viBullet->rc.top - CAMERAMANAGER->getCamera().top);
	}
}

void triBullet::fire(float x, float y, float angle, float speed)
{
	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->fire) continue;

		_viBullet->fire = true;
		_viBullet->speed = speed;
		_viBullet->angle = angle;
		_viBullet->alpha = 255;
		_viBullet->count = 0;
		_viBullet->x = _viBullet->fireX = x;
		_viBullet->y = _viBullet->fireY = y;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());
	}

	_viParticle = _vParticle.begin();
	for (_viParticle; _viParticle != _vParticle.end(); ++_viParticle)
	{
		//if (_viParticle->fire) continue;

		_viParticle->speed = speed;
		_viParticle->angle = angle;
		_viParticle->alpha = 255;
		_viParticle->count = 0;
		_viParticle->x = _viParticle->fireX = x;
		_viParticle->y = _viParticle->fireY = y;
		if (angle > PI_2)
			_viParticle->x = x - RND->getFloat(_range);
		if (angle < PI_2)
			_viParticle->x = x + RND->getFloat(_range);
		_viParticle->y = y + RND->getFloat(10) - 5;
		_viParticle->fire = false;
		_viParticle->rc = RectMakeCenter(_viParticle->x, _viParticle->y,
			_viParticle->bulletImage->getWidth(),
			_viParticle->bulletImage->getHeight());
	}
}

void triBullet::move()
{
	RECT rcTemp;

	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (!_vBullet[i].fire) continue;
		_vBullet[i].count += (i + 1) * 2;
		if (_vBullet[i].count >= _vBullet.size() * _vBullet.size() * 2)
		{
			_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
			_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;
		}
		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y,
			_vBullet[i].bulletImage->getWidth(),
			_vBullet[i].bulletImage->getHeight());

		//총알이 사거리보다 커졌을때 
		float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
			_vBullet[i].x, _vBullet[i].y);
		if (_range < distance)
		{
			EFFECTMANAGER->play("bulletPuff" + to_string(RND->getFromIntTo(1, 5)), _vBullet[i].rc.left, _vBullet[i].rc.top);
			_vBullet[i].fire = false;
		}
	}

	for (int i = 0; i < _vParticle.size(); i++)
	{
		if (_vParticle[i].fire)
		{
			_vParticle[i].count++;

			if (_vParticle[i].count > 20)
			{
				_vParticle[i].alpha -= 15;

				if (_vParticle[i].alpha <= 0)
					_vParticle[i].fire = false;
			}
		}
		else
		{
			_vParticle[i].x = _vParticle[i].fireX + cosf(_vParticle[i].angle) * RND->getFloat(_range) + RND->getFloat(10) - 5;
			_vParticle[i].y = _vParticle[i].fireY - sinf(_vParticle[i].angle) * RND->getFloat(_range) + RND->getFloat(10) - 5;

			_vParticle[i].rc = RectMakeCenter(_vParticle[i].x, _vParticle[i].y,
				_vParticle[i].bulletImage->getWidth(),
				_vParticle[i].bulletImage->getHeight());

			for (int j = 0; j < _vBullet.size(); j++)
			{
				if (IntersectRect(&rcTemp, &_vParticle[i].rc, &_vBullet[j].rc))
					_vParticle[i].fire = true;
			}
		}
	}
}

//=============================================================
//	## missileM1 ## (폭탄처럼 한발씩 발사하면서 생성하고 자동삭제)
//=============================================================
HRESULT missileM1::init(int bulletMax, float range)
{
	//총알갯수, 사거리 초기화
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void missileM1::release(void)
{
}

void missileM1::update(void)
{
	move();
}

void missileM1::render(void)
{
	_viBullet = _vBullet.begin();
	for (_viBullet; _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->bulletImage->frameRender(getMemDC(), _viBullet->rc.left, _viBullet->rc.top);
	
		_viBullet->count++;
		if (_viBullet->count % 3 == 0)
		{
			_viBullet->bulletImage->setFrameX(_viBullet->bulletImage->getFrameX() + 1);
			if (_viBullet->bulletImage->getFrameX() >= _viBullet->bulletImage->getMaxFrameX())
			{
				_viBullet->bulletImage->setFrameX(0);
			}
			_viBullet->count = 0;
		}
	}
}

void missileM1::fire(float x, float y)
{
	//총알 벡터에 담는것을 제한하자
	if (_bulletMax < _vBullet.size() + 1) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage->init("missile.bmp", 416, 64, 13, 1);
	bullet.speed = 5.0f;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	//벡터에 담기
	_vBullet.push_back(bullet);
}

void missileM1::move()
{
	_viBullet = _vBullet.begin();
	for (; _viBullet != _vBullet.end();)
	{
		_viBullet->y -= _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		//총알이 사거리보다 커졌을때
		float distance = getDistance(_viBullet->fireX, _viBullet->fireY,
			_viBullet->x, _viBullet->y);
		if (_range < distance)
		{
			_viBullet->bulletImage->release();
			SAFE_DELETE(_viBullet->bulletImage);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else
		{
			++_viBullet;
		}
	}
}

void missileM1::removeMissileM1(int index)
{
	_vBullet[index].bulletImage->release();
	SAFE_DELETE(_vBullet[index].bulletImage);
	_vBullet.erase(_vBullet.begin() + index);
}


