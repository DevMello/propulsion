#include "../momentum_app.h"

enum VarItemListIndex {
    VarItemListIndexDolphinLevel,
    VarItemListIndexDolphinAngry,
    VarItemListIndexButthurtTimer,
};

void momentum_app_scene_misc_dolphin_var_item_list_callback(void* context, uint32_t index) {
    MomentumApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static void momentum_app_scene_misc_dolphin_dolphin_level_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    app->dolphin_level = variable_item_get_current_value_index(item) + 1;
    char level_str[4];
    snprintf(level_str, 4, "%li", app->dolphin_level);
    variable_item_set_current_value_text(item, level_str);
    app->save_level = true;
    app->save_dolphin = true;
}

static void momentum_app_scene_misc_dolphin_dolphin_angry_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    app->dolphin_angry = variable_item_get_current_value_index(item);
    char angry_str[4];
    snprintf(angry_str, 4, "%li", app->dolphin_angry);
    variable_item_set_current_value_text(item, angry_str);
    app->save_angry = true;
    app->save_dolphin = true;
}

const char* const butthurt_timer_names[] = {
    "OFF",
    "30 M",
    "1 H",
    "2 H",
    "4 H",
    "6 H",
    "8 H",
    "12 H",
    "24 H",
    "48 H",
};
const uint32_t butthurt_timer_values[COUNT_OF(butthurt_timer_names)] = {
    0,
    1800,
    3600,
    7200,
    14400,
    21600,
    28800,
    43200,
    86400,
    172800,
};
static void momentum_app_scene_misc_dolphin_butthurt_timer_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, butthurt_timer_names[index]);
    momentum_settings.butthurt_timer = butthurt_timer_values[index];
    app->save_settings = true;
    app->save_dolphin = true;
}

void momentum_app_scene_misc_dolphin_on_enter(void* context) {
    MomentumApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;
    uint8_t value_index;

    char level_str[4];
    snprintf(level_str, 4, "%li", app->dolphin_level);
    item = variable_item_list_add(
        var_item_list,
        "Dolphin Level",
        DOLPHIN_LEVEL_COUNT + 1,
        momentum_app_scene_misc_dolphin_dolphin_level_changed,
        app);
    variable_item_set_current_value_index(item, app->dolphin_level - 1);
    variable_item_set_current_value_text(item, level_str);

    char angry_str[4];
    snprintf(angry_str, 4, "%li", app->dolphin_angry);
    item = variable_item_list_add(
        var_item_list,
        "Dolphin Angry",
        BUTTHURT_MAX + 1,
        momentum_app_scene_misc_dolphin_dolphin_angry_changed,
        app);
    variable_item_set_current_value_index(item, app->dolphin_angry);
    variable_item_set_current_value_text(item, angry_str);

    item = variable_item_list_add(
        var_item_list,
        "Butthurt Timer",
        COUNT_OF(butthurt_timer_names),
        momentum_app_scene_misc_dolphin_butthurt_timer_changed,
        app);
    value_index = value_index_uint32(
        momentum_settings.butthurt_timer, butthurt_timer_values, COUNT_OF(butthurt_timer_values));
    variable_item_set_current_value_index(item, value_index);
    variable_item_set_current_value_text(item, butthurt_timer_names[value_index]);

    variable_item_list_set_enter_callback(
        var_item_list, momentum_app_scene_misc_dolphin_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, MomentumAppSceneMiscDolphin));

    view_dispatcher_switch_to_view(app->view_dispatcher, MomentumAppViewVarItemList);
}

bool momentum_app_scene_misc_dolphin_on_event(void* context, SceneManagerEvent event) {
    MomentumApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(
            app->scene_manager, MomentumAppSceneMiscDolphin, event.event);
        consumed = true;
        switch(event.event) {
        default:
            break;
        }
    }

    return consumed;
}

void momentum_app_scene_misc_dolphin_on_exit(void* context) {
    MomentumApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
