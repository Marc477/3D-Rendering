
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef TECHNIQUEMULTITEX_H_
#define TECHNIQUEMULTITEX_H_

#include "RenderingTechnique.h"
#include "ShaderProgram.h"

class TechniqueMultiTex : public RenderingTechnique
{
public:

    TechniqueMultiTex();
    virtual ~TechniqueMultiTex();

    virtual void PreRender();
    virtual void PostRender();
    virtual void ReceiveInput(char aKey);

protected:

	void InitTextures();
    void InitShader();

    ShaderProgram mShader;

	unsigned int texDay;
	unsigned int texNight;
	unsigned int texMask;
};

#endif
