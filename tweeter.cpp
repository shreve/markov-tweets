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

    vector<string> strip = {"\"", "“", "”", ")", "(", ",", ".", "?"};
    map<string, string> replace = {{"&lt;", "<"}, {"&gt;", ">"}, {"&amp;", "&"}};
    Markov corpus(strip, replace);

    map<string, string> row;
    tweets >> row;

    while(tweets >> row) {
      corpus.train(row["text"]);
    }

    // corpus.report();
    cout << corpus.generate() << endl;

  } catch (const csvstream_exception &e) {
    std::cout << e.msg << std::endl;
  }
}
