#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long memTotal;
  long memFree;
  std::string line;
  std::stringstream strStream;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  // Read MemTotal
  std::getline(stream, line);
  strStream << line;
  strStream >> line >> memTotal;

  // Read MemFree
  std::getline(stream, line);
  strStream << line;
  strStream >> line >> memFree;

  return static_cast<float>(memTotal - memFree) / static_cast<float>(memTotal);
}

// Read and return the system uptime
long int LinuxParser::UpTime() {
  float uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  stream >> uptime;
  return static_cast<long int>(std::round(uptime));
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::string value;
  long activeJiffies = 0;
  long utime, stime, cutime, cstime;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    for (int i = 0; i < 14; i++) {
      stream >> value;
    }
    stream >> utime >> stime >> cutime >> cstime;

    activeJiffies = utime + stime;
    activeJiffies = activeJiffies + cutime + cstime;
  }
  return activeJiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::string key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  stream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal >> guest >> guest_nice;

  return user + nice + system + irq + softirq + steal;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::string key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  stream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal >> guest >> guest_nice;

  return idle + iowait;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int value = 0;
  std::string key;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "processes") {
      linestream >> value;
      break;
    }
  }

  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int value = 0;
  std::string key;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "procs_running") {
      linestream >> value;
      break;
    }
  }

  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string line;
  std::string key;
  int value = 0;
  float ramMB = 0.0F;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        ramMB = value / 1000.0F;
        ramMB = std::roundf(ramMB * 100.0F) / 100.0F;
        break;
      }
    }
  }
  std::stringstream strStream;
  strStream.precision(2);
  strStream << std::fixed << ramMB;
  return strStream.str();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string line;
  std::string key;
  int value = -1;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return std::to_string(value);
      }
    }
  }
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string line;
  std::string processUid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      int firstIndex = line.find(":");
      int secondIndex = line.find(":", firstIndex + 1);
      int thirdIndex = line.find(":", secondIndex + 1);

      std::string user = line.substr(0, firstIndex);
      std::string uid =
          line.substr(secondIndex + 1, thirdIndex - secondIndex - 1);
      if (processUid == uid) {
        return user;
      }
    }
  }

  return "";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::string value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  long jiffies;
  if (stream.is_open()) {
    for (int i = 0; i < 22; i++) {
      stream >> value;
    }
    jiffies = std::stol(value);
  }
  return jiffies / sysconf(_SC_CLK_TCK);
}
