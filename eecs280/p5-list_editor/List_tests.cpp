#include "List.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Add your test cases here

TEST(test_list_ctors) {
    List<int> list;
    ASSERT_TRUE(list.empty());
    ASSERT_EQUAL(list.size(), 0); 
    for (int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    List<int> list2(list); 
    ASSERT_EQUAL(list2.size(), 5);
    int j = 0;
    for (List<int>::Iterator it = list.begin(); it != list.end(); ++it, j++) {
        ASSERT_EQUAL(*it, j);
    }
    list2.push_back(6); 
    list2.push_front(10);
    ASSERT_EQUAL(list.back(), 4);
    ASSERT_EQUAL(list.size(), 5);
    ASSERT_EQUAL(list2.size(), 7);
    list2 = list; 
    ASSERT_EQUAL(list2.size(), 5);
    list.pop_front();
    ASSERT_EQUAL(list2.size(), 5);
    ASSERT_EQUAL(*list2.begin(), 0);
    List<int> list3 = list2; 
    ASSERT_EQUAL(list3.size(), 5);
    ASSERT_EQUAL(list3.back(), 4);
    
    j = 0;
    for (List<int>::Iterator it = list2.begin(); it != list2.end(); ++it, j++) {
        ASSERT_EQUAL(*it, j);
    }
}

TEST(test_fncs) {
    List<int> list;
    ASSERT_TRUE(list.empty()); 
    for (int i = 0; i < 5; ++i) {
        list.push_back(i);
        ASSERT_EQUAL(list.front(), 0);
        ASSERT_EQUAL(list.back(), i);
        ASSERT_EQUAL(i+1, list.size()); 
    }
    int j = 0;
    for (List<int>::Iterator it = list.begin(); it != list.end(); ++it, ++j) {
        ASSERT_EQUAL(*it, j);
    }
    ASSERT_EQUAL(j, list.size());
    ASSERT_FALSE(list.empty());
    for (int i = 0; i < 5; i++){
        ASSERT_EQUAL(list.front(), i); 
        ASSERT_EQUAL(list.size(), 5-i); 
        list.pop_front(); 
    } 
    ASSERT_TRUE(list.empty());
    ASSERT_EQUAL(list.size(), 0);
    list.clear(); 
    ASSERT_TRUE(list.empty()); 
    ASSERT_EQUAL(list.size(), 0); 
    List<int> list2;
    for (int i = 5; i > 0; i--){
        list2.push_front(i);
        ASSERT_EQUAL(list2.front(), i);
        ASSERT_EQUAL(list2.back(), 5);
    }
    for (int i = 0; i < 5; i++) {
        ASSERT_EQUAL(list2.front(), 1); 
        ASSERT_EQUAL(list2.size(), 5-i);  
        ASSERT_EQUAL(list2.back(), 5-i);     
        list2.pop_back();
    }
    ASSERT_EQUAL(list2.size(), 0);
    list2.push_back(10);
    ASSERT_EQUAL(list2.front(), list2.back());
    list2.clear();
    ASSERT_EQUAL(list2.size(), 0);
    ASSERT_TRUE(list2.empty());
    list.clear(); 

}

TEST(test_backward_traversal) {
    List<int> list;
    for (int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    int j = list.size();
    List<int>::Iterator it = list.end();
    do {
        --it;
        --j;
        ASSERT_EQUAL(*it, j);
    }
    while(it != list.begin());
    ASSERT_EQUAL(j, 0);
}

TEST(test_iterator){
    List<int> list; 
    list.push_back(1);
    List<int> list2; 
    List<int>::Iterator front;
    List<int>::Iterator back;
    ASSERT_TRUE(front == back);
    ASSERT_FALSE(front != back);
    back = list.end();
    ASSERT_FALSE(front == back);
    ASSERT_TRUE(front != back);
    front = list.end();
    ASSERT_TRUE(front == back);
    front = list.begin();
    ASSERT_FALSE(front == back);
    ASSERT_TRUE(front != back);
    front = list2.begin(); 
    for (int i = 5; i > 0; i--){
        front = list2.insert(front, i); 
        ASSERT_EQUAL(list2.front(), *front); 
        ASSERT_EQUAL(list2.size(), 6-i);
        ASSERT_EQUAL(list2.back(), 5);
        ASSERT_EQUAL(front, list2.begin()); 
    }
    for (int i = 0; i < 5; i++){
        ASSERT_EQUAL(list2.back(), 5);
        ASSERT_EQUAL(front, list2.begin()); 
        ASSERT_EQUAL(list2.front(), *front); 
        ASSERT_EQUAL(list2.size(), 5-i);
        front = list2.erase(front);
    }
    ASSERT_EQUAL(front, list2.end());
    list.clear();
    front = list.begin(); 
    ASSERT_TRUE(front == list.end());
    front = list.insert(front, 5); 
    ASSERT_TRUE(front == list.begin()); 
    front = list.insert(list.end(), 10); 
    ASSERT_TRUE(front == --list.end());
    front = list.erase(front); 
    ASSERT_TRUE(front == list.end());
    list.clear(); 
    list.push_back(1);
    list.push_back(3); 
    front = list.begin(); 
    front++; 
    ASSERT_FALSE(front == list.end());
    ASSERT_EQUAL(*front, 3); 
    front = list.insert(front, 2); 
    ASSERT_EQUAL(*front, 2);
    front--; 
    ASSERT_EQUAL(list.size(), 3); 
    ASSERT_EQUAL(*front, 1);
    ASSERT_EQUAL(list.front(), 1);
    ASSERT_EQUAL(list.back(), 3); 
}
TEST(test_iterator2){
    List<int> list; 
    List<int>::Iterator front;
    list.push_back(1);
    list.push_back(2);
    front = list.end(); 
    front--;  
    front = list.erase(front); 
    ASSERT_EQUAL(list.size(), 1);
    ASSERT_EQUAL(list.front(), 1);
    ASSERT_EQUAL(list.back(), 1);   
    ASSERT_TRUE(front == list.end());
    list.push_back(2);
    list.push_back(3); 
    front = list.begin(); 
    front++;
    front =list.erase(front); 
    ASSERT_EQUAL(list.size(), 2); 
    ASSERT_EQUAL(*front, 3);
    ASSERT_EQUAL(list.front(), 1);
    ASSERT_EQUAL(list.back(), 3); 
    front--; 
    front = list.erase(front); 
    ASSERT_EQUAL(*front, 3);
    list.clear(); 
    list.push_back(1);
    list.push_back(2);
    list.push_back(3); 
    front = list.begin(); 
    List<int>::Iterator it(front); 
    List<int>::Iterator it2 = front; 
    it++;
    ASSERT_EQUAL(*front, 1); 
    ASSERT_EQUAL(*it, 2); 
    ASSERT_EQUAL(*it2, 1); 
    it2 = ++it;
    ASSERT_EQUAL(*it2, 3);
    it2 = front++;
    ASSERT_EQUAL(*it2, 1); 
}

TEST(same_list_assign){
    List<int> list; 
    List<int> &list2 = list; 
    list.push_back(2);
    list.push_back(3); 
    list2 = list; 
    ASSERT_EQUAL(list.size(), 2); 
    ASSERT_EQUAL(list.front(), 2); 
}


TEST_MAIN()