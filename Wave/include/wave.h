#pragma once

/************************* Renderer ********************************/
#include <Renderer/renderer.h>
#include <Renderer/shader.h>

/************************* Lighting ********************************/
#include <Lighting/light.h>

/************************* Core ********************************/
#include <Core/core.h>
#include <Core/interfaces.h>
#include <Layer/layer_stack.h>
#include <Layer/text_layer.h>
#include <Core/settings.h>
#include <Core/engine_time.h>
#include <Core/engine.h>

/************************* Imgui ********************************/
#include <imgui.h>
#include <imgui_internal.h>

/************************* Imguizmo ********************************/
#include <ImGuizmo/ImGuizmo.h>
/************************* Utilities ********************************/
#include <Utilities/logger.h>
#include <Utilities/resource_loader.h>

/************************* Input ********************************/
#include <Input/input.h>
#include <Input/key_codes.h>
#include <Input/mouse_codes.h>

/************************* Math ********************************/
#include <Math/matrix_4f.h>
#include <Math/transform.h>

/************************* Events ********************************/
#include <Events/app_event.h>
#include <Events/key_event.h>
#include <Events/mouse_event.h>
#include <Events/scene_events.h>
#include <Events/renderer_event.h>
#include <Events/event_dispatcher.h>

/************************* Scene ********************************/
#include <Scene/entity.h>
#include <Scene/components.h>
#include <Scene/scene.h>
#include <Scene/scene_camera.h>
#include <ImGUI/scene_panel.h>

/************************* Entities ********************************/
#include <Objects/object.h>
#include <Text/text.h>

/************************* Window ********************************/
#include <Window/window.h>