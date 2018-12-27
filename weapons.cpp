#include "weapons.h"
#include "BasicTools.h"

WEAPON::WEAPON(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
}

//return true if it hits the sphere with radius r.
bool WEAPON::hitTarget_Sphere(const Vector3 &p, Real r )
{
	bool flg = false;

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
	mSceneNode->translate(mVelocity * evt.timeSinceLastFrame);
}