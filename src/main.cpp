#include <iostream>
#include <sstream>
#include <filesystem>
#include <unordered_set>


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
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  // Uncomment this block to pass the first stage
  std::cout << "$ ";

  std::unordered_set<std::string> commands;
  commands = {"type", "echo", "exit"};
  std::string input;
  while (true) {
    std::getline(std::cin, input);
    std::istringstream iss(input);
    std::string command;
    iss >> command;
    if(command == "echo"){
      std::string argument;
      std::getline(iss >> std::ws, argument);
      std::cout << argument << std::endl;
    }

    else if(command == "type"){
      std::string arg;
      std::getline(iss >> std::ws, arg);
      if(commands.find(arg) != commands.end()) 
        std::cout << arg << " is a shell builtin" << std::endl;
      else {
        std::string paths = std::getenv("PATH");
        getCommand(paths, arg);
        // std::cout << arg << ": not found\n";
      }
    }

    else if (command == "exit" && input == "exit 0") break;
    else {
      std::cout << input << ": "  << "command not found\n";
    }
    std::cout << "$ ";
  }
}
