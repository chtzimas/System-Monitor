#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

// Define the constructor
Process::Process(int pid) : procID(pid) {
  user = LinuxParser::User(procID);
  command = LinuxParser::Command(procID);
  ram = LinuxParser::Ram(procID);
  uptime = LinuxParser::UpTime(procID);
}

// TODO: Return this process's ID
int Process::Pid() const { return procID; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const 
{
  long systUptime = LinuxParser::UpTime();
  long procUptime = LinuxParser::UpTime(procID);
  long procRunning = systUptime - procUptime;
  long procTotalTime =
      LinuxParser::ActiveJiffies(procID) / sysconf(_SC_CLK_TCK);
  if (procRunning != 0)
  {
    float cpuUtil = float(procTotalTime) / float(procRunning);
    return cpuUtil;
  }
  else
    return 0.f;
}

// TODO: Return the command that generated this process
string Process::Command() const { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ return CpuUtilization() > a.CpuUtilization(); }
