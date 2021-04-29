#include "mmu.h"

Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess()
{
    Process *proc = new Process();
    proc->pid = _next_pid;

    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->type = DataType::FreeSpace;
    var->virtual_address = 0;
    var->size = _max_size;
    proc->variables.push_back(var);

    _processes.push_back(proc);

    _next_pid++;
    return proc->pid;
}

void Mmu::addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address)
{
    int i;
    Process *proc = NULL;
    for (i = 0; i < _processes.size(); i++)
    {
        if (_processes[i]->pid == pid)
        {
            proc = _processes[i];
        }
    }

    Variable *var = new Variable();
    var->name = var_name;
    var->type = type;
    var->virtual_address = address;
    var->size = size;
    if (proc != NULL)
    {
        proc->variables.push_back(var);
    }
}

void Mmu::print()
{
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            if(_processes[i]->variables[0]->type != FreeSpace)
            {
                int pid = _processes[i]->pid;
                std::string var_name = _processes[i]->variables[0]->name;
                int vir_addr = _processes[i]->variables[0]->virtual_address;
                int size = _processes[i]->variables[0]->size;

                printf(" %4u | %11s | %12d | %12d\n", pid, var_name.c_str(), vir_addr, size);
            }
        }
    }
}

void Mmu::printProcesses()
{
    for(int i = 0; i <  _processes.size(); i++)
    {
        int pid = _processes[i]->pid;
        printf(" %4u \n", pid);
    }
}

Process* Mmu::getProcess(uint32_t pid){
    for (int i = 0; i < _processes.size(); i++){
        if(_processes[i]->pid == pid){
            return _processes[i];
        }
    }
    return NULL;
}

Variable* Mmu::getVariables(uint32_t pid){
    Process* process = getProcess(pid);
    //TODO double check/ask if this is supposed to be variable 0 or how to find which one is correct
    return process->variables[0];
}

DataType Mmu::getDataType(uint32_t pid){
    Variable *var = getVariables(pid);
    return var->type;
}

int Mmu::getSize(uint32_t pid){
    Variable *var = getVariables(pid);
    return var->size;
}

void Mmu::setNewVariable(int pid, std::string set_name, int set_size){
    Process* process = getProcess(pid);
    process->variables[0]->name = set_name;
    process->variables[0]->size = set_size;
}

void Mmu::checkForGapFreeSpace(int pid){
    Process* process = getProcess(pid);
    Process* prior_process = getProcess(pid - 1);

    //if process before is also FreeSpace compine them
    if(prior_process->variables[0]->type == FreeSpace){
        prior_process->variables[0]->size = prior_process->variables[0]->size + process->variables[0]->size;
    }
}

void Mmu::resizePostNewVariableSize(int pid){
    Process* process = getProcess(pid);
    Process* next_process = getProcess(pid + 1);

    //if process before is also FreeSpace compine them
    if(next_process->variables[0]->type == FreeSpace){
        next_process->variables[0]->size = next_process->variables[0]->size - process->variables[0]->size;
    }
}