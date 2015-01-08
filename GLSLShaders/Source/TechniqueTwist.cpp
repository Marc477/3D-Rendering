
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#include "TechniqueTwist.h"

#include <stdio.h>
#include <iostream>

#include "Utilities/Image.h"
#include "Utilities/BitmapHandling.h"
#include "Utilities/TextureHandling.h"

TechniqueTwist::TechniqueTwist()
{
	level = 0;
    InitShader();
}

//////////////////////////////////////////////////////////////////////////

void TechniqueTwist::PreRender()
{   
    //Activate the shader
    mShader.Activate();

    //Tell the twist level to the shader
    glUniform1f(glGetUniformLocation(mShader.ProgramId(),"TwistLevel"), level);

}

//////////////////////////////////////////////////////////////////////////

void TechniqueTwist::PostRender()
{
    //Deactivate the shader.
    mShader.Deactivate();
}

//////////////////////////////////////////////////////////////////////////

void TechniqueTwist::ReceiveInput(char aKey)
{
    //Change the current lookup texture if the user
    //presses keys 1, 2 or 3 on the keyboard.
    switch(aKey)
    {
    case 'q':
		level += 0.1;
		break;
    case 'a' :
		level -= 0.1;
		break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void TechniqueTwist::InitShader()
{
    //Create two shader objects.
    Shader* VertexShader    = new Shader(Shader::Shader_Vertex);
    Shader* FragmentShader  = new Shader(Shader::Shader_Fragment);

    //Load and compile the source code for each shader.
    VertexShader->CompileSourceFile("Source/Twist.vx");
    FragmentShader->CompileSourceFile("Source/Phong.fg");
	
	//std::cout << VertexShader->Log() << std::endl;
	//std::cout << FragmentShader->Log() << std::endl;

    //Associate the shaders to our Toon shader program.
    mShader.AddShader(VertexShader,true);
    mShader.AddShader(FragmentShader,true);

    //Link the program.
    mShader.Link();
}

//////////////////////////////////////////////////////////////////////////


TechniqueTwist::~TechniqueTwist()
{

}
