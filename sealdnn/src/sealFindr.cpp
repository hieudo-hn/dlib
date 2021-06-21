#include <string>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <libgen.h>
#include <string.h>

void configRead(std::string *);
void executeCommand(const std::string, const std::string, const std::string);

int main(int argc, char **argv)
{
  std::string configargs[3];
  std::string dirpath, xmlFile, model;

  //if you run system call ./program XMLFILE MODEL PATH
  if (argc >= 3)
  {
    dirpath = std::string(argv[3]);
    model = std::string(argv[2]);
    xmlFile = std::string(argv[1]);
  }
  //if you run system call ./program
  else if (argc == 1)
  {
    std::cout << "Reading from config" << std::endl;
    configRead(configargs);
    dirpath = configargs[2];
    model = configargs[1];
    xmlFile = configargs[0];
  }
  else
  {
    std::cout << "Run ./program to read from config\nOr ./program YOURXMLFILE.xml YOURFOLDER1 YOURFOLDER2 ..." << std::endl;
  }

  std::string delimiter = " ";
  size_t i;
  while ((i = dirpath.find(delimiter)) != std::string::npos)
  {
    const std::string dpath = dirpath.substr(0, i);
    executeCommand(xmlFile, model, dpath);
    dirpath.erase(0, i + 1);
  }
  executeCommand(xmlFile, model, dirpath);

  return 0;
}

/**
 * Execute the system call command
 */
void executeCommand(const std::string xmlFile, const std::string model, const std::string dpath)
{
  std::string arguments = "./seal " + xmlFile + " " + model + " " + dpath;
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(dpath.c_str())) != NULL) {
  /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
      //printf ("%s/%s\n", dpath.c_str(), ent->d_name);
      arguments += " ";
      arguments += std::string(ent->d_name);
      }
    }
    closedir (dir);
  } else {
  /* could not open directory */
    perror ("Could not open");
  }

  //std::cout << arguments << std::endl;
  //adding photo files in PATH to the arguments
  //calling the executable to chip faces
  system(arguments.c_str());
}

/**
 * Read configuration from config.txt file
 */
void configRead(std::string *config)
{
  std::string line;
  std::ifstream configfile("config.txt");
  int counter = 0;
  if (configfile.is_open())
  {
    while (getline(configfile, line))
    {
      std::string arg = line.substr(line.find('=') + 1);
      config[counter] = arg;
      counter++;
    }
    configfile.close();
  }

  else
    std::cout << "Unable to open file" << std::endl;
}



