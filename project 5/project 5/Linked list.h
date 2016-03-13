#ifndef SLL_H
#define SLL_H
using namespace std;

template <typename T>
class SLL
{
	struct Node
	{
		T data;
		int frequency;
		Node *link;
		Node()
		{
			frequency = 0;
			data = 0;
			link = nullptr;
		}
		Node(const T &item, Node *ptr)
		{
			frequency = 1;
			data = item;
			link = ptr;
		}
		Node(const T &item, Node *ptr,int extra)
		{
			frequency = 0;
			data = item;
			link = ptr;
		}
	};
	Node *start, *tail;
	Node *end;			// only for queue implementation
public:
	SLL<T>()
	{
		start = tail = end = nullptr;
	}
	T get_head_data(){
		return start->data;
	}
	SLL<T>(const SLL<T> &obj)
	{
		if (!obj.start)
		{
			start = tail = end = nullptr;
			return;
		}
		start = new Node(obj.start->data, obj.start->link);
		end = start;
		Node *temp = obj.start->link;
		while (temp)
		{
			end->link = new Node(temp->data, temp->link);
			end = end->link;
			temp = temp->link;
		}
		tail = end;
		temp = nullptr;
	}

	SLL<T>* operator = (const SLL<T> &obj)
	{
		if (!obj.start)
		{
			start = tail = end = nullptr;
			return 0;
		}
		while (start)	deleteNode(start->data);
		start = new Node(obj.start->data, obj.start->link);
		end = start;
		Node *temp = obj.start->link;
		while (temp)
		{
			end->link = new Node(temp->data, temp->link);
			end = end->link;
			temp = temp->link;
		}
		tail = end;
		temp = nullptr;
		return this;
	}

	Node* preceedingNode(const T &item)
	{
		Node *current = start, *previous = nullptr;
		while (current && current->data < item)
		{
			previous = current;
			current = current->link;
		}
		return previous;
	}
	void extra_insert(const T &item,int extra)
	{
		Node *previous = preceedingNode(item);
		if (!previous)
			start = new Node(item, start,extra);
		else
			previous->link = new Node(item, previous->link,extra);
		if (start->link == nullptr)								// if inserted in empty
			tail = start;
		else if (previous && previous->link->link == nullptr)		// if inserted at end
			tail = previous->link;
	}
	void insert(const T &item)
	{
		Node *previous = preceedingNode(item);
		if (!previous)
			start = new Node(item, start);
		else
			previous->link = new Node(item, previous->link);
		if (start->link == nullptr)								// if inserted in empty
			tail = start;
		else if (previous && previous->link->link == nullptr)		// if inserted at end
			tail = previous->link;
	}

	void insertatstart(const T &item)
	{
		start = new Node(item, start);
	}

	void insertatend(const T &item)
	{
		if (!start)			// if inserting in empty list
		{
			end = new Node(item, end);
			start = end;
		}
		else
		{
			end->link = new Node(item, end->link);
			end = end->link;
			tail = end;
		}
	}

	void deleteatstart(T &x)
	{
		if (isEmpty()) return;
		x = start->data;
		Node *temp = start->link;
		delete start;
		start = temp;
		if (!temp) tail = nullptr;				// if list had just one item
		else temp = nullptr;
	}

	void deleteatend(T &x)
	{
		if (isEmpty()) return;
		x = end->data;
		Node *temp = start;
		while (temp->link != end)
			temp = temp->link;
		temp->link = end->link;
		delete end;
		end = temp;
		tail = end;
	}

	bool isEmpty()
	{
		return (!start);
	}

	void printList()
	{
		if (isEmpty())
		{
			return;
		}
		Node *ptr = start;
		while (ptr)
		{		
			cout << ptr->data << ' ' << ptr->frequency << ' ';	
			ptr = ptr->link;
		}
		
		cout << endl;
	}

	void printFull();

	void deleteNode(const T &item)
	{
		if (isEmpty()) return;
		if (start->data == item)			// if first index
		{
			T x;
			deleteatstart(x);
			return;
		}
		Node *ptr = preceedingNode(item);
		if (ptr->link->data != item)
		{
			ptr = nullptr;
			return;
		}
		if (ptr->link == tail)	{ tail = ptr; }
		Node *temp = ptr->link->link;
		delete ptr->link;
		ptr->link = temp;
	}
	bool extra_search(const T&item){
		if (isEmpty()) return false;
		if (start->data == item)			// if first index
		{
			return true;
		}
		Node *ptr = preceedingNode(item);
		if (ptr == nullptr){
			extra_insert(item, -1);
			return false;
		}
			if (ptr->link == nullptr || ptr->link->data != item)
			{
				extra_insert(item, -1);
				return false;
			}
		if (ptr->link != nullptr&&ptr->link->data == item){
			return true;
		}

	}
	bool search(const T&item){
		if (isEmpty()) return false;;
		if (start->data == item)			// if first index
		{
			start->frequency++;
			return true;
		}
		Node *ptr = preceedingNode(item);
		if (ptr->link == nullptr || ptr->link->data != item)
		{
			return false;
		}
		if (ptr->link != nullptr&&ptr->link->data == item){
			ptr->link->frequency++;
			return true;
		}

	}
	bool find_freq_of_term(const T&item, int&freq){
		if (isEmpty()) return false;;
		if (start->data == item)			// if first index		
		{
			freq=start->frequency;
			return true;
		}
		Node *ptr = preceedingNode(item);
		if (ptr == nullptr){
			return false;
		}
		if (ptr->link == nullptr || ptr->link->data != item){
			return false;
		}
		if (ptr->link != nullptr&&ptr->link->data == item){
			freq=ptr->link->frequency;
			return true;
		}
	}
	~SLL<T>()
	{
		Node *temp = start;
		while (temp)
		{
			start = temp->link;
			delete temp;
			temp = start;
		}
	}

	class iterator
	{
		Node *curr;
	protected:
		iterator(Node *ptr) { curr = ptr; }
	public:
		friend SLL<T>;
		iterator() { curr = 0; }

		bool operator == (const iterator &obj) { return (curr == obj.curr); }

		bool operator != (const iterator &obj) { return !(curr == obj.curr->link); }

		iterator& operator ++ ()		// pre increment
		{
			curr = curr->link;
			return *this;
		}

		iterator operator ++ (int)		// post increment
		{
			iterator iter = *this;
			curr = curr->link;
			return iter;
		}

		T operator * () { return curr->data; }

	};

	iterator begin()	{ return iterator(start); }
	iterator last()	{ return iterator(tail); }
};
#endif