
/*PLOVER: MISC.DFREE*/

/*
Description: malloc'd data is freed more than once.
Keywords: Size0 Complex0 Api MemMgmt DoubleFree
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

int main() {
  FILE* report;
  FILE* finalReport;
  char* line = NULL;
  size_t len = 0;
  ssize_t lineLen;

  report = fopen("report.txt", "r");
  finalReport = fopen("memory_report.txt", "w");

  if (report == NULL) {
    printf("ERROR: Unable to generate report");
    exit(EXIT_FAILURE);
  }

  fprintf(finalReport, "---------- Memory Safety Vulnerabilities Report ----------\n\n");
  fprintf(finalReport, "##########     Free Related Vulnerabilities     ##########\n");

  std::map<char*, std::string> allocations;

  while ((lineLen = getline(&line, &len, report)) != -1) {
    // If existing warning, dump in final report
    if (strstr(line, "Line") != NULL) {
      fprintf(finalReport, "%s", line);
    }

    // Record allocation/deallocation pattern 
    if (strstr(line, "Log") != NULL) {
      if (strstr(line, "Malloc") != NULL) {
        std::string metadata = "";
        char* allocation = strtok(line, " ");

        allocation = strtok(NULL, " ");
        metadata.append(allocation);
        metadata.append(" ");
        allocation = strtok(NULL, " ");
        metadata.append(allocation);

        allocation = strtok(NULL, " ");
        allocation = strtok(NULL, " ");
        allocations[allocation] = metadata;

      }

      if (strstr(line, "Free") != NULL) {
        char* deallocation = strtok(line, " ");
        deallocation = strtok(NULL,  " ");
        deallocation = strtok(NULL,  " ");
        deallocation = strtok(NULL,  " ");
        deallocation = strtok(NULL,  " ");
        if (allocations.find(deallocation) != allocations.end()) {
          allocations.erase(deallocation);
        }
      }
    }
  }
  
  // Report any remaining allocations as memory leaks
  fprintf(finalReport, "\n##########             Memory Leaks             ##########\n");
  for(std::map<char*,std::string>::iterator it=allocations.begin(); it!=allocations.end(); ++it) {
    const char* alloc_data = (it->second).c_str();
    char* alloc_data_copy = new char[(it->second).length() + 1];
    strcpy(alloc_data_copy, alloc_data);

    char* metadata = strtok(alloc_data_copy, " ");
    char* line = metadata;
    metadata = strtok(NULL, " ");
    char* col = metadata;

    fprintf(finalReport, "Line %s.%s: Warning: Memory Leak!\n", line, col);
    }
  return 0;
}

