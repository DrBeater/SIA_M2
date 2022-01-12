
#ifndef MESH_PROCESSING_APP_H
#define MESH_PROCESSING_APP_H

#include "viewer.h"
#include <Eigen/SparseCholesky>

typedef Eigen::SparseMatrix<float> SpMat;
typedef Eigen::PermutationMatrix<Eigen::Dynamic> Permutation;

class MeshProcessingApp : public Viewer {
public:
  //! Constructor
  MeshProcessingApp();
  ~MeshProcessingApp();

  void init(int w, int h, int argc, char **argv);
  void updateScene();

  // events
  void mousePressed(GLFWwindow* window, int button, int action, int mods);
  void mouseMoved(int x, int y);
  void charPressed(int key);

protected:

  bool pickAt(const Eigen::Vector2f &p, Hit &hit) const;
  bool selectAround(const Eigen::Vector2f &p) const;

  // main mesh to work on:
  Mesh* _mesh;

  float _pickingRadius = 0.1f;
  bool _pickingMode = false;

  pmp::Vertex _picked_vertex;

};

#endif // MESH_PROCESSING_APP_H
