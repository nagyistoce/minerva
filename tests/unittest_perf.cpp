#include <minerva.h>
#include <iostream>
#include <gtest/gtest.h>

using namespace std;
using namespace minerva;

TEST(PerfTest, LotsOfUnusedNArray) {
  vector<NArray> narrs;
  for(int i = 0; i < 1000; ++i) {
    narrs.push_back(NArray::Constant({10, 10}, i, {2, 2}));
  }
  for(int i = 0; i < 1000; ++i) {
    narrs[i] = narrs[i] * 100 + 1;
  }
  for(int i = 0; i < 1000; ++i) {
    //cout << "eval narry#" << i << endl;
    narrs[i].Eval();
  }
}