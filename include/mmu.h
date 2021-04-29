#ifndef __MMU_H_
#define __MMU_H_

#include <iostream>
#include <string>
#include <vector>

enum DataType : uint8_t {FreeSpace, Char, Short, Int, Float, Long, Double};

typedef struct Variable {
    std::string name;
    DataType type;
    uint32_t virtual_address;
    uint32_t size;
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable*> variables;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    uint32_t _max_size;
    std::vector<Process*> _processes;

public:
    Mmu(int memory_size);
    ~Mmu();

    uint32_t createProcess();
    void addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address);
    void print();
    void printProcesses();
    Process* getProcess(uint32_t pid);
    Variable* getVariables(uint32_t pid);
    DataType getDataType(uint32_t pid);
    int getSize(uint32_t pid);
    void setNewVariable(int pid, std::string set_name, int set_size);
    void checkForGapFreeSpace(int pid);
    void resizePostNewVariableSize(int pid);
};

#endif // __MMU_H_
