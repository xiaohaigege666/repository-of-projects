#ifndef _YON_TEXT_IFONTFAMILY_H_
#define _YON_TEXT_IFONTFAMILY_H_

namespace yon{
namespace text{

	class IFontFamily{
	protected:
		const io::SNamedPath m_namePath;
	public:
		IFontFamily(const io::path& p):m_namePath(p){}
		const io::path& getPath() const{
			return m_namePath.getPath();
		}
	};
}
}
#endif