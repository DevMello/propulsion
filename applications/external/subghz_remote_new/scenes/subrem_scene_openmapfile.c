#include "../subghz_remote_app_i.h"

void subrem_scene_openmapfile_on_enter(void* context) {
    SubGhzRemoteApp* app = context;

    if(subrem_load_from_file(app)) {
        // if(subghz_get_load_type_file(subghz) == SubGhzLoadTypeFileRaw) {
        //     subghz_rx_key_state_set(subghz, SubGhzRxKeyStateRAWLoad);
        //     scene_manager_next_scene(subghz->scene_manager, SubGhzSceneReadRAW);
        // } else {
        //     scene_manager_next_scene(subghz->scene_manager, SubGhzSceneSavedMenu);
        // }
    } else {
        scene_manager_search_and_switch_to_previous_scene(app->scene_manager, SubRemSceneStart);
    }
}

bool subrem_scene_openmapfile_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void subrem_scene_openmapfile_on_exit(void* context) {
    UNUSED(context);
}
