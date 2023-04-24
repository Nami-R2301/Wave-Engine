#pragma once

/************************* Core ********************************/
#include <Core/platform_detection.h>
#include <Core/interfaces.h>
#include <Core/core.h>
#include <Core/core_macros.h>
#include <Core/layer_stack.h>
#include <Core/settings.h>
#include <Core/engine_time.h>
#include <Core/engine.h>

/************************* Events ********************************/
#include <Events/app_event.h>
#include <Events/key_event.h>
#include <Events/mouse_event.h>
#include <Events/renderer_event.h>
#include <Events/event_dispatcher.h>

/************************* Utilities ********************************/
#include <Utilities/logger.h>
#include <Utilities/res_loader.h>

/************************* Input ********************************/
#include <Input/input.h>
#include <Input/key_codes.h>
#include <Input/mouse_codes.h>

/************************* Math ********************************/
#include <Math/matrix_4f.h>
#include <Math/transform.h>

/************************* Renderer ********************************/
#include <Renderer/renderer.h>
#include <Renderer/buffer.h>
#include <Renderer/color.h>
#include <Renderer/shader.h>
#include <Renderer/vertex.h>
#include <Renderer/vertex_array_buffer.h>
#include <Renderer/editor_camera.h>
#include <Renderer/gl_framebuffer.h>

/************************* Objects ********************************/
#include <Objects/object.h>
#include <Objects/text.h>

/************************* Window ********************************/
#include <Window/window.h>