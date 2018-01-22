/*****************************************
 * metric.cpp
 * Jonathan Miller
 *
 * File deals with reading files, storing the words
 * and creating ngrams then comparing them.
 *
 *****************************************/

#include "../include/metric.hpp"
#include <iostream>
#include <fstream>
#include <locale>


/* read a list of names from a file, add each document to the model */
void Metric::readFileList(string filename) {
  vector<string> filePaths = readNames(filename);

  vector<string>::iterator it;
  for (it = filePaths.begin(); it != --filePaths.end(); ++it) {
    //check if that file has already been encountered
    //if not, add it as a key to the "encountered" collection
    //guaranteed for first pass of program
    if (prevSimilarity.find(*it) == prevSimilarity.end()) {
      prevSimilarity[*it];

      NgramCollection current(ngrams.getN());
      map<vector<string>, vector<string> > currentNgrams;
      addTextFromFile(*it, current, &currentNgrams);
      prevSimilarity[*it] = currentNgrams;

      //check current file to rest of files in the file paths list
      for (vector<string>::iterator sit = next(it, 1); sit != filePaths.end(); ++sit) {
         //add next file to encountered collection if not encountered yet
	       if (prevSimilarity.find(*sit) == prevSimilarity.end()) {
	         prevSimilarity[*sit];
	       }

	       NgramCollection next(ngrams.getN());
	       map<vector<string>, vector<string> > nextNgrams;
	       addTextFromFile(*sit, next, &nextNgrams);
	       prevSimilarity[*sit] = nextNgrams;

	       double percentSimilarity = compareMaps(currentNgrams, nextNgrams);
         percentSimilarity = clamp(percentSimilarity);
         //if similarity is greater than 20%, flag as possible plaigarism
         printSimilarity(*it, *sit, percentSimilarity);
      }

    //Case where file has ALREADY been encountered so it's NgramCollection was
    //stored in the encountered collection
    } else {
      //get NgramCollection from encountered collection
      map<vector<string>, vector<string> > currentNgrams;
      currentNgrams = prevSimilarity[(*it)];

      for (vector<string>::iterator sit = next(it, 1); sit != filePaths.end(); ++sit) {
        //this if block should never be entered as all files should have been
        //encountered at this point. here just in case
	      if (prevSimilarity.find(*sit) == prevSimilarity.end()) {
	        prevSimilarity[*sit];
	        NgramCollection next(ngrams.getN());
	        map<vector<string>, vector<string> > nextNgrams;
	        addTextFromFile(*sit, next, &nextNgrams);
	        prevSimilarity[*sit] = nextNgrams;

	        double percentSimilarity = (compareMaps(currentNgrams, nextNgrams));
          percentSimilarity = clamp(percentSimilarity);
          printSimilarity(*it, *sit, percentSimilarity);

        //grab appropriate NgramCollection from encountered collection and compare
	      } else {
	        map<vector<string>, vector<string> > nextNgrams;
	        nextNgrams = prevSimilarity[(*sit)];
	        double percentSimilarity = (compareMaps(currentNgrams, nextNgrams));
          percentSimilarity = clamp(percentSimilarity);
	        printSimilarity(*it, *sit, percentSimilarity);
	      }
      }
    }
  }
  printReport();
}

void Metric::printReport() {
  int susDocCount = printSimilarity("", "", 0.0);
  cout << "============================" << endl;
  cout << "          SUMMARY\n============================" << endl;
  cout << susDocCount << " suspicious document pairs detected." << endl;
}

/* read a list of names from a file */
vector<string> Metric::readNames(string fileList) {
  vector<string> filePaths;
  ifstream input(fileList); // try to open file
  if (!input.is_open()) { // see if it worked
    cerr << "Error: failed to open file '" << fileList << "'\n";
    exit(-1);
  }

  string file;
  while (input >> file) {
    filePaths.push_back(file); // read names until we run out of file
  }
  input.close(); // close the file
  return filePaths;
}

/* add text from the given document file to the model */
void Metric::addTextFromFile(string filename, NgramCollection nGrams, map<vector<string>, vector<string> > *nGramList) {
  ifstream input(filename);
  std::locale loc;

  if (!input.is_open()) { // make sure it opened
    cerr << "Error: could not open file '" << filename << "', file will be skipped...\n";
    return;
  }

  string word;
  vector<string> text; // the document will be stored here
  for (unsigned i=1; i<ngrams.getN(); i++) {
    text.push_back("<START_" + to_string(i) + ">"); // add n-1 start tags
  }

  while (input >> word) {
    if (word == "a" || word == "the") {
    } else {
      text.push_back(word); // read words from the file and add them to the document
    }
  }

  for (unsigned i=1; i<ngrams.getN(); i++) {
    text.push_back("<END_" + to_string(i) + ">"); // add n-1 end tags
  }
  input.close(); // close the file

  buildNgrams(text, nGrams, nGramList); // add ngrams from the document to the model

}

/* takes a document (as a vector of words, including start/end tags), and
 * adds each n-gram to the model */
void Metric::buildNgrams(const vector<string> &text, NgramCollection nGrams, map<vector<string>, vector<string> > *nGramList) {
  auto first = text.begin(); // first element
  auto last = text.begin() + nGrams.getN(); // n-1th element

  while (last-1 != text.end()) {
    nGrams.increment(first, last); // add ngrams until we run out
    ++first;
    ++last;
  }

  *(nGramList) = nGrams.toMap();
}

/* takes vector representations of nGramCollections and compares them by percentage of matching nGrams */
double Metric::compareMaps(map<vector<string>, vector<string> > currentNgrams, map<vector<string>, vector<string> > nextNgrams) {
  double similarityCount = 0.0;
  double totalGramsCurrent = 0.0;
  double totalGramsNext = 0.0;

  totalGramsCurrent = totalGramSize(currentNgrams);
  totalGramsNext = totalGramSize(nextNgrams);

  for (auto &pair : currentNgrams) {
    map<vector<string>, vector<string> >::iterator compare = nextNgrams.find(pair.first);
    if (compare != nextNgrams.end()) {
      for (auto &word : pair.second) {
	      transform(word.begin(), word.end(), word.begin(), ::tolower);
	      for (auto &match : compare->second) {
	        transform(match.begin(), match.end(), match.begin(), ::tolower);
	        if (word == match) {
	           similarityCount++;
	        }
	      }
      }
    }
  }

  if (totalGramsCurrent > totalGramsNext) {
    return (similarityCount / totalGramsCurrent);
  } else {
    return (similarityCount / totalGramsNext);
  }
}

double Metric::totalGramSize(map<vector<string>, vector<string> > currentNgrams) {
  double totalGrams = 0.0;
  for (auto &pair : currentNgrams) {
    totalGrams += (pair.second).size();
  }
  return totalGrams;
}

int Metric::printSimilarity(string file, string match, double percentage) {
  static int susCount = 0;   //keep track of the number of 'suspicious' file pairs.

  std::cout.precision(2);   //set decimal precision for printing similarity percentage

  if (file == "" && match == "" && percentage == 0.0) {
    return susCount;
  }
  int sensInt = ngrams.getN();
  switch (sensInt) {
    case 5:
      if (percentage >= 0.2) {
        cout << file << '\n' << match << std::fixed;
        cout << '\n' << percentage*100 << "% similarity" << endl;
        cout << "-----------------------" << endl;
        susCount++;
        break;
      }

    case 3:
      if (percentage >= 0.18) {
        cout << file << '\n' << match << std::fixed;
        cout << '\n' << percentage*100 << "% similarity" << endl;
        cout << "-----------------------" << endl;
        susCount++;
        break;
      }

    case 2:
      if (percentage >= 0.15) {
        cout << file << '\n' << match << std::fixed;
        cout << '\n' << percentage*100 << "% similarity" << endl;
        cout << "-----------------------" << endl;
        susCount++;
        break;
      }

    default:
      return 0;
  }

  return 0;
}

double Metric::clamp(double similarityPercentage) {
  if (similarityPercentage > 1) {
    similarityPercentage = 1;
  }
  return similarityPercentage;
}

/* take iterators to the start/end of an Ngram, increment the appropriate
 * element of the map */
void NgramCollection::increment(vector<string>::const_iterator begin,
				vector<string>::const_iterator end) {
  assert(end - begin == n); // make sure we've got the right number of words
  string last = *(end-1); // pull off the word at the end
  vector<string> words; // make a vector containing the first n-1 words
  for (auto i = begin; i != end-1; ++i) {
    words.push_back(*i);
  }
  counts[words][last] += 1; // increment the corresponding count
}

/* concatenate each key in nGram collection with its matching value and store into a vector of strings
 */
map<vector<string>, vector<string> > NgramCollection::toMap() {
  map<vector<string>, vector<string> > ngram;
  for (auto &pair : counts) {
    vector<string> currentWord;
    for (string word : pair.first) {
      transform(word.begin(), word.end(), word.begin(), ::tolower);
      currentWord.push_back(word);
    }
    ngram[currentWord];
    for (auto &match : pair.second) {
      string matchingWord = match.first;
      transform(matchingWord.begin(), matchingWord.end(), matchingWord.begin(), ::tolower);
      ngram[currentWord].push_back(matchingWord);
    }
  }
  return ngram;
}

//exists only to test increment function
string NgramCollection::toString() const {
  std::stringstream ss;
  for (auto &i : counts) {
    for (auto &end : i.second) {
      for (auto &word : i.first) {
        ss << word << " ";
      }
      ss << end.first << " " << end.second << "\n";
    }
  }
  return ss.str();
}
