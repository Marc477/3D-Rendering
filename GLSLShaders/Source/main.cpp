#include <iostream>
#include <cassert>
#include <vector>

#include "Utilities/GLInclude.h"
#include "Utilities/Camera.h"
#include "Utilities/ModelLoader.h"
#include "Utilities/Shader.h"
#include "Utilities/ShaderProgram.h"
#include "TechniqueFactory.h"

//Camera Variables
Camera gTheCam(0,0,20);

//Scene meshes
unsigned int gBunnyMesh;
unsigned int gGargoyleMesh;
unsigned int gSphere;

//Scene Info (0 = Bunny, 1 = Gargoyle, 2 = Sphere)
unsigned int gCurrentScene = 0;
double       gEarthRotation = 90;

//Rendering techniques
vector<RenderingTechnique*> gTechniques;
RenderingTechnique* gCurrentTechnique = NULL;

void Init()
{
    //Create and Initialize the various rendering techniques.
    for(int i = 0; i < TechniqueFactory::Technique_COUNT; ++i)
    {
        gTechniques.push_back(
            TechniqueFactory::CreateTechnique((TechniqueFactory::Technique)i));
    }
    gCurrentTechnique = gTechniques[TechniqueFactory::Technique_ToonShader];

    //Load the different models used throughout the homework.
    ModelLoader Tmp;
    gBunnyMesh      = Tmp.Load("resources/Bunny.obj",true);
    gGargoyleMesh   = Tmp.Load("resources/Gargoyle.obj",true);

    //Generate the quadric used for the third scene (the sphere)
    gSphere = glGenLists(1);
    glNewList(gSphere,GL_COMPILE);
    GLUquadric* TmpQuadric = gluNewQuadric();
    gluQuadricTexture(TmpQuadric,GLU_TRUE);
    gluQuadricNormals(TmpQuadric,GLU_SMOOTH);
    gluQuadricOrientation(TmpQuadric,GLU_OUTSIDE);
    gluSphere(TmpQuadric,0.25,80,80);
    gluDeleteQuadric(TmpQuadric);
    glEndList();
}

void LightAndMaterialSetup()
{
    //Material setup
    float Am[4] = {0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Am );

    float Dm[4] = {0.4f, 0.6f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dm );

    float Sm[4] = {0.6f, 0.6f, 0.6f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Sm );

    float f = 60.0f;
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, f );

    //Light Setup
    float Al[4] = {0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_AMBIENT, Al );	

    float Dl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, Dl );	

    float Sl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_SPECULAR, Sl );

    float Posl[4] = {200.0f,200.0f,400.0f,1.0f};
    glLightfv(GL_LIGHT0,GL_POSITION,Posl);
}

//Model 1: Bunny
void DisplayScene0()
{
    glClearColor(0.4, 0.4, 0.4, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(62.7375,960.0/600.0,0.1,300);
    
    LightAndMaterialSetup();

    gTheCam.Refresh(); 

    gCurrentTechnique->PreRender();

    //Bunny
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScalef(20,20,20);
    glCallList(gBunnyMesh);
    glPopMatrix();

    gCurrentTechnique->PostRender();

    glFinish();
    glutSwapBuffers();
}

//Model 1: Gargoyle
void DisplayScene1()
{
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(62.7375,960.0/600.0,0.1,300);

    LightAndMaterialSetup();

    gTheCam.Refresh();

    gCurrentTechnique->PreRender();

    //Gargoyle
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(180,0,0,1);
    glRotatef(-90,0,1,0);
    glScalef(20,20,20);
    glCallList(gGargoyleMesh);
    glPopMatrix();

    gCurrentTechnique->PostRender();

    glFinish();
    glutSwapBuffers();
}

//Model 3: Sphere
void DisplayScene2()
{
    glClearColor(0.05, 0.05, 0.05, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(62.7375,960.0/600.0,0.1,300);

    LightAndMaterialSetup();

    gTheCam.Refresh();

    gCurrentTechnique->PreRender();

    gEarthRotation += 0.05;

    //Sphere
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(gEarthRotation,0,1,0);
    glRotatef(90,1,0,0);
    glScalef(20,20,20);
    glCallList(gSphere);
    glPopMatrix();
    

    gCurrentTechnique->PostRender();

    glFinish();
    glutSwapBuffers();
}

//Main glut display function
void Display()
{
    switch(gCurrentScene)
    {
    case 0 :
        DisplayScene0();
        break;
    case 1 :
        DisplayScene1();
        break;
    case 2:
        DisplayScene2();
    default:
        break;
    }
}

void Idle()
{
    glutPostRedisplay();
}

//Mouse buttons/////
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

void Keyboard(unsigned char aKey, int aX, int aY)
{
    gCurrentTechnique->ReceiveInput(aKey);
}

//Keyboard ///////
void SpecialKeyboard(int aKey, int aX, int aY)
{
    switch(aKey)
    {
    case GLUT_KEY_F1 :
        gCurrentTechnique
            = gTechniques[TechniqueFactory::Technique_ToonShader];
        break;
    case GLUT_KEY_F2 :
        gCurrentTechnique 
            = gTechniques[TechniqueFactory::Technique_BarlaShader];
        break;
    case GLUT_KEY_F3 :
        gCurrentTechnique 
            = gTechniques[TechniqueFactory::Technique_PlainColor];
        break;
    case GLUT_KEY_F4 :
        gCurrentTechnique 
            = gTechniques[TechniqueFactory::Technique_FixedPipeline];
        break;
    case GLUT_KEY_F5 :
        gCurrentTechnique 
            = gTechniques[TechniqueFactory::Technique_Twist];
        break;
    case GLUT_KEY_F6 :
        gCurrentTechnique 
            = gTechniques[TechniqueFactory::Technique_MultiTexture];
        gCurrentScene 
            = 2;
        break;
    case GLUT_KEY_F7 :
        gCurrentTechnique 
            = gTechniques[TechniqueFactory::Technique_Bonus];
        break;
    case GLUT_KEY_PAGE_UP :
        if(gCurrentTechnique != gTechniques[TechniqueFactory::Technique_MultiTexture])
        {
            gCurrentScene++;
            gCurrentScene %= 3;
        }
        break;
    default :
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);

    Init();

    glEnable(GL_DEPTH_TEST);
    
#if __APPLE__
    //Enable VSync on mac to prevent tearing.
    int swap_interval = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &swap_interval);
#endif
    
    glutMainLoop();
    return EXIT_SUCCESS;
}
