/**
 * @file elf_file.h
 * ELF file loader
 */
#pragma once
#include <storage/storage.h>
#include "../application_manifest.h"
#include "elf_api_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ELFFile ELFFile;

typedef struct {
    const char* name;
    uint32_t address;
} ELFMemoryMapEntry;

typedef struct {
    uint32_t debug_link_size;
    uint8_t* debug_link;
} ELFDebugLinkInfo;

typedef struct {
    uint32_t mmap_entry_count;
    ELFMemoryMapEntry* mmap_entries;
    ELFDebugLinkInfo debug_link_info;
    off_t entry;
} ELFDebugInfo;

typedef enum {
    ELFFileLoadStatusSuccess = 0,
    ELFFileLoadStatusUnspecifiedError,
    ELFFileLoadStatusNoFreeMemory,
    ELFFileLoadStatusMissingImports,
} ELFFileLoadStatus;

typedef enum {
    ElfProcessSectionResultNotFound,
    ElfProcessSectionResultCannotProcess,
    ElfProcessSectionResultSuccess,
} ElfProcessSectionResult;

typedef bool(ElfProcessSection)(File* file, size_t offset, size_t size, void* context);

/**
 * @brief Allocate ELFFile instance
 * @param storage 
 * @param api_interface 
 * @return ELFFile* 
 */
ELFFile* elf_file_alloc(Storage* storage, const ElfApiInterface* api_interface);

/**
 * @brief Free ELFFile instance
 * @param elf_file 
 */
void elf_file_free(ELFFile* elf_file);

/**
 * @brief Open ELF file
 * @param elf_file 
 * @param path 
 * @return bool 
 */
bool elf_file_open(ELFFile* elf_file, const char* path);

/**
 * @brief Load ELF file section table (load stage #1)
 * @param elf_file 
 * @return bool 
 */
bool elf_file_load_section_table(ELFFile* elf_file);

/**
 * @brief Load and relocate ELF file sections (load stage #2)
 * @param elf_file 
 * @return ELFFileLoadStatus 
 */
ELFFileLoadStatus elf_file_load_sections(ELFFile* elf_file);

/**
 * @brief Execute ELF file pre-run stage, 
 * call static constructors for example (load stage #3)
 * Must be done before invoking any code from the ELF file
 * @param elf 
 */
void elf_file_call_init(ELFFile* elf);

/**
 * @brief Check if ELF file pre-run stage was executed and its code is runnable
 * @param elf 
 */
bool elf_file_is_init_complete(ELFFile* elf);

/**
 * @brief Get actual entry point for ELF file
 * @param elf_file 
 * @return void*
 */
void* elf_file_get_entry_point(ELFFile* elf_file);

/**
 * @brief Execute ELF file post-run stage, 
 * call static destructors for example (load stage #5)
 * Must be done if any code from the ELF file was executed
 * @param elf 
 */
void elf_file_call_fini(ELFFile* elf);

/**
 * @brief Get ELF file API interface
 * @param elf_file 
 * @return const ElfApiInterface* 
 */
const ElfApiInterface* elf_file_get_api_interface(ELFFile* elf_file);

/**
 * @brief Get ELF file debug info
 * @param elf_file 
 * @param debug_info 
 */
void elf_file_init_debug_info(ELFFile* elf_file, ELFDebugInfo* debug_info);

/**
 * @brief Clear ELF file debug info generated by elf_file_init_debug_info
 * @param debug_info 
 */
void elf_file_clear_debug_info(ELFDebugInfo* debug_info);

/**
 * @brief Process ELF file section
 * 
 * @param elf_file 
 * @param name 
 * @param process_section 
 * @param context 
 * @return ElfProcessSectionResult 
 */
ElfProcessSectionResult elf_process_section(
    ELFFile* elf_file,
    const char* name,
    ElfProcessSection* process_section,
    void* context);

#ifdef __cplusplus
}
#endif
