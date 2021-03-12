#include <iostream>
#include <string>

std::string translate(std::string line, std::string argument){
  std::string result = ""; 
  
  // Drie ASCII waarden naar rechts
  if (argument == "d"){
    for (unsigned int i=0; i < line.size(); i++){
      result += line[i] + 3;
    }
  }

  // Twee ASCII waarden naar links
  else if (argument == "t"){
    for (unsigned int j=0; j < line.size(); j++){
      result += line[j] - 2;
    }
  }  

  return result; }

int main(int argc, char *argv[])
// argc is het aantal flags
// argv is een array met de daadwerkelijke argumenten
{ std::string line;

  if(argc != 2)
  { std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1; }

  while(std::getline(std::cin, line))
  { std::cout << translate(line, argv[1]) << std::endl; } 

  return 0; }