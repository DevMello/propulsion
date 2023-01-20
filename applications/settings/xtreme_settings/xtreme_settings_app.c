#include "xtreme_settings_app.h"

static bool xtreme_settings_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    XtremeSettingsApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

void xtreme_settings_reboot(void* context) {
    UNUSED(context);
    power_reboot(PowerBootModeNormal);
}

static bool xtreme_settings_back_event_callback(void* context) {
    furi_assert(context);
    XtremeSettingsApp* app = context;
    if (app->settings_changed) {
        XTREME_SETTINGS_SAVE();
        if (app->assets_changed) {
            popup_set_header(app->popup, "Rebooting...", 64, 24, AlignCenter, AlignCenter);
            popup_set_text(app->popup, "Swapping assets...", 64, 42, AlignCenter, AlignCenter);
            popup_set_callback(app->popup, xtreme_settings_reboot);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 1000);
            popup_enable_timeout(app->popup);
            view_dispatcher_switch_to_view(app->view_dispatcher, XtremeSettingsAppViewPopup);
            return true;
        }
    }
    return scene_manager_handle_back_event(app->scene_manager);
}

XtremeSettingsApp* xtreme_settings_app_alloc() {
    XtremeSettingsApp* app = malloc(sizeof(XtremeSettingsApp));
    app->gui = furi_record_open(RECORD_GUI);

    // View Dispatcher and Scene Manager
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&xtreme_settings_scene_handlers, app);
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, xtreme_settings_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, xtreme_settings_back_event_callback);

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Gui Modules
    app->var_item_list = variable_item_list_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        XtremeSettingsAppViewVarItemList,
        variable_item_list_get_view(app->var_item_list));

    app->popup = popup_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        XtremeSettingsAppViewPopup,
        popup_get_view(app->popup));

    // Set first scene
    scene_manager_next_scene(app->scene_manager, XtremeSettingsAppSceneStart);
    return app;
}

void xtreme_settings_app_free(XtremeSettingsApp* app) {
    furi_assert(app);

    // Gui modules
    view_dispatcher_remove_view(app->view_dispatcher, XtremeSettingsAppViewVarItemList);
    variable_item_list_free(app->var_item_list);
    view_dispatcher_remove_view(app->view_dispatcher, XtremeSettingsAppViewPopup);
    popup_free(app->popup);

    // View Dispatcher and Scene Manager
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    // Records
    furi_record_close(RECORD_GUI);
    free(app);
}

extern int32_t xtreme_settings_app(void* p) {
    UNUSED(p);
    XtremeSettingsApp* app = xtreme_settings_app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    xtreme_settings_app_free(app);
    return 0;
}
