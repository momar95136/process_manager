#include "process.h"

#include <sys/time.h>
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

// DONE: Return this process's ID
int Process::Pid() { return pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  auto stat = GetProcessUpTimeInfo();
  auto upTime = LinuxParser::UpTime();
  auto utime = stof(stat[13].c_str());
  auto stime = stof(stat[14].c_str());
  auto cuttime = stof(stat[15].c_str());
  auto ctime = stof(stat[16].c_str());
  auto starttime = stof(stat[21].c_str());

  auto total_time = utime + stime + cuttime + ctime;

  auto seconds = upTime -(starttime / sysconf(_SC_CLK_TCK));
  this->cpuUsage =  (total_time / sysconf(_SC_CLK_TCK))/seconds;
   
  return this->cpuUsage;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// DONE: Return this process's memory utilization
string Process::Ram() {
  this->ram = LinuxParser::Ram(pid);
  return this->ram;
}

// DONE: Return the user (name) that generated this process
string Process::User() {
  this->user = LinuxParser::User(pid);
  return user;
}

void Process::init() {
  User();
  Ram();
  Command();
  CpuUtilization();
  UpTime();
}

vector<string> Process::GetProcessUpTimeInfo() {
  string command;
  vector<string> stat;
  std::istringstream ss;
  string processLocation = LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatFilename;
  std::ifstream stream(processLocation.c_str());
  if (stream.is_open()) {
    getline(stream, command);
    stream.close();
    ss.str(command);
    string token;
    while (ss >> token) {
      stat.push_back(token);
    }
  }

  return stat;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  auto stat = GetProcessUpTimeInfo();
  auto value = atol(stat[21].c_str());
  auto uptime = value / sysconf(_SC_CLK_TCK);
  this->upTime = uptime;
  return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  
    return this->cpuUsage < a.cpuUsage;

 }