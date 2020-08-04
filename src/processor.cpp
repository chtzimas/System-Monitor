#include "processor.h"
#include "linux_parser.h"

using std::stof;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto v = LinuxParser::CpuUtilization();
  float user = stof(v[0]);
  float nice = stof(v[1]);
  float system = stof(v[2]);
  float idle = stof(v[3]);
  float iowait = stof(v[4]);
  float irq = stof(v[5]);
  float softirq = stof(v[6]);
  float steal = stof(v[7]);

  float PrevIdle = previdle + previowait;
  float Idle = idle + iowait;

  float PrevNonIdle =
      prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  float NonIdle = user + nice + system + irq + softirq + steal;

  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;

  float cpuPercentage = (totald - idled) / totald;

  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previdle = idle;
  previowait = iowait;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;

  return cpuPercentage;
}