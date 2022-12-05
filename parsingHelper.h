// FileParser V1.0 created by Benjamin Zastrow

// Needed to throw a std::runtime_exception
#include <stdexcept>
// Needed to handle strings
#include <string>
// Needed to handle vectors (arrays of variable size)
#include <vector>
// Needed to handle File-Input-Output
#include <fstream>

// Define a namespace that contains all the related methods and the struct
// A namespace is just a form of encapsulation for classes, structs, methods, variables, etc. that belong together
// You access the contents of a namespace using "[nameOfNamespace]::methodName(...)"
// The most prominent example is the namespace "std" of the C++ Standard Library
namespace parsingHelper {
  // Define a random data structure that contains some items
  struct dataSet {
    std::string a;
    std::string b;
    float c;
    int d;
    std::string e;
  };

  // Method definitions
  std::string readFileToString(std::string &fileName);
  std::string parseString(std::string &input);
  bool writeStringToFile(std::string &input, std::string &fileName);

  std::vector<std::string> convertMultiLineStringToVector(std::string &input);
  std::string convertVectorToMultiLineString(std::vector<std::string> &input);
}

// Method implementations

// Note: For performance reasons, we take references as arguments since copying entire objects instead of just addresses takes a lot longer
std::string parsingHelper::readFileToString(std::string &fileName) {
  // create an InputFileStream and try to open the file given at fileName 
  std::ifstream fileIn(fileName);

  // check if the file has been opened successfully, if not, throw an exeption
  if(!fileIn.is_open()) {
    throw std::runtime_error("Error opening input file!");
  }

  std::string retVal;

  // as long as the end of file has not been reached, fill the string char by char
  // we obtain the next char of the file using the get() method and store the char at the end of the string using push_back()
  while(fileIn.good()) {
    retVal.push_back(fileIn.get());
  }
  
  // close the fileStream
  fileIn.close();

  // return the finalized string
  return retVal;
}

std::string parsingHelper::parseString(std::string &input) {
  std::string retVal, tempString;

  // add the mandatory first line to the result string
  retVal = "FName\tLName\tDebt\tAge\tComment\n";

  // convert the file-String to a vector of strings, that represents single lines 
  // this makes it easier to handle the input line by line
  std::vector<std::string> lines = convertMultiLineStringToVector(input);
  dataSet tempData;
  bool flush = false;
  // work through line by line
  for(int i = 0; i < lines.size(); ++i) {
    // check the first letter of the line and copy the data to the corresponding struct member
    switch (lines[i][0]) {
      case 'a': tempData.a = lines[i].substr(3); break;
      case 'b': tempData.b = lines[i].substr(3); break;
      case 'c': tempData.c = atof(lines[i].substr(3).c_str()); break;
      case 'd': tempData.d = atoi(lines[i].substr(3).c_str()); break;
      case 'e': tempData.e = lines[i].substr(3); flush = true; break;
      default: break;
    }

    // if the end of one "block" (a-e) has been reached, the result string is appended by one more line
    // we need to use std::to_string(...) to convert integers, doubles, etc. into a string
    if(flush) {
      retVal.append(tempData.a + "\t" + tempData.b + "\t" + std::to_string(tempData.c) + "\t" + std::to_string(tempData.d) + "\t" + tempData.e + "\n");
      flush = false;
    }
  }
  return retVal;
}

bool parsingHelper::writeStringToFile(std::string &input, std::string &fileName) {
  // create an OutputFileStream and try to open the file given at fileName 
  std::ofstream fileOut(fileName);

  // check if the file has been opened successfully, if not, return false
  if(!fileOut.is_open()) {
    return false;
  }

  // write the vector element by element (equals line by line) into the file
  for(int i = 0; i < input.size(); ++i) {
    fileOut << input[i];
  }
  // finish the file off with a newline-symbol
  fileOut << "\n";

  // close the file and exit
  fileOut.close();
  return true;
}

std::vector<std::string> parsingHelper::convertMultiLineStringToVector(std::string &input) {
  std::vector<std::string> retVal;
  std::string line;
  // work through every character of the input string
  for(int i = 0; i < input.size(); ++i) {
    // add the current char to the string
    line.push_back(input[i]);

    // if the current char is a newline-symbol, remove it from the string, add the now finished line to the vector and clear the temporary line variable
    if(input[i] == '\n') {
      line.pop_back();
      retVal.push_back(line);
      line.clear();
    }
  }
  return retVal;
}

std::string parsingHelper::convertVectorToMultiLineString(std::vector<std::string> &input) {
  std::string retVal;
  // work through the vector line by line
  for(int i = 0; i < input.size(); ++i) {
    // add the current vector element (line) and finish the line off with a newline-symbol
    retVal.append(input[i] + '\n');
  }
  return retVal;
}
