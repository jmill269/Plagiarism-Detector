#include "../include/catch.hpp"
#include "../include/metric.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <list>

using std::string;
using std::vector;
using std::list;
using std::map;


TEST_CASE("toString and increment", "[toString],[increment]") {
  NgramCollection g3(3);

  vector<string> v;
  v.push_back("Four");
  v.push_back("score");
  v.push_back("and");
  v.push_back("seven");

  g3.increment(v.begin(), v.end()-1);
  REQUIRE(g3.toString() == "Four score and 1\n");

  g3.increment(v.begin()+1, v.end());
  REQUIRE(g3.toString() == "Four score and 1\nscore and seven 1\n");

  g3.increment(v.begin(), v.end()-1);
  REQUIRE(g3.toString() == "Four score and 2\nscore and seven 1\n");

  g3.increment(v.begin()+1, v.end());
  g3.increment(v.begin()+1, v.end());
  REQUIRE(g3.toString() == "Four score and 2\nscore and seven 3\n");
}

TEST_CASE("getN", "[getN]") {
  NgramCollection g3(3);
  NgramCollection g4(4);
  REQUIRE(g3.getN() == 3);
  REQUIRE(g4.getN() == 4);
}

TEST_CASE("toMap", "[toMap]") {
  NgramCollection g3(3);

  vector<string> v;
  v.push_back("Four");
  v.push_back("score");
  v.push_back("and");
  v.push_back("seven");

  g3.increment(v.begin(), v.end()-1);
  map<vector<string>, vector<string> > checkMap = g3.toMap();
  vector<string> four;
  four.push_back("four");
  four.push_back("score");
  vector<string> score;
  score.push_back("and");
  REQUIRE(checkMap[four] == score);
}

/***********************************************************************/


TEST_CASE("compareMaps", "[compareMaps]") {
  Metric metric(3);

  map<vector<string>, vector<string> > currentNgrams;
  vector<string> words;
  words.push_back("tim");
  vector<string> match;
  match.push_back("kim");
  currentNgrams.insert( std::pair<vector<string>, vector<string> >(words, match) );

  map<vector<string>, vector<string> > nextNgrams;
  vector<string> words2;
  words2.push_back("tim");
  vector<string> match2;
  match2.push_back("kim");
  nextNgrams.insert( std::pair<vector<string>, vector<string> >(words2, match2) );

  double check = metric.compareMaps(currentNgrams, nextNgrams);
  REQUIRE(check == 1);

  vector<string> match3;
  match3.push_back("not kim");
  nextNgrams[words2] = match3;
  double check2 = metric.compareMaps(currentNgrams, nextNgrams);
  REQUIRE(check2 == 0);
}

TEST_CASE("totalGramSize", "[totalGramSize]") {
  Metric metric(3);
  map<vector<string>, vector<string> > currentNgrams;
  vector<string> words;
  words.push_back("my");
  words.push_back("name");
  words.push_back("is");

  vector<string> match;
  match.push_back("tim");
  currentNgrams.insert( std::pair<vector<string>, vector<string> >(words, match) );

  double size = metric.totalGramSize(currentNgrams);
  REQUIRE(size == 1);
}

TEST_CASE("clamp", "[clamp]") {
  Metric metric(3);
  double over = 1.1;
  over = metric.clamp(over);
  REQUIRE(over == 1);
}
