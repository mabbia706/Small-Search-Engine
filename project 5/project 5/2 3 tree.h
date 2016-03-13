#include <iostream>
#include <vector>
#include<string>
#include <time.h>
//#include "Linked list.h"
using namespace std;
template<typename T>
class Two_Three_Tree{
private:
	struct Key {
		T data;
		Key(const T& obj){
			data = obj;
		}
	};
	struct Node{
		Key *key1, *key2, *dummy;
		Node*left, *middle, *right, *extra_ptr;
		int pos;
		bool overflow(){
			if (key1->data != 0 && key2->data != 0)
				return true;
			return false;
		}
		bool has_capacity(){
			if (key1->data == 0 || key2->data == 0)
				return true;
			return false;
		}
		bool has_one_key(){
			if (key1->data != 0 || key2->data != 0)
				return true;
			return false;
		}
		bool is_empty(){
			if (key1->data == 0 && key2->data == 0)
				return true;
			return false;
		}
		Node(){
			pos = 0;
			key1 = new Key(0);
			key2 = new Key(0);
			left = middle = right = extra_ptr = nullptr;
			dummy = nullptr;
		}
		void add(const T& Data){
			if (key1->data == 0 && key2->data == 0)
				key1 = new Key(Data);
			else if (Data > key1->data&&key1->data != 0 && key2->data == 0){
				key2 = new Key(Data);
			}
			else if (Data < key1->data&&key1->data != 0 && key2->data == 0){
				key2 = key1;
				key1 = new Key(Data);
			}
		}
	};
	void Post_Order(Node*nptr){
		if (nptr != nullptr){
			Post_Order(nptr->left);
			Post_Order(nptr->middle);
			Post_Order(nptr->right);
			delete nptr->key1;
			delete nptr->key2;
			if (nptr->dummy != nullptr)
				delete nptr->dummy;
			delete nptr;
		}
	}
	void adjust_dummy(Node*par, const T&obj){
		//dummy is my third key in case of overflow adjusting in manner so that it does not disturb order
		if (obj > par->key1->data&&obj > par->key2->data)
			par->dummy = new Key(obj);
		else if (obj < par->key1->data){
			par->dummy = par->key2;
			par->key2 = par->key1;
			par->key1 = new Key(obj);
		}
		else{
			par->dummy = par->key2;
			par->key2 = new Key(obj);
		}
	}
	void split(Node*&parpar, Node*&par, Node*&curr, const T& obj, int position){
		if (parpar->has_capacity()){//check for free space in parent node
			parpar->add(par->key2->data);//add middle key of node to be split in parent node
			if (par->pos == 0){//if there is no parent node and node to be split is root
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;//in case of overflow allocating another extra pointer
				parpar->left = new Node();
				parpar->left->add(par->key1->data);
				parpar->left->left = t_left;
				if (position == 30)//if node to be splitted is in right
					parpar->left->right = t_middle;
				else
					parpar->left->right = t_extra_ptr;
				parpar->right = new Node();
				parpar->right->add(par->dummy->data);
				parpar->right->right = t_right;
				if (position == 30)
					parpar->right->left = t_extra_ptr;
				else
					parpar->right->left = t_middle;
				root = parpar;
			}
			else if (par->pos == 1){//if node to be splitted is in left side
				T temp1 = par->dummy->data;
				T temp2 = par->key1->data;
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->left = new Node();
				parpar->left->add(temp2);
				parpar->left->left = t_left;
				parpar->left->right = t_extra_ptr;
				parpar->middle = new Node();
				parpar->middle->add(temp1);
				parpar->middle->left = t_middle;
				parpar->middle->right = t_right;
			}
			else if (par->pos == 3){// if node to be splitted is in right side
				T temp1 = par->dummy->data;
				T temp2 = par->key1->data;
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->right = new Node();
				parpar->right->add(temp1);
				parpar->right->right = t_right;
				parpar->right->left = t_extra_ptr;
				parpar->middle = new Node();
				parpar->middle->add(temp2);
				parpar->middle->left = t_left;
				parpar->middle->right = t_middle;
			}
		}
		else{//if parent node has no capacity
			Node*Breakpoint1 = nullptr;
			Node*Breakpoint2 = nullptr;
			int check = 0;
			if (par->pos == 3){
				//saving keys of splitting node
				T temp1 = par->key1->data, temp2 = par->key2->data, temp3 = par->dummy->data;
				parpar->dummy = new Key(temp2);
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->right = new Node();
				parpar->right->add(temp3);
				parpar->right->right = t_right;
				parpar->right->left = t_extra_ptr;
				parpar->extra_ptr = new Node();
				parpar->extra_ptr->add(temp1);
				parpar->extra_ptr->left = t_left;
				parpar->extra_ptr->right = t_middle;
				//using break points as my terminating condition while traversing through tree 
				//starting from bottom of tree after each execution going one level up
				Breakpoint1 = parpar->right;
				Breakpoint2 = parpar->extra_ptr;
				check = 30;//using this to send to function to tell its position
			}
			else if (par->pos == 1){
				T temp1 = par->key1->data, temp2 = par->key2->data, temp3 = par->dummy->data;
				parpar->dummy = parpar->key2;
				parpar->key2 = parpar->key1;
				parpar->key1 = new Key(temp2);
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->left = new Node();
				parpar->left->add(temp1);
				parpar->left->left = t_left;
				parpar->left->right = t_extra_ptr;
				parpar->extra_ptr = new Node();
				parpar->extra_ptr->add(temp3);
				parpar->extra_ptr->left = t_middle;
				parpar->extra_ptr->right = t_right;
				Breakpoint1 = parpar->left;
				Breakpoint2 = parpar->extra_ptr;
				check = 10;
			}
			else if (par->pos == 2){
				T temp1 = par->key1->data, temp2 = par->key2->data, temp3 = par->dummy->data;
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->dummy = parpar->key2;
				parpar->key2 = new Key(temp2);
				parpar->middle = new Node();
				parpar->middle->add(temp1);
				parpar->middle->right = t_middle;
				parpar->middle->left = t_left;
				parpar->extra_ptr = new Node();
				parpar->extra_ptr->add(temp3);
				parpar->extra_ptr->left = t_extra_ptr;
				parpar->extra_ptr->right = t_right;
				Breakpoint1 = parpar->middle;
				Breakpoint2 = parpar->extra_ptr;
				check = 20;
			}
			//traverse again till leaf
			par = new Node;
			parpar = new Node;
			curr = root;
			while (curr != Breakpoint1&&curr != Breakpoint2){
				if (obj < curr->key1->data){
					parpar = par;
					par = curr;
					curr = curr->left;
					if (curr != nullptr)
						curr->pos = 1;//giving positions to node
				}
				else if (obj > curr->key1->data&&curr->key2->data != 0 && obj < curr->key2->data&&curr->extra_ptr == nullptr){
					parpar = par;
					par = curr;
					curr = curr->middle;
					if (curr != nullptr)
						curr->pos = 2;
				}
				else if (obj > curr->key1->data&&curr->key2->data != 0 && obj < curr->key2->data&&curr->extra_ptr != nullptr){
					parpar = par;
					par = curr;
					curr = curr->extra_ptr;
					if (curr != nullptr)
						curr->pos = 4;
				}
				else if (obj > curr->key1->data&&curr->key2->data == 0){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
				else if (curr->key2->data != 0 && obj > curr->key2->data&&curr->extra_ptr == nullptr){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
				else if (curr->key2->data != 0 && obj > curr->key2->data&&curr->extra_ptr != nullptr){
					parpar = par;
					par = curr;
					curr = curr->extra_ptr;
					if (curr != nullptr)
						curr->pos = 4;
				}
			}
			split(parpar, par, curr, obj, check);
		}
	}
	Node*root;
public:
	Two_Three_Tree(){
		root = new Node();
		root = nullptr;
	}
	bool is_empty(){
		if (root == nullptr){
			delete root;
			return true;
		}
		return false;
	}
	void insert(const T& obj){
		if (is_empty()){
			root = new Node();
			root->add(obj);
			root->pos = 0;
		}
		else{
			Node*par = nullptr;
			Node*parpar = nullptr;
			Node* curr = root;
			//traversing through tree till leaf
			while (curr != nullptr){
				if (obj < curr->key1->data){
					parpar = par;
					par = curr;
					curr = curr->left;
					if (curr != nullptr)
						curr->pos = 1;//giving position 1 for left 2 for middle 3 for right
				}
				else if (obj > curr->key1->data&&curr->key2->data != 0 && obj < curr->key2->data){
					parpar = par;
					par = curr;
					curr = curr->middle;
					if (curr != nullptr)
						curr->pos = 2;
				}
				else if (obj > curr->key1->data&&curr->key2->data == 0){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
				else if (curr->key2->data != 0 && obj > curr->key2->data){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
			}
			if (par->has_capacity() && parpar == nullptr){
				par->add(obj);
			}
			else if (!par->has_capacity() && parpar == nullptr){
				adjust_dummy(par, obj);
				Node*temp = new Node();
				temp->add(par->key2->data);
				temp->right = new Node();
				temp->left = new Node();
				temp->left->add(par->key1->data);
				temp->right->add(par->dummy->data);
				root = temp;
			}
			else if (par->has_capacity() && parpar != nullptr){
				par->add(obj);
			}
			else if (!par->has_capacity() && parpar != nullptr){
				adjust_dummy(par, obj);
				//check space in nearest sibling
				bool no_free_loc = 0;
				if (par->pos == 1){
					if (parpar->middle != nullptr){
						if (parpar->middle->has_capacity()){
							parpar->middle->add(parpar->key1->data);
							parpar->key1 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
					else{
						if (parpar->right->has_capacity()){
							(parpar->key2->data != 0) ? parpar->right->add(parpar->key2->data) : parpar->right->add(parpar->key1->data);
							(parpar->key2->data != 0) ? parpar->key2 = par->dummy : parpar->key1 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
				}
				else if (par->pos == 2){
					//left rotate
					if (parpar->left != nullptr){
						if (parpar->left->has_capacity()){
							parpar->left->add(parpar->key1->data);
							parpar->key1 = par->key1;
							par->key1 = par->key2;
							par->key2 = par->dummy;
							no_free_loc = 0;
						}
						else if (parpar->right != nullptr){
							//right rotate
							if (parpar->right->has_capacity()){
								parpar->right->add(parpar->key2->data);
								parpar->key2 = par->dummy;
								no_free_loc = 0;
							}
							else
								no_free_loc = 1;
						}
					}
				}
				else if (par->pos == 3){
					if (parpar->middle != nullptr){
						if (parpar->middle->has_capacity()){
							parpar->middle->add(parpar->key2->data);
							parpar->key2 = par->key1;
							par->key1 = par->key2;
							par->key2 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
					else{
						if (parpar->left->has_capacity()){
							parpar->left->add(parpar->key1->data);
							parpar->key1 = par->key1;
							par->key1 = par->key2;
							par->key2 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
				}
				//if no space found in nearest sibling
				if (no_free_loc){
					split(parpar, par, curr, obj, par->pos * 10);
				}
			}
		}
	}
	void _print(Node*curr){
		//using pre order for printint tree
		if (curr != nullptr){
			cout << curr->key1->data << " " << curr->key2->data << endl;
			_print(curr->left);
			_print(curr->middle);
			_print(curr->right);
		}
	}
	void print(){
		_print(root);
	}
	~Two_Three_Tree(){
		Post_Order(root);
	}
};
template<>
class Two_Three_Tree<string>{
private:
	struct Key {
		string data;
		SLL<string> list;
		Key(){
			data = "";
		}
		void init(const string& obj,SLL<string>lof){
			data = obj;
			list = lof;
		}
	};
	struct Node{
		Key *key1, *key2, *dummy;
		Node*left, *middle, *right, *extra_ptr;
		int pos;
		bool overflow(){
			if (key1->data != "" && key2->data != "")
				return true;
			return false;
		}
		bool has_capacity(){
			if (key1->data == "" || key2->data == "")
				return true;
			return false;
		}
		bool has_one_key(){
			if (key1->data != "" || key2->data != "")
				return true;
			return false;
		}
		bool is_empty(){
			if (key1->data == "" && key2->data == "")
				return true;
			return false;
		}
		Node(){
			pos = 0;
			key1 = new Key();
			key2 = new Key();
			left = middle = right = extra_ptr = nullptr;
			dummy = nullptr;
		}
		void add(const string& Data,SLL<string>lof){
			if (key1->data == "" && key2->data == ""){
				key1 = new Key();
				key1->init(Data,lof);
			}
			else if (Data >=key1->data&&key1->data != "" && key2->data == ""){
				key2 = new Key();
				key2->init(Data, lof);
			}
			else if (Data <= key1->data&&key1->data != "" && key2->data == ""){
				key2 = key1;
				key1 = new Key();
				key1->init(Data, lof);
			}
		}
	};
	void Pre_Order(Node*nptr,string*filenames,int no_of_files){
		if (nptr != nullptr){
			for (int i = 0; i < no_of_files; i++){
				nptr->key1->list.extra_search(filenames[i]);
				nptr->key2->list.extra_search(filenames[i]);
			}
			Pre_Order(nptr->left,filenames,no_of_files);
			Pre_Order(nptr->middle,filenames,no_of_files);
			Pre_Order(nptr->right,filenames,no_of_files);
		}
	}
	void words_in_each_node(Node*curr,vector<string>&words){
		if (curr != nullptr){
			if (curr->key1->data!="")
			words.push_back(curr->key1->data);
			if (curr->key2->data != "")
				words.push_back(curr->key2->data);
			words_in_each_node(curr->left,words);
			words_in_each_node(curr->middle,words);
			words_in_each_node(curr->right,words);
		}
	}
	void Post_Order(Node*nptr){
		if (nptr != nullptr){
			Post_Order(nptr->left);
			Post_Order(nptr->middle);
			Post_Order(nptr->right);
			delete nptr->key1;
			delete nptr->key2;
			if (nptr->dummy != nullptr)
				delete nptr->dummy;
			delete nptr;
		}
	}
	bool check_word_freq_in_file(Node*curr,string str,string filename,int&freq){
		if (curr != nullptr){
			if (curr->key1->data == str){
				if(curr->key1->list.find_freq_of_term(filename, freq));
				return true;
			}
		else if (curr->key2->data == str){
			if(curr->key2->list.find_freq_of_term(filename,freq));
			return true;
		}
			check_word_freq_in_file(curr->left,str,filename,freq);
			check_word_freq_in_file(curr->left, str, filename, freq);
			check_word_freq_in_file(curr->left, str, filename, freq);
		}
	}
	void find_word_in_tree(Node*nptr,string str,bool&found){
		if (nptr != nullptr){
			if (nptr->key1->data == str || nptr->key2->data == str){
				found = true;
				return;
			}
			find_word_in_tree(nptr->left,str,found);
			find_word_in_tree(nptr->middle, str,found);
			find_word_in_tree(nptr->right, str,found);
		}
	}
	void adjust_dummy(Node*par, const string&obj, SLL<string>lof){
		//dummy is my third key in case of overflow adjusting in manner so that it does not disturb order
		if (obj >= par->key1->data&&obj > par->key2->data){
			par->dummy = new Key();
			par->dummy->init(obj,lof);
		}
		else if (obj <= par->key1->data){
			par->dummy = par->key2;
			par->key2 = par->key1;
			par->key1 = new Key();
			par->key1->init(obj,lof);
		}
		else{
			par->dummy = par->key2;
			par->key2 = new Key();
			par->key2->init(obj, lof);
		}
	}
	void split(Node*&parpar, Node*&par, Node*&curr, const string& obj, int position){
		if (parpar->has_capacity()){//check for free space in parent node
			parpar->add(par->key2->data,par->key2->list);//add middle key of node to be split in parent node
			if (par->pos == 0){//if there is no parent node and node to be split is root
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;//in case of overflow allocating another extra pointer
				parpar->left = new Node();
				parpar->left->add(par->key1->data,par->key1->list);
				parpar->left->left = t_left;
				if (position == 30)//if node to be splitted is in right
					parpar->left->right = t_middle;
				else
					parpar->left->right = t_extra_ptr;
				parpar->right = new Node();
				parpar->right->add(par->dummy->data,par->dummy->list);
				parpar->right->right = t_right;
				if (position == 30)
					parpar->right->left = t_extra_ptr;
				else
					parpar->right->left = t_middle;
				root = parpar;
			}
			else if (par->pos == 1){//if node to be splitted is in left side
				string temp1 = par->dummy->data;
				string temp2 = par->key1->data;
				SLL<string> lof1 = par->dummy->list;
				SLL<string> lof2 = par->key1->list;
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->left = new Node();
				parpar->left->add(temp2,lof2);
				parpar->left->left = t_left;
				parpar->left->right = t_extra_ptr;
				parpar->middle = new Node();
				parpar->middle->add(temp1,lof1);
				parpar->middle->left = t_middle;
				parpar->middle->right = t_right;
			}
			else if (par->pos == 3){// if node to be splitted is in right side
				string temp1 = par->dummy->data;
				string temp2 = par->key1->data;
				SLL<string> lof1 = par->dummy->list;
				SLL<string> lof2 = par->key1->list;
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->right = new Node();
				parpar->right->add(temp1,lof1);
				parpar->right->right = t_right;
				parpar->right->left = t_extra_ptr;
				parpar->middle = new Node();
				parpar->middle->add(temp2,lof2);
				parpar->middle->left = t_left;
				parpar->middle->right = t_middle;
			}
		}
		else{//if parent node has no capacity
			Node*Breakpoint1 = nullptr;
			Node*Breakpoint2 = nullptr;
			int check = 0;
			if (par->pos == 3){
				//saving keys of splitting node
				string temp1 = par->key1->data, temp2 = par->key2->data, temp3 = par->dummy->data;
				SLL<string> lof1 = par->key1->list;
				SLL<string> lof2 = par->key2->list;
				SLL<string> lof3 = par->dummy->list;
				parpar->dummy = new Key();
				parpar->dummy->init(temp2,lof2);
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->right = new Node();
				parpar->right->add(temp3,lof3);
				parpar->right->right = t_right;
				parpar->right->left = t_extra_ptr;
				parpar->extra_ptr = new Node();
				parpar->extra_ptr->add(temp1,lof1);
				parpar->extra_ptr->left = t_left;
				parpar->extra_ptr->right = t_middle;
				//using break points as my terminating condition while traversing through tree 
				//starting from bottom of tree after each execution going one level up
				Breakpoint1 = parpar->right;
				Breakpoint2 = parpar->extra_ptr;
				check = 30;//using this to send to function to tell its position
			}
			else if (par->pos == 1){
				string temp1 = par->key1->data, temp2 = par->key2->data, temp3 = par->dummy->data;
				SLL<string> lof1 = par->key1->list;
				SLL<string> lof2 = par->key2->list;
				SLL<string> lof3 = par->dummy->list;
				parpar->dummy = parpar->key2;
				parpar->key2 = parpar->key1;
				parpar->key1 = new Key();
				parpar->key1->init(temp2,lof2);
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->left = new Node();
				parpar->left->add(temp1,lof1);
				parpar->left->left = t_left;
				parpar->left->right = t_extra_ptr;
				parpar->extra_ptr = new Node();
				parpar->extra_ptr->add(temp3,lof3);
				parpar->extra_ptr->left = t_middle;
				parpar->extra_ptr->right = t_right;
				Breakpoint1 = parpar->left;
				Breakpoint2 = parpar->extra_ptr;
				check = 10;
			}
			else if (par->pos == 2){
				string temp1 = par->key1->data, temp2 = par->key2->data, temp3 = par->dummy->data;
				SLL<string> lof1 = par->key1->list;
				SLL<string> lof2 = par->key2->list;
				SLL<string> lof3 = par->dummy->list;
				Node*t_left = par->left;
				Node*t_middle = par->middle;
				Node*t_right = par->right;
				Node*t_extra_ptr = par->extra_ptr;
				parpar->dummy = parpar->key2;
				parpar->key2 = new Key();
				parpar->key2->init(temp2,lof2);
				parpar->middle = new Node();
				parpar->middle->add(temp1,lof1);
				parpar->middle->right = t_middle;
				parpar->middle->left = t_left;
				parpar->extra_ptr = new Node();
				parpar->extra_ptr->add(temp3,lof3);
				parpar->extra_ptr->left = t_extra_ptr;
				parpar->extra_ptr->right = t_right;
				Breakpoint1 = parpar->middle;
				Breakpoint2 = parpar->extra_ptr;
				check = 20;
			}
			//traverse again till leaf
			par = new Node;
			parpar = new Node;
			curr = root;
			while (curr != Breakpoint1&&curr != Breakpoint2){
				if (obj <= curr->key1->data){
					parpar = par;
					par = curr;
					curr = curr->left;
					if (curr != nullptr)
						curr->pos = 1;//giving positions to node
				}
				else if (obj > curr->key1->data&&curr->key2->data != "" && obj <=curr->key2->data&&curr->extra_ptr == nullptr){
					parpar = par;
					par = curr;
					curr = curr->middle;
					if (curr != nullptr)
						curr->pos = 2;
				}
				else if (obj > curr->key1->data&&curr->key2->data != "" && obj <= curr->key2->data&&curr->extra_ptr != nullptr){
					parpar = par;
					par = curr;
					curr = curr->extra_ptr;
					if (curr != nullptr)
						curr->pos = 4;
				}
				else if (obj > curr->key1->data&&curr->key2->data == ""){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
				else if (curr->key2->data != "" && obj > curr->key2->data&&curr->extra_ptr == nullptr){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
				else if (curr->key2->data != "" && obj > curr->key2->data&&curr->extra_ptr != nullptr){
					parpar = par;
					par = curr;
					curr = curr->extra_ptr;
					if (curr != nullptr)
						curr->pos = 4;
				}
			}
			split(parpar, par, curr, obj, check);
		}
	}
	Node*root;
public:
	Two_Three_Tree(){
		root = new Node();
		root = nullptr;
	}
	bool is_empty(){
		if (root == nullptr){
			//delete root;
			return true;
		}
		return false;
	}
	void check_list(Node*&curr,SLL<string>&lof,const string&obj){
		if (curr->key1->data == obj){
			//if word is repeated in same file then increase its frequency in that file
			if(curr->key1->list.search(lof.get_head_data()));
			//if same word but from differnt file add that file in list
			else if (!curr->key1->list.search(lof.get_head_data())){
				curr->key1->list.insert(lof.get_head_data());
			}
		}
		else if (curr->key2->data == obj){
			//if word is repeated in same file then increase its frequency in that file
			if(curr->key2->list.search(lof.get_head_data()));
			//if same word but from differnt file add that file in list
			else if (!curr->key1->list.search(lof.get_head_data())){
				curr->key1->list.insert(lof.get_head_data());
			}
		}
		//if it is new word then simply add
		else
			curr->add(obj, lof);
	}
	void insert(const string& obj,SLL<string>lof){
		if (is_empty()){
			root = new Node();
			root->add(obj,lof);
			root->pos = 0;
		}
		else{
			Node*par = nullptr;
			Node*parpar = nullptr;
			Node* curr = root;
			//traversing through tree till leaf
			while (curr != nullptr){
				if (obj <= curr->key1->data){
					parpar = par;
					par = curr;
					curr = curr->left;
					if (curr != nullptr)
						curr->pos = 1;//giving position 1 for left 2 for middle 3 for right
				}
				else if (obj > curr->key1->data&&curr->key2->data != "" && obj <= curr->key2->data){
					parpar = par;
					par = curr;
					curr = curr->middle;
					if (curr != nullptr)
						curr->pos = 2;
				}
				else if (obj > curr->key1->data&&curr->key2->data == ""){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
				else if (curr->key2->data != "" && obj > curr->key2->data){
					parpar = par;
					par = curr;
					curr = curr->right;
					if (curr != nullptr)
						curr->pos = 3;
				}
			}
			if (par->has_capacity() && parpar == nullptr){
				check_list(par, lof, obj);
			}
			else if (!par->has_capacity() && parpar == nullptr){
				adjust_dummy(par, obj,lof);
				Node*temp = new Node();
				temp->add(par->key2->data, par->key2->list);
				temp->right = new Node();
				temp->left = new Node();
				temp->left->add(par->key1->data,par->key1->list);
				temp->right->add(par->dummy->data,par->dummy->list);
				root = temp;
			}
			else if (par->has_capacity() && parpar != nullptr){
				check_list(par, lof, obj);
			}
			else if (!par->has_capacity() && parpar != nullptr){
				adjust_dummy(par, obj,lof);
				//check space in nearest sibling
				bool no_free_loc = 0;
				if (par->pos == 1){
					if (parpar->middle != nullptr){
						if (parpar->middle->has_capacity()){
							parpar->middle->add(parpar->key1->data, parpar->key1->list);
							parpar->key1 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
					else{
						if (parpar->right->has_capacity()){
							(parpar->key2->data != "") ? parpar->right->add(parpar->key2->data, parpar->key2->list) : parpar->right->add(parpar->key1->data, parpar->key1->list);
							(parpar->key2->data != "") ? parpar->key2 = par->dummy : parpar->key1 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
				}
				else if (par->pos == 2){
					//left rotate
					if (parpar->left != nullptr){
						if (parpar->left->has_capacity()){
							parpar->left->add(parpar->key1->data, parpar->key1->list);
							parpar->key1 = par->key1;
							par->key1 = par->key2;
							par->key2 = par->dummy;
							no_free_loc = 0;
						}
						else if (parpar->right != nullptr){
							//right rotate
							if (parpar->right->has_capacity()){
								parpar->right->add(parpar->key2->data, parpar->key2->list);
								parpar->key2 = par->dummy;
								no_free_loc = 0;
							}
							else
								no_free_loc = 1;
						}
					}
				}
				else if (par->pos == 3){
					if (parpar->middle != nullptr){
						if (parpar->middle->has_capacity()){
							parpar->middle->add(parpar->key2->data, parpar->key2->list);
							parpar->key2 = par->key1;
							par->key1 = par->key2;
							par->key2 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
					else{
						if (parpar->left->has_capacity()){
							parpar->left->add(parpar->key1->data, parpar->key1->list);
							parpar->key1 = par->key1;
							par->key1 = par->key2;
							par->key2 = par->dummy;
							no_free_loc = 0;
						}
						else
							no_free_loc = 1;
					}
				}
				//if no space found in nearest sibling
				if (no_free_loc){
					split(parpar, par, curr, obj, par->pos * 10);
				}
			}
		}
	}
	void count(vector<string>&words){
		words_in_each_node(root,words);
	}
	void search_word_frequency(string str, string filename, int&freq){
		check_word_freq_in_file(root,str,filename,freq);
	}
	void search_word(string str,bool&found){
		find_word_in_tree(root,str,found);
	}
	void _print(Node*curr){
		//using pre order for printint tree
		if (curr != nullptr){
			cout << curr->key1->data << " ";
			curr->key1->list.printList();
			cout<< " " << curr->key2->data <<" ";
			curr->key2->list.printList();
			cout << endl;
			_print(curr->left);
			_print(curr->middle);
			_print(curr->right);
		}
	}
	void print(){
		_print(root);
	}
	void add_other_files(string*filenames,int no_of_files){
		Pre_Order(root,filenames,no_of_files);
	}
	~Two_Three_Tree(){
		//Post_Order(root);
	}
};