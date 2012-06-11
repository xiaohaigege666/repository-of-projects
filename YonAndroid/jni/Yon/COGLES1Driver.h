#ifndef _YON_VIDEO_OGLES1_COGLES1DRIVER_H_
#define _YON_VIDEO_OGLES1_COGLES1DRIVER_H_

#include "IVideoDriver.h"
#include "SOGLES1Parameters.h"
#include "yonArray.h"
#include "ITexture.h"
#include "IMaterial.h"
#include "IMaterialRenderer.h"
#include "IDebugPrinter.h"
#include "IGeometryFactory.h"
#include "CFPSCounter.h"
#include "IUnit.h"
#include "IHardwareBuffer.h"
#include "COGLES1ExtensionHandler.h"


namespace yon{
namespace video{
namespace ogles1{

	struct FPSAssist{
		bool limit;
		s32 timeCounter;
		s32 frameCounter;
		u32 timeUnit;
		u32 frameUnit;
		u32 refreshedTime;
	};
	
	class COGLES1Driver:public IVideoDriver,COGLES1ExtensionHandler{
	public:

		COGLES1Driver(const SOGLES1Parameters& param,io::IFileSystem* fs,ITimer* timer,scene::IGeometryFactory* geometryFty);
		virtual ~COGLES1Driver();

		virtual const SClearSetting& getClearSetting() const{
			return m_clearSetting;
		}
		void clearView(const bool& backBuffer,const bool& zBuffer,const video::SColor& color) const;
		virtual void begin(bool backBuffer,bool zBuffer,const video::SColor& color);
		virtual void end();
		virtual void setViewPort(const core::recti& r);
		virtual const core::dimension2du& getCurrentRenderTargetSize() const;
		virtual void onResize(const core::dimension2du& size);

		virtual bool queryFeature(ENUM_VIDEO_FEATURE feature) const{
			return isFeatureAvailable(feature);
		}

		virtual void convertPosCoordinate(const core::position2di& src,core::position2df& dest){
			const core::dimension2du& size=getCurrentRenderTargetSize();
			dest.x=src.x-(f32)size.w*0.5f;
			dest.y=(f32)size.h*0.5f-src.y;
		}
		virtual void convertPosCoordinate(const core::position2di& src,core::position2di& dest){
			const core::dimension2du& size=getCurrentRenderTargetSize();
			dest.x=src.x-size.w*0.5f;
			dest.y=size.h*0.5f-src.y;
		}
		virtual void convertPosCoordinate(const core::position2df& src,core::position2df& dest){
			const core::dimension2du& size=getCurrentRenderTargetSize();
			dest.x=src.x-(f32)size.w*0.5f;
			dest.y=(f32)size.h*0.5f-src.y;
		}
		virtual void convertPosCoordinate(s32 srcX,s32 srcY,s32& destX,s32& destY){
			const core::dimension2du& size=getCurrentRenderTargetSize();
			destX=srcX-(f32)size.w*0.5f;
			destY=(f32)size.h*0.5f-srcY;
		}

		virtual ITexture* addRenderTargetTexture(const core::dimension2du& size,const io::path& name, const video::ENUM_COLOR_FORMAT format);
		//virtual bool setRenderTarget(video::ITexture* texture,bool clearBackBuffer, bool clearZBuffer,video::SColor color);


		virtual IImage* createImageFromFile(const io::path& filename);
		virtual IImage* createImageFromFile(io::IReadStream* file);

		virtual ITexture* addTexture(const core::dimension2du& size,const io::path& name, ENUM_COLOR_FORMAT format);
		virtual bool setTexture(u32 stage, const video::ITexture* texture);
		virtual ITexture* getTexture(const io::path& filename);
		virtual video::ITexture* findTexture(const io::path& filename);
		virtual void removeTexture(ITexture* texture);

		virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat);
		virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const;

		virtual void setMaterial(IMaterial* material);
		virtual IMaterial* createMaterial();

		virtual void drawUnit(scene::IUnit* unit);
		virtual void draw2DImage(const video::ITexture* texture, const core::position2di& destPos,const core::recti& sourceRect, const core::recti* clipRect,video::SColor color, bool useAlphaChannelOfTexture);
		virtual void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,ENUM_PRIMITIVE_TYPE pType,scene::ENUM_VERTEX_TYPE vType);
		virtual void draw3DLine(const core::vector3df& start,const core::vector3df& end, video::SColor color);

		virtual bool checkGLError(const c8* file,s32 line);
		static bool checkError(const c8* file,s32 line);

		void setRender3DMode();
		void setRender2DMode();
		virtual u32 getFPS() const{
			return m_FPSCounter.getFPS();
		}

		
	private:
		virtual void drawVertex(const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,ENUM_PRIMITIVE_TYPE pType,scene::ENUM_VERTEX_TYPE vType);
		void checkMaterial();
		void addTexture(video::ITexture* texture);
		video::ITexture* loadTextureFromFile(io::IReadStream* file);
		video::ITexture* createDeviceDependentTexture(IImage* image, const io::path& name);

		bool needHardwareBuffer(scene::IUnit* unit){
			return unit->getHardwareBufferUsageType()!=ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE;
		}

		core::matrix4f m_matrix[ENUM_TRANSFORM_COUNT];
		bool m_bRenderModeChange;

		video::IMaterial *m_pLastMaterial,*m_pCurrentMaterial;
		//当前使用的纹理,用于优化效率
		video::ITexture* m_currentTextures[YON_MATERIAL_MAX_TEXTURES];

		debug::IDebugPrinter* m_pDebugPrinter;

		//TODO map(irrlicht中使用SSurface)
		core::array<video::ITexture*> m_textures;
		core::array<video::IImageLoader*> m_imageLoaders;
		core::array<video::IMaterialRenderer*> m_materialRenderers;
		//TODO map
		struct SHardwareBufferPair{
			scene::IUnit* unit;
			video::IHardwareBuffer* buffer;

			~SHardwareBufferPair(){
				buffer->drop();
			}
		};
		core::array<SHardwareBufferPair*> m_pHardwareBuffers;

		CFPSCounter m_FPSCounter;
		FPSAssist m_FPSAssist;

		video::SClearSetting m_clearSetting;
		core::dimension2du m_windowSize;

		static u32 s_uInstanceCount;

#ifdef YON_COMPILE_WITH_WIN32
		bool initEGL(const HWND& hwnd);
		void printEGLConfig(EGLConfig& config);
		void printEGLAttribute(EGLint attribs[]);
		void destroyEGL();

		EGLDisplay m_eglDisplay;
		EGLSurface m_eglSurface;
		EGLContext m_eglContext;

		HDC m_hDc;
		HWND m_hWnd;
#endif
	};
}
}
}
#endif