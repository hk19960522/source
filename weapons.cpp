#include "weapons.h"
#include "BasicTools.h"

WEAPON::WEAPON(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
}

//return true if it hits the sphere with radius r.
bool WEAPON::hitTarget_Sphere(const Vector3 &p, Real r )
{
	bool flg = false;
	
	Vector3 pos = mSceneNode->getPosition();
	if (r >= pos.distance(p)) {
		// hit
		Vector3 normal = pos - p;
		normal.normalise();

		Vector3 direction = mVelocity;
		float length = direction.normalise();
		Vector3 reflection = direction - 2.0 * direction.dotProduct(normal) * normal;

		mVelocity = reflection * length;
		flg = true;
	}
	return flg;
}

//
// Decrease its life and disable it if its life is negative
// Compute the position of the weapon
// Check collision between it and the terrain surface
// Adjust its position if it his the terrain surface
//
//
void WEAPON::update(const Ogre::FrameEvent& evt)
{
    /*
    //Check up direction
    bool flg = projectScenePointOntoTerrain_PosDirection(new_pos);
    if (flg) {
        pos = new_pos+Vector3(0,1.0, 0)+Vector3(0, 1, 0)*r;
        mVelocity.y = -mVelocity.y*0.9;
    }
    //
    //Check down direction. 
    //Why do we need to do it?
    */
	mLifeTime -= evt.timeSinceLastFrame;
	if (mLifeTime < 0 ) {
		mLifeTime = 0;
		mSceneNode->setVisible(false);
		mIsAlive = false;
		return;
	}
	
	Vector3 gravity(0, -40.0, 0);
	Vector3 newPos = mSceneNode->getPosition() + mVelocity * evt.timeSinceLastFrame;
	bool flg = projectScenePointOntoTerrain_PosDirection(newPos);
    if (flg) {
        newPos = newPos+Vector3(0,1.0, 0)+Vector3(0, 1, 0)*mRadius;
        mVelocity.y = -mVelocity.y*0.9;
    }

	Vector3 pos = newPos;
	flg = projectScenePointOntoTerrain_NegDirection(pos);
    if (!flg) {
		std::cout << "Out of range\n";
		mLifeTime = 0;
		mSceneNode->setVisible(false);
		mIsAlive = false;
		return;
		return;
    }
	mVelocity += gravity * evt.timeSinceLastFrame;
	mSceneNode->setPosition(newPos);
}