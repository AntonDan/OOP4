#ifndef DS_INCLUDE_GUARD
#define DS_INCLUDE_GUARD

#pragma region CustomDataStructures

#pragma region ObjectList

#pragma region oNode

template <class T> class oNode {
private:
	T * data;
	oNode * next;

public:
	oNode(T * obj, oNode * next = NULL) {
		data = obj;
		this->next = next;
	}	

	void SetData(T * obj) {
		data = obj;
	}
	
	void SetNext(oNode * next) {
		this->next = next;
	}


	// GETTERS
	T * GetData() const {
		return data;
	}

	oNode * GetNext() const {
		return next;
	}


	// DESTRUCTOR
	~oNode() {
		data = NULL;
		next = NULL;
	}

};

#pragma endregion

#pragma region oList
template <class T> class oList {
private:
	oNode<T> * start;
	int length;

public:
	// CONSTRUCTORS
	oList() {
		start = NULL;
		length = 0;
	}

	oList(T * data) {
		start = new oNode<T>(data);
		length = 1;
	}

	// GETTERS
	int GetLength() const { return length; }
	// METHODS
	inline bool isEmpty() const { return length == 0; }

	bool  Contains(T * data) const {
		if (Empty()) return false;
		oNode<T> * ptr = start;
		int i = 0;
		while (ptr->GetData() != data && i < length - 1) {
			ptr = ptr->GetNext();
			++i;
		}

		return (ptr->GetData() == data);
	}

	int Find(T * data) {
		if (Empty()) return -1;
		oNode<T> * ptr = start;
		int i = 0;
		while (ptr->GetData() != data && i < length - 1) {
			ptr = ptr->GetNext();
			++i;
		}

		if (ptr->GetData() == data) return i;
		else return -1;
	}

	void Add(T * data) {
		oNode<T> * newnode = new oNode<T>(data);
		if (isEmpty()) {
			start = newnode;
			++length;
		} else {
			oNode<T> * ptr = start;
			while (ptr->GetNext() != NULL){
				ptr = ptr->GetNext();
			}
			ptr->SetNext(newnode);
			++length;
		}
	}

	T * Remove() {
		if (isEmpty()) return NULL;
		oNode<T> * temp = start;
		T * data = temp->GetData();
		if (length > 1) {
			start = start->GetNext();
		} else {
			start = NULL;
		}

		delete temp;
		--length;
		return data;
	}

	T * Delete(T * data) {
		if (isEmpty()) return NULL;

		oNode<T> * ptr = start;


		if (ptr->GetData() == data) return Remove();
		while (ptr->GetNext()->GetData() != data && ptr->GetNext() != NULL) {
			ptr = ptr->GetNext();
		}

		if (ptr->GetNext()->GetData() == data) {
			oNode<T> * temp = ptr->GetNext();
			T * data = temp->GetData();
			ptr->SetNext(temp->GetNext());
			delete temp;
			--length;
			return data;
		}
		return NULL;
	}

	T * Delete(int index) {
		return Delete((*this)[index]);
	}

	void Destroy() {
		while (!isEmpty()) {
			delete Remove();
		}
	}

	void Empty() {
		while (!isEmpty()) {
			Remove();
		}
	}

	void Print() const {
		oNode<T> * ptr = start;
		for (int i = 0; i < length; ++i) {
			cout << *ptr->GetData() << endl;
			ptr = ptr->GetNext();
		}
		//		cout << ptr->GetData() << endl;
	}

	// OVERLOADS
	T * operator[] (int index) const {
		if (index >= length || index < 0) return NULL;
		oNode<T> * ptr = start;
		for (int i = 0; i < index; ++i) {
			ptr = ptr->GetNext();
		}
		return ptr->GetData();
	}

	// DESTRUCTOR
	~oList() {
		while (!isEmpty()) {
			delete Remove();
		}
	}
};
#pragma endregion

#pragma endregion

#pragma endregion

#endif 