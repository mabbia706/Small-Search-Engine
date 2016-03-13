#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>
#include <fstream>
#include "hash table.h"
#include "2 3 tree.h"
#include "porter2_stemmer.h"
#include "Myfunctions.h"
using namespace std;
void resize(string*&arr,int& old_size,int new_size){
	string*temp = new string[new_size];
	int i = 0;
	for (; i < old_size; i++){
		temp[i] = arr[i];
	}
	delete[]arr;
	old_size = new_size;
	arr = temp;
}
void input_path(char*& path){
	gotoxy(40,0);
	textcolor(12);
	cout << "Welcome to My Search Engine!"<<endl;
	gotoxy(2, 2);
	textcolor(14);
	cout << "Enter path of directory: ";
	cin.getline(path, 1000);
}
void read_directory(char*& path,string*& filenames,int& no_of_files){
	dirent* entry = new dirent();
	DIR* dir = opendir(path);
	while (!dir) {
		perror("opendir");
		input_path(path);
	    dir = opendir(path);
		//exit(1);
	}
	//printf("Directory contents:\n");
	int i = 0;
	int count = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (count > 1){
			if (i == no_of_files)
				resize(filenames, no_of_files, no_of_files+1);
			filenames[i] = string(entry->d_name);
			i++;
		}
		count++;
	}

	if (closedir(dir) == -1) {
		perror("closedir");
		exit(1);
	}
	delete entry;
}
void read_files_in_directory(string*filenames,int no_of_files,string*&data,char*path){
	int j = 0;
	while (j < no_of_files){
		string file_path = string(path);
		file_path.append("/");
		file_path.append(filenames[j]);
			ifstream fin(file_path, ios::in);
			if (fin) {
			/*/	// get length of file:
				fin.seekg(0, fin.end);
				int length = fin.tellg();
				fin.seekg(0, fin.beg);
				char * buffer = new char[length + 1];
				// read data as a block:
				fin.read(buffer, length);
				buffer[length] = '\0';
				data[j] = string(buffer);
				delete[]buffer;/*/
				while (!fin.eof()){
					getline(fin, data[j], '\0');
					}
			}
		j++;
	}
}
void convert_to_lower_case(string*filenames,string*&data,int no_of_files,char*directory_path){
	for (int i = 0; i < no_of_files; i++){
		for (int j = 0; data[i][j] != '\0'; j++){
			//fining uppercase
			if (data[i][j] >= 65 && data[i][j] <= 90){
				data[i][j] += 32;//converting to lowercase
			}
		}
	}
}
bool is_stop_word(char*word){
	if (strcmp(word, "a") == 0 || strcmp(word, "able") == 0 || strcmp(word, "about") == 0)
		return true;
	else if (strcmp(word, "across") == 0 || strcmp(word, "after")==0 || strcmp(word, "all") == 0)
		return true;
	else if( strcmp(word, "almost") == 0 || strcmp(word, "also") == 0 || strcmp(word, "am") == 0)
		return true;
	else if( strcmp(word, "among") == 0 || strcmp(word, "an") == 0 || strcmp(word, "and") == 0)
		return true;
	else if( strcmp(word, "any") == 0 || strcmp(word, "are") == 0 || strcmp(word, "as") == 0)
		return true;
	else if( strcmp(word, "at") == 0 || strcmp(word, "be") == 0 || strcmp(word, "because") == 0)
		return true;
	else if( strcmp(word, "been") == 0 || strcmp(word, "but") == 0 || strcmp(word, "by") == 0)
		return true;
	else if( strcmp(word, "can") == 0 || strcmp(word, "cannot") == 0 || strcmp(word, "could") == 0)
		return true;
	else if( strcmp(word, "dear") == 0 || strcmp(word, "did") == 0 || strcmp(word, "do") == 0)
		return true;
	else if( strcmp(word, "does") == 0 || strcmp(word, "either") == 0 || strcmp(word, "else") == 0)
		return true;
	else if( strcmp(word, "ever") == 0 || strcmp(word, "every") == 0 || strcmp(word, "for") == 0)
		return true;
	else if( strcmp(word, "from") == 0 || strcmp(word, "get") == 0 || strcmp(word, "got") == 0)
		return true;
	else if( strcmp(word, "had") == 0 || strcmp(word, "has") == 0 || strcmp(word, "have") == 0)
		return true;
	else if( strcmp(word, "he") == 0 || strcmp(word, "her") == 0 || strcmp(word, "hers") == 0)
		return true;
	else if( strcmp(word, "him") == 0 || strcmp(word, "his") == 0 || strcmp(word, "how") == 0)
		return true;
	else if( strcmp(word, "however") == 0 || strcmp(word, "i") == 0 || strcmp(word, "if") == 0)
		return true;
	else if( strcmp(word, "in") == 0 || strcmp(word, "into") == 0 || strcmp(word, "is") == 0)
		return true;
	else if( strcmp(word, "it") == 0 || strcmp(word, "its") == 0 || strcmp(word, "just") == 0)
		return true;
	else if( strcmp(word, "least") == 0 || strcmp(word, "let") == 0 || strcmp(word, "like") == 0)
		return true;
	else if( strcmp(word, "likely") == 0 || strcmp(word, "may") == 0 || strcmp(word, "me") == 0)
		return true;
	else if( strcmp(word, "might") == 0 || strcmp(word, "most") == 0 || strcmp(word, "must") == 0)
		return true;
	else if( strcmp(word, "my") == 0 || strcmp(word, "neither") == 0 || strcmp(word, "no") == 0)
		return true;
	else if( strcmp(word, "nor") == 0 || strcmp(word, "not") == 0 || strcmp(word, "of") == 0)
		return true;
	else if( strcmp(word, "off") == 0 || strcmp(word, "often") == 0 || strcmp(word, "on") == 0)
		return true;
	else if( strcmp(word, "only") == 0 || strcmp(word, "or") == 0 || strcmp(word, "other") == 0)
		return true;
	else if( strcmp(word, "our") == 0 || strcmp(word, "own") == 0 || strcmp(word, "rather") == 0)
		return true;
	else if( strcmp(word, "said") == 0 || strcmp(word, "say") == 0 || strcmp(word, "says") == 0)
		return true;
	else if( strcmp(word, "she") == 0 || strcmp(word, "should") == 0 || strcmp(word, "since") == 0)
		return true;
	else if( strcmp(word, "so") == 0 || strcmp(word, "some") == 0 || strcmp(word, "than") == 0)
		return true;
	else if( strcmp(word, "that") == 0 || strcmp(word, "the") == 0 || strcmp(word, "their") == 0)
		return true;
	else if( strcmp(word, "them") == 0 || strcmp(word, "then") == 0 || strcmp(word, "there") == 0)
		return true;
	else if( strcmp(word, "these") == 0 || strcmp(word, "they") == 0 || strcmp(word, "this") == 0)
		return true;
	else if( strcmp(word, "tis") == 0 || strcmp(word, "to") == 0 || strcmp(word, "too") == 0)
		return true;
	else if( strcmp(word, "twas") == 0 || strcmp(word, "us") == 0 || strcmp(word, "wants") == 0 || strcmp(word, "was") == 0)
		return true;
	else if( strcmp(word, "we") == 0 || strcmp(word, "were") == 0 || strcmp(word, "what") == 0 || strcmp(word, "when") == 0)
		return true;
	else if( strcmp(word, "where") == 0 || strcmp(word, "which") == 0 || strcmp(word, "while") == 0 || strcmp(word, "who") == 0)
		return true;
	else if( strcmp(word, "whom") == 0 || strcmp(word, "why") == 0 || strcmp(word, "will") == 0 || strcmp(word, "with") == 0)
		return true;
	else if( strcmp(word, "would") == 0 || strcmp(word, "yet") == 0 || strcmp(word, "you") == 0 || strcmp(word, "your") == 0)
		return true;
	else if( strcmp(word, "ain't") == 0 || strcmp(word, "aren't") == 0 || strcmp(word, "can't") == 0 || strcmp(word, "could've") == 0)
		return true;
	else if( strcmp(word, "couldn't") == 0 || strcmp(word, "didn't") == 0 || strcmp(word, "doesn't") == 0 || strcmp(word, "don't") == 0)
		return true;
	else if( strcmp(word, "hasn't") == 0 || strcmp(word, "he'd") == 0 || strcmp(word, "he'll") == 0 || strcmp(word, "he's") == 0)
		return true;
	else if( strcmp(word, "how'd") == 0 || strcmp(word, "how'll") == 0 || strcmp(word, "how's") == 0 || strcmp(word, "i'd") == 0)
		return true;
	else if( strcmp(word, "i'll") == 0 || strcmp(word, "i'm") == 0 || strcmp(word, "i've") == 0 || strcmp(word, "isn't") == 0)
		return true;
	else if( strcmp(word, "it's") == 0 || strcmp(word, "might've") == 0 || strcmp(word, "mightn't") == 0)
		return true;
	else if( strcmp(word, "must've") == 0 || strcmp(word, "mustn't") == 0 || strcmp(word, "shan't") == 0)
		return true;
	else if( strcmp(word, "she'd") == 0 || strcmp(word, "she'll") == 0 || strcmp(word, "she's") == 0)
		return true;
	else if( strcmp(word, "should've") == 0 || strcmp(word, "shouldn't") == 0 || strcmp(word, "that'll") == 0)
		return true;
	else if( strcmp(word, "that's") == 0 || strcmp(word, "there's") == 0 || strcmp(word, "they'd") == 0)
		return true;
	else if( strcmp(word, "they'll") == 0 || strcmp(word, "they're") == 0 || strcmp(word, "they've") == 0)
		return true;
	else if( strcmp(word, "wasn't") == 0 || strcmp(word, "we'd") == 0 || strcmp(word, "we'll") == 0)
		return true;
	else if( strcmp(word, "we're") == 0 || strcmp(word, "weren't") == 0 || strcmp(word, "what'd") == 0)
		return true;
	else if(strcmp(word, "what's") == 0 || strcmp(word, "when'd") == 0 || strcmp(word, "when'll") == 0)
	return true;
	else if(strcmp(word, "when's") == 0 || strcmp(word, "where'd") == 0 || strcmp(word, "where'll") == 0)
	return true;
	else if(strcmp(word, "where's") == 0 || strcmp(word, "who'd") == 0 || strcmp(word, "who'll") == 0)
	return true;
	else if(strcmp(word, "who's") == 0 || strcmp(word, "why'd") == 0 || strcmp(word, "why'll") == 0)
	return true;
	else if( strcmp(word, "why's") == 0 || strcmp(word, "won't") == 0 || strcmp(word, "would've") == 0)
	return true;
	else if(strcmp(word, "wouldn't") == 0 || strcmp(word, "you'd") == 0 || strcmp(word, "you'll") == 0)
	return true;
	else if (strcmp(word, "you're") == 0 || strcmp(word, "you've") == 0)
	return true;
	else
	return false;
}
void tokenize_data(string*filenames, string*&data, int no_of_files, char*directory_path){
	char delimiters[] = ",\"'.[]{}():_!`-?;/\| ~@#%&^\n\t ";
	for (int i = 0; i < no_of_files; i++){
		string tokenized_str;//string to get all tokenized data and output it on particular file in tokenized form
		char*temp_str = new char[data[i].size() + 1];
		memcpy(temp_str, data[i].c_str(),data[i].size());
		temp_str[data[i].size()] = NULL;
		char*next_token = nullptr;
		char*token = strtok_s(temp_str, delimiters, &next_token);
		while (token != NULL){
			if (!is_stop_word(token)){//check for stop word if it is then ignore it
				string stemmed_token = string(token);//stemming every word
				Porter2Stemmer::stem(stemmed_token);//using porter stemmer
				tokenized_str.append(stemmed_token);//appendind
				tokenized_str.append("\n");
			}
			token = strtok_s(NULL, delimiters, &next_token);
		}
		data[i] = tokenized_str;
		string filepath = string(directory_path);
		filepath.append("/");
		filepath.append(filenames[i]);
		ofstream fout(filepath, ios::out);
		fout << data[i];
		delete[]temp_str;
	}

}
void indexing(hash_table<Two_Three_Tree<string>>&hash,string*filenames, string*data, int no_of_files, int&total_words, int**&doc_vector){
	unsigned int index = 0;
	char str_temp[45];
	string str;
	for (int i = 0; i <no_of_files; i++){
		int j = 0;
		while (j < data[i].size()){
			int k = 0;
			for (; data[i][j] != '\n'; j++,k++){
				str_temp[k] = data[i][j];
			}
			str_temp[k] = '\0';
			str = str_temp;
			index = hash.hash_function(str);
			hash.insert(index, str, filenames[i]);
			j++;
		}
	}
	vector<string>words;
	hash.count_words(words);
	total_words = words.size();
	doc_vector = new int*[no_of_files];
	for (int i = 0; i < no_of_files; i++){
		doc_vector[i] = new int[total_words];
	}
//	for (int i = 0; i < words.size(); i++){
	//	cout << words[i]<<" ";
//	}
	//cout << endl;
	//function for printing each node and adding exta files
	hash.print(filenames, no_of_files);
	//making vector for each document
	int frequency = 0;//frequency of that term in document
	for (int i=0; i < no_of_files; i++){
		//cout << filenames[i] << " ";
		for (int j = 0; j <total_words; j++){
			index = hash.hash_function(words[j]);
			hash.search_freq_in_file(index,words[j],filenames[i],frequency);
			doc_vector[i][j] = frequency;
			//cout << doc_vector[i][j] << " ";
		}
		//cout << endl;
	}
	
}
void convert_query_to_lower_case(string&query){
	for (int i = 0; i < query.size(); i++){
		if (query[i] >= 65 && query[i]<= 90){
			query[i] += 32;
		}
	}
}
void tokenize_query(string& query,int&s_o_q){
	char delimiters[] = ",\"'.[]{}():_!`-?;/\| ~@#%&^\n\t ";
	string tokenized_str;//string to get all tokenized data and output it on particular file in tokenized form
	char*temp_str = new char[query.size() + 1];
	memcpy(temp_str, query.c_str(), query.size());
	temp_str[query.size()] = NULL;
	char*next_token = nullptr;
	char*token = strtok_s(temp_str, delimiters, &next_token);
	while (token != NULL){
		if (!is_stop_word(token)){//check for stop word if it is then ignore it
			string stemmed_token = string(token);//stemming every word
			Porter2Stemmer::stem(stemmed_token);//using porter stemmer
			tokenized_str.append(stemmed_token);//appendind
			tokenized_str.append("\n");
			s_o_q++;
		}
		token = strtok_s(NULL, delimiters, &next_token);
	}
	query = tokenized_str;
}
void input_query(hash_table<Two_Three_Tree<string>>&hash,string&query, int*&query_vector, int&size_of_query_vector){
	gotoxy(2,4);
	textcolor(10);
	cout << "Enter your query: ";
	getline(cin,query);
	convert_query_to_lower_case(query);
	tokenize_query(query,size_of_query_vector);
	query_vector = new int[size_of_query_vector];
	for (int i = 0; i < size_of_query_vector; i++){
		query_vector[i]=-1;
	}
	vector<string> myquery;
	char temp[1000];
	string foo;
	int i = 0;
	for (; query[i] !='\0';){
		int k = 0;
		for (; query[i] != '\n';i++, k++)
			temp[k] = query[i];
		temp[k] = '\0';
		foo = string(temp);
		myquery.push_back(foo);
		i++;
	}
	vector<string>evaluated;
	bool present = false;
	bool present_in_word_list = 0;
	for (int i = 0; i < myquery.size(); i++){
		for (int l = 0; l < evaluated.size(); l++){
			if (myquery[i] == evaluated[l]){
				present = true;
			break;
		}
		}
		string mystr = myquery[i];
		unsigned int index = hash.hash_function(mystr);
		hash.search_word(index, mystr, present_in_word_list);
		if (!present&&!present_in_word_list){
			query_vector[i] = 0;
		}
		//if already evaluated dont need to count again
		if (!present)
		evaluated.push_back(myquery[i]);
		if (!present&&present_in_word_list){
			for (int j = 0; j < myquery.size(); j++){
				if (myquery[j] == myquery[i]){
					if (query_vector[i] == -1)
						query_vector[i]++;
					query_vector[i]++;
				}
			}
		}
		present = false;
		present_in_word_list = false;
	}
	//removing frequency of repeated elements
	for (int i = 0; i < size_of_query_vector; i++){
		if (query_vector[i] == -1){
			int index = i;
			for (; index < size_of_query_vector - 1; index++){
				query_vector[index] = query_vector[index + 1];
			}
			size_of_query_vector--;
			i--;//again check it from current position
		}
	}
}
void calc_cos0(int**doc_vector, int*query_vector,float*&cos0,int nof,int soqv,int total_words){
	for (int i = 0; i < nof; i++){
		float dot_product = 0;
		float product_mags = 0;
		float mag_doc = 0;
		float mag_query = 0;
		for (int j = 0; j < soqv; j++){
			dot_product += doc_vector[i][j] * query_vector[j];
			mag_query += query_vector[j] * query_vector[j];
		}
		for (int k = 0; k < total_words; k++){
			mag_doc += doc_vector[i][k] * doc_vector[i][k];
		}
		mag_doc = sqrt(mag_doc);
		mag_query = sqrt(mag_query);
		product_mags = mag_doc*mag_query;
		if (product_mags == 0)
			cos0[i] =-1;
		else
		cos0[i] = dot_product/product_mags;
	}
}
void final_output(float*cos0,string*filenames,int nof){
	int count = 0;
	for (int i = 0; i < nof; i++){
		if (cos0[i] == 0||cos0[i]==-1){
			count++;
		}
	}
	if (count == nof){
		clrscr();
		gotoxy(6,40);
		textcolor(9);
		cout << "sorry your query could not match any result" << endl;
	}
	else{
		float max1=-1, max2=-1, max3 = -1,max4=-1,max5=-1;
		int index1 = -1, index2 = -1, index3 = -1, index4 = -1, index5 = -1;
		max1 = cos0[0];
		for (int i = 0; i < nof; i++){
			if (cos0[i]>max1){
				max1 = cos0[i];
				index1 = i;
			}
		}
		for (int i = 0; i < nof; i++){
			if (cos0[i]<max1&&cos0[i]>max2){
				max2 = cos0[i];
				index2 = i;
			}
		}
		for (int i = 0; i < nof; i++){
			if (cos0[i]<max1&&cos0[i]<max2&&cos0[i]>max3){
				max3 = cos0[i];
				index3 = i;
			}
		}
		for (int i = 0; i < nof; i++){
			if (cos0[i]<max1&&cos0[i]<max2&&cos0[i]<max3&&cos0[i]>max4){
				max4 = cos0[i];
				index4 = i;
			}
		}
		for (int i = 0; i < nof; i++){
			if (cos0[i]<max1&&cos0[i]<max2&&cos0[i]<max3&&cos0[i]<max4&&cos0[i]>max5){
				max5 = cos0[i];
				index5 = i;
			}
		}
		textcolor(13);
		gotoxy(2, 6);
		if (index1 != -1)
		cout << filenames[index1] << endl;
		gotoxy(2, 6);
		if (index2 != -1)
		cout << filenames[index2] << endl;
		gotoxy(2, 6);
		if ( index3 != -1)
		cout << filenames[index3] << endl;
		gotoxy(2, 6);
		if (index4 != -1)
		cout << filenames[index4] << endl;
		gotoxy(2, 6);
		if ( index5 != -1)
		cout << filenames[index5] << endl;
	}
}
int main(){
	unsigned int size = 9973;//largest prime under 10000
	hash_table<Two_Three_Tree<string>> hash(size);
	int no_of_files = 1,total_words=0;
	char*directory_path = new char[1000];
	string *filenames = new string[no_of_files],query="";
	input_path(directory_path);
	read_directory(directory_path,filenames,no_of_files);
	string*data = new string[no_of_files];
	read_files_in_directory(filenames, no_of_files,data,directory_path);
	convert_to_lower_case(filenames,data,no_of_files,directory_path);
	tokenize_data(filenames, data, no_of_files, directory_path);
	int**doc_vector,*query_vector,size_of_query_vector=0;//document vector
	indexing(hash,filenames, data, no_of_files, total_words,doc_vector );
	input_query(hash,query,query_vector,size_of_query_vector);
	float *cos0=new float[no_of_files];
	calc_cos0(doc_vector, query_vector,cos0,no_of_files, size_of_query_vector,total_words);
	final_output(cos0,filenames,no_of_files);
	delete[]directory_path;
	delete[]filenames;
	delete[]data;
	delete[]query_vector;
	for (int i = 0; i < no_of_files; i++)
		delete[]doc_vector[i];
	delete[]doc_vector;
	delete[]cos0;
	return 0;	
}