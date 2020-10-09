#include "processor.h"
#include<vector>
#include<string>
#include "linux_parser.h"


using std::vector;
using std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 


vector<string> utilization = LinuxParser::CpuUtilization();
   vector<float> calc;
   float sum, total;
   for(auto i : utilization)
   {
       sum += atoi(i.c_str());
   }
   float idelTime = atoi( utilization[3].c_str());
   total = idelTime / sum;
   total = 1.0 - total;   

   return total;

 }