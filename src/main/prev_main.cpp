//start 

std::vector<TreeTriangle> * merged;

std::vector<TreeTriangle> t1;
std::vector<TreeTriangle> t2;
BSP_tree* tree1;
BSP_tree* tree2;

/* function to reset our viewport after a window resize */

/* function to handle key press events */
void handleKeyPress( SDL_keysym *keysym )
{
    switch ( keysym->sym )
	{
	case SDLK_ESCAPE:
	    /* ESC key was pressed */
	    Quit( 0 );
	    break;
	case SDLK_F1:
	    /* F1 key was pressed
	     * this toggles fullscreen mode
	     */
	    SDL_WM_ToggleFullScreen( surface );
	    break;
	case SDLK_1: render_model = DEFAULT; break;
	case SDLK_2: render_model = AONLY; break;
	case SDLK_3: render_model = BONLY; break;
	case SDLK_4: render_model = ANOTB; break;
	case SDLK_5: render_model = BNOTA; break;
	case SDLK_6: render_model = AUNIONB; break;
	case SDLK_7: render_model = APLUSB; break;
	case SDLK_DOWN: ry+= 2; break;
	case SDLK_UP: ry-= 2; break;
	case SDLK_RIGHT: rx+= 2; break;
	case SDLK_LEFT: rx-= 2; break;
	default:
	    break;
	}

    return;
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
	GLfloat light0_ambient[] = {.5, .5, .5, 0};
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
int drawGLScene( void )
{
    /* rotational vars for the triangle and quad, respectively */
    static GLfloat rtri, rquad;
    /* These are to calculate our fps */
    static GLint T0     = 0;
    static GLint Frames = 0;


    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Move Right 3 Units */
    glLoadIdentity( );
    glTranslatef( 1.5f, 0.0f, -6.0f );

    /* Rotate The Quad On The X axis ( NEW ) */
    glRotatef( ry, 1.0f, 0.0f, 0.0f );
    glRotatef(rx, 0.0f, 1.0f, 0.0f);

    /* Set The Color To Blue One Time Only */
    glColor3f( 0.5f, 0.5f, 1.0f);
  GLfloat model1_ambient[] = {.2, .2, .8, 1.0};
      GLfloat model1_diffuse[] = {.2, .2, .8, 1.0};
      GLfloat model1_specular[] = {.80, .80, .80};
      GLfloat model1_shine[] = {121};

      GLfloat model2_ambient[] = {.8, .2, .2, 1.0};
      GLfloat model2_diffuse[] = {.8, .2, .2, 1.0};
      GLfloat model2_specular[] = {.80, .80, .80};
      GLfloat model2_shine[] = {121};

      GLfloat boolean_ambient[] = {.2, .8, .2, 1.0};
      GLfloat boolean_diffuse[] = {.2, .8, .2, 1.0};
      GLfloat boolean_specular[] = {.80, .80, .80};
      GLfloat boolean_shine[] = {121};


      int stride = sizeof(Vertex);

    switch(render_model){
    case DEFAULT:
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glMaterialfv(GL_FRONT, GL_AMBIENT, model1_ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, model1_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, model1_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, model1_shine);
      glVertexPointer(3, GL_FLOAT, stride, meshdata1->vertices);
      glNormalPointer(GL_FLOAT,stride, &(meshdata1->vertices[0].normal));
      glDrawElements(GL_TRIANGLES, meshdata1->num_triangles * 3, GL_UNSIGNED_INT, meshdata1->triangles); 

      glMaterialfv(GL_FRONT, GL_AMBIENT, model2_ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, model2_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, model2_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, model2_shine);

      glVertexPointer(3, GL_FLOAT, stride, meshdata2->vertices);
      glNormalPointer(GL_FLOAT,stride, &(meshdata2->vertices[0].normal));
      glDrawElements(GL_TRIANGLES, meshdata2->num_triangles * 3, GL_UNSIGNED_INT, meshdata2->triangles); 
      break;
    default:
      glMaterialfv(GL_FRONT, GL_AMBIENT, boolean_ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, boolean_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, boolean_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, boolean_shine);
      glVertexPointer(3, GL_FLOAT, stride, bool_data[render_model].vertices);
      glNormalPointer(GL_FLOAT,stride, &(bool_data[render_model].vertices[0].normal));
      glDrawElements(GL_TRIANGLES, bool_data[render_model].num_triangles * 3, GL_UNSIGNED_INT,
		     bool_data[render_model].triangles); 
    }
   
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

    /* Increase The Rotation Variable For The Triangle ( NEW ) */
    //    rtri  += 0.2f;
    /* Decrease The Rotation Variable For The Quad     ( NEW ) */
    //    rquad -=0.15f;


    return( TRUE );
}

void normalize_shift_mesh(Mesh *mesh, Vector3 pos){
  Vector3 mean = Vector3::Zero();
  for(int i =0; i < mesh->vertices.size(); i++){
    mean+= mesh->vertices[i].position;
  }
  mean/= mesh->vertices.size();

  
  float max_length = -1;
  for(int i =0; i < mesh->vertices.size(); i++){
    float dist = length(mesh->vertices[i].position-mean);
    if(dist > max_length)
      max_length = dist;
  }
  
  mesh->scale(2.0/max_length);
  //  mesh.scale(.5);

  mean = Vector3::Zero();
  for(int i =0; i < mesh->vertices.size(); i++){
    mean+= mesh->vertices[i].position;
  }
  mean/= mesh->vertices.size();

  mesh->translate(pos-mean);
}

void load_mesh(){
  mesh1->load();
  mesh2->load();
}

void copy_mesh_data(){
  meshdata1 = new MeshData();
  meshdata2 = new MeshData();
  
  meshdata1->num_vertices = mesh1->vertices.size();
    meshdata1->vertices = new Vertex[meshdata1->num_vertices];
    meshdata1->num_triangles = mesh1->triangles.size();
    meshdata1->triangles = new Triangle[meshdata1->num_triangles];
    for ( int i = 0; i < meshdata1->num_vertices; ++i ) {
        meshdata1->vertices[i].position = mesh1->vertices[i].position;
        meshdata1->vertices[i].normal = mesh1->vertices[i].normal;
    }
    for ( int i = 0; i < meshdata1->num_triangles; ++i ) {
        for ( int j = 0; j < 3; ++j ) {
            meshdata1->triangles[i].vertices[j] = mesh1->triangles[i].vertices[j];
        }
    }

  meshdata2->num_vertices = mesh2->vertices.size();
    meshdata2->vertices = new Vertex[meshdata2->num_vertices];
    meshdata2->num_triangles = mesh2->triangles.size();
    meshdata2->triangles = new Triangle[meshdata2->num_triangles];
    for ( int i = 0; i < meshdata2->num_vertices; ++i ) {
        meshdata2->vertices[i].position = mesh2->vertices[i].position;
        meshdata2->vertices[i].normal = mesh2->vertices[i].normal;
    }
    for ( int i = 0; i < meshdata2->num_triangles; ++i ) {
        for ( int j = 0; j < 3; ++j ) {
            meshdata2->triangles[i].vertices[j] = mesh2->triangles[i].vertices[j];
        }
    }

}

void create_bsp(){

  for(int i =0; i < mesh1->triangles.size(); i++){
    TreeTriangle tri;    
    for(int j =0; j <3; j++){
      tri.vertices[j] = mesh1->vertices[mesh1->triangles[i].vertices[j]].position;
    }
    t1.push_back(tri);
  }

  for(int i =0; i < mesh2->triangles.size(); i++){
    TreeTriangle tri;
    for(int j =0; j <3; j++){
      tri.vertices[j] = mesh2->vertices[mesh2->triangles[i].vertices[j]].position;
    }
    t2.push_back(tri);
  }

  tree1 = create_tree(t1);
  tree2 = create_tree(t2);

}

void merge_bsp(){
  merged = merge_trees(tree1, tree2);
  // merged = merge_trees(t1, t2, tree1, tree2);
}

void convert_bsp_to_mesh()
{
  for(int model_idx =0; model_idx < 6; model_idx++){
    int vert_idx = 0;
    Vertex * verts;
    Triangle * triangles;
    std::vector<TreeTriangle> tri = merged[model_idx];
    bool_data[model_idx].num_vertices = tri.size()*3;
    bool_data[model_idx].num_triangles = tri.size();
    verts = new Vertex[bool_data[model_idx].num_vertices];
    triangles = new Triangle[bool_data[model_idx].num_triangles];
    for(int i =0; i <tri.size(); i++){
      for(int k = 0; k<3; k++){
	triangles[i].vertices[k] = vert_idx;
	verts[vert_idx].position = tri[i].vertices[k];
	verts[vert_idx].normal = tri[i].normal();
	//	verts[vert_idx].normal *= .5;
	vert_idx++;
      }
	
    }
    bool_data[model_idx].triangles = triangles;
    bool_data[model_idx].vertices = verts;
  }

}
