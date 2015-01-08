
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef TECHNIQUEGEOFLAT_H_
#define TECHNIQUEGEOFLAT_H_

#include "RenderingTechnique.h"
#include "ShaderProgram.h"

//Simple flat rendering
class TechniqueGeoFlat : public RenderingTechnique
{
public:

    TechniqueGeoFlat();
    virtual ~TechniqueGeoFlat();

    virtual void PreRender();
    virtual void PostRender();
    virtual void ReceiveInput(char aKey);

protected:

    void InitShader();

	struct Color{
		float R;
		float G;
		float B;
		Color(float r, float g, float b){R=r;G=g;B=b;}
	};

	Color flatColor;

    ShaderProgram mShader;
};

#endif //TECHNIQUETOON_H_
