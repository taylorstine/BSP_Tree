

#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "bsptree/mesh.hpp"
#include "math/vector.hpp"
#define ASSERT(condition){if(!(condition)){std::cerr<<"ASSERTION FAILED: "<<#condition<<"@"<<__FILE__<<"("<<__LINE__<<")"<<std::endl;}}

const int SCREEN_HEIGHT = 900;
const int SCREEN_WIDTH = 700;
const int SCREEN_BPP = 32;
SDL_Event event;
Mesh mesh;

bool initGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
  //  glOrtho(0.0, (float)1.0, 0.0, 1.0, -1.0, 1.0);
  GLenum error = glGetError();
  if(error != GL_NO_ERROR){
    std::cout<<"Gremlins in openGL init!\n"<<gluErrorString(error)<<std::endl;
    return false;
  }
  return true;
}

bool init()
{
  ASSERT(SDL_Init(SDL_INIT_EVERYTHING)>=0);
  ASSERT(SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
			  SCREEN_BPP, SDL_OPENGL) != NULL);
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

void clean_up()
{
}
void update()
{
  
}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
		
	//set the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//	gluPerspective(camera->get_fov_degrees(), camera->get_aspect_ratio(), camera->get_near_clip(), camera->get_far_clip());
	gluPerspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 0.1, 100.0);
	
	Vector3 cameraPosition = Vector3(0.0, 2.5, 5.5);//camera->get_position();
	Vector3 cameraDirection = Vector3(0.0, 0.0, -1.0);//camera->get_direction();
	Vector3 cameraUp = Vector3(0.0, 1.0, 0.0);//camera->get_up();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		cameraDirection.x + cameraPosition.x, 
		cameraDirection.y + cameraPosition.y, 
		cameraDirection.z + cameraPosition.z,
		cameraUp.x, cameraUp.y, cameraUp.z);
	

	//RENDER POOL
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//load the pool transformation matrix
	
	//	glEnableClientState(GL_VERTEX_ARRAY);
	//	glEnableClientState(GL_NORMAL_ARRAY);

	//	glNormalPointer(GL_DOUBLE, 0, poolMesh->normals);
	float verts[mesh.vertices.size()*3];
	for(int i =0; i < mesh.vertices.size(); i++){
	  for(int j = 0; j < 3; j++){
	    verts[i*3+j] = mesh.vertices[i].position[j];
	  }

	}
	//	glVertexPointer(3, GL_DOUBLE, 0, verts);



	//set the parameters for the lighting of the pool
	glPushMatrix();
	glLoadIdentity();
	GLfloat pool_specular[] = {0, 0, 0, 1.0};
	GLfloat pool_diffuse[] = {1.0, .2, .1};
	GLfloat pool_ambient[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat pool_shine[] = {200};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT,	pool_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pool_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, pool_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, pool_shine);
	glPopMatrix();
	

	//draw the pool
	unsigned int tri_idx[mesh.triangles.size() *3];
	for(int i =0; i < mesh.vertices.size(); i++){
	  for(int j = 0; j < 3; j++){
	    tri_idx[i*3+j] = mesh.triangles[i].vertices[j];
	  }
	}

	//	glDrawElements(GL_TRIANGLES, mesh.triangles.size(), GL_UNSIGNED_INT, &tri_idx);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0, 0.0, -10.0);
	glVertex3f(50.0, 50.0, -10.0);
	glVertex3f(100.0, 0.0, -10.0);
	glEnd();
  SDL_GL_SwapBuffers();
  
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

void load_mesh(){
  mesh.filename = "models/pool.obj";
  mesh.load();
}

int main(int argc, char* argv[])
{
  init();
  load_mesh();
  main_loop();
  
}
