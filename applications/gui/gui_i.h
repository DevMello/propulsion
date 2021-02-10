#pragma once

#include "gui.h"

#include <furi.h>
#include <m-array.h>
#include <stdio.h>

#include "canvas.h"
#include "canvas_i.h"
#include "view_port.h"
#include "view_port_i.h"

#define GUI_DISPLAY_WIDTH 128
#define GUI_DISPLAY_HEIGHT 64

#define GUI_STATUS_BAR_X 0
#define GUI_STATUS_BAR_Y 0
#define GUI_STATUS_BAR_WIDTH GUI_DISPLAY_WIDTH
#define GUI_STATUS_BAR_HEIGHT 8

#define GUI_MAIN_X 0
#define GUI_MAIN_Y 9
#define GUI_MAIN_WIDTH GUI_DISPLAY_WIDTH
#define GUI_MAIN_HEIGHT (GUI_DISPLAY_HEIGHT - GUI_MAIN_Y)

#define GUI_THREAD_FLAG_DRAW (1 << 0)
#define GUI_THREAD_FLAG_INPUT (1 << 1)
#define GUI_THREAD_FLAG_ALL (GUI_THREAD_FLAG_DRAW | GUI_THREAD_FLAG_INPUT)

ARRAY_DEF(ViewPortArray, ViewPort*, M_PTR_OPLIST);

struct Gui {
    // Thread and lock
    osThreadId_t thread;
    osMutexAttr_t mutex_attr;
    osMutexId_t mutex;
    // Layers and Canvas
    ViewPortArray_t layers[GuiLayerMAX];
    Canvas* canvas;
    // Input
    osMessageQueueId_t input_queue;
    PubSub* input_events;
};

ViewPort* gui_view_port_find_enabled(ViewPortArray_t array);

/* Update GUI, request redraw
 * Real redraw event will be issued only if view_port is currently visible
 * Setting view_port to NULL forces redraw, but must be avoided
 * @param gui, Gui instance
 * @param view_port, ViewPort instance or NULL
 */
void gui_update(Gui* gui, ViewPort* view_port);

void gui_input_events_callback(const void* value, void* ctx);

void gui_lock(Gui* gui);

void gui_unlock(Gui* gui);