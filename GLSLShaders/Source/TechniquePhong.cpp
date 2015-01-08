
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#include "TechniquePhong.h"

#include <stdio.h>
#include <iostream>

#include "Image.h"
#include "BitmapHandling.h"
#include "TextureHandling.h"

TechniquePhong::TechniquePhong()
{
    InitShader();
}

//////////////////////////////////////////////////////////////////////////

void TechniquePhong::PreRender()
{   
    //Activate the shader
    mShader.Activate();

}

//////////////////////////////////////////////////////////////////////////

void TechniquePhong::PostRender()
{
    //Deactivate the shader.
    mShader.Deactivate();
}

//////////////////////////////////////////////////////////////////////////

void TechniquePhong::ReceiveInput(char aKey)
{
    //Change the current lookup texture if the user
    //presses keys 1, 2 or 3 on the keyboard.
    switch(aKey)
    {
    case '1':
    case '2' :
    case '3' :
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void TechniquePhong::InitShader()
{
    //Create two shader objects.
    Shader* VertexShader    = new Shader(Shader::Shader_Vertex);
    Shader* FragmentShader  = new Shader(Shader::Shader_Fragment);

    //Load and compile the source code for each shader.
    VertexShader->CompileSourceFile("Source/Phong.vx");
    FragmentShader->CompileSourceFile("Source/Phong.fg");
	
	std::cout << VertexShader->Log() << std::endl;
	std::cout << FragmentShader->Log() << std::endl;

    //Associate the shaders to our Toon shader program.
    mShader.AddShader(VertexShader,true);
    mShader.AddShader(FragmentShader,true);

    //Link the program.
    mShader.Link();
}

//////////////////////////////////////////////////////////////////////////


TechniquePhong::~TechniquePhong()
{

}
