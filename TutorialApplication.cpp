//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"
#include "BasicTools.h"
#include "read_data.h"

using namespace Ogre;
BasicTutorial_00::BasicTutorial_00(void) {
	mFlgMotion = true;
	mToggle = 2.5;
	mKeyPressed = false;
	//
	mMainChar = 0;
	mCameraDistance = 0;
	mKeyPressedZoomMode = ZOOM_NONE;
	mCameraDistanceAdjustSpeed = 0;
	mCameraDistanceSlowDownSpeed = 0;

	mSphereTimeCount = 0;

	mDialogueScore = 0;
	mDialogueUpdateSpeed = 0;
	mDialoguePosition = 0;
	mDialogueGrowDirection = true;

	mEnergy = 1.0;
}

//
//handle the mouse motion (passive) event
//
bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg )
{
	bool flg = BaseApplication::mouseMoved(arg);
	mMainChar->updateViewDirection();
	return flg;
}

//handle the key pressed event
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
	//bool flg = BaseApplication::keyPressed(arg);
	bool flg = true;
	
	if (arg.key == OIS::KC_W) {
		mMainChar->setWalkForward();
	}
	
	if (arg.key == OIS::KC_S) {
		mMainChar->setWalkBackward();
	}

	if (arg.key == OIS::KC_P) {
		MONSTER::PARTICLE_ACTIVATE = !MONSTER::PARTICLE_ACTIVATE;
		if (!MONSTER::PARTICLE_ACTIVATE) {
			mMonsterMgr->setMonstersParticleSystemState(false);
		}
	}

	if (arg.key == OIS::KC_A) {
		mKeyPressedZoomMode |= ZOOM_IN;
	}

	if (arg.key == OIS::KC_D) {
		mKeyPressedZoomMode |= ZOOM_OUT;
	}

	if (arg.key == OIS::KC_SPACE)
	{
		mMainChar->setFireAction_Normal();
	}

	return flg;
}

//handle the key released event
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
	bool flg = true;

	if (arg.key == OIS::KC_W) {
		mMainChar->unsetWalkForward();
	}
	
	if (arg.key == OIS::KC_S) {
		mMainChar->unsetWalkBackward();
	}

	if (arg.key == OIS::KC_A) {
		mKeyPressedZoomMode ^= ZOOM_IN;
	}

	if (arg.key == OIS::KC_D) {
		mKeyPressedZoomMode ^= ZOOM_OUT;
	}

	if (arg.key == OIS::KC_SPACE)
	{
		mMainChar->unsetFireAction_Normal();
	}

	if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}
	return flg;
}
void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(
	ST_EXTERIOR_CLOSE, "primary");
	mSceneMgrArr[1] =mSceneMgrArr[0];

}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,100,0));
	//mCamera->lookAt(Ogre::Vector3(0,50,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam2");
	mCamera->setPosition(Ogre::Vector3(750,500,750));
	mCamera->lookAt(Ogre::Vector3(751,0,750));
	mCamera->setNearClipDistance(5);
	//mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
    mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	vp->setVisibilityMask(VIEWPORT_0);
}

void BasicTutorial_00::createViewport_01(void)
{
	mCamera = mCameraArr[1];

	Ogre::Viewport* vp = mWindow->addViewport(
		mCamera,
		1,
		0.75,
		0,
		0.25,
		0.25
		);
	vp->setBackgroundColour(Ogre::ColourValue(0,1,0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setOverlaysEnabled(false);
	vp->setVisibilityMask(VIEWPORT_1);
}

void BasicTutorial_00::createScene_00(void) 
{
    /////////////////////////////////////
	DATA_READER::readData(); //Read the data file here.
	bt_Init(mTrayMgr, mSceneMgrArr[0], mCameraArr[0]); //Set the pointers of the objects so that the other functions can use the objects.
    /////////////////////////////////////

	mSceneMgr = mSceneMgrArr[0];
	
	ColourValue fadeColour(0.9, 0.9, 0.9); 
	// set fog
	if (DATA_READER::isEnabledExpFog()) {
		//mSceneMgr->setFog(FOG_EXP, fadeColour, DATA_READER::getExpFogDensity()); 
	}
	// set shawdow
	if (DATA_READER::isEnabledShadow()) {
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	}
	// set skybox
	mSceneMgr->setSkyBox(true, "Examples/TrippySkyBox");

	// set tarrian
	mSceneMgr->setWorldGeometry("terrain.cfg");
	
		

	Vector3 org = Vector3(750, 51, 750);
    //mSceneMgr->setAmbientLight( ColourValue( 0.7, 0.7, 0.7 ) ); 


	initParticleSystemForExplosion();
	Vector3 pos;
	setOffParticleSystem(fNode, "explosion", pos);

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT);
	//light->setType(Light::LT_DIRECTIONAL);
	light->setPosition(Vector3(0, 450, 250)+org); 
	light->setDiffuseColour(0.7, 0.7, 0.7);		
	light->setSpecularColour(1.0, 1.0, 1.0);	
	light->setDirection(Vector3(1, 1, 1));
	mLight0 = light;

	// plane
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 3000, 3000, 
		20, 20, 
		true, 
		1, 
		5, 5, 
		Ogre::Vector3::UNIT_Z);
	Entity *ent = mSceneMgr->createEntity("GroundEntity", "ground");
	ent->setMaterialName(DATA_READER::getWaterMaterialName());
	mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(750, DATA_READER::getWaterCoord_Y(), 750))->attachObject(ent);

	// Large Sphere
	ent = mSceneMgr->createEntity("Large Sphere", "sphere.mesh");
	ent->setMaterialName("Examples/Rockwall");
	mLargeSphere = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLargeSphere->attachObject(ent);
	mLargeSphere->scale(2, 2, 2);
	mLargeSphere->setPosition(900, 100, 900);

	mMainChar = new MAIN_CHAR(mSceneMgrArr[0]);
	mMainChar->createGameObj("m", "robot.mesh");

	mMainChar->attachCamera(mCameraArr[0]);

	mMainChar->setMaxBulletsNum(DATA_READER::getMaxBulletsNum());
	mMainChar->setVisibilityFlags(VIEWPORT_1);
	mMainChar->getWeaponManager()->setTarget(mLargeSphere, 200.0);
	mMainChar->setTopCamera(mCameraArr[1]);
	mMainChar->initParticleSystem();
	mMainChar->setOffParticleSystem(true);

	// set player's spotlight
	mPlayerSpotlight = mSceneMgr->createLight("PlayerSpotlight");
	mPlayerSpotlight->setType(Light::LT_SPOTLIGHT);
	mPlayerSpotlight->setPosition(mMainChar->getPosition() + Vector3(0, 10, 0)); 
	mPlayerSpotlight->setDiffuseColour(0.0, 1.0, 1.0);		
	mPlayerSpotlight->setSpecularColour(1.0, 1.0, 1.0);
	mPlayerSpotlight->setDirection(0, 1, 0);
	mPlayerSpotlight->setSpotlightRange(Degree(30), Degree(90));

	mMonsterMgr = new MONSTER_MANAGER(mSceneMgrArr[0]);

	mMonsterMgr->setTargetForMonsters(mMainChar);
	mMonsterMgr->setMaxMonstersNum(DATA_READER::getMaxMonstersNum());

	// UI setting
	// Number
	mNumberDialogue = new DIGIT_STRING_DIALOGUE(mSceneMgr, "Examples/Number");

	// Life bar
	mEnergyBar = new BAR_2D(mSceneMgr, "wago_game/LifeBar", NULL);
	mEnergyBar->setDefaultPos(0.05, 0.3);
	mEnergyBar->setVisibilityFlags(VIEWPORT_0);
	mSpeedBar = new BAR_2D(mSceneMgr, "wago_game/LifeBar", NULL);
	mSpeedBar->setDefaultPos(0.05, 0.2);
	mSpeedBar->setVisibilityFlags(VIEWPORT_0);
}

void BasicTutorial_00::createScene_01(void) 
{
	return;
}

/*
Implement the game logic in this function.
*/
bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
	mToggle += evt.timeSinceLastFrame;
    //

	
	
	// camera
	if (mKeyPressedZoomMode & ZOOM_IN) {
		std::cout << "camera down\n";
		Vector3 pos = mCameraArr[1]->getPosition();
		pos.y -= 10;
		mCameraArr[1]->setPosition(pos);
	}

	if (mKeyPressedZoomMode & ZOOM_OUT) {
		std::cout << "camera up\n";
		Vector3 pos = mCameraArr[1]->getPosition();
		pos.y += 10;
		mCameraArr[1]->setPosition(pos);
	}

	mMainChar->update(evt, mEnergy);	
    mMonsterMgr->update(evt);
	largeSphereMove(evt);
	numberDialogueUpdate(evt);
	barUpdate(evt);

	mPlayerSpotlight->setPosition(mMainChar->getPosition() + Vector3(0, 20, 0)); 
	return flg;
}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
	mCamera = mCameraArr[0];
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
	mCamera = mCameraArr[0];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
}

void BasicTutorial_00::initParticleSystemForExplosion() {
	fNode = static_cast<SceneNode*>(
		mSceneMgr->getRootSceneNode()->createChild());
	//ParticleSystem* p = mSceneMgr->createParticleSystem(
	//		"explosion", "Examples/JetEngine1");
	ParticleSystem* p = mSceneMgr->createParticleSystem(
		"explosion", "Examples/Smoke");

	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(false);
	fNode->attachObject(p);
	fNode->setPosition(Vector3::ZERO);
	fNode->setVisible(false);
}

void BasicTutorial_00::setOffParticleSystem(
	Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos) 
{
	fNode->setPosition(pos);
	fNode->setVisible(true);
	MovableObject *s = fNode->getAttachedObject(pname);
	ParticleSystem* p =static_cast<ParticleSystem*>(s);
	if (p == NULL) return;
	p->setVisible(true);
	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(true);
}

void BasicTutorial_00::largeSphereMove(const Ogre::FrameEvent& evt)
{
	mSphereTimeCount += evt.timeSinceLastFrame;
	Vector3 pos = mLargeSphere->getPosition();
	bool flg = projectScenePointOntoTerrain_PosDirection(pos);
	if (flg == false) {
		projectScenePointOntoTerrain_NegDirection(pos);
	}

	pos.y += (cos(mSphereTimeCount * 2.0) + 1.0) * 300.0 + 300;
	mLargeSphere->setPosition(pos);
}

void BasicTutorial_00::numberDialogueUpdate(const Ogre::FrameEvent& evt)
{

	mDialogueUpdateSpeed += evt.timeSinceLastFrame;
	if (mDialogueUpdateSpeed > 0.05) {
		mDialogueUpdateSpeed -= 0.05;
		if (mDialogueGrowDirection)
			mDialogueScore++;
		else 
			mDialogueScore--;
	}

	if (mDialogueGrowDirection)
		mDialoguePosition += evt.timeSinceLastFrame;
	else 
		mDialoguePosition -= evt.timeSinceLastFrame;

	if (mDialogueScore > 1000 && mDialogueGrowDirection) {
		mDialoguePosition -= evt.timeSinceLastFrame;
		mDialogueScore = 999;
		mDialogueGrowDirection = false;
	}
	if (mDialogueScore < 0 && !mDialogueGrowDirection) {
		mDialoguePosition += evt.timeSinceLastFrame;
		mDialogueScore = 0;
		mDialogueGrowDirection = true;
	}

	mNumberDialogue->setScore(mDialogueScore,0.05+mDialoguePosition*0.01, 0.05);
}

void BasicTutorial_00::barUpdate(const Ogre::FrameEvent& evt)
{
	if ((mMainChar->getActionMode() & ACTION_WALK_FORWARD) ||
		(mMainChar->getActionMode() & ACTION_WALK_BACKWARD)) {
		mEnergy -= evt.timeSinceLastFrame * 0.1;
	}
	else {
		mEnergy += evt.timeSinceLastFrame * 0.5;
	}

	mEnergy = std::min(1.0f, std::max(0.0f, mEnergy));

	mEnergyBar->setSplit2Parts(true);
	mEnergyBar->setInfo(mEnergy, 1.0);
	mEnergyBar->update(0, 0);

	mSpeedBar->setSplit2Parts(true);
	mSpeedBar->setInfo(mEnergy * mEnergy * mEnergy, 1.0);
	mSpeedBar->update(0, 0);
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
