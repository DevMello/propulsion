#include "../subghz_i.h"
#include "../helpers/subghz_custom_event.h"

void subghz_scene_delete_raw_callback(GuiButtonType result, InputType type, void* context) {
    furi_assert(context);
    SubGhz* subghz = context;
    if((result == GuiButtonTypeRight) && (type == InputTypeShort)) {
        view_dispatcher_send_custom_event(
            subghz->view_dispatcher, SubGhzCustomEventSceneDeleteRAW);
    } else if((result == GuiButtonTypeLeft) && (type == InputTypeShort)) {
        view_dispatcher_send_custom_event(
            subghz->view_dispatcher, SubGhzCustomEventSceneDeleteRAWBack);
    }
}

void subghz_scene_delete_raw_on_enter(void* context) {
    SubGhz* subghz = context;
    FuriString* frequency_str;
    FuriString* modulation_str;
    FuriString* text_out;
    FuriString* file_name;
    text_out = furi_string_alloc();
    file_name = furi_string_alloc();

    path_extract_filename(subghz->file_path, file_name, true);
    furi_string_cat_printf(
        text_out, "\e#Delete %s?\e#\nRAW signal\n", furi_string_get_cstr(file_name));
    furi_string_free(file_name);

    frequency_str = furi_string_alloc();
    modulation_str = furi_string_alloc();
    subghz_txrx_get_frequency_and_modulation(subghz->txrx, frequency_str, modulation_str);

    furi_string_cat_printf(
        text_out,
        "%s %s",
        furi_string_get_cstr(frequency_str),
        furi_string_get_cstr(modulation_str));

    widget_add_text_box_element(
        subghz->widget, 0, 0, 128, 54, AlignCenter, AlignTop, furi_string_get_cstr(text_out), false);

    furi_string_free(frequency_str);
    furi_string_free(modulation_str);
    furi_string_free(text_out);

    widget_add_button_element(
        subghz->widget, GuiButtonTypeRight, "Delete", subghz_scene_delete_raw_callback, subghz);
    widget_add_button_element(
        subghz->widget, GuiButtonTypeLeft, "Cancel", subghz_scene_delete_raw_callback, subghz);

    view_dispatcher_switch_to_view(subghz->view_dispatcher, SubGhzViewIdWidget);
}

bool subghz_scene_delete_raw_on_event(void* context, SceneManagerEvent event) {
    SubGhz* subghz = context;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubGhzCustomEventSceneDeleteRAW) {
            furi_string_set(subghz->file_path_tmp, subghz->file_path);
            if(subghz_delete_file(subghz)) {
                if(subghz_rx_key_state_get(subghz) != SubGhzRxKeyStateRAWLoad) {
                    subghz_rx_key_state_set(subghz, SubGhzRxKeyStateIDLE);
                    scene_manager_next_scene(subghz->scene_manager, SubGhzSceneDeleteSuccess);
                } else {
                    scene_manager_next_scene(subghz->scene_manager, SubGhzSceneSaved);
                }

            } else {
                scene_manager_search_and_switch_to_previous_scene(
                    subghz->scene_manager, SubGhzSceneStart);
            }
            return true;
        } else if(event.event == SubGhzCustomEventSceneDeleteRAWBack) {
            return scene_manager_previous_scene(subghz->scene_manager);
        }
    }
    return false;
}

void subghz_scene_delete_raw_on_exit(void* context) {
    SubGhz* subghz = context;
    widget_reset(subghz->widget);
}
