#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "utils.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, long systemTime)
    : _pid(pid),
      _user(LinuxParser::User(pid)),
      _command(LinuxParser::Command(pid)),
      _sysUpTime(systemTime),
      _lastSysUpTime(0),
      _upTime(LinuxParser::UpTime(pid)),
      _activeJiffies(LinuxParser::ActiveJiffies(pid)),
      _lastActiveJiffies(0) {}

Process::Process(const Process& process)
    : _pid(process._pid),
      _user(process._user),
      _command(process._command),
      _sysUpTime(process._sysUpTime),
      _lastSysUpTime(process._lastSysUpTime),
      _upTime(process._upTime),
      _activeJiffies(process._activeJiffies),
      _lastActiveJiffies(process._lastActiveJiffies) {}

// Return this process's ID
int Process::Pid() { return _pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  float jiffiesDiff = _activeJiffies - _lastActiveJiffies;
  float seconds = (_sysUpTime - _upTime) - (_lastSysUpTime - _upTime);
  return std::min(secureDiv(secureDiv(static_cast<float>(jiffiesDiff),
                                      static_cast<float>(sysconf(_SC_CLK_TCK))),
                            seconds),
                  1.0F);
}

// Return the command that generated this process
string Process::Command() { return _command; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(_pid); }

// Return the user (name) that generated this process
string Process::User() { return _user; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return _sysUpTime - _upTime; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

void Process::update(long systemTime) {
  _lastSysUpTime = _sysUpTime;
  _sysUpTime = systemTime;
  _lastActiveJiffies = _activeJiffies;
  _activeJiffies = LinuxParser::ActiveJiffies(_pid);
}
