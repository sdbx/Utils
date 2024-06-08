#include "hhss_cpp.h"
#include "generator.h"
#include "random_formatting_map.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    cout << "missing arguments\n"
         << "usage: ./hhss_cpp hsr.dat usr.dat NUM_SENTENCES\n";
    return 1;
  }

  locale::global(locale(".UTF-8"));

  ifstream fs_sentences(argv[1]);
  if (!fs_sentences.is_open()) {
    cout << "failed to open file '" << argv[1] << "'.\n";
    return 1;
  }
  ifstream fs_users(argv[2]);
  if (!fs_sentences.is_open()) {
    cout << "failed to open file '" << argv[1] << "'.\n";
    return 1;
  }

  generator<string> sentences;
  random_formatting_map formatter;

  string s;
  while (getline(fs_sentences, s)) {
    if (s.empty() || s[0] == '#') {
      continue;
    }
    sentences.push(s);
  }
  while (getline(fs_users, s)) {
    if (s.empty() || s[0] == '#') {
      continue;
    }
    formatter.put("user", s);
  }

  const auto num_sentences = stoull(argv[3]);
  for (size_t i = 0; i < num_sentences; ++i) {
    cout << formatter(sentences.next()) << '\n';
  }

  return 0;
}
