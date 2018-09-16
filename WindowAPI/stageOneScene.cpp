#include "stdafx.h"
#include "stageOneScene.h"

HRESULT stageOneScene::init(void)
{
	_playerManager = new playerManager;
	_playerManager->init();

	_pixelTiles = new image;
	_pixelTiles->init(TILESIZEX, TILESIZEY);

	_playerManager->getClu()->setX(WINSIZEX / 2);
	_playerManager->getClu()->setY(TILESIZEY - WINSIZEY);

	_camDebug = _sceneSwitch = false;
	_alpha = 255;
	
	if (_playerManager->getPlayerCharacter() == CLU)
		_rcCamera = RectMakeCenter(_playerManager->getClu()->getX(), _playerManager->getClu()->getY() - _playerManager->getClu()->getPlayerImage(_playerManager->getClu()->getState())->getFrameHeight() / 3, WINSIZEX, WINSIZEY);
	else if (_playerManager->getPlayerCharacter() == BART)
		_rcCamera = RectMakeCenter(_playerManager->getBart()->getX(), _playerManager->getBart()->getY() - _playerManager->getBart()->getPlayerImage(_playerManager->getClu()->getState())->getFrameHeight() / 3, WINSIZEX, WINSIZEY);

	CAMERAMANAGER->setCamera(_rcCamera);
	COLLISIONMANAGER->setPixelMap(_pixelTiles);
	MONSTERMANAGER->init(1);
	EFFECTMANAGER->init();

	MONSTERMANAGER->setPlayerManager(_playerManager);
	
	this->mapLoad();

	return S_OK;
}

void stageOneScene::release(void)
{
	_playerManager->release();
	SAFE_DELETE(_playerManager);
	_pixelTiles->release();
	SAFE_DELETE(_pixelTiles);
}

void stageOneScene::update(void)
{
	_playerManager->update();
	MONSTERMANAGER->update();

	//temporary
	if (KEYMANAGER->isOnceKeyDown('Q'))
		_sceneSwitch = true;

	this->cameraAdjustment();
	
	if (!_sceneSwitch)
	{
		if (_alpha > 0)
			_alpha -= 5;
	}
	else
	{
		_alpha += 5;
		if (_alpha >= 255)
			SCENEMANAGER->loadScene("����");
	}
}

void stageOneScene::render(void)
{
	PatBlt(_pixelTiles->getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);

	//����Ÿ�� ��Ʈ ����
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (!CAMERAMANAGER->CameraIn(_tiles[i].rc)) continue;
		
		IMAGEMANAGER->frameRender(_tiles[i].tileLabel, getMemDC(), _tiles[i].rc.left - CAMERAMANAGER->getCamera().left, _tiles[i].rc.top - CAMERAMANAGER->getCamera().top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
		IMAGEMANAGER->frameRender("pixel_map", _pixelTiles->getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
	
		if (_tiles[i].obj == OBJECT_NONE) continue;
		IMAGEMANAGER->frameRender(_tiles[i].tileLabel, getMemDC(), _tiles[i].rc.left - CAMERAMANAGER->getCamera().left, _tiles[i].rc.top - CAMERAMANAGER->getCamera().top, _tiles[i].objFrameX, _tiles[i].objFrameY);
		//IMAGEMANAGER->frameRender(_tiles[0].tileLabel, getMemDC(), 100, 100, _tiles[0].objFrameX, _tiles[0].objFrameY);
		IMAGEMANAGER->frameRender("pixel_map", _pixelTiles->getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
		
	}
	if (KEYMANAGER->isToggleKey('P'))
	{
		_pixelTiles->render(getMemDC(), -CAMERAMANAGER->getCamera().left, -CAMERAMANAGER->getCamera().top);
	}

	MONSTERMANAGER->render(getMemDC());
	_playerManager->render();
	
	if (_alpha > 0)
		IMAGEMANAGER->alphaRender("solid_black", getMemDC(), _alpha);
}

void stageOneScene::cameraAdjustment()
{
	if (KEYMANAGER->isOnceKeyDown('C'))
		_camDebug = !_camDebug;

	if (_camDebug)
	{
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			_rcCamera.left -= 30;
			_rcCamera.right -= 30;
		}

		if (KEYMANAGER->isStayKeyDown('D'))
		{
			_rcCamera.left += 30;
			_rcCamera.right += 30;
		}
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			_rcCamera.top -= 30;
			_rcCamera.bottom -= 30;
		}
		if (KEYMANAGER->isStayKeyDown('S'))
		{
			_rcCamera.top += 30;
			_rcCamera.bottom += 30;
		}
	}
	else
	{
		if (_playerManager->getPlayerCharacter() == CLU)
			_rcCamera = RectMakeCenter(_playerManager->getClu()->getX(), _playerManager->getClu()->getY() - _playerManager->getClu()->getPlayerImage(_playerManager->getClu()->getState())->getFrameHeight() / 3, WINSIZEX, WINSIZEY);
		else if (_playerManager->getPlayerCharacter() == BART)
			_rcCamera = RectMakeCenter(_playerManager->getBart()->getX(), _playerManager->getBart()->getY() - _playerManager->getBart()->getPlayerImage(_playerManager->getClu()->getState())->getFrameHeight() / 3, WINSIZEX, WINSIZEY);
	}

	if (KEYMANAGER->isOnceKeyDown('I') || KEYMANAGER->isOnceKeyDown('O'))
	{
		this->playerSwitch();
	}

	if (_rcCamera.left <= 0)
	{
		_rcCamera.left = 0;
		_rcCamera.right = _rcCamera.left + WINSIZEX;
	}
	else if (_rcCamera.right >= TILESIZEX)
	{
		_rcCamera.left = TILESIZEX - WINSIZEX;
		_rcCamera.right = _rcCamera.left + WINSIZEX;
	}
	if (_rcCamera.top <= 0)
	{
		_rcCamera.top = 0;
		_rcCamera.bottom = _rcCamera.top + WINSIZEY;
	}
	else if (_rcCamera.bottom >= TILESIZEY)
	{
		_rcCamera.top = TILESIZEY - WINSIZEY;
		_rcCamera.bottom = _rcCamera.top + WINSIZEY;
	}

	_rcCamera = RectMake(_rcCamera.left, _rcCamera.top, WINSIZEX, WINSIZEY);
	CAMERAMANAGER->setCamera(_rcCamera);
}

void stageOneScene::playerSwitch()
{
}

void stageOneScene::mapLoad(void)
{
	HANDLE file;
	DWORD read;

	file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);


	CloseHandle(file);
}
