
/* 
	Marc-Antoine Desbiens
	October 2012
*/

#ifndef RENDERINGTECHNIQUE_H_
#define RENDERINGTECHNIQUE_H_

//Generic rendering technique class interface. This class
//controls the rendering for the geometry. Place the required
//shaders or textures related to a given rendering technique
//in a class derived from this.
class RenderingTechnique
{
public:

    //The constructor is called once when the application
    //starts.
    RenderingTechnique(){};

    //The destructor is called when the application closes.
    virtual ~RenderingTechnique(){};

    //This method is called before rendering the
    //geometry. (Load shader or textures, configure
    //the shader's uniform variables, etc.)
    virtual void PreRender() = 0; 

    //This method is called after rendering the
    //geometry. (Unload shader or textures here)
    virtual void PostRender() = 0;

    //If the technique is active, this method receives the
    //user's keyboard input. The pressed key is passed through
    //the aKey argument.
    virtual void ReceiveInput(char aKey) = 0;
};

#endif //RENDERINGTECHNIQUE_H_
