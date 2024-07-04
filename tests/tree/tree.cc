#include "../../third_party/catch.hpp"
#include "../../src/classes/tree/tree.h"
#include <string>

TEST_CASE("testing insertion in tree class [TREECLASS]"){
  tree<int> t;
  t.insert("", 10);
  t.insert("r", 15);
  t.insert("l", 20);

  REQUIRE(t.search(20) == true);
}

TEST_CASE("testing inorder traversal in tree class [TREECLASS]"){
  tree<char> t;
  t.insert("", 'c');
  t.insert("l", 'w');
  t.insert("r", 'd');

  std::vector<char> v = t.inorder();
  std::vector<char> check = {'w','c','d'};
  REQUIRE(check == v);
}

TEST_CASE("testing iterators for tree class [TREE]"){
  tree<std::string> t;
  t.insert("", "hello");
  t.insert("r", "world");
  t.insert("l", "universe");
  std::vector<std::string> v;
  for(auto it = t.begin(); it != t.end(); it++){
    v.push_back(*(it));
  }
  std::vector<std::string> check = {"universe", "hello", "world"};
  REQUIRE(check == v);
}

TEST_CASE("testing level order traversal in tree class [TREECLASS]"){
  tree<int> t;
  t.insert("", 1);
  t.insert("l", 2);
  t.insert("r", 3);
  t.insert("ll", 4);
  t.insert("lr", 5);
  t.insert("rl", 6);
  t.insert("rr", 7);
  t.insert("lll", 8);
  t.insert("llr", 9);
  t.insert("lrl", 10);
  std::vector<std::vector<int>> produced = t.level_order();
  std::vector<std::vector<int>> sol = {{1},{2,3},{4,5,6,7},{8,9,10}};
  REQUIRE(produced == sol);
}
