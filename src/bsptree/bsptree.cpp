#include "bsptree.hpp"
#include "math/vector.hpp"
#define EPSILON 1e-3
#define ASSERT(condition){if(!(condition)){std::cerr<<"ASSERTION FAILED: "<<#condition<<"@"<<__FILE__<<"("<<__LINE__<<")"<<std::endl;}}


TreeTriangle::TreeTriangle(){
  vertices[0] = Vector3::Zero();
  vertices[1] = Vector3::Zero();
  vertices[2] = Vector3::Zero();
}
TreeTriangle::TreeTriangle(Vector3 a, Vector3 b, Vector3 c){
  vertices[0] = Vector3(a[0], a[1], a[2]);
  vertices[1] = Vector3(b[0], b[1], b[2]);
  vertices[2] = Vector3(c[0], c[1], c[2]);
}

Vector3 TreeTriangle::normal(){
  Vector3 a = vertices[1] - vertices[0];
  Vector3 b = vertices[2] - vertices[1];
  Vector3 n = cross(a,b);
  //  ASSERT(length(n)>= 1e-10);
  return normalize(n);
}
BSP_tree::BSP_tree():front(NULL), back(NULL), parent(NULL){}
BSP_tree::BSP_tree(TreeTriangle t):front(NULL), back(NULL), parent(NULL){
  this->triangle.vertices[0] = Vector3((t.vertices[0])[0], (t.vertices[0])[1], (t.vertices[0])[2]);
  this->triangle.vertices[1] =  Vector3((t.vertices[1])[0], (t.vertices[1])[1], (t.vertices[1])[2]);
  this->triangle.vertices[2] = Vector3((t.vertices[2])[0], (t.vertices[2])[1], (t.vertices[2])[2]);
}

//f classifies if a point is in front or behind a plane by returning
//a corresponding positive or negative scalar
float BSP_tree::f(Vector3 p){
  Vector3 n = triangle.normal();
  return dot(n, p-triangle.vertices[0]);
}

float f(Vector3 p, TreeTriangle triangle){
  Vector3 n = triangle.normal();
  return dot(n, p-triangle.vertices[0]);
}

template<class T>
void swap(T& a, T& b){
  T c = b;
  b = a;
  a = c;
}

Vector3 intersect(const Vector3 n, const Vector3 p0, const Vector3 a, const Vector3 c){
  float num = dot(n,a);
  Vector3 cma = c-a;
  float denom = dot(n,cma);
  float D = - dot(n,p0);
  float t = -(num+D)/denom;
  return a + t * (c-a);
}

BSP_tree* create_tree(std::vector<TreeTriangle> triangles){
  BSP_tree *tree = new BSP_tree(triangles.back());
  triangles.pop_back();
  tree->add(triangles);
  return tree;
}

void BSP_tree::add(std::vector<TreeTriangle> to_add)
{
  TreeTriangle t;
  while(!to_add.empty()){
    BSP_tree * ancestor = NULL;
    BSP_tree* root = this;
    t = to_add.back();
    to_add.pop_back();
    while(root!=NULL){
      Vector3 &a = t.vertices[0];
      Vector3 &b = t.vertices[1];
      Vector3 &c = t.vertices[2];
      float fa = f(a);
      float fb = f(b);
      float fc = f(c);
      if(fabs(fa) < EPSILON)
	fa = 0.0;
      if(fabs(fb) < EPSILON)
	fb = 0.0;
      if(fabs(fc) < EPSILON)
	fc = 0.0;
  
      //all points are on the front side of the plane
      if(fa<0  && fb<0 && fc<0){
	if(root->back == NULL){
	  root->back = new BSP_tree(t);
	  root->back->parent = root;
	  break;
	}
	ancestor = root;
	root = root->back;
	continue;
      }
      //all points are on the back side of the plane
      else if(fa>0  && fb>0 && fc>0){
	if(root->front == NULL){
	  root->front = new BSP_tree(t);
	  root->front->parent = root;
	  break;
	}
	ancestor = root;
	root = root->front;
	continue;
      }
      else if(fa==0  && fb==0 && fc==0){
	while(root != NULL){
	  root = root->front;
	}
	if(root->front == NULL){
	  root->front = new BSP_tree(t);
	  root->front->parent = root;
	}
	ancestor = root;
	root = root->front;
	continue;
      }
      //we have to split the triangle
      else{
	//std::cout<<"Split"<<std::endl;
	if(fa*fc>=0){
	  swap(fb, fc);
	  swap(b,c);
	  swap(fa, fb);
	  swap(a,b);
	}
	else if(fb*fc>=0){
	  swap(fa,fc);
	  swap(a,c);
	  swap(fa,fb);
	  swap(a,b);
	}
	Vector3 A =intersect(triangle.normal(), triangle.vertices[0], a, c);
	Vector3 B =intersect(triangle.normal(), triangle.vertices[0], b, c);
	TreeTriangle T1(a,b,A);
	TreeTriangle T2(b,B,A);
	TreeTriangle T3(A,B,c);
	to_add.push_back(T1);
	to_add.push_back(T2);
	to_add.push_back(T3);
	break;
      }
    } 
    //    std::cout<<to_add.size()<<std::endl;
  }
}


void traverse(BSP_tree* node, std::vector<TreeTriangle> &list)
{
  std::vector<BSP_tree*> stack;
  stack.push_back(node);
  BSP_tree * cur;
  while(!stack.empty()){
    cur = stack.back();
    stack.pop_back();
    list.push_back(cur->triangle);
    if(cur->front != NULL) stack.push_back(cur->front);
    if(cur->back != NULL) stack.push_back(cur->back);
  }
}

void traverse(BSP_tree* node)
{
  std::vector<BSP_tree*> stack;
  stack.push_back(node);
  BSP_tree * cur;
  while(!stack.empty()){
    cur = stack.back();
    stack.pop_back();
    std::cout<<"("<<cur->triangle.vertices[0][0]<<", "<<
      cur->triangle.vertices[0][1]<<", "<<cur->triangle.vertices[0][2]<<")"<<std::endl;
    if(cur->front != NULL) stack.push_back(cur->front);
    if(cur->back != NULL) stack.push_back(cur->back);
  }
}

void insert(BSP_tree * tree, std::vector<TreeTriangle> list,
	    std::vector<TreeTriangle> &inside, std::vector<TreeTriangle> &outside)
{
  std::vector<TreeTriangle> to_add = list;
  TreeTriangle t;
  while(!to_add.empty()){
    BSP_tree* root = tree;
    t = to_add.back();
    to_add.pop_back();
    while(root!=NULL){
      Vector3 &a = t.vertices[0];
      Vector3 &b = t.vertices[1];
      Vector3 &c = t.vertices[2];
      float fa = f(a, root->triangle);
      float fb = f(b, root->triangle);
      float fc = f(c, root->triangle);
      if(fabs(fa) < EPSILON || fa!=fa)
	fa = 0.0;
      if(fabs(fb) < EPSILON || fb!=fb)
	fb = 0.0;
      if(fabs(fc) < EPSILON || fc!=fc)
	fc = 0.0;
      assert(fa==fa);
      assert(fb==fb);
      assert(fc==fc);
      
  
      //all points are on the front side of the plane
      if(fa<=0  && fb<=0 && fc<=0){
	if(root->back == NULL){
	  inside.push_back(t);
	  break;
	}
	root = root->back;
	continue;
      }
      //all points are on the back side of the plane
      else if(fa>=0  && fb>=0 && fc>=0){
	if(root->front == NULL){
	  outside.push_back(t);
	  break;
	}
	root = root->front;
	continue;
      }
      //we have to split the triangle
      else{
	//std::cout<<"Split"<<std::endl;
	if(fa*fc>=0){
	  swap(fb, fc);
	  swap(b,c);
	  swap(fa, fb);
	  swap(a,b);
	}
	else if(fb*fc>=0){
	  swap(fa,fc);
	  swap(a,c);
	  swap(fa,fb);
	  swap(a,b);
	}
	Vector3 A =intersect(root->triangle.normal(), root->triangle.vertices[0], a, c);
	Vector3 B =intersect(root->triangle.normal(), root->triangle.vertices[0], b, c);
	TreeTriangle T1(a,b,A);
	TreeTriangle T2(b,B,A);
	TreeTriangle T3(A,B,c);
	to_add.push_back(T1);
	to_add.push_back(T2);
	to_add.push_back(T3);
	break;
      }
    } 
  }
  
  
}


std::vector<TreeTriangle>* merge_trees(BSP_tree* A, BSP_tree* B){
  std::vector<TreeTriangle> * list = new std::vector<TreeTriangle>[6];
  std::vector<TreeTriangle> A_list, B_list;
  traverse(A, A_list);
  traverse(B, B_list);
  std::vector<TreeTriangle>B_out, B_in;
  insert(A, B_list, B_in, B_out);

  std::vector<TreeTriangle>A_out, A_in;
  insert(B, A_list, A_in, A_out);
  
  std::vector<TreeTriangle> Apb;
  Apb.insert(Apb.end(), A_out.begin(), A_out.end());
  Apb.insert(Apb.end(), B_out.begin(), B_out.end());
  std::vector<TreeTriangle> Aub;
  Aub.insert(Aub.end(), A_in.begin(), A_in.end());
  Aub.insert(Aub.end(), B_in.begin(), B_in.end());
  
  //  list[0] = A_in;
  list[AONLY] = A_list;
  list[BONLY] = B_list;
  list[ANOTB] = A_out;
  list[BNOTA] = B_out;
  list[AUNIONB] = Aub;
  list[APLUSB] = Apb;
  return list;
  
}

std::vector<TreeTriangle>* merge_trees(std::vector<TreeTriangle> A_list, std::vector<TreeTriangle> B_list,
				       BSP_tree *A, BSP_tree * B){
  std::vector<TreeTriangle> * list = new std::vector<TreeTriangle>[6];
  std::vector<TreeTriangle>B_out, B_in;
  insert(A, B_list, B_in, B_out);

  std::vector<TreeTriangle>A_out, A_in;
  insert(B, A_list, A_in, A_out);

  B_in.insert(B_in.end(), A_in.begin(), A_in.end());
  //  list[0] = A_in;
  list[0] = A_out;
  list[1] = B_in;
  list[2] = B_out;
  return list;
  
}
