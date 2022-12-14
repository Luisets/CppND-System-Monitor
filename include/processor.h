#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
    long _lastActiveJiffies = 0;
    long _lastIdleJiffies = 0;
    long _lastTotalJiffies = 0;
};

#endif