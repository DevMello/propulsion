#include "../momentum_app.h"

enum VarItemListIndex {
    VarItemListIndexFlipperName, // TODO: Split into name, mac, serial
};

void momentum_app_scene_misc_spoof_var_item_list_callback(void* context, uint32_t index) {
    MomentumApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void momentum_app_scene_misc_spoof_on_enter(void* context) {
    MomentumApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(var_item_list, "Flipper Name", 0, NULL, app);
    variable_item_set_current_value_text(item, app->device_name);

    variable_item_list_set_enter_callback(
        var_item_list, momentum_app_scene_misc_spoof_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, MomentumAppSceneMiscSpoof));

    view_dispatcher_switch_to_view(app->view_dispatcher, MomentumAppViewVarItemList);
}

bool momentum_app_scene_misc_spoof_on_event(void* context, SceneManagerEvent event) {
    MomentumApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, MomentumAppSceneMiscSpoof, event.event);
        consumed = true;
        switch(event.event) {
        case VarItemListIndexFlipperName:
            scene_manager_next_scene(app->scene_manager, MomentumAppSceneMiscSpoofName);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void momentum_app_scene_misc_spoof_on_exit(void* context) {
    MomentumApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
