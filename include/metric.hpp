#ifndef _METRIC_HPP
#define _METRIC_HPP

/*****************************************
 * metric.hpp
 * Jonathan Miller
 * Header file for Metric class
 *
 *****************************************/

#include <vector>
#include <list>
#include <map>
#include <string>
#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <cstring>

using std::cout;
using std::string;
using std::endl;
using std::vector;
using std::map;
using std::ifstream;
using std::cerr;
using std::to_string;

class NgramCollection {
public:
  NgramCollection(unsigned num) : n(num) { srand(time(NULL)); }
  void increment(vector<string>::const_iterator begin,
		 vector<string>::const_iterator end);
  unsigned getN() const { return n; }
  map<vector<string>, vector<string> > toMap();
  string toString() const;
private:
  //the collection of entries in this NgramCollection
  map<vector<string>, map<string, unsigned> > counts;

  //the number of items in our NgramCollection
  unsigned n;
};

class Metric {
public:

  Metric(int n) : ngrams(n) { assert(n > 1); }

  void readFileList(string filename);
  void addTextFromFile(string filename, NgramCollection nGrams, map<vector<string>, vector<string> > *nGramList);
  double compareMaps(map<vector<string>, vector<string> > currentNgrams, map<vector<string>, vector<string> > nextNgrams);
  double totalGramSize(map<vector<string>, vector<string> > currentNgrams);
  int printSimilarity(string file, string match, double percentage);
  double clamp(double similarityPercentage);
  void printReport();

private:
  vector<string> readNames(string fileList);
  void buildNgrams(const vector<string> &text, NgramCollection nGrams, map<vector<string>, vector<string> > *nGramList);

  NgramCollection ngrams;
  map<string, map<vector<string>, vector<string> > > prevSimilarity;
};


#endif
