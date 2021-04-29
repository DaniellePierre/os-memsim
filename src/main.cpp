//Danielle Pierre & Lillian Krueger
//make
//./bin/memsim 2048

#include <iostream>
#include <string>
#include <cstring>
#include "mmu.h"
#include "pagetable.h"

void printStartMessage(int page_size);
void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table);
void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table);
void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory);
void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table);
void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table);

void splitString(std::string text, char d, std::vector<std::string>& result); //Approval from Professor Marrinan - From OS-osshell

DataType findDataType(std::string data_type_as_string);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // Print opening instuction message
    int page_size = std::stoi(argv[1]);
    printStartMessage(page_size);

    // Create physical 'memory'
    uint32_t mem_size = 67108864;
    void *memory = malloc(mem_size); // 64 MB (64 * 1024 * 1024)

    // Create MMU and Page Table
    Mmu *mmu = new Mmu(mem_size);
    PageTable *page_table = new PageTable(page_size);

    // Prompt loop
    std::string command;
    std::cout << "> ";
    std::getline (std::cin, command);
    
    //std::vector<std::string> arguments;
    //splitString(command, ' ', arguments);

    while (command != "exit") {
        // Handle command
        std::vector<std::string> arguments;
        splitString(command, ' ', arguments);

             
        // TODO: implement this!
        if(arguments.at(0) == "create"){//MINIMUM

            int text_size;
            int data_size;

            text_size = std::stoi(arguments.at(1));
            data_size = std::stoi(arguments.at(2));
            
            createProcess(text_size, data_size, mmu, page_table);

        }else if(arguments.at(0) == "allocate"){//MINIMUM
            
            int pid;
            DataType type_of_data;
            int num_elements;

            pid = std::stoi(arguments.at(1));
            type_of_data = findDataType(arguments.at(3));
            num_elements = std::stoi(arguments.at(4));

            allocateVariable(pid, arguments.at(2), type_of_data, num_elements, mmu, page_table);

        }else if(arguments.at(0) == "set"){//MINIMUM
        int pid;
        std::string name;
        int offset;
        std::string value;        

        pid = std::stoi(arguments.at(1));
        name = arguments.at(2);
        offset = std::stoi(arguments.at(3));

        for(int i = 4; i < arguments.size(); i++){
            value = arguments.at(i);
            setVariable(pid, name, offset, &value, mmu, page_table, memory);
            offset++;

        }

        }else if(arguments.at(0) == "print"){//MINIMUM

            if(arguments.at(1) == "mmu"){ //if <object> is "mmu", print the MMU memory table
                mmu->print();
            }else if(arguments.at(1) == "page"){ //If <object> is "page", print the page table (do not need to print anything for free frames)
                page_table->print();
            }else if(arguments.at(1) == "processes"){ //If <object> is "processes", print a list of PIDs for processes that are still running
                mmu->printProcesses();
            }else{ //If <object> is a "<PID>:<var_name>", print the value of the variable for that process

            }

        }else if(arguments.at(0) == "free"){

        }else if(arguments.at(0) == "terminate"){

        }else{

            printf("error: command not recognized\n");

        }

        // Get next command
        std::cout << "> ";
        std::getline (std::cin, command);
    }

    // Cean up
    free(memory);
    delete mmu;
    delete page_table;

    return 0;
}

void printStartMessage(int page_size)
{
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes." << std:: endl;
    std::cout << "Commands:" << std:: endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std:: endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)" << std:: endl;
    std::cout << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)" << std:: endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)" << std:: endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std:: endl;
    std::cout << "  * print <object> (prints data)" << std:: endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std:: endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std:: endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running" << std:: endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process" << std:: endl;
    std::cout << std::endl;
}

void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - create new process in the MMU
    int pid;
    pid = mmu->createProcess();
    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    allocateVariable(pid, "<TEXT>", DataType::Char, text_size, mmu, page_table);
    allocateVariable(pid, "<GLOBALS>", DataType::Char, data_size, mmu, page_table);
    allocateVariable(pid, "<STACK>", DataType::Char, 65536, mmu, page_table);
    //   - print pid
    printf("%d\n", pid);
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    int num_bytes = 0;
    if(type == DataType::Char){
        num_bytes = num_elements;
    }else if(type == DataType::Short){
        num_bytes = num_elements * 2;
    }else if(type == DataType::Int || type == DataType::Float){
        num_bytes = num_elements * 4;
    }else if(type == DataType::Long || type == DataType::Double){
        num_bytes = num_elements * 8;
    }

    int virtualAddress = 0;

    Process* process = mmu->getProcess(pid);
    DataType typeOfProcess = mmu->getDataType(pid);
    int sizeOfProcess = mmu->getSize(pid);

    //first go through mmu and find a FREESPACE
    while((typeOfProcess != FreeSpace) && (sizeOfProcess < num_bytes)){
        //continue until a FreeSpace is found
        int nextPid = pid++;
        process = mmu->getProcess(nextPid);
        typeOfProcess = mmu->getDataType(nextPid);
        sizeOfProcess = mmu->getSize(nextPid);
    }

    //check the page size and make sure an element is not crossing page boundries
    int physical_address = page_table->getPhysicalAddress(pid, virtualAddress);
    int page_size = page_table->getPageSize();
    int space_on_page = page_size - physical_address;
    int gap = space_on_page % num_bytes;
    if(gap != 0){
    //not enough space on the page to fit a whole element
        physical_address = physical_address + gap;
    }

    //if not just add it or fix and add it
    int page_number = page_table->getPageNumber(physical_address);
    //add the variable to that spot
    //make sure the page exists that you are trying to put something one
    //if not make a new one
    //make sure enough frames exist
    //if not make more starting with the first unused frame number
    //should all happen in addEntry
    page_table->addEntry(pid, page_number);
    
    //change the name to the variable name and use the correct ammount of size
    mmu->setNewVariable(pid, var_name, sizeOfProcess);   

    //resize the post FREESPACE variable with the correct size 
    //TODO next line seg faulting check this out
    //mmu->resizePostNewVariableSize(pid);

    //add one before new vairble if you had to correct to fit elements on a page
    //TODO next line seg faulting check this out
    //mmu->checkForGapFreeSpace(pid);

    //   - insert variable into MMU
    mmu->addVariableToProcess(pid, var_name, type, num_bytes, virtualAddress);

    //   - print virtual memory address
    mmu->print();
}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory)
{
    int physical_address;
    int virtual_address = 0;
    
    // TODO: implement this!
    //   - look up physical address for variable based on its virtual address / offset
    //OR physical_address = page_table->getPhysicalAddress(pid, )??
    
    //physical_address = page_table->getPhysicalAddress(pid); //need something else here

    //   - insert `value` into `memory` at physical address
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array)
    
}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove entry from MMU
    //   - free page if this variable was the only one on a given page
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    //   - free all pages associated with given process
}


DataType findDataType(std::string data_type_as_string){
    if(data_type_as_string == "int"){
        return DataType::Int;
    }else if(data_type_as_string == "double"){
        return DataType::Double;
    }else if(data_type_as_string == "char"){
        return DataType::Char;
    }else if(data_type_as_string == "long"){
        return DataType::Long;
    }else if(data_type_as_string == "float"){
        return DataType::Float;
    }else if(data_type_as_string == "short"){
        return DataType::Short;
    }
    printf("error didn't have a valid data type passed");
    return DataType::Int;
}


//Recieved approval to use this method from Professor Marrinan - from OS-osshell homework
/*
   text: string to split
   d: character delimiter to split `text` on
   result: vector of strings - result will be stored here
*/
void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}