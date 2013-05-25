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
};

struct Btree{
  TriangleList tree_triangles;
  VertexList tree_vertices;
  NormalList tree_normals;
  const Mesh * mesh;
  Bnode * root;
  //trigger to tell us when to display triangles and tree triangle information
  //during tree creation

  Btree(const Mesh * mesh);
  ~Btree();
  void add(TriangleList triangles);

  inline Vertex V0( Triangle triangle ){return tree_vertices[ triangle.indices[ 0 ]];}
  inline Vertex V1( Triangle triangle ){return tree_vertices[ triangle.indices[ 1 ] ];}
  inline Vertex V2( Triangle triangle ){return tree_vertices[triangle.indices[2]];}
  inline Vector3 P0( Triangle triangle ){return V0( triangle ).position;}
  inline Vector3 P1( Triangle triangle ){return V1( triangle ).position;}
  inline Vector3 P2( Triangle triangle ){return V2( triangle ).position;}
  inline int NORMAL_INDEX( Bnode * node){return tree_triangles[node->triangle_index].normal_index;}
  inline Vector3 NORMAL( Bnode* node ){return tree_normals[NORMAL_INDEX(node)];}
  inline Vector3 Q0( Bnode* node ){return P0(tree_triangles[node->triangle_index]);}
  inline float D( Bnode* node ){return -dot(NORMAL( node ), Q0( node ) );}
  inline int LAST_TRIANGLE_INDEX(){return tree_triangles.size()-1;}
  inline int LAST_VERTEX_INDEX(){return tree_vertices.size()-1;}
  inline bool B_IS_ODD_MAN_OUT(float fa, float fc){return fa*fc>=0.0f;}
  inline bool A_IS_ODD_MAN_OUT(float fb, float fc){return fb*fc>=0.0f;}


private:
  inline Vertex compute_intersect(Bnode* const, const Vertex& a, const Vertex& c);
};

inline float distance_to_plane(const Vector3 &to_be_tested, const Vector3 &normal,
			const Vector3 &point_on_plane);

#endif
