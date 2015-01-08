
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef TECHNIQUEPHONG_H_
#define TECHNIQUEPHONG_H_

#include "RenderingTechnique.h"
#include "Utilities/ShaderProgram.h"

class TechniquePhong : public RenderingTechnique
{
public:

    TechniquePhong();
    virtual ~TechniquePhong();

    virtual void PreRender();
    virtual void PostRender();
    virtual void ReceiveInput(char aKey);

protected:

    void InitShader();

    ShaderProgram mShader;
};

#endif
