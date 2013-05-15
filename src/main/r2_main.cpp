/*
 * This code was created by Jeff Molofee '99 
 * (ported to Linux/SDL by Ti Leggett '01)
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 * 
 * or for port-specific comments, questions, bugreports etc. 
 * email to leggett@eecs.tulane.edu
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "bsptree/mesh.hpp"
#include "bsptree/bsptree.hpp"
#include "SDL.h"

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT 600
#define SCREEN_BPP     32
#define ASSERT(condition){if(!(condition)){std::cerr<<"ASSERTION FAILED: "<<#condition<<"@"<<__FILE__<<"("<<__LINE__<<")"<<std::endl;}}



/* Define our booleans */
#define TRUE  1
#define FALSE 0

/* This is our SDL surface */
SDL_Surface *surface;
SDL_Event event;

Mesh mesh1;
MeshData meshdata1;

Mesh mesh2;
MeshData meshdata2;

BSP_tree* tree1;
BSP_tree* tree2;

/* function to release/destroy our resources and restoring the old desktop */
void Quit( int returnCode )
{
    /* clean up the window */
    SDL_Quit( );

    /* and exit appropriately */
    exit( returnCode );
}

void update(){}

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;
 
    /* Protect against a divide by zero */
   if ( height == 0 )
	height = 1;

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
    Vector3 camera_position = Vector3(1.0, 5.0, 1.0);
    Vector3 camera_direction =normalize(Vector3(0.0, -.8, -1.0));
    Vector3 camera_reference = camera_position + camera_direction;
    Vector3 camera_up = Vector3(0.0, 1.0, 0.0);
    gluLookAt(camera_position.x, camera_position.y, camera_position.z,
  	    camera_reference.x, camera_reference.y, camera_reference.z,
  	    camera_up.x, camera_up.y, camera_up.z);

    //glFrustum(-1.0, 1.0, -1.0, 1.0, .1f, 1.0f);

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return( TRUE );
}

/* general OpenGL initialization function */
int initGL( void )
{

    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );

    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
    glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	

	//Vector for the position of the light
	Vector3 light0Vec(-2, -2, -1);
	//light poisition
	GLfloat light0_position[] = {light0Vec.x, light0Vec.y, light0Vec.z, 1.0};
	//ambient light settings
	GLfloat light0_ambient[] = {0, 0, 0, 0};
	//diffuse light settings
	GLfloat light0_diffuse[] = {1, 1, 1, 1};
	//specular light settings
	GLfloat light0_specular[] = {1, 1, 1, 1};

	//setting the light with the above parameters
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    return( TRUE );
}

/* Here goes our drawing code */
int render( void )
{
    /* rotational vars for the triangle and quad, respectively */
    static GLfloat rtri, rquad;
    /* These are to calculate our fps */
    static GLint T0     = 0;
    static GLint Frames = 0;

    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   int stride = sizeof(Vertex);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
    GLfloat model1_ambient[] = {1, 1, 1, 1.0};
    GLfloat model1_diffuse[] = {.2, .2, .8, 1.0};
    GLfloat model1_specular[] = {.80, .80, .80};
   GLfloat model1_shine[] = {121};
   glMaterialfv(GL_FRONT, GL_AMBIENT, model1_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, model1_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, model1_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, model1_shine);

   glVertexPointer(3, GL_FLOAT, stride, meshdata1.vertices);
   glNormalPointer(GL_FLOAT,stride, &(meshdata1.vertices[0].normal));
   glDrawElements(GL_TRIANGLES, meshdata1.num_triangles * 3, GL_UNSIGNED_INT, meshdata1.triangles); 

   GLfloat model2_ambient[] = {1, 1, 1, 1.0};
   GLfloat model2_diffuse[] = {.8, .2, .2, 1.0};
   GLfloat model2_specular[] = {.80, .80, .80};
   GLfloat model2_shine[] = {121};
   glMaterialfv(GL_FRONT, GL_AMBIENT, model2_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, model2_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, model2_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, model2_shine);

   glVertexPointer(3, GL_FLOAT, stride, meshdata2.vertices);
   glNormalPointer(GL_FLOAT,stride, &(meshdata2.vertices[0].normal));
   glDrawElements(GL_TRIANGLES, meshdata2.num_triangles * 3, GL_UNSIGNED_INT, meshdata2.triangles); 
   
    /* Draw it to the screen */
    SDL_GL_SwapBuffers( );

    /* Gather our frames per second */
    Frames++;
    {
	GLint t = SDL_GetTicks();
	if (t - T0 >= 5000) {
	    GLfloat seconds = (t - T0) / 1000.0;
	    GLfloat fps = Frames / seconds;
	    printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
	    T0 = t;
	    Frames = 0;
	}
    }
    return( TRUE );
}

void normalize_shift_mesh(Mesh &mesh, Vector3 pos){
  Vector3 mean = Vector3::Zero();
  for(int i =0; i < mesh.vertices.size(); i++){
    mean+= mesh.vertices[i].position;
  }
  mean/= mesh.vertices.size();

  
  float max_length = -1;
  for(int i =0; i < mesh.vertices.size(); i++){
    float dist = length(mesh.vertices[i].position-mean);
    if(dist > max_length)
      max_length = dist;
  }
  
  mesh.scale(3.0/max_length);
  //  mesh.scale(.5);

  mean = Vector3::Zero();
  for(int i =0; i < mesh.vertices.size(); i++){
    mean+= mesh.vertices[i].position;
  }
  mean/= mesh.vertices.size();

  mesh.translate(pos-mean);
}

void load_mesh(){
  mesh1.load();
  mesh2.load();
}

void copy_mesh_data(){
  meshdata1.num_vertices = mesh1.vertices.size();
    meshdata1.vertices = new Vertex[meshdata1.num_vertices];
    meshdata1.num_triangles = mesh1.triangles.size();
    meshdata1.triangles = new Triangle[meshdata1.num_triangles];
    for ( int i = 0; i < meshdata1.num_vertices; ++i ) {
        meshdata1.vertices[i].position = mesh1.vertices[i].position;
        meshdata1.vertices[i].normal = mesh1.vertices[i].normal;
    }
    for ( int i = 0; i < meshdata1.num_triangles; ++i ) {
        for ( int j = 0; j < 3; ++j ) {
            meshdata1.triangles[i].vertices[j] = mesh1.triangles[i].vertices[j];
        }
    }

  meshdata2.num_vertices = mesh2.vertices.size();
    meshdata2.vertices = new Vertex[meshdata2.num_vertices];
    meshdata2.num_triangles = mesh2.triangles.size();
    meshdata2.triangles = new Triangle[meshdata2.num_triangles];
    for ( int i = 0; i < meshdata2.num_vertices; ++i ) {
        meshdata2.vertices[i].position = mesh2.vertices[i].position;
        meshdata2.vertices[i].normal = mesh2.vertices[i].normal;
    }
    for ( int i = 0; i < meshdata2.num_triangles; ++i ) {
        for ( int j = 0; j < 3; ++j ) {
            meshdata2.triangles[i].vertices[j] = mesh2.triangles[i].vertices[j];
        }
    }

}

void create_bsp(){
  TreeTriangleList t1;
  TreeTriangleList t2;

  for(int i =0; i < mesh1.triangles.size(); i++){
    TreeTriangle tri;    
    for(int j =0; j <3; j++){
      tri.vertices[j] = mesh1.vertices[mesh1.triangles[i].vertices[j]].position;
    }
    t1.push_back(tri);
  }

  for(int i =0; i < mesh2.triangles.size(); i++){
    TreeTriangle tri;
    for(int j =0; j <3; j++){
      tri.vertices[j] = mesh2.vertices[mesh2.triangles[i].vertices[j]].position;
    }
    t2.push_back(tri);
  }

  tree1 = create_tree(t1);
  tree2 = create_tree(t2);

}

void merge_bsp(){
  merge_trees(tree1, tree2);
}

bool init(){
  ASSERT(SDL_Init(SDL_INIT_EVERYTING)>=0);
  int videoFlags;
  videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
  videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
  //videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
  videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  ASSERT(SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
			  SCREEN_BPP, videoFlags) != NULL);

  resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );
  std::cout<<"window created"<<std::endl;
  ASSERT(initGL());
  SDL_WM_SetCaption("BSP tree", NULL);
  
  
  return true;

}

void handle_keypress(SDLKey key, bool &quit)
{
  switch(key){
  case SDLK_ESCAPE: quit = true; break;
  default: break;
  }
}

void main_loop()
{
  bool quit = false;
  while(!quit){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
	quit = true;
      }
      else if (event.type == SDL_KEYDOWN)
	handle_keypress(event.key.keysym.sym, quit);
    }
    render();
    update();
  }
}
int main( int argc, char **argv )
{
  mesh1.filename = "models/pool.obj";
  mesh2.filename = "models/cube.obj";
  if(argc > 1){
    mesh1.filename = argv[1];
  }
  if(argc>2){
    mesh2.filename = argv[2];
  }
    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;
    /* main loop variable */
    int done = FALSE;
    /* used to collect events */
    SDL_Event event;
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;
    /* whether or not the window is active */
    int isActive = TRUE;

    init();
    load_mesh();
    std::cout<<"mesh loaded"<<std::endl;
    normalize_shift_mesh(mesh1, Vector3::Zero());
    normalize_shift_mesh(mesh2, Vector3(1.0, 0.0, 0.0));
    std::cout<<"mesh normalize"<<std::endl;
    copy_mesh_data();
    std::cout<<"mesh copied"<<std::endl;
    create_bsp();
    std::cout<<"bsp created"<<std::endl;
    //merge_bsp();
    std::cout<<"bsp merged"<<std::endl;
    main_loop();

    Quit(0);
    /* resize the initial window */
    return( 0 );
}
