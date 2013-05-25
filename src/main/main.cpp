#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include "math/math.hpp"
#include "math/vector.hpp"
#include "mesh.hpp"
#include "bsptree.hpp"

#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 900
#define SCREEN_BPP     32
#define ASSERT(condition){if(!(condition)){std::cerr<<"ASSERTION FAILED: "<<#condition<<"@"<<__FILE__<<"("<<__LINE__<<")"<<std::endl;}}
template<class T> inline void SWAP(T &a, T &b){T c = b; b = a; a = c;}


SDL_Surface * surface;
int video_flags= 0x0;
int Width = SCREEN_WIDTH;
int Height = SCREEN_HEIGHT;
bool Running;
Mesh mesh;
float rx, ry;
Btree* tree;
char * filename;
bool draw_outline= false;

GLfloat model_ambient[] =   {.2, .2, .8, 1.0};
GLfloat model_diffuse[] =   {.4, .4, .8, 1.0};
GLfloat model_specular[] =  {1.0, 1.0, 1.0};
GLint   model_shine =       50;
GLfloat model_ambient_and_diffuse[] = {.4, .4, .8, 1.0};


void clean_up(){}
void Quit(int return_code){
  SDL_Quit();
  exit(return_code);
}

bool resize_window( int width, int height )
{
  //  GLfloat ratio = (GLfloat)width/(GLfloat)height;
  //glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
  glViewport( 0, 0, ( GLsizei )SCREEN_WIDTH, ( GLsizei )SCREEN_HEIGHT );
  GLfloat ratio = (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT;
    
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
  Vector3 camera_position = Vector3(0.0, 0.0, -5.0);//Vector3(1.0, 5.0, 1.0);
  Vector3 camera_direction = Vector3(0.0, 0.0, 1.0);//normalize(Vector3(0.0, -.8, -1.0));
  Vector3 camera_reference = camera_position + camera_direction;
  Vector3 camera_up = Vector3(0.0, 1.0, 0.0);
  gluLookAt(camera_position.x, camera_position.y, camera_position.z,
  	    camera_reference.x, camera_reference.y, camera_reference.z,
  	    camera_up.x, camera_up.y, camera_up.z);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  return true;
}

bool init_SDL(){
  const SDL_VideoInfo * videoInfo;
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    std::cout<<"Error starting SDL "<<SDL_GetError()<<std::endl;
    Quit(1);
  }
  video_flags = SDL_OPENGL;
  video_flags |= SDL_GL_DOUBLEBUFFER;
  video_flags |= SDL_RESIZABLE;
  if(videoInfo->hw_available)
    video_flags |= SDL_HWSURFACE;
  if(videoInfo->blit_hw)
    video_flags |= SDL_HWACCEL;
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
  surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
			     SCREEN_BPP, video_flags);
  if(!surface){
    std::cout<<"Video mode set failed: "<<SDL_GetError()<<std::endl;
    Quit(1);
  }
  return true;
}

bool init_GL(){

  glShadeModel(GL_SMOOTH);
  glClearColor(.2f, .2f, .2f, 1.0f);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  //Vector for the position of the light
  Vector3 light0Vec(-2, -2, -1);
  //light poisition
  GLfloat light0_position[] = {light0Vec.x, light0Vec.y, light0Vec.z, 1.0};
  //ambient light settings
  GLfloat light0_ambient[] = {.8, .8, .8, 0};
  //diffuse light settings
  GLfloat light0_diffuse[] = {1, 1, 1, 1};
  //specular light settings
  GLfloat light0_specular[] = {1, 1, 1, 1};

  //setting the light with the above parameters
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

  return true;
}


void process_keypress(const SDL_KeyboardEvent& key){
  switch(key.keysym.sym){
  case SDLK_LEFT: rx+=2; break;
  case SDLK_RIGHT: rx-=2; break;
  case SDLK_UP: ry+=2; break;
  case SDLK_DOWN: ry-=2; break;
  case SDLK_ESCAPE: Running = false; break;
  case SDLK_RETURN: std::cout<<"rx "<<rx<<", ry "<<ry<<std::endl; break;
  case SDLK_o: std::cout<<"drawing outline"<<std::endl;
    draw_outline = !draw_outline; 
    break;
  default: break;
  }
}

void process_keystate(){
  Uint8 *keystate = SDL_GetKeyState(NULL);
  if(keystate[SDLK_LEFT]) rx+=2;
  if(keystate[SDLK_RIGHT]) rx-=2;
  if(keystate[SDLK_UP]) ry+=2;
  if(keystate[SDLK_DOWN]) ry-=2;
}

void handle_events(){
  SDL_Event event;
  process_keystate();
  while(SDL_PollEvent(&event)){
    switch(event.type){
    case SDL_QUIT: Quit(0); break;
    case SDL_KEYDOWN: process_keypress(event.key); break;
    case SDL_VIDEORESIZE: resize_window(event.resize.w, event.resize.h);
      //    case SDL_MOUSEMOTION: process_mouse_motion(); break;
      //    case SDL_MOUSEBUTTON: process_
      
    }
  }
}
void update(){}
void render(){
  static GLint T0 = 0;
  static GLint Frames = 0;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glRotatef(ry, 1.0f, 0.0f, 0.0f);
  glRotatef(rx, 0.0f, 1.0f, 0.0f);

  glTranslatef(-mesh.COM.x, -mesh.COM.y, -mesh.COM.z);

  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  glColor3fv( model_ambient_and_diffuse );
  //  glColorMaterial( GL_FRONT, GL_SPECULAR );
  //  glColor3fv( model_specular );
  //  glMaterialfv(GL_FRONT, GL_AMBIENT, model_ambient);
  glMaterialfv( GL_FRONT, GL_SPECULAR, model_specular );
  //  glMaterialfv(GL_FRONT, GL_DIFFUSE, model_diffuse);
  glMateriali( GL_FRONT, GL_SHININESS, model_shine );

  int stride = sizeof(Vertex);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, stride, &mesh.vertices[0].normal);
  glVertexPointer(3, GL_FLOAT, stride, &mesh.vertices[0].position);
  glDrawElements(GL_TRIANGLES, mesh.num_triangles *3,
		 GL_UNSIGNED_INT, &mesh.indices[0]);

  /*  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
     glVertex3f(-1.0, -1.0, -0.0);
     glVertex3f(-1.0, 1.0, -0.0);
     glVertex3f(1.0, 1.0, -0.0);
      glVertex3f(1.0, -1.0, -0.0);
      glEnd();*/

  if(draw_outline){
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0f);
    glDrawElements(GL_POINTS, mesh.num_vertices,
		   GL_UNSIGNED_INT, &mesh.indices[0]);
  }

  SDL_GL_SwapBuffers();
  Frames++;
  {
    GLint t = SDL_GetTicks();
    if(t - T0 >= 5000){
      GLfloat seconds = (t-T0)/1000.0;
      GLfloat fps = Frames/seconds;
      std::cout<<Frames<<" frames in "<<seconds<<" = "<<fps<<" FPS"<<std::endl;
      T0 = t;
      Frames = 0;
    }
  }

}
void main_loop(){
  Running = true;
  while(Running){
    handle_events();
    render();
    update();
  }
}

void init(){
  ASSERT(init_SDL());
  ASSERT(init_GL());
  resize_window(SCREEN_WIDTH, SCREEN_HEIGHT);
  mesh.load(filename);
  rx = -22.0;
  ry = -32.0;
  tree = new Btree(&mesh);
}

void handle_args( int argc, char **argv ){
  if(argc > 1)
    filename = argv[1];
  else
    filename = "models/armadillo.obj";
}

int main( int argc, char **argv )
{
  handle_args(argc, argv);
  init();
  main_loop();
  clean_up();
    return( 0 );
}






