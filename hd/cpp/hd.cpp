#include <iostream>
#include <string>

using namespace std;

void trim(string &s);

int main() {
  string line;
  while (getline(cin, line)) {
    trim(line);
    if (line.length() != 0 && !line.starts_with("# ")) {
      cout << "# ";
    }
    cout << line << endl;
  }
}

inline void trim(string &s) {
  auto not_space = [](unsigned char ch) { return !isspace(ch); };

  s.erase(s.begin(), find_if(s.begin(), s.end(), not_space));
  s.erase(find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
}