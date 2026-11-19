#include "builtin_dot_core.h"

#include <gvc/gvplugin.h>

extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;

#ifdef __APPLE__
/* Quartz renderer (png, jpeg, pdf, ...) without its CoreText textlayout, so
 * node sizes and with them SVG/DOT/JSON output stay identical to builds
 * without quartz. gvrender_quartz creates its own CoreText layout per span. */
extern gvplugin_installed_t gvrender_quartz_types;
extern gvplugin_installed_t gvloadimage_quartz_types;
extern gvplugin_installed_t gvdevice_quartz_types;

static gvplugin_api_t gv_bazel_quartz_apis[] = {
    {API_render, &gvrender_quartz_types},
    {API_loadimage, &gvloadimage_quartz_types},
    {API_device, &gvdevice_quartz_types},
    {(api_t)0, 0},
};

static gvplugin_library_t gv_bazel_quartz_library = {"quartz",
                                                     gv_bazel_quartz_apis};
#endif

const lt_symlist_t gv_bazel_builtin_dot_core[] = {
    {"gvplugin_dot_layout_LTX_library", &gvplugin_dot_layout_LTX_library},
    {"gvplugin_neato_layout_LTX_library", &gvplugin_neato_layout_LTX_library},
    {"gvplugin_core_LTX_library", &gvplugin_core_LTX_library},
#ifdef __APPLE__
    {"gvplugin_quartz_LTX_library", &gv_bazel_quartz_library},
#endif
    {0, 0},
};

const lt_symlist_t *gv_bazel_dot_core_builtins(void) {
    return gv_bazel_builtin_dot_core;
}
