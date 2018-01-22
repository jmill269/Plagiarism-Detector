/****************************************
 * plagiarism.cpp
 *
 * Contains main method that calls other
 * functions from other files
 *
 * Jonathan Miller
 *
 ***************************************/
#include <iostream>
#include "../include/metric.hpp"

/* function returns an int representing the n-gram size based on user-specified sensitivity */
int readSensitivity(string &sensitivity) {
  int nGramSize = 0;
  switch (tolower(sensitivity.at(0))) {
  case 'l':
    return (nGramSize = 5);
  case 'm':
    return (nGramSize = 3);
  case 'h':
    return (nGramSize = 2);
  default:
    cout << "ERROR: Invalid sensitivity argument" << endl;
  }
  return 0;
}

void printUsage() {
  cout << "USAGE: plagiarism [input file] [l|m|h]" << endl;
}

int main(int argc, char* argv[]) {
  string sens;
  if (argc == 2) {
      sens = "m";     //if no second parameter is given, use medium (n-gram of size 3) by default.
    } else if (argv[2]) {    //if there's a second parameter, store it in the string sens
      sens = argv[2];
    }

  if (argc < 2) {
    printUsage();
    return 1;
  } else {
    string input(argv[1]);
    int sensitivity = readSensitivity(sens);
    Metric metric(sensitivity);
    cout << "List of suspicious document pairs:" << endl;
    metric.readFileList(input);
  }
  return 0;
}




