#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  auto pids = LinuxParser::Pids();
  updateProccess(pids);
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

// Update the alive processes
void System::updateProccess(std::vector<int>& pids) {
  std::unordered_map<int, Process> processMap;
  long systemTime = UpTime();
  for (auto process : processes_) {
    processMap[process.Pid()] = process;
  }

  std::vector<Process> processesAlive;
  for (auto pid : pids) {
    if (processMap.find(pid) != processMap.end()) {
      processMap[pid].update(systemTime);
      processesAlive.emplace_back(processMap[pid]);
    } else {
      processesAlive.emplace_back(Process(pid, systemTime));
    }
  }
  std::sort(processesAlive.begin(), processesAlive.end());
  processes_ = processesAlive;
}