#ifndef __MONSTERS_H__
#define __MONSTERS_H__
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>

#include "game_obj.h"
#include "game_obj.h"
class MONSTER : public GAME_OBJ {
protected:
	void updateViewDirection();

	float mTimeCount;
	float mSpeed;
	SceneNode *particleSystem;
	void move(const Ogre::FrameEvent& evt);
public:
    static bool PARTICLE_ACTIVATE;

	MONSTER(SceneManager *a_SceneMgr);
	virtual void update(const Ogre::FrameEvent& evt);
};

#endif