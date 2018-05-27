#include "./vendor/csvstream/csvstream.h"
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

using std::string;
using std::set;
using std::map;
using std::cout;
using std::endl;


class Markov {
  set<string> vocab;
  map<string, std::vector<string> > chain_vocab;
  std::vector<string> chars_to_remove;
  map<string, string> chars_to_replace;

public:

  Markov() {};

  Markov(std::vector<string> &remove, std::map<string, string> &replace):
    chars_to_remove(remove), chars_to_replace(replace) {};

  void train(string in) {
    std::stringstream sin(in);
    string last_word = "";
    string word = "";

    while (sin >> word) {
      sanitize(word);

      if (word != "") {
        vocab.insert(word);

        chain_vocab[last_word].push_back(word);
        last_word = word;
      }
    }
  }

  string generate() {
    return generate(140);
  }

  string generate(unsigned int max_length) {
    string result = "";
    string s = random_word();
    while (result.size() < max_length) {
      s = next_word(s);
      if ((result.size() + s.size() + 1) < max_length) {
        result += (s + " ");
      } else {
        break;
      }
    }

    return result.substr(0, result.length() - 1);
  }

  void report() {
    for (auto word:vocab) {
      std::cout << word << "  ";
    }
    std::cout << std::endl;

    for (auto pair:chain_vocab) {
      cout << pair.first << ": ";
      for (auto word:pair.second) {
        cout << word << ", ";
      }
      cout << endl;
    }
  }

private:
  void sanitize(string &word) {
    remove_chars(word, chars_to_remove);
    replace_chars(word, chars_to_replace);

    if (word.substr(0, 4) == "http") {
      word = "";
    }

    if (word.substr(0, 1) == "@") {
      word = "";
    }
  }

  void remove_chars(string &word, std::vector<string> &list) {
    for (auto ch:list) {
      std::string::size_type i = word.find(ch);
      while (i != std::string::npos) {
        word.erase(i, ch.length());
        i = word.find(ch);
      }
    }
  }

  void replace_chars(string &word, map<string, string> &list) {
    for (auto ch:list) {
      std::string::size_type i = word.find(ch.first);
      while(i != std::string::npos) {
        word.replace(i, ch.first.length(), ch.second);
        i = word.find(ch.first);
      }
    }
  }

  string next_word(string &word) {
    if (!chain_vocab[word].empty()) {
      int index = random_index(chain_vocab[word].size());
      string value = chain_vocab[word][index];
      chain_vocab[word].erase(chain_vocab[word].begin() + index);
      return value;
    } else {
      return random_word();
    }
  }

  string random_word() {
    string word = "";
    while (word == "") {
      int index = random_index(vocab.size());
      auto it = vocab.begin();
      advance(it, index);
      word = *it;
    }
    return word;
  }

  int random_index(int container_size) {
    srand(time(NULL));
    return rand() % container_size;
  }
};
