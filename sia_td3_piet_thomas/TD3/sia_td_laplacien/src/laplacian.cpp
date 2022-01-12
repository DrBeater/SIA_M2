
#include "laplacian.h"
#include "mesh.h"
#include <Eigen/SparseCholesky>

using namespace Eigen;
using namespace pmp;

typedef SparseMatrix<float> SpMat;
typedef PermutationMatrix<Dynamic> Permutation;
typedef Eigen::Triplet<double> T;

double cotan_weight(const SurfaceMesh &mesh, pmp::Halfedge he) {
  auto points = mesh.get_vertex_property<Point>("v:point");

  // TODO

  return 1;
}

/// Computes the Laplacian matrix in matrix \a L using cotangent weights or the
/// graph Laplacian if useCotWeights==false.
void create_laplacian_matrix(const SurfaceMesh &mesh, SpMat &L,
                             bool useCotWeights) {
  // number of vertices in mesh
  int n = (int)mesh.n_vertices();

  std::vector<T> tripletList;
  tripletList.reserve(n);

  SurfaceMesh::VertexIterator v_it;
  for(v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it){
    float w = 0;
    Vertex vi = *v_it;

    for (auto vj : mesh.vertices(vi)) {
      float wij = 1; //fix wij weights to 1
      tripletList.push_back(T(vi.idx(), vj.idx(), wij));
      w += wij;
    }

    tripletList.push_back(T(vi.idx(), vi.idx(), -w));
  }

  L.setFromTriplets(tripletList.begin(), tripletList.end());
}

/// Computes the permutation putting selected vertices (mask==1) first, and the
/// others at the end. It returns the number of selected vertices.
int create_permutation(const SurfaceMesh &mesh, Permutation &perm) {
  auto masks = mesh.get_vertex_property<int>("v:mask");

  // number of vertices in mesh
  int n = (int)mesh.n_vertices();

  perm.resize(n);
  int nb_unknowns = 0, nb_knowns = n-1;
  
  SurfaceMesh::VertexIterator v_it;
  for (v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    Vertex vi = *v_it;
    if (masks[vi] == 1) {

      perm.indices()[vi.idx()] = nb_unknowns;
      ++nb_unknowns;

    } else {

      perm.indices()[vi.idx()] = nb_knowns;
      --nb_knowns;

    }
  }
  return nb_unknowns;
}

/// Performs the poly-harmonic interpolation (order k) over the selected
/// vertices (mask==1) of the vertex attributes u. For each vertex V of index i,
///     if  mask[V]!=1 then u.col(i) is used as input constraints,
///     otherwise, mask[V]==1, and u.col(i) is replaced by the poly-harmonic
///     interpolation of the fixed values.
void poly_harmonic_interpolation(const SurfaceMesh &mesh,
                                 Ref<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> > u,
                                 int k) {
  // Number of vertices in the mesh
  int n = (int)mesh.n_vertices();

  // 1 - Create the sparse Laplacian matrix
  SpMat L(n,n);
  create_laplacian_matrix(mesh, L, false);
  SpMat L_tmp = SpMat(L);
  for (int i = 1; i < k; ++i) {
    L = L * L_tmp;
  }

  // 2 - Create the permutation matrix putting the fixed values at the end,
  //     and the true unknown at the beginning
  Permutation perm;
  int nb = create_permutation(mesh, perm);

  // 3 - Apply the permutation to both rows (equations) and columns (unknowns),
  //     i.e., L = P * L * P^-1

  L = L.twistedBy(perm);

  SpMat L00 = L.topLeftCorner(nb, nb);
  SpMat L01 = L.topRightCorner(nb, n - nb);

  // 4 - solve L * [x^T u^T]^T = 0, i.e., L00 x = - L01 * u

  MatrixXf u_inv = perm * u.transpose();
  SimplicialLDLT<SpMat> solver;
  solver.compute(L00);
  u_inv.topRows(nb) = solver.solve(- L01 * u_inv.bottomRows(n - nb));


  // 5 - Copy back the results to u

  u_inv  = perm.inverse() * u_inv;
  u = u_inv.transpose();
}
