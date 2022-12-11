#include <unordered_map>
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    auto pids = LinuxParser::Pids();
    updateProccess(pids);
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

void System::updateProccess(std::vector<int> &pids)
{
    std::unordered_map<int, Process> processMap;
    for (auto process : processes_)
    {
        processMap[process.Pid()] = process;
    }

    std::vector<Process> processesAlive;
    for (auto pid : pids)
    {
        if (processMap.find(pid) != processMap.end())
        {
            processesAlive.emplace_back(processMap[pid]);
        }
        else
        {
            processesAlive.emplace_back(Process(pid));
        }
    }
    processes_ = processesAlive;
}