#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <regex>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Global to avoid IO
int TotalProcesses_count = 0;
int RunningProcesses_count = 0;

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
  return value;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // total used memory = MemTotal - MemFree
  // Non cache / buffer memory (green) total used memory - (Buffers + Cached
  // Memory) Buffers (blue) = Buffers Cached memory (yellow) = Cached
  // +SReclaimable -Shmem Swap = swapTotal -SwapFree

  string location = LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename;
  std::ifstream file(location.c_str());
  if (file.is_open()) {
    string total_mem;
    getline(file, total_mem);

    // remove spaces
    total_mem.erase(std::remove(total_mem.begin(), total_mem.end(), ' '),
                    total_mem.end());

    // extract number portion only
    std::string MemTotal_v = std::regex_replace(
        total_mem, std::regex("[^0-9]*([0-9]+).*"), std::string("$1"));

    string mem_free;
    getline(file, mem_free);
    mem_free.erase(std::remove(mem_free.begin(), mem_free.end(), ' '),
                   mem_free.end());

    std::string mem_free_v = std::regex_replace(
        mem_free, std::regex("[^0-9]*([0-9]+).*"), std::string("$1"));

    /* string mem_available;
    getline(file, mem_available);
    mem_available.erase(
        std::remove(mem_available.begin(), mem_available.end(), ' '),
        mem_available.end());

    string buffer;
    getline(file, buffer);
    buffer.erase(std::remove(buffer.begin(), buffer.end(), ' '), buffer.end());

    string cache;
    getline(file, cache);
    cache.erase(std::remove(cache.begin(), cache.end(), ' '), cache.end()); */

    double total_mem_double = atof(MemTotal_v.c_str());
    double mem_free_double = atof(mem_free_v.c_str());

    double tmu = total_mem_double - mem_free_double;
    file.close();

    return (tmu / total_mem_double);
  }

  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string location = LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename;
  std::ifstream file(location.c_str());
  if (file.is_open()) {
    getline(file, uptime);
    std::replace(uptime.begin(), uptime.end(), ' ', '_');
    uptime = uptime.substr(0, uptime.find('_'));
    file.close();
    auto v = atol(uptime.c_str());
    return v;
  }

  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  size_t NOT_FOUND = -1;
  string location = LinuxParser::kProcDirectory + kStatFilename;
  std::ifstream file(location.c_str());
  if (file.is_open()) {
    string read{};
    while (getline(file, read)) {
      if (read.find(kProcesses) != NOT_FOUND) {
        break;
      }
    }

    auto result = read.substr(kProcesses.length(), read.length());
    file.close();
    TotalProcesses_count = atoi(result.c_str());
    return TotalProcesses_count;
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  size_t NOT_FOUND = -1;
  string location = LinuxParser::kProcDirectory + kStatFilename;
  std::ifstream file(location.c_str());
  if (file.is_open()) {
    string read{};
    while (getline(file, read)) {
      if (read.find(kProcessesRunning) != NOT_FOUND) {
        break;
      }
    }

    auto result = read.substr(kProcessesRunning.length(), read.length());
    file.close();
    RunningProcesses_count = atoi(result.c_str());
    return RunningProcesses_count;
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }