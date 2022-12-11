#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
    : _pid(pid),
      _user(LinuxParser::User(pid)),
      _command(LinuxParser::Command(pid)),
      _upTime(LinuxParser::ActiveJiffies(pid)),
      _lastUptime(0) {}

Process::Process(const Process& process)
    : _pid(process._pid),
      _user(process._user),
      _command(process._command),
      _lastActiveSecs(process._lastActiveSecs),
      _lastUptime(process._lastUptime) {}

// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long activeSecs = LinuxParser::ActiveJiffies(_pid) / sysconf(_SC_CLK_TCK);

  long activeSecsDiff = activeSecs - _lastActiveSecs;
  long uptimeDiff = _upTime - _lastUptime;
  return static_cast<float>(activeSecsDiff) / static_cast<float>(uptimeDiff);
}

// TODO: Return the command that generated this process
string Process::Command() { return _command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(_pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return _user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}

void Process::update() {
  _lastUptime = _upTime;
  _upTime = LinuxParser::UpTime(_pid);

  _lastActiveSecs = _activeSecs;
  _activeSecs = LinuxParser::ActiveJiffies(_pid) / sysconf(_SC_CLK_TCK);

}
