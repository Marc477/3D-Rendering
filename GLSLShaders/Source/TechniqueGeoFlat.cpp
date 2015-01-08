
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#include "TechniqueGeoFlat.h"

#include "Utilities/Image.h"
#include "Utilities/BitmapHandling.h"
#include "Utilities/TextureHandling.h"

TechniqueGeoFlat::TechniqueGeoFlat()
: flatColor(Color(1,0,0))
{
    InitShader();
}

//////////////////////////////////////////////////////////////////////////

void TechniqueGeoFlat::PreRender()
{   
    //Activate the shader
    mShader.Activate();

    //Tell the shader which color to use
    glUniform4f( glGetUniformLocation(mShader.ProgramId(),"FlatColor"), flatColor.R, flatColor.G, flatColor.B, 1 );

}

//////////////////////////////////////////////////////////////////////////

void TechniqueGeoFlat::PostRender()
{
    //Deactivate the shader.
    mShader.Deactivate();
}

//////////////////////////////////////////////////////////////////////////

void TechniqueGeoFlat::ReceiveInput(char aKey)
{
    //Change the current lookup texture if the user
    //presses keys 1, 2 or 3 on the keyboard.
    switch(aKey)
    {
    case '1':
        flatColor = Color(1,0,0);
        break;
    case '2' :
        flatColor = Color(0,1,0);
        break;
    case '3' :
        flatColor = Color(0,0,1);
        break;
	case '4' :
        flatColor = Color(1,1,1);
        break;
	case '5' :
        flatColor = Color(0,0,0);
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void TechniqueGeoFlat::InitShader()
{
    //Create two shader objects.
    Shader* VertexShader    = new Shader(Shader::Shader_Vertex);
    Shader* FragmentShader  = new Shader(Shader::Shader_Fragment);

    //Load and compile the source code for each shader.
    VertexShader->CompileSourceFile("Source/GeoFlat.vx");
    FragmentShader->CompileSourceFile("Source/GeoFlat.fg");

    //Associate the shaders to our Toon shader program.
    mShader.AddShader(VertexShader,true);
    mShader.AddShader(FragmentShader,true);

    //Link the program.
    mShader.Link();
}

//////////////////////////////////////////////////////////////////////////


TechniqueGeoFlat::~TechniqueGeoFlat()
{
}
