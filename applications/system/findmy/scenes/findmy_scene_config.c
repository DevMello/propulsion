#include "../findmy_i.h"

enum VarItemListIndex {
    VarItemListIndexBroadcastInterval,
    VarItemListIndexTransmitPower,
    VarItemListIndexRegisterTag,
    VarItemListIndexAbout,
};

void findmy_scene_config_broadcast_interval_changed(VariableItem* item) {
    FindMy* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    findmy_change_broadcast_interval(app, index + 1);
    char str[5];
    snprintf(str, sizeof(str), "%ds", app->broadcast_interval);
    variable_item_set_current_value_text(item, str);
    variable_item_set_current_value_index(item, app->broadcast_interval - 1);
}

void findmy_scene_config_transmit_power_changed(VariableItem* item) {
    FindMy* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    findmy_change_transmit_power(app, index);
    char str[7];
    snprintf(str, sizeof(str), "%ddBm", app->transmit_power);
    variable_item_set_current_value_text(item, str);
    variable_item_set_current_value_index(item, app->transmit_power);
}

void findmy_scene_config_callback(void* context, uint32_t index) {
    furi_assert(context);
    FindMy* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void findmy_scene_config_on_enter(void* context) {
    FindMy* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(
        var_item_list,
        "Broadcast Interval",
        10,
        findmy_scene_config_broadcast_interval_changed,
        app);
    // Broadcast Interval is 1-10, so use 0-9 and offset indexes by 1
    variable_item_set_current_value_index(item, app->broadcast_interval - 1);
    char broadcast_interval_s[5];
    snprintf(broadcast_interval_s, sizeof(broadcast_interval_s), "%ds", app->broadcast_interval);
    variable_item_set_current_value_text(item, broadcast_interval_s);

    item = variable_item_list_add(
        var_item_list, "Transmit Power", 7, findmy_scene_config_transmit_power_changed, app);
    variable_item_set_current_value_index(item, app->transmit_power);
    char transmit_power_s[7];
    snprintf(transmit_power_s, sizeof(transmit_power_s), "%ddBm", app->transmit_power);
    variable_item_set_current_value_text(item, transmit_power_s);

    item = variable_item_list_add(var_item_list, "Register Tag", 0, NULL, NULL);
    item = variable_item_list_add(var_item_list, "Matthew KuKanich, Thanks to Chapoly1305, WillyJL, OpenHaystack, Testers", 1, NULL, NULL);
    variable_item_set_current_value_text(item, "Credits");

    variable_item_list_set_enter_callback(var_item_list, findmy_scene_config_callback, app);

    variable_item_list_set_selected_item(
        var_item_list, scene_manager_get_scene_state(app->scene_manager, FindMySceneConfig));

    view_dispatcher_switch_to_view(app->view_dispatcher, FindMyViewVarItemList);
}

bool findmy_scene_config_on_event(void* context, SceneManagerEvent event) {
    FindMy* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, FindMySceneConfig, event.event);
        consumed = true;
        switch(event.event) {
        case VarItemListIndexRegisterTag:
            scene_manager_next_scene(app->scene_manager, FindMySceneConfigMac);
            break;
        case VarItemListIndexAbout:
            break;
        default:
            break;
        }
    }

    return consumed;
}

void findmy_scene_config_on_exit(void* context) {
    FindMy* app = context;
    VariableItemList* var_item_list = app->var_item_list;

    variable_item_list_reset(var_item_list);
}