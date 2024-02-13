#ifndef GUARD_le_mesh_H
#define GUARD_le_mesh_H

#include "le_core.h"

struct le_mesh_o;

/*

  A modern mesh API:

  + we want the mesh to be able to draw itself
  + we want a mesh to be able to optimize itself

  + we want to have a pure-cpu mesh as well as a mesh that exists on the gpu.
  + how should we draw a mesh?




*/

// clang-format off
struct le_mesh_api {

    typedef uint16_t default_index_type;
    typedef float default_vertex_type[3];
    typedef float default_uv_type[2];

	struct le_mesh_interface_t {

		le_mesh_o *    ( * create                   ) ( );
		void           ( * destroy                  ) ( le_mesh_o* self );

		void (*clear)(le_mesh_o* self);


		// I would like to change the api so that the mesh will copy into a given pointer
		// (bounded by size or num_bytes)

		void (*get_vertices )( le_mesh_o *self, size_t* count, float const **   vertices); 	// 3 floats per vertex
		void (*get_normals  )( le_mesh_o *self, size_t* count, float const **   normals ); 	// 3 floats per vertex
		void (*get_colours  )( le_mesh_o *self, size_t* count, float const **   colours ); 	// 4 floats per vertex
		void (*get_uvs      )( le_mesh_o *self, size_t* count, float const **   uvs     ); 	// 3 floats per vertex
		void (*get_tangents )( le_mesh_o *self, size_t* count, float const **   tangents); 	// 3 floats per vertex
		void (*get_indices  )( le_mesh_o *self, size_t* count, uint16_t const ** indices ); // 1 uint16_t per index

		void (*get_data     )( le_mesh_o *self, size_t* numVertices, size_t* numIndices, float const** vertices, float const **normals, float const **uvs, float const  ** colours, uint16_t const **indices);

		// New API

		void (*write_into_vertices )( le_mesh_o *self, float * const vertices, size_t * num_bytes);

		// PLY import

		bool (*load_from_ply_file)( le_mesh_o *self, char const *file_path );

	};

	le_mesh_interface_t       le_mesh_i;
};
// clang-format on
LE_MODULE( le_mesh );
LE_MODULE_LOAD_DEFAULT( le_mesh );

#ifdef __cplusplus

namespace le_mesh {
const auto         api       = le_mesh_api_i;
static const auto& le_mesh_i = api->le_mesh_i;
} // namespace le_mesh

class LeMesh : NoCopy, NoMove {
#	ifndef this_i
#		define this_i le_mesh::le_mesh_i

	le_mesh_o* self;

  public:
    LeMesh()
	    : self( this_i.create() ) {
	}

	~LeMesh() {
		this_i.destroy( self );
	}

	void clear() {
		this_i.clear( self );
	}

	void getVertices( size_t* count, float const** pVertices = nullptr ) {
		this_i.get_vertices( self, count, pVertices );
	}

	void getTangents( size_t* count, float const** pTangents = nullptr ) {
		this_i.get_tangents( self, count, pTangents );
	}

	void getColours( size_t* count, float const** pColours = nullptr ) {
		this_i.get_colours( self, count, pColours );
	}

	void getNormals( size_t* count, float const** pNormals = nullptr ) {
		this_i.get_vertices( self, count, pNormals );
	}

	void getUvs( size_t* count, float const** pUvs = nullptr ) {
		this_i.get_uvs( self, count, pUvs );
	}

	void getIndices( size_t* count, uint16_t const** pIndices = nullptr ) {
		this_i.get_indices( self, count, pIndices );
	}

	void getData( size_t* numVertices, size_t* numIndices, float const** pVertices = nullptr, float const** pNormals = nullptr, float const** pUvs = nullptr, float const** pColours = nullptr, uint16_t const** pIndices = nullptr ) {
		this_i.get_data( self, numVertices, numIndices, pVertices, pNormals, pUvs, pColours, pIndices );
	}

	bool loadFromPlyFile( char const* file_path ) {
		return this_i.load_from_ply_file( self, file_path );
	}

	operator auto() {
		return self;
	}
#		undef this_i
#	endif
};

namespace le {
using Mesh = LeMesh;
}

#endif // __cplusplus

#endif
