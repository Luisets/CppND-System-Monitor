#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process() = default;
  Process(int pid, long systemTime);
  Process(const Process& process);
  Process& operator=(const Process&) = default;
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  void update(long systemTime);

 private:
  int _pid;
  std::string _user;
  std::string _command;
  long _sysUpTime;
  long _lastSysUpTime;
  long int _upTime;
  long _activeJiffies;
  long _lastActiveJiffies;
};

#endif