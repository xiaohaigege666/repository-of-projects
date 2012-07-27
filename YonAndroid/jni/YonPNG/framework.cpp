#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;
IRandomizer* randomizer=NULL;

IModel* planeLPNG8=NULL;
IModel* planeLPNG24=NULL;
IModel* planeLAPNG8=NULL;
IModel* planeLAPNG24=NULL;
IModel* planeRGBPNG8=NULL;
IModel* planeRGBPNG24=NULL;
IModel* planeRGBAPNG8=NULL;
IModel* planeRGBAPNG24=NULL;
IModel* teapotModel=NULL;
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
			switch(evt.mouseInput.type)
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
	params.windowSize.w=400;
	params.windowSize.h=400;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/png/");
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/png/");
	fs->addWorkingDirectory("media/");
#endif

	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA,true);
	videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA,true);
	

	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	/*shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	teapotModel->setPosition(core::vector3df(50,-50,0));
	shap->drop();
	unit->drop();
	entity->drop();*/

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	//const core::vector3df trot=teapotModel->getRotation();
	//teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	sceneMgr->render(videoDriver);

	gfAdapter->clearZ(-1000);
	gfAdapter->drawImage("lpng8.png",0,0,32,32,10,10);
	gfAdapter->drawImage("lpng24.png",0,0,32,32,50,10);
	gfAdapter->drawImage("lapng8.png",0,0,64,32,100,10,true);
	gfAdapter->drawImage("lapng24.png",0,0,64,32,170,10,true);

	gfAdapter->drawImage("rgbpng8.png",0,0,64,64,10,100);
	gfAdapter->drawImage("rgbpng24.png",0,0,64,64,100,100);

	gfAdapter->drawImage("rgbapng8.png",0,0,256,256,10,200,true);
	gfAdapter->drawImage("rgbapng24.png",0,0,256,256,180,80,true);

	gfAdapter->drawImage("fixerror.png",0,0,32,32,340,10,true);

	gfAdapter->drawImage("interlace.png",0,0,128,128,0,200,true);

	gfAdapter->drawImage("3.png",0,0,512,512,0,200,true);
	gfAdapter->render();

	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}