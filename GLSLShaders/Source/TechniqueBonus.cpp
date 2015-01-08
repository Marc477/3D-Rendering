
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#include "TechniqueBonus.h"

#include <stdio.h>
#include <iostream>

#include "Utilities/Image.h"
#include "Utilities/BitmapHandling.h"
#include "Utilities/TextureHandling.h"

TechniqueBonus::TechniqueBonus()
	:BonusColor(0.4,0.4,0.4)
{
	fxPos = 0;
	fxPosMax = 50;
	modelSize = 0.4;
	fxSpeed = 0.1;
    InitShader();
}

//////////////////////////////////////////////////////////////////////////

void TechniqueBonus::PreRender()
{   
    //Activate the shader
    mShader.Activate();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

	//Update light inversion effect position
	fxPos+=0.1; 
	if(fxPos > fxPosMax*10.0) fxPos = 0.0;

	glUniform4f(glGetUniformLocation(mShader.ProgramId(),"BonusColor"), BonusColor.R, BonusColor.G, BonusColor.B, 1.0);
	glUniform1f(glGetUniformLocation(mShader.ProgramId(),"FxValue"), fxPos);
	glUniform1f(glGetUniformLocation(mShader.ProgramId(),"FxValueMax"), fxPosMax);
	glUniform1f(glGetUniformLocation(mShader.ProgramId(),"ModelSize"), modelSize);
}

//////////////////////////////////////////////////////////////////////////

void TechniqueBonus::PostRender()
{
    //Deactivate the shader.
    mShader.Deactivate();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

//////////////////////////////////////////////////////////////////////////

void TechniqueBonus::ReceiveInput(char aKey)
{
    //Change the current lookup texture if the user
    //presses keys 1, 2 or 3 on the keyboard.
    switch(aKey)
    {
    case '1':
        BonusColor = Color(1.0,0.1,0.1);
        break;
    case '2' :
        BonusColor = Color(0.1,1.0,0.1);
        break;
    case '3' :
        BonusColor = Color(0.1,0.1,1.0);
        break;
	case '4' :
        BonusColor = Color(0.4,0.4,0.4);
        break;
    default:
        break;
    }
}


//////////////////////////////////////////////////////////////////////////

void TechniqueBonus::InitShader()
{
    //Create two shader objects.
    Shader* VertexShader    = new Shader(Shader::Shader_Vertex);
    Shader* FragmentShader  = new Shader(Shader::Shader_Fragment);

    //Load and compile the source code for each shader.
    VertexShader->CompileSourceFile("Source/Bonus.vx");
    FragmentShader->CompileSourceFile("Source/Bonus.fg");
	
	//std::cout << VertexShader->Log() << std::endl;
	//std::cout << FragmentShader->Log() << std::endl;

    //Associate the shaders to our Toon shader program.
    mShader.AddShader(VertexShader,true);
    mShader.AddShader(FragmentShader,true);

    //Link the program.
    mShader.Link();
}


//////////////////////////////////////////////////////////////////////////


TechniqueBonus::~TechniqueBonus()
{

}
