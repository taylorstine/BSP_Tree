#ifndef _TJS_BSPTREE
#define _TJS_BSPTREE
#include "math/vector.hpp"
#include <vector>
enum render_type{AONLY, BONLY, ANOTB, BNOTA, AUNIONB, APLUSB, DEFAULT};

struct TreeTriangle{
  Vector3 vertices[3];
  TreeTriangle();
  TreeTriangle(Vector3 a, Vector3 b, Vector3 c);
  Vector3 normal();
};

struct BSP_tree{
  TreeTriangle triangle;
  BSP_tree * front;
  BSP_tree * back;
  BSP_tree *parent;
  BSP_tree();
  BSP_tree(TreeTriangle t);
  void add(TreeTriangle t);
  void add(std::vector<TreeTriangle> to_add);
  float f(Vector3 p);
  inline bool isempty(){return this == NULL;}

};



template<class T>
void swap(T& a, T& b);

Vector3 intersect(Vector3 n, Vector3 p0, Vector3 a, Vector3 c);
BSP_tree * create_tree(std::vector<TreeTriangle> triangles);
std::vector<TreeTriangle>* merge_trees(BSP_tree* A, BSP_tree* B);
std::vector<TreeTriangle>* merge_trees(std::vector<TreeTriangle>,
				       std::vector<TreeTriangle>,
				       BSP_tree* A, BSP_tree* B);
void traverse(BSP_tree* node, std::vector<TreeTriangle> &list);
void traverse(BSP_tree* node);
void insert(const BSP_tree*, const std::vector<TreeTriangle>, std::vector<TreeTriangle>&,
	    std::vector<TreeTriangle>&);

#endif
