#include <vector>
#include <string>
#include"Linked list.h"
using namespace std;
template<typename T>
class hash_table{
private:
	T*table;
	unsigned int size;
public:
	hash_table(int len) :size(len){
		table = new T[size];
	}
	unsigned int hash_function(string str){
		unsigned int i = 0, sum = str[i], variable = 3;
		i++;
		while (i < str.size()){
			int ascii = str[i];
			sum = sum + variable*ascii;
			variable *= 3;
			i++;
		}
		return sum%size;
	}
	void search_freq_in_file(unsigned int index, string str, string filename,int&frequency){
		table[index].search_word_frequency(str, filename, frequency);
	}
	void search_word(unsigned int index, string str,bool&found){
		table[index].search_word(str,found);
	}
	void insert(unsigned int index,string str,string filename){
	     SLL<string> lof;
		lof.insert(filename);
		table[index].insert(str,lof);
	}
	void count_words(vector<string>&words){	
		for (int i = 0; i < size; i++){
			if (!table[i].is_empty()){
				table[i].count(words);
			}
		}
	}
	void print(string*filenames,int no_of_files){
		for (int i = 0; i < size; i++){
			if (!table[i].is_empty()){
				table[i].add_other_files(filenames,no_of_files);
				//table[i].print();
			}
		}
	}
	~hash_table(){
		delete[]table;
	}
};