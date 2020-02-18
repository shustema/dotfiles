#include <bitset>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

using b16 = bitset<16>;

int main() {

  // check for qm
  ifstream qmc("qm.cc");
  if (qmc.fail()) {
    cout << "qm.cc must be in parent directory" << endl;
    return 1;
  }
  qmc.close();

  ifstream qm("qm");
  if (qm.fail())
    system("g++ qm.cc -o qm");
  qm.close();

  for (auto N : {2, 4}) {
    vector<int> outputs;
    for (b16 i(0); i.to_ulong() < pow(2, N);
         i = b16(i.to_ulong() + 1ULL))
      for (b16 j(0); j.to_ulong() < pow(2, N);
           j = b16(j.to_ulong() + 1ULL))
        for (bool k : {false, true}) {
          bool ret = k;
          for (int depth = 0; depth < N; depth++)
            ret = (i[depth] and j[depth]) or
                  (ret and i[depth]) or (ret and j[depth]);
          if (ret)
            outputs.push_back((i.to_ulong() << (N + 1)) +
                              (j.to_ulong() << 1) + k);
        }

    // open file stream
    filebuf fb;
    fb.open("../in/" + to_string(N) + "_bit.txt",
            ios::out);
    ostream os(&fb);

    // make header
    os << to_string(N) + "_bit\n";
    for (string s : {"A", "B"})
      for (int i = N - 1; i >= 0; i--)
        os << s + "(" + to_string(i) + ") ";
    os << "Cin\n";

    // add data
    for (auto i : outputs)
      os << i << "\n";
    fb.close();

    // run QM
    string sys_call = "./qm ../in/" + to_string(N) +
                      "_bit.txt > ../out/" + to_string(N) +
                      "out.txt";
    system(sys_call.c_str());
  }

  return 0;
}
