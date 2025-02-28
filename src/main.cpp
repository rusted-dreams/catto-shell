#include <iostream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>


std::string getPathEnv() {
  char* path = std::getenv("PATH");
  return path ? std::string(path) : "";
}


bool fileExists(const std::string& file){
  return std::filesystem::exists(file) && std::filesystem::is_regular_file(file);
}

void runExecutable(std::vector<std::string>& commands){
  if(commands.empty()) return;
  char* cmd = commands[0].data();
  std::vector<char*> argv;
  for(auto& arg:commands){
    argv.push_back(arg.data());
  }
  argv.push_back(nullptr);

  if(fork() == 0) {
    execvp(cmd, argv.data());
    std::cerr << cmd << ": command not found\n";
    exit(1);
  }
  wait(nullptr);
}

void getCommand(const std::string paths, std::string arg){
  std::string dir;
  std::stringstream ss(paths);
  
  while(std::getline(ss, dir, ':')){
    // search for the command in each dir from the path env var.
    for(const auto& entry: std::filesystem::directory_iterator(dir)){
      if(entry.path().filename() == arg) {
        std::string command = entry.path();
        std::cout << arg << " is " << command << std::endl;
        return;
      }
    }
  }
  std::cout << arg << ": not found" << std::endl;
}

int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  // Uncomment this block to pass the first stage
  std::cout << "$ ";
  
  std::string paths = getPathEnv();
  
  std::unordered_set<std::string> built_in;
  built_in = {"type", "echo", "exit", "pwd", "cd"};
  std::string input;
  while (true) {
    std::getline(std::cin, input);
    
    // std::cout << input << endl;
    if (input == "exit 0") break;

    std::istringstream iss(input);
    std::string command;
    std::vector<std::string> commands;
    while(iss >> command){
      commands.push_back(command);
    }
    if(commands[0] == "echo"){
      // std::string argument;
      for(int i=1; i<commands.size(); i++){
        std::cout << commands[i] << " ";
      }
      std::cout << std::endl;
      // std::cout << argument << std::endl;
    }


    else if(commands[0] == "type"){
      std::string arg = commands[1];
      if(built_in.find(arg) != built_in.end()) 
        std::cout << arg << " is a shell builtin" << std::endl;
      else {
        getCommand(paths, arg);
      }
    }

    else if(commands[0] == "pwd"){
      std::string currPath = std::filesystem::current_path();
      std::cout << currPath << std::endl;
    }

    else if(commands[0] == "cd"){
      std::filesystem::current_path(commands[1]);
    }
    
    else {
      runExecutable(commands);
    }
    std::cout << "$ ";
  }
}
