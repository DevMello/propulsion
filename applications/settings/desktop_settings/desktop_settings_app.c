#include <furi.h>
#include <gui/modules/popup.h>
#include <gui/scene_manager.h>
#include <namechanger/namechanger.h>
#include <flipper_format/flipper_format.h>
#include <power/power_service/power.h>

#include <desktop/desktop.h>
#include <desktop/views/desktop_view_pin_input.h>

#include "desktop_settings_app.h"
#include "scenes/desktop_settings_scene.h"

static bool desktop_settings_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    DesktopSettingsApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool desktop_settings_back_event_callback(void* context) {
    furi_assert(context);
    DesktopSettingsApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

DesktopSettingsApp* desktop_settings_app_alloc(void) {
    DesktopSettingsApp* app = malloc(sizeof(DesktopSettingsApp));

    app->gui = furi_record_open(RECORD_GUI);
    app->dialogs = furi_record_open(RECORD_DIALOGS);
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&desktop_settings_scene_handlers, app);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, desktop_settings_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, desktop_settings_back_event_callback);

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    app->popup = popup_alloc();
    app->submenu = submenu_alloc();
    app->variable_item_list = variable_item_list_alloc();
    app->pin_input_view = desktop_view_pin_input_alloc();
    app->pin_setup_howto_view = desktop_settings_view_pin_setup_howto_alloc();
    app->pin_setup_howto2_view = desktop_settings_view_pin_setup_howto2_alloc();

    view_dispatcher_add_view(
        app->view_dispatcher, DesktopSettingsAppViewMenu, submenu_get_view(app->submenu));
    view_dispatcher_add_view(
        app->view_dispatcher,
        DesktopSettingsAppViewVarItemList,
        variable_item_list_get_view(app->variable_item_list));
    view_dispatcher_add_view(
        app->view_dispatcher, DesktopSettingsAppViewIdPopup, popup_get_view(app->popup));
    view_dispatcher_add_view(
        app->view_dispatcher,
        DesktopSettingsAppViewIdPinInput,
        desktop_view_pin_input_get_view(app->pin_input_view));
    view_dispatcher_add_view(
        app->view_dispatcher,
        DesktopSettingsAppViewIdPinSetupHowto,
        desktop_settings_view_pin_setup_howto_get_view(app->pin_setup_howto_view));
    view_dispatcher_add_view(
        app->view_dispatcher,
        DesktopSettingsAppViewIdPinSetupHowto2,
        desktop_settings_view_pin_setup_howto2_get_view(app->pin_setup_howto2_view));

    // Text Input
    app->text_input = text_input_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        DesktopSettingsAppViewTextInput,
        text_input_get_view(app->text_input));

    return app;
}

void desktop_settings_app_free(DesktopSettingsApp* app) {
    furi_assert(app);

    bool temp_save_name = app->save_name;
    // Save name if set or remove file
    if(temp_save_name) {
        Storage* storage = furi_record_open(RECORD_STORAGE);
        if(strcmp(app->device_name, "") == 0) {
            storage_simply_remove(storage, NAMECHANGER_PATH);
        } else {
            FlipperFormat* file = flipper_format_file_alloc(storage);

            do {
                if(!flipper_format_file_open_always(file, NAMECHANGER_PATH)) break;
                if(!flipper_format_write_header_cstr(file, NAMECHANGER_HEADER, NAMECHANGER_VERSION))
                    break;
                if(!flipper_format_write_string_cstr(file, "Name", app->device_name)) break;
            } while(0);

            flipper_format_free(file);
        }
        furi_record_close(RECORD_STORAGE);
    }

    // Variable item list
    view_dispatcher_remove_view(app->view_dispatcher, DesktopSettingsAppViewMenu);
    view_dispatcher_remove_view(app->view_dispatcher, DesktopSettingsAppViewVarItemList);
    view_dispatcher_remove_view(app->view_dispatcher, DesktopSettingsAppViewIdPopup);
    view_dispatcher_remove_view(app->view_dispatcher, DesktopSettingsAppViewIdPinInput);
    view_dispatcher_remove_view(app->view_dispatcher, DesktopSettingsAppViewIdPinSetupHowto);
    view_dispatcher_remove_view(app->view_dispatcher, DesktopSettingsAppViewIdPinSetupHowto2);
    // TextInput
    view_dispatcher_remove_view(app->view_dispatcher, DesktopSettingsAppViewTextInput);
    text_input_free(app->text_input);

    variable_item_list_free(app->variable_item_list);
    submenu_free(app->submenu);
    popup_free(app->popup);
    desktop_view_pin_input_free(app->pin_input_view);
    desktop_settings_view_pin_setup_howto_free(app->pin_setup_howto_view);
    desktop_settings_view_pin_setup_howto2_free(app->pin_setup_howto2_view);
    // View dispatcher
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);
    // Records
    furi_record_close(RECORD_DIALOGS);
    furi_record_close(RECORD_GUI);
    free(app);

    if(temp_save_name) {
        power_reboot(PowerBootModeNormal);
    }
}

extern int32_t desktop_settings_app(void* p) {
    UNUSED(p);

    DesktopSettingsApp* app = desktop_settings_app_alloc();
    Desktop* desktop = furi_record_open(RECORD_DESKTOP);

    desktop_api_get_settings(desktop, &app->settings);

    scene_manager_next_scene(app->scene_manager, DesktopSettingsAppSceneStart);

    view_dispatcher_run(app->view_dispatcher);

    desktop_api_set_settings(desktop, &app->settings);
    furi_record_close(RECORD_DESKTOP);

    desktop_settings_app_free(app);

    return 0;
}
