/*
 * @file paging.hpp (orginal)
 */

#include <cstdint>

typedef struct PageDirectoryEntry {

    uint32_t present            : 1;   
    uint32_t read_write         : 1;    
    uint32_t user_supervisor    : 1;
    uint32_t write_through      : 1;
    uint32_t cache_disabled     : 1;    
    uint32_t accessed           : 1;    
    uint32_t reserved           : 1;    
    uint32_t page_size          : 1; 
    uint32_t global             : 1;    
    uint32_t available          : 1; 
    uint32_t frame_addr         : 1;            // frame address (lower 20 bits)

} __attribute__((packed)) PageDirectoryEntry;

typedef struct {
    PageDirectoryEntry entries [512];
} __attribute__((alligned(0x1000))) PageTable;

typedef struct {
    uint64_t PDP_i;
    uint64_t PD_i;
    uint64_t PT_i;
    uint64_t P_i;
} PageIndexer;

typedef struct {
    PageTable* PML4;
} PageTableManager;

PageIndexer PageMapIndexer(uint64_t virtualAddress);

PageTableManager PTmanager(uint64_t virtualAddress);

void MapMemory(void* virtualMemory, void* physicalMemory, PageTableManager manager);
void switchPML4(PageTable* PML4);

extern PageTableManager g_Pagetablemanager;
