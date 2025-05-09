#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <sstream>
#include <string> 

using namespace std;    

TEST(test_stub) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty()); 
    tree.insert(5);
    tree.insert(7); 
    tree.insert(8); 
    tree.insert(1); 
    ASSERT_FALSE(tree.empty()); 
    ASSERT_EQUAL(tree.size(), 4);
    ASSERT_EQUAL(tree.height(), 3);
    ASSERT_TRUE(tree.find(8) != tree.end());

    tree.insert(3);
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.max_element() == 8);
    ASSERT_TRUE(*tree.min_element() == 1);
    BinarySearchTree<int> tree2 = tree; 
    ASSERT_TRUE(tree2.check_sorting_invariant()); 
    ASSERT_TRUE(*tree2.max_element() == 8);
    ASSERT_TRUE(*tree2.min_element() == 1);
    *tree2.begin() = 6; 
    ASSERT_EQUAL(*tree.begin(), 1); 
    ASSERT_EQUAL(*tree.min_greater_than(7), 8);
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_EQUAL(*tree.min_greater_than(3), 5);
    ASSERT_EQUAL(*tree.min_greater_than(4), 5);
    ASSERT_EQUAL(tree.min_greater_than(8), tree.end());
    ASSERT_EQUAL(*tree.min_greater_than(0), 1);
    ASSERT_EQUAL(*tree.min_greater_than(1), 3);
}
TEST(bst_min){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.min_element() == tree.end()); 
    ASSERT_TRUE(tree.max_element() == tree.end()); 
    tree.insert(5); 
    tree.insert(2); 
    ASSERT_TRUE(tree.check_sorting_invariant());
    tree.insert(10); 
    tree.insert(1); 
    tree.insert(3); 
    ASSERT_TRUE(tree.check_sorting_invariant());
    tree.insert(8);
    tree.insert(15); 
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_EQUAL(*tree.min_element(), 1);
    ASSERT_EQUAL(*tree.max_element(), 15);
    BinarySearchTree<int> tree2;
    tree2.insert(5); 
    tree2.insert(8); 
    tree2.insert(10);
    ASSERT_TRUE(tree2.check_sorting_invariant()); 
    ASSERT_EQUAL(tree2.height(), 3); 
    ASSERT_EQUAL(*tree2.min_element(), 5); 
    BinarySearchTree<int> tree3; 
    tree3.insert(5); 
    tree3.insert(3); 
    tree3.insert(1); 
    ASSERT_TRUE(tree3.check_sorting_invariant());
    ASSERT_EQUAL(tree3.height(), 3); 
    ASSERT_EQUAL(*tree3.max_element(), 5); 
}

TEST(bst_public_test) {
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);

  ASSERT_TRUE(tree.find(5) != tree.end());

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 7);
  ASSERT_TRUE(*tree.min_element() == 3);
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);

  cout << "cout << tree.to_string()" << endl;
  cout << tree.to_string() << endl << endl;

  cout << "cout << tree" << endl << "(uses iterators)" << endl;
  cout << tree << endl << endl;

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  cout << "preorder" << endl;
  cout << oss_preorder.str() << endl << endl;
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  cout << "inorder" << endl;
  cout << oss_inorder.str() << endl << endl;
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");

  *tree.begin() = 10; 
  ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(ASF){
    BinarySearchTree<int> b;
    b.insert(1);
    b.insert(0);
    // change first datum to 2, resulting in the first broken tree above
    *b.begin() = 2;
    ASSERT_FALSE(b.check_sorting_invariant());
}


TEST_MAIN()
