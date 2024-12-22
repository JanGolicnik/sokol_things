#include "lib/marrow/marrow.h"

#define SOKOL_IMPL
#define SOKOL_GLCORE
#include "lib/sokol/sokol_app.h"
#include "lib/sokol/sokol_gfx.h"
#include "lib/sokol/sokol_glue.h"
#include "lib/sokol/sokol_log.h"
#include "triangle.h"

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

void init(void) {
    sg_setup(&(sg_desc) {
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    float vertices[] = {
        0.0f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   1.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,   0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,   1.0f,
    };
    state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc) {
        .data = SG_RANGE(vertices),
        .label = "triangle-vertices",
    });

    sg_shader shd = sg_make_shader(triangle_shader_desc(sg_query_backend()));

    state.pip = sg_make_pipeline(&(sg_pipeline_desc) {
        .shader = shd,
        .layout = {
            .attrs = {
                [ATTR_triangle_position].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_triangle_color0].format = SG_VERTEXFORMAT_FLOAT4,
                [ATTR_triangle_brightness0].format = SG_VERTEXFORMAT_FLOAT,
            },
        },
        .label = "triangle-pipeline"
    });

    state.pass_action = (sg_pass_action) {
        .colors[0] = { .load_action = SG_LOADACTION_CLEAR, .clear_value = {1.0f, 0.0f, 0.0f, 1.0f} },
    };
}

void frame(void) {
    float g = state.pass_action.colors[0].clear_value.g + 0.01f;
    state.pass_action.colors[0].clear_value.g = (g > 1.0f) ? 0.0f : g;

    sg_begin_pass(&(sg_pass){ .action = state.pass_action, .swapchain = sglue_swapchain() });
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&state.bind);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sg_shutdown();
}

sapp_desc sokol_main(i32 argc, char* argv[]) {
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = 1000,
        .height = 1000,
        .window_title = "brum :D",
        .icon.sokol_default = true,
    };
}
