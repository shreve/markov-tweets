#include "markov.h"

using namespace std;

int main(int argc, char ** argv) {

  if (argc != 2) {
    cout << "Usage: tweeter TWEET_SOURCE" << endl << endl;
    cout << "  TWEET_SOURCE must be a csv-formatted file" << endl;
    exit(EXIT_FAILURE);
  }

  try {
    csvstream tweets(argv[1]);
    map<string, string> row;
    vector<string> strip = {"\"", "“", "”", ")", "(", ",", ".", "?"};
    map<string, string> replace;
    replace["&lt;"] = "<";
    replace["&gt;"] = ">";
    Markov corpus(strip, replace);
    tweets >> row;

    while(tweets >> row) {
      string text = row["text"];
      corpus.train(row["text"]);
    }

    // corpus.report();
    cout << corpus.generate() << endl;

  } catch (csvstream_exception e) {
    std::cout << e.msg << std::endl;
  }
}
