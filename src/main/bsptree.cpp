#include "bsptree.hpp"
#include "math/vector.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

#define EPSILON 1e-8
#define MINIMUM_CLAMP(val)(fabs(val)<EPSILON ? (val) = 0 : (val) = (val))
template<class T> inline void SWAP(T &a, T &b){T c = b; b = a; a = c;}

Btree::Btree(const Mesh *mesh):root(NULL){
  this->mesh = mesh;
  this->tree_normals = mesh->normals;
  this->tree_vertices = mesh->vertices;
  this->tree_triangles.push_back(mesh->triangles[0]);
  this->root = new Bnode(LAST_TRIANGLE_INDEX());
  std::cout<<"Creating BSP tree..."<<std::endl;
  
  //randomize the triangles
  TriangleList to_add = mesh->triangles;
  std::srand( unsigned ( std::time(NULL) ) );
  std::random_shuffle(to_add.begin(), to_add.end());
  add(to_add);
  std::cout<<"BSP tree complete!"<<std::endl;
  std::cout<<tree_triangles.size()<<" tree triangles for "<<
    mesh->triangles.size()<< " mesh triangles"<<std::endl;
}

inline float distance_to_plane(const Vector3 &to_be_tested, const Vector3 &normal,
			const Vector3 &point_on_plane){
  return dot(normal, to_be_tested) + dot(-normal, point_on_plane);
}

inline Vertex Btree::compute_intersect(Bnode * const current, const Vertex &a, const Vertex &c) {
  Vertex V;
  Vector3 N = NORMAL( current );
  float d = D( current );
  Vector3 CA = c.position - a.position;
  Vector3 A = a.position;
  float t = -( dot( N,A ) + d )/( dot( N,CA ) );
  V.position = a.position + t * ( CA );
  V.normal = normalize( ( 1-t ) * a.normal + t*c.normal);
  //  V.tex_coord = a.tex_coord + t *( c.tex_coord - a.tex_coord );
  return V;
}

//for some reason there is an infinitle loop where the triangle
//splits itself and then continues to try to split itself
void Btree::add(TriangleList triangles){
  float fa=0.0, fb=0.0, fc=0.0;
  Vertex a,b,c;
  int a_idx = -1, b_idx = -1, c_idx = -1, A_idx = -1, B_idx = -1;
  int current_triangle_size = -1, current_tree_triangle_size = -1;
  Vector3 tri_p0, tri_p1, tri_p2, current_q0, current_normal;
  //set the current node to the root
  Bnode *current = this->root;

  while(!triangles.empty()){
    if(triangles.size()%1000 == 0 && current_triangle_size != triangles.size()){
      std::cout<<triangles.size()<<" triangles left"<<std::endl;
      current_triangle_size = triangles.size();    
    }

    //get the current triangle
    Triangle tri = triangles.back();

    tri_p0 = P0( tri );
    tri_p1 = P1( tri );
    tri_p2 = P2( tri );
    current_normal = NORMAL( current );
    current_q0 = Q0( current );
    
    //store the vertices of the triangle for splitting later
    a = V0( tri );
    b = V1( tri );
    c = V2( tri );
    
    //store the triangle indices, in case we need to split
    a_idx = tri.indices[ 0 ];
    b_idx = tri.indices[ 1 ];
    c_idx = tri.indices[ 2 ];

    //find the (signed) distance of each vertex to the
    //current partitioning plane
    fa = distance_to_plane( tri_p0, current_normal, current_q0 );
    fb = distance_to_plane( tri_p1, current_normal, current_q0 );
    fc = distance_to_plane( tri_p2, current_normal, current_q0 );

    //if the values of the distances of the vertices are smaller than
    //a certain value, just set the distance to zero
    MINIMUM_CLAMP(fa);
    MINIMUM_CLAMP(fb);
    MINIMUM_CLAMP(fc);

    //here we test for a redundant triangle i.e. the distance to the plane
    //is zero, so there is no point in adding it
    if(fa == 0 && fb == 0 && fc == 0){
      triangles.pop_back();
    }

    //If all vertices in the triangle are completley in back of the partioing triangle plane
    if(fa<=0 && fb<=0 && fc<=0){
      //if the back tree is empty
      if(current->back == NULL){
	//done classifying that triangle
	triangles.pop_back();
	//add the triangle to the tree's list of triangles
	tree_triangles.push_back(tri);
	//create a new node in the tree, and assign the last triangle index to it
	current->back = new Bnode(LAST_TRIANGLE_INDEX());
	current = this->root;
      }
      //if the front node is not empty
      else{
	//move to the front node and continue
	current = current->back;
      }
    }
    //if all vertices in the triangle are completly in front of the partitioning triangle plane
    else if(fa>=0 && fb>=0 && fc>=0){
      //if the front node is empty
      if(current->front == NULL){
	//done classifying this triangle
	triangles.pop_back();
	//add this triangle to the tree's triangle list
	tree_triangles.push_back(tri);
	//create a new node
	current->front = new Bnode(LAST_TRIANGLE_INDEX());
	current = this->root;
      }
      //if the front is not empty
      else{
	//set the front to the current node and continue
	current = current->front;
      }
    }
    //if some of the vertices of the triangle are in front
    //and some are behind, then we have to split the triangle
    else{
      triangles.pop_back();

      //Here we test for a special case where one of the vertices
      //lies exactly on a splitting line.  In this case we make two triangles
      //instead of three
      if(fa == 0.0f || fb == 0.0f || fc == 0.0f){
	//	std::cout<<"special split at "<<current->triangle_index<<" tree triangle index"<<std::endl;
	if(fa == 0.0f){
	  SWAP( a,c ); SWAP( a,b );
	  //	  SWAP( fa,fc ); SWAP( fa,fb );
	  SWAP( a_idx,c_idx ); SWAP( a_idx,b_idx);
	}
	else if(fb == 0.0f){
	  SWAP( b,c ); SWAP( a,b );
	  //	  SWAP( fb,fc ); SWAP( fa,fb );
	  SWAP( b_idx,c_idx ); SWAP( a_idx,b_idx );
	}
	Vertex A = compute_intersect( current, a, b );
	tree_vertices.push_back(A);
	A_idx = LAST_VERTEX_INDEX();	  

	Triangle t1;
	t1.indices[0] = a_idx;
	t1.indices[1] = A_idx;
	t1.indices[2] = c_idx;
	t1.normal_index = NORMAL_INDEX( current );
	triangles.push_back(t1);
	
	Triangle t2;
	t2.indices[0] = b_idx;
	t2.indices[1] = c_idx;
	t2.indices[2] = A_idx;
	t2.normal_index = NORMAL_INDEX( current );
	triangles.push_back(t2);
	}
      //Here we determine which points are on which side of the triangle, 
      //and then swap the points so that point c is always the only point 
      //on one side of the plane and a and b are on the other side
      else{
	//	std::cout<<"regular split at "<<current->triangle_index<<" tree triangle index"<<std::endl;
	if(B_IS_ODD_MAN_OUT(fa,fc)){
	  SWAP( b,c ); SWAP( a,b );
	  //	  SWAP( fb,fc ); SWAP( fa,fb );
	  SWAP( b_idx,c_idx ); SWAP( a_idx,b_idx );
	}
	else if(A_IS_ODD_MAN_OUT(fb,fc)){
	  SWAP( a,c ); SWAP( a,b );
	  //	  SWAP( fa,fc ); SWAP( fa,fb );
	  SWAP( a_idx,c_idx ); SWAP( a_idx,b_idx );
	}
	//find the intersection points A and B
	Vertex A = compute_intersect( current, a, c);
	Vertex B = compute_intersect( current, b, c);

	//Add the intersection points to the tree vertex list
	tree_vertices.push_back(A);
	A_idx = LAST_VERTEX_INDEX();
	tree_vertices.push_back(B);
	B_idx = LAST_VERTEX_INDEX();

	//create the triangles to add to the pile to be added
	Triangle t1;
	t1.indices[0] = a_idx;
	t1.indices[1] = b_idx;
	t1.indices[2] = A_idx;
	t1.normal_index = NORMAL_INDEX( current );
	triangles.push_back(t1);
      
	Triangle t2;
	t2.indices[0] = b_idx;
	t2.indices[1] = B_idx;
	t2.indices[2] = A_idx;
	t2.normal_index = NORMAL_INDEX( current );
	triangles.push_back(t2);

	Triangle t3;
	t3.indices[0] = A_idx;
	t3.indices[1] = B_idx;
	t3.indices[2] = c_idx;
	t3.normal_index = NORMAL_INDEX( current );
	triangles.push_back(t3);
      }
    }
  }
}

Bnode::Bnode():front(NULL), back(NULL), triangle_index(-1){}
Bnode::Bnode(int idx):front(NULL), back(NULL){triangle_index = idx;}
