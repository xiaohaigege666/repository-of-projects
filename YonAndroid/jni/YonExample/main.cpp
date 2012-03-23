#include "stdio.h"
#include<windows.h>

#include <crtdbg.h>
#include <typeinfo>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |  _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::debug;
using namespace yon::video;
using namespace yon::scene;

#pragma comment(lib, "Yon.lib")


#ifdef YON_HIDE_CONSOLE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif//YON_HIDE_CONSOLE


void print(const matrix4f& m){
	for(int i=0;i<4;++i)
		printf("%.2f\t%.2f\t%.2f\t%.2f\n",m.m[0][i],m.m[1][i],m.m[2][i],m.m[3][i]);
	printf("\n");
}

struct Object : public virtual core::IReferencable{
	int a;
	int b;
};

#define _CRTDBG_MAP_ALLOC
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//setlocale(LC_ALL,"chs");
	setlocale(LC_CTYPE,"UTF-8");

	

	SYonEngineParameters params;
	params.windowSize.w=400;
	params.windowSize.h=400;
	IYonEngine* pYE=CreateEngine(params);

	IVideoDriver* driver=pYE->getVideoDriver();
	ISceneManager* sceneMgr=pYE->getSceneManager();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();

	/*IEntity* cube=geometryFty->createCube();
	IModel* model=sceneMgr->addModel(cube);
	cube->drop();*/

	IEntity* sphere=geometryFty->createSphere();
	IModel* model=sceneMgr->addModel(sphere);
	sphere->drop();

	/*ILogger* logger=Logger;
	logger->setAppender(MASK_APPENDER_CONSOLE|MASK_APPENDER_FILE|MASK_APPENDER_VS);
	//logger->setAppender(MASK_APPENDER_CONSOLE);
	logger->setLevel(ENUM_LOG_LEVEL_DEBUG);
	int num=0;*/
	while(pYE->run()){
		driver->begin(true,COLOR_GRAY);

		sceneMgr->render(driver);

		//smgr->drawAll();
		//guienv->drawAll();

		//driver->endScene();
		//Sleep(20);
		//printf("run\n");
		//num++;
		//Logger->debug("%s\n","test���");
		//logger->info("TEST\n");
		driver->end();
	}
#if 0
	IEntity* cube=geometryFty->createCube();
	cube->drop();
#endif 
#if 0
	Object* o=new Object();
	list<Object*> l;
	l.push_back(o);
	o->drop();
	l.clear();
#endif

#if 0
	array<u32> arr(12);
	array<u32> brr(13);
	//Ч����irrlicht�൱
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr=brr;
	}
	Logger->info("end\n");
#endif
#if 0
	//Ч�ʱ�irrlicht��1/3
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		array<u32> arr(12);
		array<u32> brr(13);
		arr=brr;
	}
	Logger->info("end\n");
#endif
#if 0
	//10�򼶱�,1��20��
	array<u32> arr;
	Logger->info("start\n");
	for(u32 i=0;i<100000;++i){
		arr.insert(1,0);
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");
#endif
#if 0
	//ǧ��,2��,���·���50��
	array<u32> arr;
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr.push(1);
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");

	//ǧ��,1.5��,���·���50��
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr.pop();
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");
#endif
	pYE->drop();

	
#if 0
	//setlocale(LC_ALL,"zh_CN.utf8");
	setlocale(LC_ALL,"chs");

	core::stringc str;
	printf("%d,%s\n",str.length(),str);

	str+="test";
	printf("%d,%s\n",str.length(),str);

	core::stringc str1=str+"test";
	printf("%d,%s\n",str1.length(),str1);

	core::stringw wstr(L"%s",L"��������");
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	wstr+=L"�л����񹲺͹�";
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	core::stringw wstr1=wstr+L"test";
	wprintf(L"%d,%s\n",wstr1.length(),wstr1);

	core::stringc tstr("���");
	printf("%s\n\n\n",tstr);

	printf("%s\n",(tstr+"1").c_str());
#endif

#if 0
	matrix4f m;
	m.makeIdentity();
	print(m);

	//vector3df v(3,2,3);
	m.setTranslation(3,2,3);
	print(m);

	matrix4f n;
	n.makeIdentity();
	n.translate(1,2,3);
	print(n);

	matrix4f r;
	r=m*n;
	print(r);

	f32 temp[]={2,0,0,1,0,1,0,2,0,0,1,3,0,0,0,1};
	matrix4f l(temp);
	r=m*l;
	printf("==>");
	print(r);

	print(IDENTITY_MATRIX);

	l.makeInverse();
	print(l);

	//r.perspective(60,2,1,100);
	//r.ortho(-5,5,-3,3,1,100);
	r.lookAt(1,2,3,6,5,4,0,1,0);
	//r.makeIdentity();
	//r.scale(1,2,3);
	print(r);

	r.rotate(60,1,1,1);
	print(r);

	f32 temp2[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	matrix4f k(temp2);
	print(k);
	k.makeTranspose();
	print(k);

	
#endif

	//getchar();
	return 0;
}