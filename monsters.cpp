#include "monsters.h"
#include "BasicTools.h"
#include <algorithm>

bool MONSTER::PARTICLE_ACTIVATE = true;

MONSTER::MONSTER(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
	mTimeCount = 0.0;
	PARTICLE_ACTIVATE = true;
	mSpeed = 10.0;
}

/*
Update the view direction of the monster.
*/
void MONSTER::updateViewDirection()
{
	this->mSceneNode->yaw(Radian(30));
	Vector3 target = this->mTarget->getPosition();
	target.y = this->getPosition().y;
	this->mSceneNode->lookAt(target, Node::TransformSpace::TS_WORLD, Vector3::UNIT_Z);
	//std::cout << this->mTarget->getPosition() << std::endl;
}

/*
Update the position and view direction of the monster.
*/
void MONSTER::update(const Ogre::FrameEvent& evt)
{
    //compute the position of the monster
    //call updateViewDirection( )
	move(evt);

	mTimeCount += evt.timeSinceLastFrame;
	Vector3 pos = getPosition();
	bool flg = projectScenePointOntoTerrain_PosDirection(pos);
	if (flg == false) {
		projectScenePointOntoTerrain_NegDirection(pos);
	}

	pos.y += (cos(mTimeCount * 3) + 1.0) * 40.0 + 20;
	this->setPosition(pos);

	this->updateViewDirection();
}

void MONSTER::move(const Ogre::FrameEvent& evt) 
{
	Vector3 displace = mTarget->getPosition() - this->mSceneNode->getPosition();
	displace.y = 0.0;

	float distance = displace.normalise();
	if (distance < 50) {
		this->mSceneNode->translate(-displace * mSpeed * evt.timeSinceLastFrame);
	}
	else {
		this->mSceneNode->translate(displace * mSpeed * evt.timeSinceLastFrame);
	}

	if (distance < 80 && PARTICLE_ACTIVATE) {
		this->setOffParticleSystem(true);
	}
	else {
		this->setOffParticleSystem(false);
	}
}