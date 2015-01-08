#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

#include "Utilities/GLInclude.h"
#include "Utilities/Camera.h"
#include "Utilities/ModelLoader.h"

#include "StencilShadowRender.h"

//Camera Variables
Camera gTheCam(0,0,20);

//Scene meshes
Model   gModels[7];
Model   gScene;
int     gCurModel = 0;

//Light position
bool    gAutoLight = false;
float   gLightRot = 0.0;
float   gLightHeight = 40.0;

//Scene Info (0 = Bunny, 1 = Gargoyle, 2 = Sphere)
unsigned int gCurrentScene = 0;

void Init()
{
    //Material Orange
    //////////////////////////////////////////////////////////////////////////

    Model::Material Orange;
    Orange.Ambient[0] =   0.6f;   Orange.Ambient[1] =   0.3f;
    Orange.Ambient[2] =   0.3f;   Orange.Ambient[3] =   1.0f;

    Orange.Diffuse[0] =   1.0f;   Orange.Diffuse[1] =   150/255.0f;
    Orange.Diffuse[2] =   14/255.0f;   Orange.Diffuse[3] =   1.0f;

    Orange.Specular[0]=   0.6f;   Orange.Specular[1]=   0.6f;
    Orange.Specular[2]=   0.6f;   Orange.Specular[3]=   1.0f;

    Orange.Shininess =    60.0f;

    //Material Green
    //////////////////////////////////////////////////////////////////////////
    Model::Material Green;
    Green.Ambient[0] =   0.3f;   Green.Ambient[1] =   0.4f;
    Green.Ambient[2] =   0.35f;  Green.Ambient[3] =   1.0f;

    Green.Diffuse[0] =   150/255.0f;   Green.Diffuse[1] =   230/255.0f;
    Green.Diffuse[2] =   30/255.0f;   Green.Diffuse[3] =   1.0f;

    Green.Specular[0]=   0.6f;   Green.Specular[1]=   0.6f;
    Green.Specular[2]=   0.6f;   Green.Specular[3]=   1.0f;

    Green.Shininess =    30.0f;

    //Material Red
    //////////////////////////////////////////////////////////////////////////
    Model::Material Red;
    Red.Ambient[0] =   0.5f;   Red.Ambient[1] =   0.2f;
    Red.Ambient[2] =   0.2f;  Red.Ambient[3] =   1.0f;

    Red.Diffuse[0] =   237/255.0f;   Red.Diffuse[1] =   28/255.0f;
    Red.Diffuse[2] =   36/255.0f;   Red.Diffuse[3] =   1.0f;

    Red.Specular[0]=   0.6f;   Red.Specular[1]=   0.6f;
    Red.Specular[2]=   0.6f;   Red.Specular[3]=   1.0f;

    Red.Shininess =    30.0f;

    //Knight Mesh
    //////////////////////////////////////////////////////////
    ModelLoader Tmp;
    gModels[0]      = Tmp.Load("resources/Knight.obj",true);

    gModels[0].SetMaterial(Green);
    gModels[0].Init();

    //Engi Low Def Mesh
    //////////////////////////////////////////////////////////
    gModels[1]   = Tmp.Load("resources/Engineer_LowDef.obj",true);

    gModels[1].SetMaterial(Orange);
    gModels[1].Init();

    //Engi High Def Mesh
    //////////////////////////////////////////////////////////////////////////
    gModels[3]   = Tmp.Load("resources/Engineer_HiDef.obj",true);

    gModels[3].SetMaterial(Orange);
    gModels[3].Init();

    //Pyro Mesh
    //////////////////////////////////////////////////////////////////////////
    gModels[2]   = Tmp.Load("resources/Pyro_LowDef.obj",true);

    gModels[2].SetMaterial(Red);
    gModels[2].Init();

    //Pyro HD Mesh
    //////////////////////////////////////////////////////////////////////////
    gModels[4]   = Tmp.Load("resources/Pyro_HiDef.obj",true);

    gModels[4].SetMaterial(Red);
    gModels[4].Init();


    //Demoman Mesh
    //////////////////////////////////////////////////////////////////////////
    gModels[5]   = Tmp.Load("resources/Demoman.obj",true);

    gModels[5].SetMaterial(Orange);
    gModels[5].Init();


    //Buddha
    //////////////////////////////////////////////////////////////////////////
    gModels[6]   = Tmp.Load("resources/Buddha.obj",true);

    gModels[6].SetMaterial(Green);
    gModels[6].Init();



    //Scene
    //////////////////////////////////////////////////////////
    unsigned int ListId = glGenLists(1);

    glNewList(ListId,GL_COMPILE);
    glBegin(GL_QUADS);

    glNormal3f(0,1,0);
    glVertex3f(50,-0.4,-50);
    glNormal3f(0,1,0);
    glVertex3f(-50,-0.4,-50);
    glNormal3f(0,1,0);
    glVertex3f(-50,-0.4,50);
    glNormal3f(0,1,0);
    glVertex3f(50,-0.4,50);
    glEnd();
    glEndList();

    gScene = Model( vector<Vector3>(),
                    vector<Model::Triangle>(),
                    ListId);

    Model::Material Mat3;
    Mat3.Ambient[0] =   0.5f;   Mat3.Ambient[1] =   0.4f;
    Mat3.Ambient[2] =   0.3f;   Mat3.Ambient[3] =   1.0f;

    Mat3.Diffuse[0] =   0.5f;   Mat3.Diffuse[1] =   0.4f;
    Mat3.Diffuse[2] =   0.3f;   Mat3.Diffuse[3] =   1.0f;

    Mat3.Specular[0]=   0.0f;   Mat3.Specular[1]=   0.0f;
    Mat3.Specular[2]=   0.0f;   Mat3.Specular[3]=   1.0f;

    Mat3.Shininess =    60.0f;

    gScene.SetMaterial(Mat3);
    gScene.Init();

    //Init the shadow rendering technique
    //////////////////////////////////////////////////////////
    InitRendering();
}

void LightSetup()
{
    //Light Setup
    float Al[4] = {0.3f, 0.3f, 0.3f, 1.0f };
    glLightfv( GL_LIGHT0, GL_AMBIENT, Al );	

    float Dl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, Dl );	

    float Sl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_SPECULAR, Sl );

    float LightX = 40.0f*cos(gLightRot);
    float LightZ = 40.0f*sin(gLightRot);

    if(gAutoLight)
    {
        gLightRot+= 0.01f;
    }
    

    float Posl[4] = {LightX,gLightHeight,LightZ,1.0f};
    glLightfv(GL_LIGHT0,GL_POSITION,Posl);
}

void DisplayScene()
{
    glClearColor(0.1, 0.1, 0.4, 1);
    glClearStencil(0);

	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | 
            GL_DEPTH_BUFFER_BIT | 
            GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(62.7375,960.0/600.0,0.5,500);
    
    gTheCam.Refresh();
    LightSetup();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScalef(20,20,20);

    RenderScene(gModels[gCurModel],gScene);    

    glPopMatrix();
    
    glutSwapBuffers();
}

void Display()
{
    DisplayScene();
}

void Idle()
{
    glutPostRedisplay();
}

void MouseButton(int aButton, int aState, int aX, int aY)
{
    Camera::MouseEventArgs Args;
    
    switch(aButton)
    {
        case GLUT_LEFT_BUTTON :     Args.mButton = Camera::Mouse_Left;  break;
        case GLUT_RIGHT_BUTTON :    Args.mButton = Camera::Mouse_Right; break;
        case GLUT_MIDDLE_BUTTON :   Args.mButton = Camera::Mouse_Middle;break;
        default :                   Args.mButton = Camera::Mouse_None;  break;
    }
    
    switch(aState)
    {
        case GLUT_UP :      Args.mState = Camera::State_Up;     break;
        case GLUT_DOWN :    Args.mState = Camera::State_Down;   break;
        default :           Args.mState = Camera::State_None;   break;
    }
    
    Args.mCoordX = aX;
    Args.mCoordY = aY;
    
    gTheCam.UpdateControls(Args);
}

void MouseMove(int aX, int aY)
{
    Camera::MouseEventArgs Args;
    
    Args.mCoordX = aX;
    Args.mCoordY = aY;
    
    Args.mButton = Camera::Mouse_None;
    Args.mState = Camera::State_None;
    
    gTheCam.UpdateControls(Args);
}

//Keys !!
void SpecialKeyboard(int aKey, int aX, int aY)
{
    switch(aKey)
    {
        case GLUT_KEY_PAGE_UP :
                gCurModel++;
                gCurModel %= 7;
        break;
        case GLUT_KEY_PAGE_DOWN :
                gCurModel--;
                if(gCurModel < 0)
                {
                    gCurModel = 6;
                }
        break;
        case GLUT_KEY_LEFT :
            gLightRot+= 0.05;
            break;
        case GLUT_KEY_RIGHT :
            gLightRot -=0.05;
            break;
        case GLUT_KEY_UP :
            gLightHeight += 0.5;
            if(gLightHeight > 50.0f)
            {
                gLightHeight = 50.0f;
            }
            break;
        case GLUT_KEY_DOWN :
            gLightHeight -= 0.5;
            if(gLightHeight < 0.0f)
            {
                gLightHeight = 0.0f;
            }
    }

    glutPostRedisplay();
}

void Keyboard(unsigned char aKey, int aX, int aY)
{
    switch(aKey)
    {
    case ' ' :
        gAutoLight = !gAutoLight;
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_RGBA | 
        GLUT_DOUBLE | 
        GLUT_DEPTH | 
        GLUT_STENCIL);

    glutInitWindowSize(960, 600);
    
    glutCreateWindow("GLUT Program");


#if WIN32
    GLenum err = glewInit();
        assert(err == GLEW_OK);
#endif
    
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMove);
    glutSpecialFunc(SpecialKeyboard);
    glutKeyboardFunc(Keyboard);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Init();
    
#if __APPLE__
    //Enable VSync on mac to prevent tearing.
    int swap_interval = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &swap_interval);
#endif
    
    glutMainLoop();
    return EXIT_SUCCESS;
}