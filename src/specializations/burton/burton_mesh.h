/*~--------------------------------------------------------------------------~*
 *  @@@@@@@@ @@       @@@@@@@@ @@     @@ @@
 * /@@///// /@@      /@@///// //@@   @@ /@@
 * /@@      /@@      /@@       //@@ @@  /@@
 * /@@@@@@@ /@@      /@@@@@@@   //@@@   /@@
 * /@@////  /@@      /@@////     @@/@@  /@@
 * /@@      /@@      /@@        @@ //@@ /@@
 * /@@      /@@@@@@@@/@@@@@@@@ @@   //@@/@@
 * //       //////// //////// //     // // 
 * 
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

#ifndef flexi_burton_mesh_h
#define flexi_burton_mesh_h

#include <string>

#include "flexi/state/state.h"
#include "flexi/execution/task.h"
#include "flexi/specializations/burton/burton_types.h"

/*!
 * \file burton_mesh.h
 * \authors bergen
 * \date Initial file creation: Sep 02, 2015
 */

namespace flexi {

/*----------------------------------------------------------------------------*
 * class burton_mesh_t
 *----------------------------------------------------------------------------*/

/*!
  \class burton_mesh_t burton.h
  \brief burton_mesh_t A specialization of the flexi low-level mesh topology,
    state and execution models.
 */
class burton_mesh_t
{
private:

  using private_mesh_t = mesh_topology_t<burton_mesh_types_t>;

public:

#ifndef MESH_EXECUTION_POLICY
  // for now: use default execution policy
  using mesh_execution_t = execution_t<>;
#else
  using mesh_execution_t = execution_t<MESH_STORAGE_POLICY>;
#endif

  /*!
    \brief Type defining the data attachment sites on the mesh.
   */
  using attachment_site_t = burton_mesh_traits_t::attachment_site_t;

  /*!
    \brief Accessor type.
   */
  template<typename T>
  using accessor_t = burton_mesh_traits_t::mesh_state_t::accessor_t<T>;

  /*!
    \brief Register state for the named variable at the given attachment
    site.

    \param key A const_string_t name for the state variable, e.g., "density".
    \param site The data attachement site where the state variable should
      be defined.  Valid sites are defined in flexi::burton_mesh_traits_t.
    \param attributes A bitfield specifying various attributes of the state.

    \return An accessor to the newly registered state.
   */
  template<typename T>
  decltype(auto) register_state_(const const_string_t && key,
    attachment_site_t site, bitfield_t::field_type_t attributes = 0x0) {

    switch(site) {
      case attachment_site_t::vertices:
        return state_.register_state<T>(key, num_vertices(),
          attachment_site_t::vertices, attributes);
        break;
      case attachment_site_t::edges:
        return state_.register_state<T>(key, num_edges(),
          attachment_site_t::edges, attributes);
        break;
      case attachment_site_t::cells:
        return state_.register_state<T>(key, num_cells(),
          attachment_site_t::cells, attributes);
        break;
      case attachment_site_t::corners:
        return state_.register_state<T>(key, num_corners(),
          attachment_site_t::corners, attributes);
        break;
      default:
        assert(false && "Error: invalid state registration site.");
    } // switch

  } // register_state_

  /*!
    FIXME
   */
  template<typename T, size_t NS = flexi_user_space>
  decltype(auto) access_state_(const const_string_t && key) {
    return state_.accessor<T,NS>(key);
  } // access_state_

  /*!
    FIXME
   */
  template<typename T, size_t NS = flexi_user_space>
  decltype(auto) access_type_() {
    return state_.accessors<T,NS>();
  } // access_type_

  /*!
    FIXME
   */
  template<typename T, typename P>
  decltype(auto) access_type_if_(P && predicate) {
    return state_.accessors<T,P>(std::forward<P>(predicate));
  } // access_type_if

  /*!
    FIXME
   */
  decltype(auto) state_attributes_(const const_string_t && key) {
    return state_.meta_data<>((key)).attributes;
  } // state_attribtutes_

  /*--------------------------------------------------------------------------*
   * FIXME: Other crap
   *--------------------------------------------------------------------------*/

  // Geometry
  using real_t = burton_mesh_traits_t::real_t;
  using point_t = burton_mesh_traits_t::point_t;
  using vector_t = burton_mesh_traits_t::vector_t;

  // Entity types
  using vertex_t = burton_mesh_types_t::vertex_t;
  using edge_t = burton_mesh_types_t::edge_t;
  using cell_t = burton_mesh_types_t::cell_t;
  using quadrilateral_cell_t = burton_mesh_types_t::quadrilateral_cell_t;
  using corner_t = burton_mesh_types_t::corner_t;

  //! Default constructor
  burton_mesh_t() {}

  //! Copy constructor (disabled)
  burton_mesh_t(const burton_mesh_t &) = delete;

  //! Assignment operator (disabled)
  burton_mesh_t &operator=(const burton_mesh_t &) = delete;

  //! Destructor
  ~burton_mesh_t() {}

  /*!
    \brief Return the topological dimension of the mesh.

    \return auto A non-negative number describing the highest dimension
      of the entities in the mesh, e.g., 3 for a three-dimensional mesh.
   */
  static constexpr auto dimension() {
    return burton_mesh_traits_t::dimension;
  } // dimension

  /*--------------------------------------------------------------------------*
   * Vertex Interface
   *--------------------------------------------------------------------------*/

  /*!
    \brief Return number of vertices in the mesh.
    \return size_t The number of vertices in the mesh.
   */
  size_t num_vertices() const {
    return mesh_.num_entities<0,0>();
  } // num_vertices

  /*!
    \brief Return all vertices in the mesh.
    \return auto Return all vertices in the mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto vertices() {
    return mesh_.entities<0,0>();
  } // vertices

  /*!
    \brief Return vertices associated with entity instance of template type E.
    \tparam E entity type of instance to return vertices for.
    \param e instance of entity to return vertices for.
    \return auto Return vertices associated with entity instance e as a
      sequence.
   */
  template <class E>
  auto vertices(E *e) {
    return mesh_.entities<0,0>(e);
  } // vertices

  /*!
    FIXME
   */
  template<size_t M, class E>
  auto vertices(domain_entity<M, E> & e) {
    return mesh_.entities<0, M>(e);
  }

  /*!
    \brief Return ids for all vertices in the mesh.
    \return auto Ids for all vertices in the mesh.
   */
  auto vertex_ids() {
    return mesh_.entity_ids<0,0>();
  } // vertex_ids

  /*!
    \brief Return vertex ids associated with entity instance of template type E.
    \tparam E entity type of instance to return vertex ids for.
    \param e instance of entity to return vertex ids for.
    \return auto Return vertex ids associated with entity instance e as a
      sequence.
   */
  template <class E>
  auto vertex_ids(E *e) {
    return mesh_.entity_ids<0,0>(e);
  } // vertex_ids

  /*--------------------------------------------------------------------------*
   * Edge Interface
   *--------------------------------------------------------------------------*/

  /*!
    \brief Return the number of mesh edges.
    \return size_t The number of mesh edges.
   */
  size_t num_edges() const {
    return mesh_.num_entities<1,0>();
  } // num_edges

  /*!
    \brief Return all edges in the mesh.
    \return auto Return all edges in the mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto edges() {
    return mesh_.entities<1,0>();
  } // edges

  /*!
    \brief Return edges associated with entity instance of template type E.
    \tparam E entity type of instance to return edges for.
    \param e instance of entity to return edges for.
    \return auto Return edges associated with entity instance e as a sequence.
   */
  template <class E>
  auto edges(E *e) {
    return mesh_.entities<1,0>(e);
  } // edges

  /*!
    \brief Return ids for all edges in the mesh.
    \return auto Ids for all edges in the mesh.
   */
  auto edge_ids() {
    return mesh_.entity_ids<1,0>();
  } // edge_ids

  /*!
    \brief Return edge ids associated with entity instance of template type E.
    \tparam E entity type of instance to return edge ids for.
    \param e instance of entity to return edge ids for.
    \return auto Return edge ids associated with entity instance e as a
      sequence.
   */
  template <class E>
  auto edge_ids(E *e) {
    return mesh_.entity_ids<1,0>(e);
  } // edge_ids

  /*!
    FIXME
   */
  template<size_t M, class E>
  auto edges(domain_entity<M, E>& e) {
    return mesh_.entities<1,M>(e);
  } // edges

  /*--------------------------------------------------------------------------*
   * Face Interface
   *--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*
   * Cell Interface
   *--------------------------------------------------------------------------*/

  /*!
    Return the number of cells in the mesh.
    \return size_t The number of cells in the mesh.
   */
  size_t num_cells() const {
    return mesh_.num_entities<dimension(),0>();
  } // num_cells

  /*!
    \brief Return all cells in the mesh.
    \return auto Return all cells in the mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto cells() {
    return mesh_.entities<dimension(),0>();
  } // cells

  /*!
    \brief Return cells associated with entity instance of template type E.
    \tparam E entity type of instance to return cells for.
    \param e instance of entity to return cells for.
    \return auto Return cells associated with entity instance e as a
      sequence.
   */
  template <class E>
  auto cells(E *e) {
    return mesh_.entities<dimension(),0>(e);
  } // cells

  /*!
    FIXME
   */
  template<size_t M, class E>
  auto cells(domain_entity<M, E>& e) {
    return mesh_.entities<dimension()>(e);
  } // cells

  /*!
    \brief Return ids for all cells in the mesh.
    \return auto Ids for all cells in the mesh.
   */
  auto cell_ids() {
    return mesh_.entity_ids<dimension(),0>();
  } // cell_ids

  /*!
    \brief Return cell ids associated with entity instance of template type E.
    \tparam E entity type of instance to return cell ids for.
    \param e instance of entity to return cell ids for.
    \return auto Return cell ids associated with entity instance e as a
      sequence.
   */
  template <class E>
  auto cell_ids(E *e) {
    return mesh_.entity_ids<dimension(),0>(e);
  } // cell_ids

  /*--------------------------------------------------------------------------*
   * Corner Interface
   *--------------------------------------------------------------------------*/

  /*!
    \brief Return number of corners in the mesh.
   */
  size_t num_corners() const {
    return mesh_.num_entities<1,1>();
  } // num_corners

  /*!
    \brief Return all corners in the mesh.
    \return auto Return all corners in the mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto corners() {
    return mesh_.entities<1,1>();
  } // corners

  /*!
    \brief Return corners associated with entity instance of template type E.
    \tparam E entity type of instance to return corners for.
    \param e instance of entity to return corners for.
    \return auto Return corners associated with entity instance e as a
      sequence.
   */
  template <class E>
  auto corners(E *e) {
    return mesh_.entities<1,1>(e);
  } // corners

#if 0
  /*!
    FIXME
   */
  template<size_t M, class E>
  auto corners(domain_entity<M, E> & e) {
    return mesh_.entities<1, M>(e);
  } 
#endif

  /*!
    \brief Return ids for all corners in the mesh.
    \return auto Ids for all corners in the mesh.
   */
  auto corner_ids() {
    return mesh_.entity_ids<1,1>();
  } // corner_ids

  /*!
    \brief Return corner ids associated with entity instance of template type E.
    \tparam E entity type of instance to return corner ids for.
    \param e instance of entity to return corner ids for.
    \return auto Return corner ids associated with entity instance e as a
      sequence.
   */
  template <class E>
  auto corner_ids(E *e) {
    return mesh_.entity_ids<1,1>(e);
  } // corner_ids

//
//
//
//
//

  /*!
    Create a vertex in the mesh.

    \param pos The position (coordinates) for the vertex.
   */
  // FIXME: Complete changes to state storage
  vertex_t * create_vertex(const point_t &pos) {
    auto p = access_state_<point_t,flexi_internal>("coordinates");
    p[num_vertices()] = pos;

    auto v = mesh_.make<vertex_t>(state_);
    mesh_.add_entity<0, 0>(v);

    return v;
  }

  /*!
    Create a cell in the mesh.

    \param verts The vertices defining the cell.
   */
  cell_t * create_cell(std::initializer_list<vertex_t *> verts) {
    // FIXME: Add element types
    cell_t * c = mesh_.make<quadrilateral_cell_t>();
    mesh_.add_entity<dimension(), 0>(c);

    // FIXME: Need to make mesh interface more general
    mesh_.init_cell<0>(c, verts);
    return c;
  } // create_cell

  /*!
    FIXME
   */

  void dump(){
    mesh_.dump();
  }

  /*!
    FIXME
   */
  void init_parameters(size_t vertices) {

    // register coordinate state
    state_.register_state<point_t,flexi_internal>("coordinates", vertices,
      attachment_site_t::vertices, persistent);

  } // init_parameters

  /*!
    \verbatim

    After cells and vertices for the mesh have been defined, call init() to
    create the dual mesh entities. The following picture shows the vertex
    definitions used to create the dual mesh. For each vertex definition there
    is a corresponding point_t variable containing the geometry of the vertex,
    which is computed from the primal mesh vertex coordinates.

    v3-----e2-----v2
    |              |
    |              |
    e3     cv     e1
    |              |
    |              |
    v0-----e0-----v1

    The wedge indexing is shown below. A wedge is defined by three vertices in
    the order w = {cv, v*, e*}.

    v3------e2-------v2
    | \      |      / |
    |   \  w6|w5  /   |
    |  w7 \  |  / w4  |
    |       \|/       |
    e3------cv-------e1
    |       /|\       |
    |  w0 /  |  \ w3  |
    |   /  w1|w2  \   |
    | /      |      \ |
    v0------e0-------v1

    Corners are defined by the two wedges sharing the center vertex cv and a
    primal vertex v*.
    c0 = {w0, w1}
    c1 = {w2, w3}
    c2 = {w4, w5}
    c3 = {w6, w7}

    \endverbatim
   */
  void init() {
    mesh_.init<0>();
  } // init

// ATTIC...
#if 0
    std::vector<vertex_t *> dual_vertices;

    size_t poff(0);
    // add primal vertices to dual mesh
    for(auto v: mesh_.vertices<0>()) {
      mesh_.add_vertex<1>(v.entity());
      dual_vertices.push_back(v.entity());
      ++poff;
    } // for

    // create zone center vertices for dual mesh
    size_t coff(0);
    for(auto c: mesh_.cells<0>()) {
      auto vertices = mesh_.vertices(c).to_vec();

      // retrieve points for each of the primal vertices
      const auto v0p = vertices[0]->coordinates();
      const auto v1p = vertices[1]->coordinates();
      const auto v2p = vertices[2]->coordinates();
      const auto v3p = vertices[3]->coordinates();

      // compute the center vertex
      const auto centroid = flexi::centroid({v0p, v1p, v2p, v3p});

      // create the actual dual-mesh vertex
      auto vertex = mesh_.make<vertex_t>(centroid, &state_);
      mesh_.add_vertex<1>(vertex);
      // FIXME: Don't need this
      vertex->set_rank<1>(1);
      dual_vertices.push_back(vertex);
      ++coff;
    } // for

    // create edge center vertices for dual mesh
    for(auto e: mesh_.edges<0>()) {
      auto vertices = mesh_.vertices(e).to_vec();

      // retrieve points of edge
      auto v0p = vertices[0]->coordinates();
      auto v1p = vertices[1]->coordinates();

      // compute the midpoint vertex
      auto midpoint = flexi::midpoint(v0p, v1p);

      // create the actual dual-mesh vertex
      auto vertex = mesh_.make<vertex_t>(midpoint, &state_);
      mesh_.add_vertex<1>(vertex);
      // FIXME: Don't need this
      vertex->set_rank<1>(2);
      dual_vertices.push_back(vertex);
    } // for

    // create wedges and corners
//    auto vertices = mesh_.vertices<1>().to_vec();
    for(auto c: mesh_.cells<0>()) {

      auto edges = mesh_.edges(c).to_vec();
      size_t cnt(edges.size());
      size_t tail(cnt-1);
      size_t head(0);

      for(auto v: mesh_.vertices(c)) {
        auto p = dual_vertices[v.id()];
        auto z = dual_vertices[poff + c.id()];
        auto etail = dual_vertices[poff + coff + tail];
        auto ehead = dual_vertices[poff + coff + head];

        // make tail wedge
        wedge_t * w = mesh_.make<wedge_t>();
        mesh_.add_cell<1>(w);
        mesh_.init_cell<1>(w, {p, z, etail});

        // make head wedge
        w = mesh_.make<wedge_t>();
        mesh_.add_cell<1>(w);
        mesh_.init_cell<1>(w, {p, z, ehead});

        // add corner

        // advance indices
        tail = (tail+1)%cnt;
        ++head;
      } // for
    } // for

    mesh_.init<1>();
#endif
  /*!
    Get the centroid of a cell.
    
    \param[in] cell The cell to return the centroid for.
    \return a point_t that is the centroid.

    FIXME : This doesn't do anything.
    FIXME : need const iterator for entity group!!!
    point_t centroid(const cell_t *c) const {

  */
  point_t centroid(cell_t *c) {
    return {0,0};
  }

  /*!
    Compute the midpoint of an edge.

    \param[in] cell The edge to return the midpoint for.
    \return a point_t that is the midpoint.
  */
  point_t midpoint(const edge_t *e) const {
    auto vs = mesh_.entities<0,0>(e).to_vec();
    return point_t(flexi::midpoint(vs[0]->coordinates(),vs[1]->coordinates()));
  }

private:

  private_mesh_t mesh_;

  burton_mesh_traits_t::mesh_state_t state_;

}; // class burton_mesh_t

using mesh_t = burton_mesh_t;

} // namespace flexi

#endif // flexi_burton_mesh_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/