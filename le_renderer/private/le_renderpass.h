#ifndef GUARD_LE_RENDERPASS_H
#define GUARD_LE_RENDERPASS_H

#include "le_renderer/le_renderer.h"
#include <vector>
// ----------------------------------------------------------------------

#define LE_RENDERPASS_MARKER_EXTERNAL "rp-external"

#ifdef __cplusplus
extern "C" {
#endif

void register_le_renderpass_api( void *api_ );

#ifdef __cplusplus
}
#endif

/*

  TODO: look if you can make image_attachment_info_o a pointer - created and owned by the renderer,
  so that we can reduce the size of this object, and so that we can remove the headers upon we de-
  pend currently;

*/

struct le_renderpass_o {

	le::RenderPassType                                    type = le::RenderPassType::eUndefined;
	uint64_t                                              id;
	
	bool isRoot = false; // whether pass *must* be processed
	
	uint64_t                                              execution_order = 0;
	std::vector<le_renderer_api::image_attachment_info_o> imageAttachments;

	uint64_t readResources[32];
	uint64_t writeResources[32];

	uint32_t readResourceCount  = 0;
	uint32_t writeResourceCount = 0;

	struct ColorAttachmentInfo* colorAttachmentInfo[8];

	le_renderer_api::pfn_renderpass_setup_t   callbackSetup              = nullptr;
	le_renderer_api::pfn_renderpass_execute_t callbackExecute            = nullptr;
	void *                                    execute_callback_user_data = nullptr;

	char debugName[ 32 ];
};

#endif //GUARD_LE_RENDERPASS_H
