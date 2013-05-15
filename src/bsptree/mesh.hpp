#ifndef _TJS_MESH_
#define _TJS_MESH_

#include "math/vector.hpp"
#include <vector>
#include <cassert>

struct Vertex
{
  Vector3 position;
  Vector3 normal;
};

struct Triangle
{
  unsigned int vertices[3];
  //Vector3 normal;
};

struct Mesh
{
  Mesh();
  virtual ~Mesh();
  typedef std::vector<Triangle> TriangleList;
  typedef std::vector<Vertex> VertexList;
  typedef int size_t;
  
  TriangleList triangles;
  VertexList vertices;
  std::string filename;
  void calculate_normals();
  bool load();
  void translate(Vector3 t);
  void scale(float s);

private:
      // prevent copy/assignment
  Mesh( const Mesh& );
  Mesh& operator=( const Mesh& );

};

struct MeshData
{
    // array of vertices
    Vertex* vertices;
    // size of vertex array
    size_t num_vertices;

    // array of triangles
    Triangle* triangles;
    // size of triangle array
    size_t num_triangles;
  MeshData();
  ~MeshData();
};

#endif
