#ifndef _YON_GUI_MYGUI_IMYGUIVERTEXBUFFER_H_
#define _YON_GUI_MYGUI_IMYGUIVERTEXBUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include "IShap.h"

namespace yon{
namespace gui{
namespace mygui{

	class IMyGUIVertexBuffer : public MyGUI::IVertexBuffer{
	public:
		virtual void* getClientArrayPtr() const = 0;
		virtual scene::IShap* getShap() const = 0;
		virtual void fillShapIndices() = 0;
	};
}
}
}
#endif