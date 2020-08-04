#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
  throw std::runtime_error("File " + kOSPath + " not found\n");
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    return kernel;
  }
  throw std::runtime_error("File " + kProcDirectory + kVersionFilename +
                           " not found\n");
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key, value;
  float memTotal, memFree;
  int count = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        ++count;
        memTotal = stof(value);
      } else if (key == "MemFree:") {
        ++count;
        memFree = stof(value);
      }
      if (count == 2) {
        if (memTotal != 0)
          return (memTotal - memFree) / memTotal;
        else
          throw std::runtime_error(
              "MemTotal = 0. Unable to calculate memory usage\n");
      }
    }
  }
  throw std::runtime_error("File " + kProcDirectory + kMeminfoFilename +
                           " not found\n");
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, uptime, idletime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return stol(uptime);
  }
  throw std::runtime_error("File " + kProcDirectory + kUptimeFilename +
                           " not found\n");
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, value, uTime, sTime, cuTime, csTime;
  long activeJiffies;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (auto i = 0; i < 13; ++i) linestream >> value;
    linestream >> uTime >> sTime >> cuTime >> csTime;
    activeJiffies = stol(uTime) + stol(sTime) + stol(cuTime) + stol(csTime);
    return activeJiffies;
  }
  throw std::runtime_error("File " + kProcDirectory + std::to_string(pid) +
                           kStatFilename + " not found\n");
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    vector<string> v{};
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (!linestream.eof()) {
      linestream >> value;
      if (value != "cpu") v.emplace_back(value);
    }
    return v;
  }
  throw std::runtime_error("File " + kProcDirectory + kStatFilename +
                           " not found\n");
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") return stoi(value);
    }
  }
  throw std::runtime_error("File " + kProcDirectory + kStatFilename +
                           " not found\n");
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") return stoi(value);
    }
  }
  throw std::runtime_error("File " + kProcDirectory + kStatFilename +
                           " not found\n");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) value += line;
    return line;
  }
  throw std::runtime_error("File " + kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename + " not found\n");
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {  // convert to MB
        std::stringstream ss;
        if (stof(value) < 1) {
          ss << std::fixed << std::setprecision(4) << stof(value) / 1000;
          ss >> value;
        } else if (stoi(value) < 10) {
          ss << std::fixed << std::setprecision(3) << stof(value) / 1000;
          ss >> value;
        } else if (stoi(value) < 100) {
          value[1] = '0';
          ss << std::fixed << std::setprecision(2) << stof(value) / 1000;
          ss >> value;
        } else if (stoi(value) < 1000) {
          value[2] = '0';
          ss << std::fixed << std::setprecision(2) << stof(value) / 1000;
          ss >> value;
        } else {
          value = std::to_string(stoi(value) / 1000);
        }
        return value;
      }
    }
  }
  throw std::runtime_error("File " + kProcDirectory + std::to_string(pid) +
                           kStatusFilename + " not found\n");
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") return value;
    }
  }
  throw std::runtime_error("File " + kProcDirectory + std::to_string(pid) +
                           kStatusFilename + " not found\n");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, username, password, userid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> password >> userid;
      if (userid == Uid(pid)) return username;
    }
  }
  throw std::runtime_error("File " + kPasswordPath + " not found\n");
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
  string line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  long uptimeInSeconds;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (auto i = 0; i < 22; ++i) linestream >> value;
    uptimeInSeconds = stol(value) / sysconf(_SC_CLK_TCK);
    return uptimeInSeconds;
  }
  throw std::runtime_error("File " + kProcDirectory + std::to_string(pid) +
                           kStatFilename + " not found\n");
}