
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef TECHNIQUEBONUS_H_
#define TECHNIQUEBONUS_H_

#include "RenderingTechnique.h"
#include "Utilities/ShaderProgram.h"

class TechniqueBonus : public RenderingTechnique
{
public:

    TechniqueBonus();
    virtual ~TechniqueBonus();

    virtual void PreRender();
    virtual void PostRender();
    virtual void ReceiveInput(char aKey);

protected:

    void InitShader();
	void Timer(int time);

	struct Color{
		float R;
		float G;
		float B;
		Color(float r, float g, float b){R=r;G=g;B=b;}
	};

	Color BonusColor;
	float fxPos;
	float fxPosMax;
	float modelSize; //average vertex distance from (0,0,0) in object coord
	float fxSpeed;

    ShaderProgram mShader;
};

#endif
