
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#include "TechniqueMultiTex.h"

#include <stdio.h>
#include <iostream>

#include "Image.h"
#include "BitmapHandling.h"
#include "TextureHandling.h"

TechniqueMultiTex::TechniqueMultiTex()
{
	InitTextures();
    InitShader();
}

//////////////////////////////////////////////////////////////////////////

void TechniqueMultiTex::PreRender()
{   
    //Activate the shader
    mShader.Activate();

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texDay);

	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texNight);

	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,texMask);

    //Tell the twist level to the shader
    glUniform1i(glGetUniformLocation(mShader.ProgramId(),"DaySampler"), 0);
	glUniform1i(glGetUniformLocation(mShader.ProgramId(),"NightSampler"), 1);
	glUniform1i(glGetUniformLocation(mShader.ProgramId(),"MaskSampler"), 2);

}

//////////////////////////////////////////////////////////////////////////

void TechniqueMultiTex::PostRender()
{
    //Deactivate the shader.
    mShader.Deactivate();

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,NULL);

	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,NULL);

	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,NULL);
}

//////////////////////////////////////////////////////////////////////////

void TechniqueMultiTex::ReceiveInput(char aKey)
{
    //Change the current lookup texture if the user
    //presses keys 1, 2 or 3 on the keyboard.
    switch(aKey)
    {
    case 'q':
		break;
    case 'a' :
		break;
    default:
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void TechniqueMultiTex::InitTextures()
{
    Image<ColorRGB> TmpImage;

    //Load the color scheme #1 image and send it to the GPU as a texture.
    LoadBmp("Resources/Earth_DayMap.bmp",TmpImage);
    texDay = LoadTexture(TmpImage);

    //Load the color scheme #2 image and send it to the GPU as a texture.
    LoadBmp("Resources/Earth_NightMap.bmp",TmpImage);
    texNight = LoadTexture(TmpImage);

    //Load the color scheme #3 image and send it to the GPU as a texture.
    LoadBmp("Resources/Earth_SpecMap.bmp",TmpImage);
    texMask = LoadTexture(TmpImage);
}

//////////////////////////////////////////////////////////////////////////

void TechniqueMultiTex::InitShader()
{
    //Create two shader objects.
    Shader* VertexShader    = new Shader(Shader::Shader_Vertex);
    Shader* FragmentShader  = new Shader(Shader::Shader_Fragment);

    //Load and compile the source code for each shader.
    VertexShader->CompileSourceFile("Source/MultiTex.vx");
    FragmentShader->CompileSourceFile("Source/MultiTex.fg");
	
	//std::cout << VertexShader->Log() << std::endl;
	//std::cout << FragmentShader->Log() << std::endl;

    //Associate the shaders to our Toon shader program.
    mShader.AddShader(VertexShader,true);
    mShader.AddShader(FragmentShader,true);

    //Link the program.
    mShader.Link();
}

//////////////////////////////////////////////////////////////////////////


TechniqueMultiTex::~TechniqueMultiTex()
{
	FreeTexture(texDay);
    FreeTexture(texNight);
    FreeTexture(texMask);
}
