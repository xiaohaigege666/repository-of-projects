#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ICamera* pCamera2=NULL;
ILogger* logger=NULL;

IModel* cubeModel=NULL;
IModel* planeModel=NULL;
IModel* teapotModel=NULL;
video::ITexture* rtt=NULL;
f32 factor=1.1f;

class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				logger->debug("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.touchInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			}
		}
		return false;
	}
};

bool init(void *pJNIEnv,ICallback* pcb,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=60;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapterWindow();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera2=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0));
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT,true);

	//IMaterial* material;
	IShap *shap,*shap1,*shap2;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	{
		SMaterial& material=cubeModel->getMaterial(0);
		//material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
		material.MaterialType=ENUM_MATERIAL_TYPE_TRANSPARENT_REF;
	}
	cubeModel->setPosition(core::vector3df(100,100,0));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createTeapot(7,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	teapotModel->setPosition(core::vector3df(0,-70,0));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	{
		SMaterial& material=planeModel->getMaterial(0);
		material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
		material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
		material.BlendDst=ENUM_BLEND_FACTOR_ONE;
		material.setTexture(0,videoDriver->getTexture("aura.png"));
	}
	planeModel->setPosition(core::vector3df(0,0,0));
	shap->drop();
	unit->drop();
	entity->drop();

	
	pCamera2->setEventReceivable(false);
	rtt = videoDriver->addRenderTargetTexture(core::dimension2d<u32>(256,256), "RTT",video::ENUM_COLOR_FORMAT_R8G8B8A8);
	pCamera2->onResize(rtt->getSize());
	cubeModel->setMaterialTexture(0, rtt); 

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
//#define USE_SCENE_MGR
s32 temp=0;
void drawFrame(){

	videoDriver->begin();

	//rtt->beginRTT(true,true,video::SColor(0xFF133E67));
#ifndef USE_SCENE_MGR
	pCamera2->setNeedUpload();
	pCamera2->render(videoDriver);
#endif
	videoDriver->setRenderTarget(rtt,true,true,COLOR_BLUE);

#ifdef USE_SCENE_MGR
	teapotModel->setVisible(true);
	cubeModel->setVisible(false);
	planeModel->setVisible(false);

#else
	static core::position2di ps[4];
#define TO_PS(x,y,w,h) \
	ps[0].set(x,y+h); \
	ps[1].set(x,y); \
	ps[2].set(x+w,y); \
	ps[3].set(x+w,y+h);

	static core::rectf r(0,1,1,0);
	gfAdapter->clearZ(1000);
	TO_PS(0,0,128,128)
	gfAdapter->drawRegion(videoDriver->getTexture("aura.png"),r,ps);
	gfAdapter->render(); 

	videoDriver->draw2DLine(core::ORIGIN_POSITION2DI,core::position2di(100,100));
#endif

#ifdef USE_SCENE_MGR
	sceneMgr->render(videoDriver);
	teapotModel->setVisible(false);
	cubeModel->setVisible(true);
	planeModel->setVisible(true);
#endif

	//rtt->endRTT(true);
	videoDriver->setRenderTarget(NULL,true,true,COLOR_DEFAULT);
	//videoDriver->setViewPort(core::recti(0,0,videoDriver->getCurrentRenderTargetSize().w,videoDriver->getCurrentRenderTargetSize().h));

#ifdef USE_SCENE_MGR
	const core::vector3df crot=cubeModel->getRotation();
	cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

	const core::vector3df trot=teapotModel->getRotation();
	teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	const core::vector3df psca=planeModel->getScale();
	if(psca.x>4)
		factor= 0.9f;
	else if(psca.x<2)
		factor=1.1f;
	planeModel->setScale(psca*factor);
	sceneMgr->render(videoDriver);
#else
	pCamera->setNeedUpload();
	pCamera->render(videoDriver);
#endif
	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);

#ifndef USE_SCENE_MGR
	gfAdapter->clearZ(1000);
	TO_PS(100,100,128,128)
	gfAdapter->drawRegion(rtt,r,ps);
	gfAdapter->render();
#endif

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}