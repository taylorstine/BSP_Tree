#ifndef _TAYLOR_JOSPEH_BSP_
#define _TAYLOR_JOSEPH_BSP_
#include "mesh.hpp"

struct Bnode{
  Bnode *front;
  Bnode *back;
  int triangle_index;
  
  Bnode();
  Bnode(int triangle_index);
  ~Bnode();
  void add(Triangle tri);
  
  inline Triangle TRIANGLE(){return this->tree->tree_triangles[ triangle_index]; }
  inline Vertex V0(Triangle tri){return this->tree->tree_vertices[TRIANGLE().indices[ 0 ]];}
  inline Vertex V1(Triangle tri){return this->tree->tree_vertices[TRIANGLE().indices[ 1 ]];}
  inline Vertex V2(Triangle tri){return this->tree->tree_vertices[TRIANGLE().indices[ 2 ]];}
  inline Vector3 P0(Triangle tri){return V0().position;}
  inline Vector3 P1(Triangle tri){return V1().position;}
  inline Vector3 P2(Triangle tri){return V2().position;}
  inline int NORMAL_INDEX(){return TRIANGLE().normal_index;}
  inline Vector3 NORMAL( ){return this->tree->tree_normals[NORMAL_INDEX()];}
  inline float D( ){return -dot(NORMAL(), P0() );}


private:
  Btree *tree;
};

struct Btree{
  TriangleList tree_triangles;
  VertexList tree_vertices;
  NormalList tree_normals;
  const Mesh * mesh;
  Bnode * root;

  Btree(const Mesh * mesh);
  ~Btree();
  void add(TriangleList triangles);

  inline int LAST_TRIANGLE_INDEX(){return tree_triangles.size()-1;}
  inline int LAST_VERTEX_INDEX(){return tree_vertices.size()-1;}
  inline bool B_IS_ODD_MAN_OUT(float fa, float fc){return fa*fc>=0.0f;}
  inline bool A_IS_ODD_MAN_OUT(float fb, float fc){return fb*fc>=0.0f;}

private:
  inline Vertex compute_intersect(Bnode* const, const Vertex& a, const Vertex& c);
};

inline float distance_to_plane(const Vector3 &to_be_tested, 
			       const Vector3 &normal,
			       const Vector3 &point_on_plane);

#endif
