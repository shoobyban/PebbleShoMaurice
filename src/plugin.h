#pragma once
#ifndef PLUGIN_H
#define PLUGIN_H

typedef int (*void_fn)( void );
typedef int (*load_fn)( Layer*, GRect );
typedef struct plugin_t {
    load_fn load;
    void_fn redraw;
    void_fn unload;
} plugin_t;

void register_plugin(load_fn load,void_fn redraw,void_fn unload);
void call_plugins_redraw();
void call_plugins_unload();

#endif /* PLUGIN_H */
