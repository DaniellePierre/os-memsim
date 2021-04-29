#include "pagetable.h"

PageTable::PageTable(int page_size)
{
    _page_size = page_size;
}

PageTable::~PageTable()
{
}

std::vector<std::string> PageTable::sortedKeys()
{
    std::vector<std::string> keys;

    std::map<std::string, int>::iterator it;
    for (it = _table.begin(); it != _table.end(); it++)
    {
        keys.push_back(it->first);
    }

    std::sort(keys.begin(), keys.end(), PageTableKeyComparator());

    return keys;
}

void PageTable::addEntry(uint32_t pid, int page_number)
{
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    int frame = 0; 
    // Find free frame
    // TODO: implement this! Make sure page exists first
    _table[entry] = frame;
}

int PageTable::getPhysicalAddress(uint32_t pid, uint32_t virtual_address)
{
    // Convert virtual address to page_number and page_offset
    int page_number = 0;
    int page_offset = 0;
    int temp_page_size = _page_size; 

    while(temp_page_size != 0){
        page_offset++;
        temp_page_size  = temp_page_size / 2;
    }

    page_number = 64 - page_offset;

    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0)
    {
        // TODO: implement this!
        address = virtual_address + page_offset;
    }

    return address;
}

int PageTable::getPageSize(){
    return _page_size;
}

int PageTable::getPageNumber(int virtual_address){
    int page_offset = 0;
    int temp_page_size = _page_size; 

    while(temp_page_size != 0){
        page_offset++;
        temp_page_size  = temp_page_size / 2;
    }

    int page_number = 64 - page_offset;
    return page_number;
}

void PageTable::print()
{
    int i;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    std::vector<std::string> keys = sortedKeys();

    for (i = 0; i < keys.size(); i++)
    {
        // TODO: print all pages
        size_t value = keys[i].find("|");
        int pid = std::stoi(keys[i].substr(0, value));
        int page = std::stoi(keys[i].substr(value+1));
        int frame = _table[keys[i]];

        printf(" %4u | %11d | %12d\n", pid, page, frame);
    }
}
