#ifndef JaggedArray_
#define JaggedArray_

template <class T> 
class JaggedArray {
public:
	JaggedArray(); //default constructor
	JaggedArray(unsigned int bins); //bin constructor
	JaggedArray(const JaggedArray& j){copy(j); } //copy constructor
	~JaggedArray(); //destructor

	JaggedArray& operator = (const JaggedArray& j);

	//GETTERS
	unsigned int numElements() const {return numElements_; }
	unsigned int numBins() const {return numBins_; }
	unsigned int numElementsInBin(unsigned int bin) const;
	T getElement(unsigned int bin, unsigned int element) const;
	bool isPacked() const {return isPacked_; }
	//SETTERS
	void clear();
	void addElement(unsigned int bin, T element);
	void removeElement(unsigned int bin, unsigned int slot);
	void pack();
	void unpack();
	void print();

private:
	void copy(const JaggedArray& j);
	bool isBinValid(unsigned int bin);

	//REPRESENTATION
	bool isPacked_; //true when packed
	unsigned int numElements_;
	unsigned int numBins_;
	int* offsets_;
	T* packed_values_;
	T** unpacked_values_;
	int* counts_;


};
//====================BEGIN MEMBERS==============================//
#include <iostream>
#include <cstdlib>

//error reporting macros
#define PACKED_ERROR {\
	std::cerr << "Cannot edit JaggedArray while it is in packed state" << std::endl; \
	std::cerr << "you should call JaggedArray::unpack() before attempting edit" << std::endl; }

#define BIN_INDEX_ERROR { std::cerr << "Bin index error, not a valid bin" << std::endl; }

//default constructor
template<typename T> JaggedArray<T>::JaggedArray() \
	: isPacked_(false), numElements_(0), numBins_(0), offsets_(NULL), \
	packed_values_(NULL), unpacked_values_(NULL), counts_(0) {}

//bin constructor
template<typename T> JaggedArray<T>::JaggedArray(unsigned int bins) \
	: isPacked_(false), numElements_(0), numBins_(bins), offsets_(NULL), \
	packed_values_(NULL), unpacked_values_(NULL), counts_(NULL) {
		//initialize the array of bin counts
		counts_ = new int[numBins_];
	}

template<typename T> JaggedArray<T>::~JaggedArray() {
	if(offsets_ != NULL) delete [] offsets_;
	if(packed_values_ != NULL) delete [] packed_values_;
	if(counts_ != NULL) delete [] counts_;
	if(unpacked_values_ != NULL) delete [] unpacked_values_;
}

template<typename T> JaggedArray<T>& JaggedArray<T>::operator = (const JaggedArray<T>& j)
{	
	if(j.isPacked()) {

	}
	return *this;
}
//=========================================================================
//GETTERS
template<typename T> unsigned int JaggedArray<T>::numElementsInBin(unsigned int bin) const {
	if(isBinValid(bin)){
		return counts_[bin];
	} else {}
}

//=========================================================================
//Packing and Unpacking
template<typename T> void JaggedArray<T>::pack() {
	//make sure that there are elements to allocate
	if(numElements_) {
		packed_values_ = new T[numElements_];

	} else {
		offsets_ = NULL;

	}
}


template<typename T> void JaggedArray<T>::unpack() {
	if(isPacked_) {
		for(int i = 0; i < numBins_; i++) {

		}
	}
	// if it is already unpacked nothing to be done
}

//=========================================================================
//Editing representation
template<typename T> T JaggedArray<T>::getElement(unsigned int bin, unsigned int element) const {
	if(isPacked_) {
		//now we must do a little math to find our bin
		if(bin < numBins_-1) {

		}
	} else {

	}

}

template<typename T> void JaggedArray<T>::removeElement(unsigned int bin, unsigned int slot) {
	if(isPacked_) {PACKED_ERROR; exit(1);}
	if (1);
}

template<typename T> void JaggedArray<T>::addElement(unsigned int bin, T obj)
{
	if(!isPacked_) {
		if(isBinValid(bin)) {
			//allocate new storage for that bin
			int num_new_elements = counts_[bin] + 1;
			T* temp = new T[num_new_elements];
			//copy the old elements into the temp array
			for(int i = 0; i < num_new_elements -1; i ++) {
				temp[i] = unpacked_values_[bin][i];
			}
			//add the new element
			temp[num_new_elements-1] = obj;
			//swap the arrays
			unpacked_values_[bin] = temp;
		}
	}
	//if we do not return then we end up here
	PACKED_ERROR; exit(1);
}

template<typename T> void JaggedArray<T>::clear() {
	
}

//=========================================================================
//Utility functions

template<typename T> bool JaggedArray<T>::isBinValid(unsigned int bin) {
	return bin < numBins_;
}

template<typename T> void JaggedArray<T>::print() {
	std::cout << "print not implemented" << std::endl;
}


#endif
