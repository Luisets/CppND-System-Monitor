#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    long activeJiffies = LinuxParser::ActiveJiffies();
    long idleJiffies = LinuxParser::IdleJiffies();
    long totalJiffies = activeJiffies + idleJiffies;

    long idleJiffiesDiff = idleJiffies - _lastIdleJiffies;
    long totalJiffiesDiff = totalJiffies - _lastTotalJiffies;

    _lastActiveJiffies = activeJiffies;
    _lastIdleJiffies = idleJiffies;
    _lastTotalJiffies = totalJiffies;

    return static_cast<float>(totalJiffiesDiff - idleJiffiesDiff) / static_cast<float>(totalJiffiesDiff);
}