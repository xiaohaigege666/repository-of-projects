//////////////////////////////////////////////////////////////////////////////////
// SPARK Yon Rendering library												//
// Copyright (C) 2009															//
// Thibault Lescoat -  info-tibo <at> orange <dot> fr							//
// Julien Fryer - julienfryer@gmail.com											//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////

#include "RenderingAPIs/Yon/SPK_YONLineRenderer.h"
#include "RenderingAPIs/Yon/SPK_YONBuffer.h"

namespace SPK
{
namespace YON
{
	const std::string YONLineRenderer::YON_BUFFER_NAME("SPK_YONLineRenderer_Buffer");

	YONLineRenderer::YONLineRenderer(yon::IYonEngine* d,float length,float width) :
		YONRenderer(d),
		LineRendererInterface(length,width)
	{
		//material.Thickness = width;
	}

	void YONLineRenderer::createBuffers(const Group& group)
	{
		currentBuffer = dynamic_cast<YONBuffer*>(group.createBuffer(getBufferName(),
																	YONBufferCreator(device,
																		NB_VERTICES_PER_QUAD,
																		NB_INDICES_PER_QUAD),
																	getVBOFlag(),
																	false));
		
		size_t nbTotalIndices = group.getParticles().getNbReserved() << 1;
		
		//yon::scene::IIndexBuffer& indexBuffer = currentBuffer->getIndexBuffer();
		yon::core::array<yon::u16>& indexBuffer = currentBuffer->getIndexBuffer();
		/*if (indexBuffer.getType() == yon::video::EIT_32BIT)
        {
            yon::u32* indices = reinterpret_cast<yon::u32*>(indexBuffer.pointer());
            for (size_t t = 0; t < nbTotalIndices; ++t)
                indices[t] = t;
        }
        else*/
        {
            yon::u16* indices = reinterpret_cast<yon::u16*>(indexBuffer.pointer());
            for (size_t t = 0; t < nbTotalIndices; ++t)
                indices[t] = t;
        }

		//currentBuffer->getMeshBuffer().setDirty(yon::scene::EBT_INDEX);
		currentBuffer->getMeshBuffer().setIndicesDirty();
		currentBuffer->setVBOInitialized(true);
	}

	void YONLineRenderer::render(const Group& group)
	{
		if (!prepareBuffers(group))
			return;

		for (size_t t = 0; t < group.getNbParticles() << 1; t += 2)
		{
			const Particle& particle = group.getParticle(t >> 1);

			yon::video::SColor color(spk2yon(
			 particle.getParamCurrentValue(PARAM_ALPHA),
			 particle.getR(),
			 particle.getG(),
			 particle.getB()));

            currentBuffer->getVertexBuffer()[t].pos = spk2yon(particle.position());
			currentBuffer->getVertexBuffer()[t].color = color;
            
			currentBuffer->getVertexBuffer()[t + 1].pos = spk2yon(particle.position() + particle.velocity() * length);
			currentBuffer->getVertexBuffer()[t + 1].color = color;
		}
		//currentBuffer->getMeshBuffer().setDirty(yon::scene::EBT_VERTEX);
		currentBuffer->getMeshBuffer().setVerticesDirty();

		yon::video::IVideoDriver* driver = device->getVideoDriver();
        driver->setMaterial(material);
        /*driver->drawVertexPrimitiveList(
			currentBuffer->getVertexBuffer().pointer(),
			group.getNbParticles() * NB_VERTICES_PER_QUAD,
			currentBuffer->getIndexBuffer().pointer(),
			group.getNbParticles(),
			yon::video::EVT_STANDARD,
			yon::scene::EPT_LINES,
			currentBuffer->getIndexBuffer().getType());*/
		driver->drawVertexPrimitiveList(currentBuffer->getVertexBuffer().pointer(),
			group.getNbParticles()*NB_VERTICES_PER_QUAD,
			currentBuffer->getIndexBuffer().pointer(),
			group.getNbParticles(),yon::video::ENUM_PRIMITIVE_TYPE_LINES);
	}
}}