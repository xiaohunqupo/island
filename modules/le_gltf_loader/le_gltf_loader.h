#ifndef GUARD_LE_GLTF_LOADER_H
#define GUARD_LE_GLTF_LOADER_H

#include <stdint.h>
#include "le_core/le_core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct le_gltf_document_o;
struct le_renderer_o;
struct le_command_buffer_encoder_o;
struct le_resource_info_t;
struct le_pipeline_manager_o;

struct GltfUboMvp;

struct le_resource_handle_t;

void register_le_gltf_loader_api( void *api );

// clang-format off
struct le_gltf_loader_api {
	static constexpr auto id      = "le_gltf_loader";
	static constexpr auto pRegFun = register_le_gltf_loader_api;

	struct gltf_document_interface_t {

		le_gltf_document_o * ( * create                   ) ( );
		void                 ( * destroy                  ) ( le_gltf_document_o* self );

		bool                 ( *load_from_text            ) ( le_gltf_document_o* self, char const * path);
		void                 ( *setup_resources           ) ( le_gltf_document_o *self, le_renderer_o *renderer, le_pipeline_manager_o* pipeline_manager );
		void                 ( *get_resource_infos        ) ( le_gltf_document_o *self, le_resource_info_t **infos, le_resource_handle_t const **handles, size_t *numResources );
		void                 ( *upload_resource_data      ) ( le_gltf_document_o *self, le_command_buffer_encoder_o *encoder );
		void                 ( *draw                      ) ( le_gltf_document_o *self, le_command_buffer_encoder_o *encoder,  GltfUboMvp const * mvp );
	};

	gltf_document_interface_t       document_i;
};
// clang-format on

#ifdef __cplusplus
} // extern c

namespace le_gltf_loader {
#	ifdef PLUGINS_DYNAMIC
const auto api = Registry::addApiDynamic<le_gltf_loader_api>( true );
#	else
const auto api = Registry::addApiStatic<le_gltf_loader_api>();
#	endif

static const auto &gltf_document_i = api -> document_i;

} // namespace le_gltf_loader

#endif

#endif
