#include "mesh.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

struct TriIndex
{
    int vertex;
    int normal;
    int tcoord;

    bool operator<( const TriIndex& rhs ) const {
        if ( vertex == rhs.vertex ) {
            if ( normal == rhs.normal ) {
                return tcoord < rhs.tcoord;
            } else {
                return normal < rhs.normal;
            }
        } else {
            return vertex < rhs.vertex;
        }
    }
};

struct Face
{
    TriIndex v[3];
};

enum ObjFormat
{
    VERTEX_ONLY = 1 << 0,
    VERTEX_UV = 1 << 1,
    VERTEX_NORMAL = 1 << 2,
    VERTEX_UV_NORMAL = 1 << 3
};

Mesh::Mesh() { }
Mesh::~Mesh() { 
  triangles.clear();
  vertices.clear();
}


bool Mesh::load()
{
    std::cout << "Loading mesh from '" << filename << "'..." << std::endl;

    std::string line;
    std::ifstream file( filename.c_str() );

    typedef std::vector< Vector3 > PositionList;
    typedef std::vector< Vector3 > NormalList;
    typedef std::vector< Vector2 > UVList;
    typedef std::vector< Face > FaceList;

    static const char* scan_vertex = "%d";
    static const char* scan_vertex_uv = "%d/%d";
    static const char* scan_vertex_normal = "%d//%d";
    static const char* scan_vertex_uv_normal = "%d/%d/%d";

    size_t num_vertex;
    TriIndex tri[4];

    FaceList face_list;
    PositionList position_list;
    NormalList normal_list;
    UVList uv_list;

    int line_num = 0;

    std::string token;

    triangles.clear();

    ObjFormat format = VERTEX_ONLY;

    typedef std::map< TriIndex, unsigned int > VertexMap;
    VertexMap vertex_map;

    if ( !file.is_open() ) {
        std::cout << "Error opening file '" << filename << "' for mesh loading.\n";
        return false;
    }

    while ( getline( file, line ) )
    {
        std::stringstream stream( line );
        stream >> token;
        line_num++;

        if ( token == "v" ) {

            Vector3 position;
            stream >> position.x >> position.y >> position.z;

            if ( stream.fail() ) {
                std::cerr << "position syntax error on line " << line_num << std::endl;
                return false;
            }

            position_list.push_back( position );

        } else if ( token == "vn" ) {
            Vector3 normal;
            stream >> normal.x >> normal.y >> normal.z;

            if( stream.fail() ) {
                std::cerr << "normal syntax error on line " << line_num << std::endl;
                return false;
            }
            normal_list.push_back( normal );

        } else if ( token == "vt" ) {

            Vector2 uv;
            stream >> uv.x >> uv.y;

            if ( stream.fail() ) {
                std::cerr << "uv syntax error on line " << line_num << std::endl;
                return false;
            }

            uv_list.push_back( uv );

        } else if ( token == "f" ) {

            std::vector< std::string > face_tokens;
            std::string vert;

            while ( true ) {
                stream >> vert;
                if( stream.fail() )
                    break;
                face_tokens.push_back( vert );
            }

            // if it's the first time parsing a face, figure out the face format
            if ( face_list.size() == 0 ) {
                std::string token = face_tokens[0];

                if ( token.find( "//" ) != std::string::npos ) {
                    format = VERTEX_NORMAL;
                } else if ( token.find( '/' ) == std::string::npos ) {
                    format = VERTEX_ONLY;
                } else {
                    size_t p1 = token.find( '/' );
                    size_t p2 = token.rfind( '/' );
                    if ( p1 == p2 ) {
                        format = VERTEX_UV;
                    } else {
                        format = VERTEX_UV_NORMAL;
                    }
                }
            }

            num_vertex = face_tokens.size();

            if ( num_vertex > 4 || num_vertex < 3 ) {
                std::cerr << "Syntax error at line " << line_num
                          << ", face has incorrect number of vertices" << std::endl;
                return false;
            }

            for ( size_t i = 0; i < num_vertex; ++i ) {
                switch ( format )
                {
                case VERTEX_ONLY:
                    sscanf( face_tokens[i].c_str(),
                            scan_vertex,
                            &tri[i].vertex );
                    tri[i].normal = 0;
                    tri[i].tcoord = 0;
                    break;

                case VERTEX_UV:
                    sscanf( face_tokens[i].c_str(),
                            scan_vertex_uv,
                            &tri[i].vertex,
                            &tri[i].tcoord );
                    tri[i].normal = 0;
                    break;

                case VERTEX_NORMAL:
                    sscanf( face_tokens[i].c_str(),
                            scan_vertex_normal,
                            &tri[i].vertex,
                            &tri[i].normal );
                    tri[i].tcoord = 0;
                    break;

                case VERTEX_UV_NORMAL:
                    sscanf( face_tokens[i].c_str(),
                            scan_vertex_uv_normal,
                            &tri[i].vertex,
                            &tri[i].tcoord,
                            &tri[i].normal );
                    break;

                default:
                    std::cerr << "Syntax error, unrecongnized face format at line "
                              << line_num << std::endl;
                    break;
                }
            }

            for ( size_t i = 0; i < num_vertex; ++i ) {
                tri[i].vertex--;
                tri[i].normal--;
                tri[i].tcoord--;
            }

            Face f1 = { { tri[0], tri[1], tri[2] } };
            face_list.push_back( f1 );

            if ( num_vertex == 4 ) {
                Face f2 = { { tri[2], tri[3], tri[0] } };
                face_list.push_back( f2 );
            }

        } else if ( token == " " ) {

        } else {
            //std::cerr << "Unknown token on line " << line_num << std::endl;
        }

        token.clear();
        line.clear();
    }

    // build vertex list using map for shared vertices

    triangles.reserve( face_list.size() );
    vertices.reserve( face_list.size() * 2 );

    // current vertex index, for creating new vertices
    unsigned int vert_idx_counter = 0;

    for ( size_t i = 0; i < face_list.size(); ++i ) {
        const Face& face = face_list[i];
        Triangle tri;
        for ( size_t j = 0; j < 3; ++j ) {
            // two vertices are only actually the same one if the vertex,
            // normal, and tcoord are all the same. use the map to check this.
            std::pair< VertexMap::iterator, bool > rv = vertex_map.insert( std::make_pair( face.v[j], vert_idx_counter ) );
            if ( rv.second ) {
                Vertex v;
                v.position = position_list[face.v[j].vertex];
                vertices.push_back( v );
                vert_idx_counter++;
            }

            tri.vertices[j] = rv.first->second;
        }
        triangles.push_back( tri );
    }
    
    calculate_normals();
    std::cout << "Successfully loaded mesh '" << filename << "'.\n";
    return true;
}

void Mesh::calculate_normals(){
  Vector3 a;
  Vector3 b;
  for(int i =0; i < vertices.size();i++){
    vertices[i].normal = Vector3::Zero();
  }
  for(int i =0; i < triangles.size(); i++){
    a = vertices[triangles[i].vertices[1]].position-
      vertices[triangles[i].vertices[0]].position;
    b = vertices[triangles[i].vertices[1]].position-
      vertices[triangles[i].vertices[2]].position;
    Vector3 norm = normalize(cross(a,b));
    vertices[triangles[i].vertices[0]].normal =
      (vertices[triangles[i].vertices[0]].normal+
       norm)*.5;
    vertices[triangles[i].vertices[1]].normal =
      (vertices[triangles[i].vertices[1]].normal+
       norm)*.5;
    vertices[triangles[i].vertices[2]].normal =
      (vertices[triangles[i].vertices[2]].normal+
       norm)*.5;
  }
  for(int i =0; i < vertices.size(); i++){
    vertices[i].normal = normalize(vertices[i].normal);
  }
}
void Mesh::translate(Vector3 t)
{
  for(int i = 0; i < vertices.size(); i++){
    vertices[i].position += t;
  }
}

void Mesh::scale(float s)
{
  for(int i = 0; i < vertices.size(); i++){
    vertices[i].position*= s;
  }
}

MeshData::MeshData():vertices(NULL), triangles(NULL),
		     num_vertices(0), num_triangles(0){}

MeshData::~MeshData(){
  delete [] vertices;
  delete [] triangles;
}
