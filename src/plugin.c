#include "config.h"

plugin_t plugins[30];
int plugins_num = 0;

void register_plugin(load_fn load,void_fn redraw,void_fn unload) {
    plugin_t plugin;
    plugin.load = load;
    plugin.redraw = redraw;
    plugin.unload = unload;
    plugins[plugins_num++] = plugin;
}

void call_plugins_redraw() {
    for (int i=0;i<plugins_num;i++) {
        plugins[i].redraw();
    }
}

void call_plugins_unload() {
    for (int i=0;i<plugins_num;i++) {
        plugins[i].unload();
    }
}
