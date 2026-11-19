#include "swift_mvp.h"

#include <cgraph/cgraph.h>
#include <stdio.h>
#include <string.h>

static int check_render(const char *engine, const char *format,
                        const char *signature, size_t signature_size) {
    GVC_t *ctx = gv_bazel_swift_mvp_context(0);
    Agraph_t *graph = agmemread("digraph { a -> b; b -> c; a -> c }");
    char *data = NULL;
    size_t size = 0;
    int success = 0;

    if (ctx && graph && gvLayout(ctx, graph, engine) == 0) {
        success = gvRenderData(ctx, graph, format, &data, &size) == 0 &&
                  data && size >= signature_size &&
                  memcmp(data, signature, signature_size) == 0;
        gvFreeRenderData(data);
        gvFreeLayout(ctx, graph);
    }
    if (graph)
        agclose(graph);
    if (ctx && gvFreeContext(ctx) != 0)
        success = 0;
    if (!success)
        fprintf(stderr, "Graphviz failed to render %s as %s\n", engine, format);
    return success;
}

int main(void) {
    if (!check_render("sfdp", "svg", "<?xml", 5))
        return 1;
#ifdef __APPLE__
    if (!check_render("dot", "png", "\x89PNG\r\n\x1a\n", 8))
        return 1;
#endif
    return 0;
}
