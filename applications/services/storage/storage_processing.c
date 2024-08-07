#include "storage_processing.h"
#include <m-list.h>
#include <m-dict.h>

#define TAG "Storage"

#define STORAGE_PATH_PREFIX_LEN 4u
_Static_assert(
    sizeof(STORAGE_ANY_PATH_PREFIX) == STORAGE_PATH_PREFIX_LEN + 1,
    "Any path prefix len mismatch");
_Static_assert(
    sizeof(STORAGE_EXT_PATH_PREFIX) == STORAGE_PATH_PREFIX_LEN + 1,
    "Ext path prefix len mismatch");
_Static_assert(
    sizeof(STORAGE_INT_PATH_PREFIX) == STORAGE_PATH_PREFIX_LEN + 1,
    "Int path prefix len mismatch");

#define FS_CALL(_storage, _fn) ret = _storage->fs_api->_fn;

static bool storage_type_is_valid(StorageType type) {
#ifdef FURI_RAM_EXEC
    return type == ST_EXT;
#else
    return type < ST_ERROR;
#endif
}

static StorageData* get_storage_by_file(File* file, StorageData* storages) {
    StorageData* storage_data = NULL;

    for(uint8_t i = 0; i < STORAGE_COUNT; i++) {
        if(storage_has_file(file, &storages[i])) {
            storage_data = &storages[i];
        }
    }

    return storage_data;
}

static const char* cstr_path_without_vfs_prefix(FuriString* path) {
    const char* path_cstr = furi_string_get_cstr(path);
    return path_cstr + MIN(STORAGE_PATH_PREFIX_LEN, strlen(path_cstr));
}

static StorageType storage_get_type_by_path(FuriString* path) {
    StorageType type = ST_ERROR;
    const char* path_cstr = furi_string_get_cstr(path);

    if(furi_string_size(path) > STORAGE_PATH_PREFIX_LEN) {
        if(path_cstr[STORAGE_PATH_PREFIX_LEN] != '/') {
            return ST_ERROR;
        }
    }

    if(memcmp(path_cstr, STORAGE_EXT_PATH_PREFIX, strlen(STORAGE_EXT_PATH_PREFIX)) == 0) {
        type = ST_EXT;
    } else if(memcmp(path_cstr, STORAGE_INT_PATH_PREFIX, strlen(STORAGE_INT_PATH_PREFIX)) == 0) {
        type = ST_INT;
    } else if(memcmp(path_cstr, STORAGE_MNT_PATH_PREFIX, strlen(STORAGE_MNT_PATH_PREFIX)) == 0) {
        type = ST_MNT;
    } else if(memcmp(path_cstr, STORAGE_ANY_PATH_PREFIX, strlen(STORAGE_ANY_PATH_PREFIX)) == 0) {
        type = ST_ANY;
    }

    return type;
}

FS_Error storage_get_data(Storage* app, FuriString* path, StorageData** storage) {
    StorageType type = storage_get_type_by_path(path);

    if(storage_type_is_valid(type)) {
        // Any storage phase-out: redirect "/any" to "/ext"
        if(type == ST_ANY) {
            FURI_LOG_W(
                TAG,
                STORAGE_ANY_PATH_PREFIX " is deprecated, use " STORAGE_EXT_PATH_PREFIX " instead");
            furi_string_replace_at(
                path, 0, strlen(STORAGE_EXT_PATH_PREFIX), STORAGE_EXT_PATH_PREFIX);
            type = ST_EXT;
        }

        furi_assert(type == ST_EXT || type == ST_MNT);

        if(storage_data_status(&app->storage[type]) != StorageStatusOK) {
            return FSE_NOT_READY;
        }

        *storage = &app->storage[type];

        return FSE_OK;
    } else {
        return FSE_INVALID_NAME;
    }
}

static void storage_path_trim_trailing_slashes(FuriString* path) {
    while(furi_string_end_with(path, "/")) {
        furi_string_left(path, furi_string_size(path) - 1);
    }
}

/******************* File Functions *******************/

bool storage_process_file_open(
    Storage* app,
    File* file,
    FuriString* path,
    FS_AccessMode access_mode,
    FS_OpenMode open_mode) {
    bool ret = false;
    StorageData* storage;
    file->error_id = storage_get_data(app, path, &storage);

    if(file->error_id == FSE_OK) {
        if(storage_path_already_open(path, storage)) {
            file->error_id = FSE_ALREADY_OPEN;
        } else {
            if(access_mode & FSAM_WRITE) {
                storage_data_timestamp(storage);
            }
            storage_push_storage_file(file, path, storage);

            const char* path_cstr_no_vfs = cstr_path_without_vfs_prefix(path);
            FS_CALL(storage, file.open(storage, file, path_cstr_no_vfs, access_mode, open_mode));
        }
    }

    return ret;
}

bool storage_process_file_close(Storage* app, File* file) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, file.close(storage, file));
        storage_pop_storage_file(file, storage);

        StorageEvent event = {.type = StorageEventTypeFileClose};
        furi_pubsub_publish(app->pubsub, &event);
    }

    return ret;
}

static uint16_t
    storage_process_file_read(Storage* app, File* file, void* buff, uint16_t const bytes_to_read) {
    uint16_t ret = 0;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, file.read(storage, file, buff, bytes_to_read));
    }

    return ret;
}

static uint16_t storage_process_file_write(
    Storage* app,
    File* file,
    const void* buff,
    uint16_t const bytes_to_write) {
    uint16_t ret = 0;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        storage_data_timestamp(storage);
        FS_CALL(storage, file.write(storage, file, buff, bytes_to_write));
    }

    return ret;
}

static bool storage_process_file_seek(
    Storage* app,
    File* file,
    const uint32_t offset,
    const bool from_start) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, file.seek(storage, file, offset, from_start));
    }

    return ret;
}

static uint64_t storage_process_file_tell(Storage* app, File* file) {
    uint64_t ret = 0;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, file.tell(storage, file));
    }

    return ret;
}

static bool storage_process_file_expand(Storage* app, File* file, const uint64_t size) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, file.expand(storage, file, size));
    }

    return ret;
}

static bool storage_process_file_truncate(Storage* app, File* file) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        storage_data_timestamp(storage);
        FS_CALL(storage, file.truncate(storage, file));
    }

    return ret;
}

static bool storage_process_file_sync(Storage* app, File* file) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        storage_data_timestamp(storage);
        FS_CALL(storage, file.sync(storage, file));
    }

    return ret;
}

static uint64_t storage_process_file_size(Storage* app, File* file) {
    uint64_t ret = 0;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, file.size(storage, file));
    }

    return ret;
}

static bool storage_process_file_eof(Storage* app, File* file) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, file.eof(storage, file));
    }

    return ret;
}

/******************* Dir Functions *******************/

bool storage_process_dir_open(Storage* app, File* file, FuriString* path) {
    bool ret = false;
    StorageData* storage;
    file->error_id = storage_get_data(app, path, &storage);

    if(file->error_id == FSE_OK) {
        if(storage_path_already_open(path, storage)) {
            file->error_id = FSE_ALREADY_OPEN;
        } else {
            storage_push_storage_file(file, path, storage);
            FS_CALL(storage, dir.open(storage, file, cstr_path_without_vfs_prefix(path)));
        }
    }

    return ret;
}

bool storage_process_dir_close(Storage* app, File* file) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, dir.close(storage, file));
        storage_pop_storage_file(file, storage);

        StorageEvent event = {.type = StorageEventTypeDirClose};
        furi_pubsub_publish(app->pubsub, &event);
    }

    return ret;
}

bool storage_process_dir_read(
    Storage* app,
    File* file,
    FileInfo* fileinfo,
    char* name,
    const uint16_t name_length) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, dir.read(storage, file, fileinfo, name, name_length));
    }

    return ret;
}

bool storage_process_dir_rewind(Storage* app, File* file) {
    bool ret = false;
    StorageData* storage = get_storage_by_file(file, app->storage);

    if(storage == NULL) {
        file->error_id = FSE_INVALID_PARAMETER;
    } else {
        FS_CALL(storage, dir.rewind(storage, file));
    }

    return ret;
}

/******************* Common FS Functions *******************/

static FS_Error
    storage_process_common_timestamp(Storage* app, FuriString* path, uint32_t* timestamp) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    if(ret == FSE_OK) {
        *timestamp = storage_data_get_timestamp(storage);
    }

    return ret;
}

static FS_Error storage_process_common_stat(Storage* app, FuriString* path, FileInfo* fileinfo) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    if(ret == FSE_OK) {
        FS_CALL(storage, common.stat(storage, cstr_path_without_vfs_prefix(path), fileinfo));
    }

    return ret;
}

static FS_Error storage_process_common_remove(Storage* app, FuriString* path) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    do {
        if(ret != FSE_OK) break;

        if(storage_path_already_open(path, storage)) {
            ret = FSE_ALREADY_OPEN;
            break;
        }

        storage_data_timestamp(storage);
        FS_CALL(storage, common.remove(storage, cstr_path_without_vfs_prefix(path)));
    } while(false);

    return ret;
}

static FS_Error storage_process_common_rename(Storage* app, FuriString* old, FuriString* new) {
    FS_Error ret = FSE_OK;

    do {
        const StorageType storage_type_old = storage_get_type_by_path(old);
        const StorageType storage_type_new = storage_get_type_by_path(new);

        // Different filesystems, return to caller to do copy + remove
        if(storage_type_old != storage_type_new) {
            ret = FSE_NOT_IMPLEMENTED;
            break;
        }

        // Same filesystem, use fast rename
        StorageData* storage;
        ret = storage_get_data(app, old, &storage);

        if(ret != FSE_OK) break;

        if(storage_path_already_open(old, storage)) {
            ret = FSE_ALREADY_OPEN;
            break;
        }

        storage_data_timestamp(storage);
        FS_CALL(
            storage,
            common.rename(
                storage, cstr_path_without_vfs_prefix(old), cstr_path_without_vfs_prefix(new)));
    } while(false);

    return ret;
}

static FS_Error storage_process_common_mkdir(Storage* app, FuriString* path) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    if(ret == FSE_OK) {
        storage_data_timestamp(storage);
        FS_CALL(storage, common.mkdir(storage, cstr_path_without_vfs_prefix(path)));
    }

    return ret;
}

static FS_Error storage_process_common_fs_info(
    Storage* app,
    FuriString* path,
    uint64_t* total_space,
    uint64_t* free_space) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    if(ret == FSE_OK) {
        FS_CALL(
            storage,
            common.fs_info(storage, cstr_path_without_vfs_prefix(path), total_space, free_space));
    }

    return ret;
}

static bool
    storage_process_common_equivalent_path(Storage* app, FuriString* path1, FuriString* path2) {
    bool ret = false;

    do {
        const StorageType storage_type1 = storage_get_type_by_path(path1);
        const StorageType storage_type2 = storage_get_type_by_path(path2);

        // Paths on different storages are of course not equal
        if(storage_type1 != storage_type2) break;

        StorageData* storage;
        const FS_Error status = storage_get_data(app, path1, &storage);

        if(status != FSE_OK) break;

        FS_CALL(
            storage,
            common.equivalent_path(furi_string_get_cstr(path1), furi_string_get_cstr(path2)));

    } while(false);

    return ret;
}

/****************** Raw SD API ******************/
// TODO FL-3521: think about implementing a custom storage API to split that kind of api linkage
#include "storages/storage_ext.h"

static FS_Error storage_process_sd_format(Storage* app) {
    FS_Error ret = FSE_OK;

    if(storage_data_status(&app->storage[ST_EXT]) == StorageStatusNotReady) {
        ret = FSE_NOT_READY;
    } else {
        ret = sd_format_card(&app->storage[ST_EXT]);
        storage_data_timestamp(&app->storage[ST_EXT]);
    }

    return ret;
}

static FS_Error storage_process_sd_unmount(Storage* app) {
    FS_Error ret = FSE_OK;

    do {
        StorageData* storage = &app->storage[ST_EXT];
        if(storage_data_status(storage) == StorageStatusNotReady) {
            ret = FSE_NOT_READY;
            break;
        }

        if(storage_open_files_count(storage)) {
            ret = FSE_DENIED;
            break;
        }

        sd_unmount_card(storage);
        storage_data_timestamp(storage);
    } while(false);

    return ret;
}

static FS_Error storage_process_sd_mount(Storage* app) {
    FS_Error ret = FSE_OK;

    do {
        StorageData* storage = &app->storage[ST_EXT];
        if(storage_data_status(storage) != StorageStatusNotReady) {
            ret = FSE_NOT_READY;
            break;
        }

        ret = sd_mount_card(storage, true);
        storage_data_timestamp(storage);
    } while(false);

    return ret;
}

static FS_Error storage_process_sd_info(Storage* app, SDInfo* info) {
    FS_Error ret = FSE_OK;

    if(storage_data_status(&app->storage[ST_EXT]) == StorageStatusNotReady) {
        ret = FSE_NOT_READY;
    } else {
        ret = sd_card_info(&app->storage[ST_EXT], info);
    }

    return ret;
}

static FS_Error storage_process_sd_status(Storage* app) {
    FS_Error ret;
    StorageStatus status = storage_data_status(&app->storage[ST_EXT]);

    switch(status) {
    case StorageStatusOK:
        ret = FSE_OK;
        break;
    case StorageStatusNotReady:
        ret = FSE_NOT_READY;
        break;
    default:
        ret = FSE_INTERNAL;
        break;
    }

    return ret;
}

/******************** Aliases processing *******************/

void storage_process_alias(
    Storage* app,
    FuriString* path,
    FuriThreadId thread_id,
    bool create_folders) {
    if(furi_string_start_with(path, STORAGE_APP_DATA_PATH_PREFIX)) {
        FuriString* apps_data_path_with_appsid = furi_string_alloc_set(APPS_DATA_PATH "/");
        furi_string_cat(apps_data_path_with_appsid, furi_thread_get_appid(thread_id));

        // "/data" -> "/ext/apps_data/appsid"
        furi_string_replace_at(
            path,
            0,
            strlen(STORAGE_APP_DATA_PATH_PREFIX),
            furi_string_get_cstr(apps_data_path_with_appsid));

        // Create app data folder if not exists
        if(create_folders &&
           storage_process_common_stat(app, apps_data_path_with_appsid, NULL) != FSE_OK) {
            furi_string_set(apps_data_path_with_appsid, APPS_DATA_PATH);
            storage_process_common_mkdir(app, apps_data_path_with_appsid);
            furi_string_cat(apps_data_path_with_appsid, "/");
            furi_string_cat(apps_data_path_with_appsid, furi_thread_get_appid(thread_id));
            storage_process_common_mkdir(app, apps_data_path_with_appsid);
        }

        furi_string_free(apps_data_path_with_appsid);
    } else if(furi_string_start_with(path, STORAGE_APP_ASSETS_PATH_PREFIX)) {
        FuriString* apps_assets_path_with_appsid = furi_string_alloc_set(APPS_ASSETS_PATH "/");
        furi_string_cat(apps_assets_path_with_appsid, furi_thread_get_appid(thread_id));

        // "/assets" -> "/ext/apps_assets/appsid"
        furi_string_replace_at(
            path,
            0,
            strlen(STORAGE_APP_ASSETS_PATH_PREFIX),
            furi_string_get_cstr(apps_assets_path_with_appsid));

        furi_string_free(apps_assets_path_with_appsid);

    } else if(furi_string_start_with(path, STORAGE_INT_PATH_PREFIX)) {
        furi_string_replace_at(
            path, 0, strlen(STORAGE_INT_PATH_PREFIX), STORAGE_EXT_PATH_PREFIX "/.int");

        FuriString* int_on_ext_path = furi_string_alloc_set(STORAGE_EXT_PATH_PREFIX "/.int");
        if(storage_process_common_stat(app, int_on_ext_path, NULL) != FSE_OK) {
            storage_process_common_mkdir(app, int_on_ext_path);
        }
        furi_string_free(int_on_ext_path);
    }
}

/****************** API calls processing ******************/

void storage_process_message_internal(Storage* app, StorageMessage* message) {
    FuriString* path = NULL;

    switch(message->command) {
    // File operations
    case StorageCommandFileOpen:
        path = furi_string_alloc_set(message->data->fopen.path);
        storage_process_alias(app, path, message->data->fopen.thread_id, true);
        message->return_data->bool_value = storage_process_file_open(
            app,
            message->data->fopen.file,
            path,
            message->data->fopen.access_mode,
            message->data->fopen.open_mode);
        break;
    case StorageCommandFileClose:
        message->return_data->bool_value =
            storage_process_file_close(app, message->data->fopen.file);
        break;
    case StorageCommandFileRead:
        message->return_data->uint16_value = storage_process_file_read(
            app,
            message->data->fread.file,
            message->data->fread.buff,
            message->data->fread.bytes_to_read);
        break;
    case StorageCommandFileWrite:
        message->return_data->uint16_value = storage_process_file_write(
            app,
            message->data->fwrite.file,
            message->data->fwrite.buff,
            message->data->fwrite.bytes_to_write);
        break;
    case StorageCommandFileSeek:
        message->return_data->bool_value = storage_process_file_seek(
            app,
            message->data->fseek.file,
            message->data->fseek.offset,
            message->data->fseek.from_start);
        break;
    case StorageCommandFileTell:
        message->return_data->uint64_value =
            storage_process_file_tell(app, message->data->file.file);
        break;
    case StorageCommandFileExpand:
        message->return_data->bool_value = storage_process_file_expand(
            app, message->data->fexpand.file, message->data->fexpand.size);
        break;
    case StorageCommandFileTruncate:
        message->return_data->bool_value =
            storage_process_file_truncate(app, message->data->file.file);
        break;
    case StorageCommandFileSync:
        message->return_data->bool_value =
            storage_process_file_sync(app, message->data->file.file);
        break;
    case StorageCommandFileSize:
        message->return_data->uint64_value =
            storage_process_file_size(app, message->data->file.file);
        break;
    case StorageCommandFileEof:
        message->return_data->bool_value = storage_process_file_eof(app, message->data->file.file);
        break;

    // Dir operations
    case StorageCommandDirOpen:
        path = furi_string_alloc_set(message->data->dopen.path);
        storage_process_alias(app, path, message->data->dopen.thread_id, true);
        message->return_data->bool_value =
            storage_process_dir_open(app, message->data->dopen.file, path);
        break;
    case StorageCommandDirClose:
        message->return_data->bool_value =
            storage_process_dir_close(app, message->data->file.file);
        break;
    case StorageCommandDirRead:
        message->return_data->bool_value = storage_process_dir_read(
            app,
            message->data->dread.file,
            message->data->dread.fileinfo,
            message->data->dread.name,
            message->data->dread.name_length);
        break;
    case StorageCommandDirRewind:
        message->return_data->bool_value =
            storage_process_dir_rewind(app, message->data->file.file);
        break;

    // Common operations
    case StorageCommandCommonTimestamp:
        path = furi_string_alloc_set(message->data->ctimestamp.path);
        storage_process_alias(app, path, message->data->ctimestamp.thread_id, false);
        message->return_data->error_value =
            storage_process_common_timestamp(app, path, message->data->ctimestamp.timestamp);
        break;
    case StorageCommandCommonStat:
        path = furi_string_alloc_set(message->data->cstat.path);
        storage_process_alias(app, path, message->data->cstat.thread_id, false);
        message->return_data->error_value =
            storage_process_common_stat(app, path, message->data->cstat.fileinfo);
        break;
    case StorageCommandCommonRemove:
        path = furi_string_alloc_set(message->data->path.path);
        storage_process_alias(app, path, message->data->path.thread_id, false);
        message->return_data->error_value = storage_process_common_remove(app, path);
        break;
    case StorageCommandCommonRename: {
        FuriString* old_path = furi_string_alloc_set(message->data->rename.old);
        FuriString* new_path = furi_string_alloc_set(message->data->rename.new);
        storage_process_alias(app, old_path, message->data->rename.thread_id, false);
        storage_process_alias(app, new_path, message->data->rename.thread_id, false);
        message->return_data->error_value = storage_process_common_rename(app, old_path, new_path);
        furi_string_free(old_path);
        furi_string_free(new_path);
        break;
    }
    case StorageCommandCommonMkDir:
        path = furi_string_alloc_set(message->data->path.path);
        storage_process_alias(app, path, message->data->path.thread_id, true);
        message->return_data->error_value = storage_process_common_mkdir(app, path);
        break;
    case StorageCommandCommonFSInfo:
        path = furi_string_alloc_set(message->data->cfsinfo.fs_path);
        storage_process_alias(app, path, message->data->cfsinfo.thread_id, false);
        message->return_data->error_value = storage_process_common_fs_info(
            app, path, message->data->cfsinfo.total_space, message->data->cfsinfo.free_space);
        break;
    case StorageCommandCommonResolvePath:
        storage_process_alias(
            app, message->data->cresolvepath.path, message->data->cresolvepath.thread_id, true);
        break;

    case StorageCommandCommonEquivalentPath: {
        FuriString* path1 = furi_string_alloc_set(message->data->cequivpath.path1);
        FuriString* path2 = furi_string_alloc_set(message->data->cequivpath.path2);
        storage_path_trim_trailing_slashes(path1);
        storage_path_trim_trailing_slashes(path2);
        storage_process_alias(app, path1, message->data->cequivpath.thread_id, false);
        storage_process_alias(app, path2, message->data->cequivpath.thread_id, false);
        // Comparison is done on path name, same beginning of name != same file/folder
        // Check with a / suffixed to ensure same file/folder name
        furi_string_cat(path1, "/");
        furi_string_cat(path2, "/");
        if(message->data->cequivpath.truncate) {
            furi_string_left(path2, furi_string_size(path1));
        }
        message->return_data->bool_value =
            storage_process_common_equivalent_path(app, path1, path2);
        furi_string_free(path1);
        furi_string_free(path2);
        break;
    }

    // SD operations
    case StorageCommandSDFormat:
        message->return_data->error_value = storage_process_sd_format(app);
        break;
    case StorageCommandSDUnmount:
        message->return_data->error_value = storage_process_sd_unmount(app);
        break;
    case StorageCommandSDMount:
        message->return_data->error_value = storage_process_sd_mount(app);
        break;
    case StorageCommandSDInfo:
        message->return_data->error_value =
            storage_process_sd_info(app, message->data->sdinfo.info);
        break;
    case StorageCommandSDStatus:
        message->return_data->error_value = storage_process_sd_status(app);
        break;

    // Virtual operations
    case StorageCommandVirtualInit:
        File* image = message->data->virtualinit.image;
        StorageData* image_storage = get_storage_by_file(image, app->storage);
        message->return_data->error_value = storage_process_virtual_init(image_storage, image);
        break;
    case StorageCommandVirtualFormat:
        message->return_data->error_value = storage_process_virtual_format(&app->storage[ST_MNT]);
        break;
    case StorageCommandVirtualMount:
        message->return_data->error_value = storage_process_virtual_mount(&app->storage[ST_MNT]);
        break;
    case StorageCommandVirtualUnmount:
        message->return_data->error_value = storage_process_virtual_unmount(&app->storage[ST_MNT]);
        break;
    case StorageCommandVirtualQuit:
        message->return_data->error_value = storage_process_virtual_quit(&app->storage[ST_MNT]);
        break;
    }

    if(path != NULL) { //-V547
        furi_string_free(path);
    }

    api_lock_unlock(message->lock);
}

void storage_process_message(Storage* app, StorageMessage* message) {
    storage_process_message_internal(app, message);
}
