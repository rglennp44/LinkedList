// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/LinkedList.h"

#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

class test_LinkedList : public ::testing::Test {
protected:
	static void SetUpTestCase(){
	}

	// This function runs only once before any TEST_F function
	static void TearDownTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open())
		outgrade.clear();

		outgrade << (int)std::ceil(100*total_grade/max_grade);
		outgrade.close();

		std::cout << "Total Grade is : " << (int)std::ceil(100*total_grade/max_grade) << std::endl;
	}

	// This function runs after all TEST_F functions have been executed
	void add_points_to_grade(int points){
		max_grade += points;
		if(!::testing::Test::HasFailure()){
			total_grade += points;
		}
	}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after ever TEST_F function
	void TearDown() override {}
	
	static int total_grade;
	static int max_grade;
};

int test_LinkedList::total_grade = 0;
int test_LinkedList::max_grade = 0;

TEST_F(test_LinkedList, TestInitialization){
  LinkedList mylist;
  EXPECT_FALSE(mylist.GetTop()); // expect top to be NULL
  add_points_to_grade(1);
  shared_ptr<node> root = mylist.InitNode(42);
  EXPECT_TRUE(root);  // expect root itself to have a pointer (not NULL)
  add_points_to_grade(1);
  EXPECT_EQ(42, root->data);
  add_points_to_grade(1);
  EXPECT_FALSE(root->next); // expect next pointer to be null
  add_points_to_grade(1);
}

shared_ptr<node> build_three_node_list_helper(int one, int two, int three) {
  shared_ptr<node> top(new node);
  top->data = one;
  top->next = shared_ptr<node>(new node);
  top->next->data = two;
  top->next->next = shared_ptr<node>(new node);
  top->next->next->data = three;
  top->next->next->next = NULL;
  return top;
}

TEST_F(test_LinkedList, TestReport){
  LinkedList mylist;

  string exp("");
  EXPECT_EQ("",mylist.Report());
  add_points_to_grade(2);

  // create a three node list and test the report result
  shared_ptr<node> top = build_three_node_list_helper(1, 2, 3);
  // replace mylist top_ptr_ with this new top
  mylist.SetTop(top);
  EXPECT_EQ("1 2 3 ",mylist.Report());
  add_points_to_grade(2);
}

TEST_F(test_LinkedList, TestAppendData){
  LinkedList mylist;
  shared_ptr<node> top = build_three_node_list_helper(42, 74, 51);
  mylist.SetTop(top);
  mylist.AppendData(10);
  // get a cursor for appended data
  shared_ptr<node> cursor = mylist.GetTop()->next->next;
  EXPECT_TRUE(cursor);
  add_points_to_grade(1);

  EXPECT_FALSE(cursor->next); // expect to be NULL
  add_points_to_grade(1);

  EXPECT_EQ(10,cursor->data);
  add_points_to_grade(1);

  // try appending one more node
  mylist.AppendData(102);
  // update cursor to point to fifth member
  cursor = mylist.GetTop()->next->next->next;
  EXPECT_TRUE(cursor);  // expect not to be NULL
  add_points_to_grade(1);

  EXPECT_FALSE(cursor->next); // expect to be NULL
  add_points_to_grade(1);

  EXPECT_EQ(102,cursor->data);
  add_points_to_grade(1);
}

TEST_F(test_LinkedList, TestAppendNode) {

  LinkedList mylist;
  
  // test appending to empty list
  shared_ptr<node> ap_five = mylist.InitNode(5);
  // add node 15 to an empty list
  mylist.Append(ap_five); 
  // ensure the list has a non-null top
  EXPECT_TRUE(mylist.GetTop()); // Expect not to be NULL
  add_points_to_grade(1);
  // ensure first element is the ap_nd node by coparing addresses
  EXPECT_EQ(ap_five.get(), mylist.GetTop().get()); 
  add_points_to_grade(1);
  // ensure first element's value is 15
  EXPECT_EQ(15,mylist.GetTop()->data); 
  add_points_to_grade(1);
  
  // create a node and store the pointer to it
  shared_ptr<node> ap_ten = mylist.InitNode(10);
  mylist.Append(ap_ten);
  EXPECT_EQ(10,mylist.GetTop()->next->data);
  add_points_to_grade(1);
  EXPECT_FALSE(mylist.GetTop()->next->next); // expect to be NULL
  add_points_to_grade(1);
  
  // try appending one more
  shared_ptr<node> ap_nn = mylist.InitNode(99);
  mylist.Append(ap_nn);
  EXPECT_EQ(99,mylist.GetTop()->next->next->data);
  add_points_to_grade(1);
  EXPECT_FALSE(mylist.GetTop()->next->next->next); // expect to b NULL
  add_points_to_grade(1);
}

bool expect_all_helper(int vals[], int size, shared_ptr<node> top) {
  bool ret = true;
  shared_ptr<node> cursor = top;
  for (int i=0; i < size; i++) {
    if (cursor == NULL || cursor->data != vals[i]) {
      if (cursor == NULL) {
        cout << "Cursor became null." << endl;
      } else if (cursor->data != vals[i]) {
        cout << cursor->data << " != " << vals[i] << endl;
      }
      ret = false;
      break;
    }
    cursor = cursor->next;
  }
  return ret;
}

TEST_F(test_LinkedList, TestInsertData) {
  LinkedList mylist;

  shared_ptr<node> threenode = build_three_node_list_helper(30, 20, 10);
  mylist.SetTop(threenode); // list is now 30, 20, 10
  
  int initvals[] = { 30, 20, 10 };
  EXPECT_TRUE(expect_all_helper(initvals, 3,mylist.GetTop()));

  mylist.InsertData(0,4); // list is now 4, 30, 20, 10

  int vals[] = { 4, 30, 20, 10 };
  EXPECT_TRUE(expect_all_helper(vals, 4, mylist.GetTop())); 
  
  mylist.InsertData(2,-78); // list is now 4, 30, -78, 20, 10
  int vals2[] = { 4, 30, -78, 20, 10 };
  EXPECT_TRUE(expect_all_helper(vals2, 5, mylist.GetTop()));
  
  mylist.InsertData(5,99); // list is now 4, 30, -78, 20, 10, 99
  int vals3[] = { 4, 30, -78, 20, 10, 99 };
  EXPECT_TRUE(expect_all_helper(vals3, 6, mylist.GetTop()));
}

TEST_F(test_LinkedList, TestInsertNode) {
  LinkedList mylist;

  shared_ptr<node> threenode = build_three_node_list_helper(7, 98, -34);
  mylist.SetTop(threenode);

  // add at beginning
  shared_ptr<node> nd_five = mylist.InitNode(5);
  mylist.Insert(0,nd_five);
  int vals[] = {5, 7, 98, -34};
  EXPECT_TRUE(expect_all_helper(vals, 4, mylist.GetTop()));
  
  // add in middle
  shared_ptr<node> nd_middle = mylist.InitNode(20);
  mylist.Insert(2,nd_middle);
  int vals2[] = { 5, 7, 20, 98, -34 };
  EXPECT_TRUE(expect_all_helper(vals2, 5, mylist.GetTop()));

  // add at end
  shared_ptr<node> nd_ending = mylist.InitNode(800);
  mylist.Insert(5,nd_ending);
  int vals3[] = { 5, 7, 20, 98, -34, 800 };
  EXPECT_TRUE(expect_all_helper(vals3, 6, mylist.GetTop()));
}

TEST_F(test_LinkedList, TestRemove) {
  LinkedList mylist;
  
  shared_ptr<node> threenode = build_three_node_list_helper(7, 86, 210);
  mylist.SetTop(threenode);

  int vals[] = {7, 86, 210 };
  EXPECT_TRUE(expect_all_helper(vals, 3, mylist.GetTop()));
  
  // remove start
  mylist.Remove(0);
  int vals2[] = {86, 210 };
  EXPECT_TRUE(expect_all_helper(vals2, 2, mylist.GetTop()));
  
  // reset and remove mid
  threenode = build_three_node_list_helper(7, 86, 210);
  mylist.SetTop(threenode);
  mylist.Remove(1);
  int vals3[] = {7, 210 };
  EXPECT_TRUE(expect_all_helper(vals3, 2, mylist.GetTop()));

  // reset and remove end
  threenode = build_three_node_list_helper(7, 86, 210);
  mylist.SetTop(threenode);
  mylist.Remove(2);
  int vals4[] = {7, 86 };
  EXPECT_TRUE(expect_all_helper(vals4, 2, mylist.GetTop()));
}


TEST_F(test_LinkedList, TestSize) {
  LinkedList mylist;
  EXPECT_EQ(0, mylist.Size());
  
  shared_ptr<node> threenode = build_three_node_list_helper(76, 12, 423);
  mylist.SetTop(threenode);
  EXPECT_EQ(3, mylist.Size());
}

TEST_F(test_LinkedList, TestContains) {
  LinkedList mylist;
  shared_ptr<node> threenode = build_three_node_list_helper(7, 0, -210);
  mylist.SetTop(threenode);

  EXPECT_TRUE(mylist.Contains(-210));
  EXPECT_TRUE(mylist.Contains(0));
  EXPECT_TRUE(mylist.Contains(7));
  EXPECT_FALSE(mylist.Contains(12));
  EXPECT_FALSE(mylist.Contains(-120));
  EXPECT_FALSE(mylist.Contains(55));
}