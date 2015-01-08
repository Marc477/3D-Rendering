
/*
  Marc-Antoine Desbiens
  November 2012
*/

#include "StencilShadowRender.h"

ShaderProgram mShaderPhong;
ShaderProgram mShaderShadow;

void InitRendering()
{
	//Create two shader objects.
    Shader* VertexShader    = new Shader(Shader::Shader_Vertex);
    Shader* FragmentShader  = new Shader(Shader::Shader_Fragment);
	
    //Load and compile the source code for each shader.
    VertexShader->CompileSourceFile("Source/Phong.vx");
    FragmentShader->CompileSourceFile("Source/Phong.fg");
	std::cout <<  VertexShader->Log() << std::endl;
	std::cout <<  FragmentShader->Log() << std::endl;

    //Associate the shaders to our Toon shader program.
    mShaderPhong.AddShader(VertexShader,true);
    mShaderPhong.AddShader(FragmentShader,true);

    //Link the program.
    mShaderPhong.Link();

	//Create two shader objects.
    Shader* VertexShader2    = new Shader(Shader::Shader_Vertex);
    Shader* FragmentShader2  = new Shader(Shader::Shader_Fragment);
	
    //Load and compile the source code for each shader.
    VertexShader2->CompileSourceFile("Source/Shadow.vx");
    FragmentShader2->CompileSourceFile("Source/Shadow.fg");
	std::cout <<  VertexShader2->Log() << std::endl;
	std::cout <<  FragmentShader2->Log() << std::endl;

    //Associate the shaders to our Toon shader program.
    mShaderShadow.AddShader(VertexShader2,true);
    mShaderShadow.AddShader(FragmentShader2,true);

    //Link the program.
    mShaderShadow.Link();

	std::cout <<  mShaderPhong.Log() << std::endl;
	std::cout <<  mShaderShadow.Log() << std::endl;

}

void drawShadowVolume(Model& aShadowCaster)
{
	Vector3 norm, normOther, v[3];
	int nbTriangles = aShadowCaster.Triangles().size();
	int nbSegments = aShadowCaster.Segments().size();

	//Draw triangles top
	for(int i=0; i <nbTriangles; i++)
	{
		v[0] = aShadowCaster.Vertices()[aShadowCaster.Triangles()[i].PtId1];
		v[1] = aShadowCaster.Vertices()[aShadowCaster.Triangles()[i].PtId2];
		v[2] = aShadowCaster.Vertices()[aShadowCaster.Triangles()[i].PtId3];

		norm = aShadowCaster.Triangles()[i].normal;

		glBegin(GL_TRIANGLES);
		{
			glNormal3f(norm.X(), norm.Y(), norm.Z());
			glVertex3f(v[0].X(), v[0].Y(), v[0].Z());
			glVertex3f(v[2].X(), v[2].Y(), v[2].Z());
			glVertex3f(v[1].X(), v[1].Y(), v[1].Z());
		}
		glEnd();
	}

	//Draw shadow volumes quads on each segment of triangles
	for(int i=0; i <nbSegments; i++)
	{
		v[0] = aShadowCaster.Vertices()[aShadowCaster.Segments()[i].PtId1];
		v[1] = aShadowCaster.Vertices()[aShadowCaster.Segments()[i].PtId2];

		norm = aShadowCaster.Triangles()[aShadowCaster.Segments()[i].TriId1].normal;

		if(aShadowCaster.Segments()[i].TriId2 != -1)
			normOther = aShadowCaster.Triangles()[aShadowCaster.Segments()[i].TriId2].normal;
		else
			normOther = Vector3(0,0,0);

		glBegin(GL_QUADS);
		{
			glNormal3f(norm.X(), norm.Y(), norm.Z());
			glVertex3f(v[1].X(), v[1].Y(), v[1].Z());
			glVertex3f(v[0].X(), v[0].Y(), v[0].Z());
			glNormal3f(normOther.X(), normOther.Y(), normOther.Z());
			glVertex3f(v[0].X(), v[0].Y(), v[0].Z());
			glVertex3f(v[1].X(), v[1].Y(), v[1].Z());
		}
		glEnd();
	}
}

void RenderScene(Model& aShadowCaster, Model& aSceneGeometry)
{
	float noLight[4] = {0.2f, 0.2f, 0.2f, 1.0f };

	//Initialize buffer test
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDepthRange(0.0f,1.0f);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilFunc(GL_EQUAL, 0x0, 0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	mShaderPhong.Activate();

	//Draw the scene for the first time (ambiant only, with a very small amount of diffuse)
	glPushAttrib(GL_LIGHTING_BIT);
		glLightfv( GL_LIGHT0, GL_DIFFUSE, noLight );
		glLightfv( GL_LIGHT0, GL_SPECULAR, noLight );
		aShadowCaster.DrawMesh();
		aSceneGeometry.DrawMesh();
	glPopAttrib();

	//Prepare shadow volumes
	glDepthRange(0.04f,1.0f); //PolygonOffset (Based on frustrum range instead of arbitrary values with glPolygonOffset())
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	mShaderShadow.Activate();
	
	//Render front faces
	glCullFace(GL_BACK);
	glStencilFunc(GL_ALWAYS, 0x1, 0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	drawShadowVolume(aShadowCaster);

	//Render back faces
	glCullFace(GL_FRONT);
	glStencilFunc(GL_LESS, 0x0, 0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
	drawShadowVolume(aShadowCaster);

	//Render scene again where there is no shadow
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glStencilFunc(GL_EQUAL, 0x0, 0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glDisable(GL_CULL_FACE);
	glDepthRange(0.0f,1.0f);
	mShaderPhong.Activate();

	aSceneGeometry.DrawMesh();
	aShadowCaster.DrawMesh();
	
}