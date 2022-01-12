#include "mesh.h"
#include "quad.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include <pmp/SurfaceNormals.h>

using namespace std;
using namespace Eigen;
using namespace pmp;

Mesh::~Mesh() {
  if (_ready) {
    glDeleteBuffers(2, _vbo);
    glDeleteVertexArrays(1, &_vao);
  }
}

void Mesh::load(const string &filename) {
  cout << "Loading: " << filename << endl;

  _halfEdgeMesh.read(filename);
  SurfaceNormals::compute_face_normals(_halfEdgeMesh);
  SurfaceNormals::compute_vertex_normals(_halfEdgeMesh);

  // vertex properties
  VertexProperty<Point> vertices =
      _halfEdgeMesh.get_vertex_property<Point>("v:point");
  VertexProperty<Normal> vnormals =
      _halfEdgeMesh.get_vertex_property<Normal>("v:normal");
  VertexProperty<TexCoord> texcoords =
      _halfEdgeMesh.get_vertex_property<TexCoord>("v:texcoord");
  VertexProperty<Color> colors =
      _halfEdgeMesh.get_vertex_property<Color>("v:color");

  // vertex iterator
  SurfaceMesh::VertexIterator vit;

  Vector4f pos;
  Vector3f normal;
  Vector2f tex;
  Vector4f color;
  for (vit = _halfEdgeMesh.vertices_begin();
       vit != _halfEdgeMesh.vertices_end(); ++vit) {
    pos << static_cast<Eigen::Vector3f>(vertices[*vit]), 1.f;
    normal = static_cast<Eigen::Vector3f>(vnormals[*vit]);
    if (texcoords)
      tex = static_cast<Eigen::Vector2f>(texcoords[*vit]);
    if (colors)
      color << static_cast<Eigen::Vector3f>(colors[*vit]), 1.f;
    else
      color = Vector4f(0.85f, 0.85f, 0.85f, 1.f);

    _vertices.push_back(Vertex(pos, color, normal, tex));
  }

  // face iterator
  SurfaceMesh::FaceIterator fit, fend = _halfEdgeMesh.faces_end();
  // vertex circulator
  SurfaceMesh::VertexAroundFaceCirculator fvit, fvend;
  pmp::Vertex v0, v1, v2;
  for (fit = _halfEdgeMesh.faces_begin(); fit != fend; ++fit) {
    fvit = fvend = _halfEdgeMesh.vertices(*fit);
    v0 = *fvit;
    ++fvit;
    v2 = *fvit;

    do {
      v1 = v2;
      ++fvit;
      v2 = *fvit;
      _indices.push_back(v0.idx());
      _indices.push_back(v1.idx());
      _indices.push_back(v2.idx());
    } while (++fvit != fvend);
  }
}

void Mesh::init() {
  glGenVertexArrays(1, &_vao);
  glGenBuffers(2, _vbo);

  glBindVertexArray(_vao);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * _indices.size(),
               _indices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(),
               _vertices[0].position.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  _ready = true;
}

void Mesh::display(Shader *shader) {
  if (!_ready)
    init();

  glBindVertexArray(_vao);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[0]);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);

  int vertex_loc = shader->getAttribLocation("vtx_position");
  if (vertex_loc >= 0) {
    glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)0);
    glEnableVertexAttribArray(vertex_loc);
  }

  int color_loc = shader->getAttribLocation("vtx_color");
  if (color_loc >= 0) {
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(sizeof(Vector4f)));
    glEnableVertexAttribArray(color_loc);
  }

  int normal_loc = shader->getAttribLocation("vtx_normal");
  if (normal_loc >= 0) {
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(2 * sizeof(Vector4f)));
    glEnableVertexAttribArray(normal_loc);
  }

  int tangent_loc = shader->getAttribLocation("vtx_tangent");
  if (tangent_loc >= 0) {
    glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(2 * sizeof(Vector4f)));
    glEnableVertexAttribArray(tangent_loc);
  }

  int bitangent_loc = shader->getAttribLocation("vtx_bitangent");
  if (bitangent_loc >= 0) {
    glVertexAttribPointer(bitangent_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(2 * sizeof(Vector4f)));
    glEnableVertexAttribArray(bitangent_loc);
  }

  int texCoord_loc = shader->getAttribLocation("vtx_texcoord");
  if (texCoord_loc >= 0) {
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(sizeof(Vector3f) + 2 * sizeof(Vector4f)));
    glEnableVertexAttribArray(texCoord_loc);
  }

  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);

  if (vertex_loc >= 0)
    glDisableVertexAttribArray(vertex_loc);
  if (color_loc >= 0)
    glDisableVertexAttribArray(color_loc);
  if (normal_loc >= 0)
    glDisableVertexAttribArray(normal_loc);
  if (tangent_loc >= 0)
    glDisableVertexAttribArray(tangent_loc);
  if (bitangent_loc >= 0)
    glDisableVertexAttribArray(bitangent_loc);
  if (texCoord_loc >= 0)
    glDisableVertexAttribArray(texCoord_loc);

  glBindVertexArray(0);
}

// Builds the quad
// p0 ---- p1
// |     / |
// |  /    |
// p2 ---- p3

void Mesh::addQuad(Mesh *mesh, const Vector4f &p0, const Vector4f &p1,
                   const Vector4f &p2, const Vector4f &p3) {
  uint idx = mesh->_vertices.size();
  mesh->_vertices.push_back(Vertex(p0));
  mesh->_vertices.push_back(Vertex(p1));
  mesh->_vertices.push_back(Vertex(p2));
  mesh->_vertices.push_back(Vertex(p3));
  mesh->_indices.push_back(idx);
  mesh->_indices.push_back(idx + 1);
  mesh->_indices.push_back(idx + 2);
  mesh->_indices.push_back(idx + 1);
  mesh->_indices.push_back(idx + 3);
  mesh->_indices.push_back(idx + 2);
}

Mesh *Mesh::computeShadowVolume(const Vector3f &lightPos) {

  Mesh *mesh = new Mesh();
  
  Vector3f L = (_transformation.inverse() * Vector4f(lightPos[0], lightPos[1], lightPos[2], 1)).head(3);

  pmp::VertexProperty<Point> vertices = _halfEdgeMesh.get_vertex_property<Point>("v:point");
  pmp::FaceProperty<Point> normals = _halfEdgeMesh.get_face_property<Point>("f:normal");

  SurfaceMesh::EdgeIterator eit;
  for(eit = _halfEdgeMesh.edges_begin(); eit != _halfEdgeMesh.edges_end(); ++eit) {

    //on récupère les faces adjacentes
    const pmp::Face f1 = _halfEdgeMesh.face(*eit, 0);
    const pmp::Face f2 = _halfEdgeMesh.face(*eit, 1);
    Vector3f n0 = Vector3f(normals[f1][0], normals[f1][1], normals[f1][2]);
    Vector3f n1 = Vector3f(normals[f2][0], normals[f2][1], normals[f2][2]);

    //on calcule les distance signées (L - p).n
    const pmp::Vertex vtx = _halfEdgeMesh.vertex(*eit, 0);

    Vector3f p = Vector3f(vertices[vtx][0], vertices[vtx][1], vertices[vtx][2]);
    
    float dist0 = (L - p).dot(n0);
    float dist1 = (L - p).dot(n1);
    
    //on vérifie qu'il s'agit bien d'une face éclairée et une face à l'ombre qui sont adjacentes à l'arrête 
    if (dist0 < 0 && dist1 > 0 || dist1 < 0 && dist0 > 0) {
      pmp::Vertex v0, v1;
      if (dist0 < 0 && dist1 > 0) {
        v0 = _halfEdgeMesh.vertex(*eit, 0);
        v1 = _halfEdgeMesh.vertex(*eit, 1);
      } else {
        v0 = _halfEdgeMesh.vertex(*eit, 1);
        v1 = _halfEdgeMesh.vertex(*eit, 0);
      }
      
      //on crée un quad avec pour extrémités v0, v1, v0-L et v1-L
      Vector4f end0 = Vector4f(vertices[v0][0], vertices[v0][1], vertices[v0][2], 1);
      Vector4f end1 = Vector4f(vertices[v1][0], vertices[v1][1], vertices[v1][2], 1);
      Vector4f end2 = Vector4f(vertices[v0][0] - L[0], vertices[v0][1] - L[1], vertices[v0][2] - L[2], 0);
      Vector4f end3 = Vector4f(vertices[v1][0] - L[0], vertices[v1][1] - L[1], vertices[v1][2] - L[2], 0);
      mesh->addQuad(mesh, end0, end1, end2, end3);
    }
  }
  
  return mesh;
}
