#ifndef GUARD_LE_SWAPCHAIN_VK_H
#define GUARD_LE_SWAPCHAIN_VK_H

#include <stdint.h>
#include "le_core.h"

struct le_swapchain_o;
struct le_backend_o;

struct VkDevice_T;
struct VkPhysicalDevice_T;
struct VkSurfaceKHR_T;
struct VkSemaphore_T;
struct VkImage_T;
struct VkImageView_T;
struct VkQueue_T;
struct VkSurfaceFormatKHR;

struct le_swapchain_settings_t {
	enum Type : uint32_t {
		LE_SWAPCHAIN_UNDEFINED = 0,
		LE_KHR_SWAPCHAIN       = 1,
		LE_DIRECT_SWAPCHAIN,
		LE_IMG_SWAPCHAIN,
	};
	Type                     type            = LE_SWAPCHAIN_UNDEFINED;
	uint32_t                 imagecount_hint = 3;
	le_swapchain_settings_t* p_next          = nullptr;
};

struct le_swapchain_vk_api {

	// clang-format off

	struct swapchain_interface_t {
		le_swapchain_o *          ( *create                   ) ( le_backend_o* backend, const le_swapchain_settings_t* settings );
		le_swapchain_o *          ( *create_from_old_swapchain) ( le_swapchain_o* old_swapchain);

		void                      ( *destroy                  ) ( le_swapchain_o* self );

		bool                      ( *present                  ) ( le_swapchain_o* self, VkQueue_T* queue, VkSemaphore_T* renderCompleteSemaphore, uint32_t* pImageIndex);
		bool                      ( *acquire_next_image       ) ( le_swapchain_o* self, VkSemaphore_T* semaphore_, uint32_t* pImageIndex_ );
		VkSurfaceFormatKHR*       ( *get_surface_format       ) ( le_swapchain_o* self );

		VkImage_T*                ( *get_image                ) ( le_swapchain_o* self, uint32_t index_);
		uint32_t                  ( *get_image_width          ) ( le_swapchain_o* self );
		uint32_t                  ( *get_image_height         ) ( le_swapchain_o* self );
		size_t                    ( *get_image_count          ) ( le_swapchain_o* self );
		

		//
		// todo: we should rename this to request_backend_settings
		//
		//
		// we then request both the instance extensions and the device extensions
		// but we also set the data frame count based on what the settings say ...
		// we will have to test how to make sure that we don't request too many data frames.

		bool                      ( *request_backend_capabilities )(const le_swapchain_settings_t* settings);

		le_swapchain_settings_t* ( *settings_create   ) ( le_swapchain_settings_t::Type type );
		le_swapchain_settings_t* ( *settings_clone    ) ( le_swapchain_settings_t const * src_settings );
		void                     ( *settings_destroy  ) ( le_swapchain_settings_t* settings );

	};

	struct swapchain_ref_count_inferface_t {
		void (*inc_ref)(le_swapchain_o*);
		void (*dec_ref)(le_swapchain_o*);
	};

	// clang-format on

	swapchain_interface_t swapchain_i;        // base (public) interface, forwards to either:
	swapchain_interface_t swapchain_khr_i;    // (private) khr swapchain interface
	swapchain_interface_t swapchain_img_i;    // (private) image swapchain interface
	swapchain_interface_t swapchain_direct_i; // (private) direct mode swapchain interface

	swapchain_ref_count_inferface_t swapchain_ref_i; // reference count interface
};

LE_MODULE( le_swapchain_vk );
LE_MODULE_LOAD_DEFAULT( le_swapchain_vk );

#ifdef __cplusplus

namespace le_swapchain_vk {

static const auto& api             = le_swapchain_vk_api_i;
static const auto& swapchain_i     = api->swapchain_i;
static const auto& swapchain_ref_i = api->swapchain_ref_i;

} // namespace le_swapchain_vk

#endif // __cplusplus
#endif // GUARD_PAL_BACKEND_VK_H
