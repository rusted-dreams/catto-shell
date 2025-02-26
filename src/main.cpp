#include <iostream>
#include <sstream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  std::cout << "$ ";

  std::string input;
  while (true) {
    std::getline(std::cin, input);
    std::istringstream iss(input);
    std::string command;
    iss >> command;
    if(command == "echo"){
      std::string argument;
      std::getline(iss, argument);
      std::cout << argument << std::endl;
    }
    else if (input == "exit 0") break;
    else {
      std::cout << input << ": "  << "command not found\n";
    }
    std::cout << "$ ";
  }
}
