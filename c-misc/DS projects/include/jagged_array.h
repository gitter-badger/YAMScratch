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
	bool isBinValid(unsigned int bin) const;

	//REPRESENTATION
	bool isPacked_; //true when packed
	unsigned int numElements_;
	unsigned int numBins_;
	int* offsets_;
	T* packed_values_;
	T** unpacked_values_;
	unsigned int* counts_;


};
//====================BEGIN MEMBERS==============================//
#include <iostream>
#include <cstdlib>

//error reporting macros
#define PACKED_ERROR {\
	std::cerr << "Cannot edit JaggedArray while it is in packed state" << std::endl; \
	std::cerr << "you should call JaggedArray::unpack() before attempting edit" << std::endl; }
#define BIN_INDEX_ERROR(ask_bin,max_bin) { std::cerr << "Bin index error, bin index is an unsigned int" << std::endl; \
							std::cerr << "you asked for bin index = " << (ask_bin) << std::endl; \
							std::cerr << "maximum bin was bin index = " << (max_bin) << std::endl; }
#define SLOT_INDEX_ERROR { std::cerr << "Slot index error" << std::endl; }

//default constructor
template<typename T> JaggedArray<T>::JaggedArray() \
	: isPacked_(false), numElements_(0), numBins_(0), offsets_(NULL), \
	packed_values_(NULL), unpacked_values_(NULL), counts_(0) {}

//bin constructor
template<typename T> JaggedArray<T>::JaggedArray(unsigned int bins) \
	: isPacked_(false), numElements_(0), numBins_(bins), offsets_(NULL), \
	packed_values_(NULL), unpacked_values_(NULL), counts_(NULL) {
		//initialize the array of bin counts
		counts_ = new unsigned int[numBins_];
	}

template<typename T> JaggedArray<T>::~JaggedArray() {
	//delete all the arrays
	if(offsets_ != NULL) delete [] offsets_;
	if(packed_values_ != NULL) delete [] packed_values_;
	if(counts_ != NULL) delete [] counts_;
	if(unpacked_values_ != NULL) delete [] unpacked_values_;
}

template<typename T> JaggedArray<T>& JaggedArray<T>::operator = (const JaggedArray<T>& j) {	
	if(this != &j) {
		
		this->copy(j);
	}
	return *this;
}

template<typename T> void JaggedArray<T>::copy(const JaggedArray& j) {
	
}
//=========================================================================
//GETTERS
template<typename T> unsigned int JaggedArray<T>::numElementsInBin(unsigned int bin) const {
	if(this->isBinValid(bin)){
		return counts_[bin];
	} else {BIN_INDEX_ERROR(bin,this->numBins_); exit(1); }
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
		for(unsigned int i = 0; i < numBins_; i++) {

		}
	}
	// if it is already unpacked nothing to be done
}

//=========================================================================
//Editing representation
template<typename T> T JaggedArray<T>::getElement(unsigned int bin, unsigned int element) const {
	if(this->isPacked_) { //if packed we must do a little math to find our bin
		if(bin < this->numBins_-1) {
			//compute the available slots in this bin
			unsigned int num_slots = this->offsets_[bin+1] - this->offsets_[bin];
			if(element < num_slots){ //check that the slot is valid
				return this->packed_values_[(offsets_[bin] + element)];
			} else {SLOT_INDEX_ERROR; exit(1); }
		} else if(bin == (numBins_ -1) ) {
			//the number of elements in the last slot
			unsigned int num_slots = this->numElements_ - this->offsets_[bin];
			if (element < num_slots) {
				return this->packed_values_[(offsets_[bin] + element)];
			} else { SLOT_INDEX_ERROR; exit(1); }
		} else {BIN_INDEX_ERROR(bin, this->numBins_); exit(1); } // the bin number was invalid
	} else { // the list is not packed
		if(bin < this->numBins_) { //check that bins are a valid
			//now check that the element is within the size of the bin
			if(element < this->counts_[bin]) {
				return this->unpacked_values_[bin][element];
			} else {SLOT_INDEX_ERROR; exit(1); }
		} else { BIN_INDEX_ERROR(bin, this->numBins_); exit(1); }
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
			for(int i = 0; i < num_new_elements - 1; i ++) {
				temp[i] = unpacked_values_[bin][i];
			}
			//add the new element
			temp[num_new_elements-1] = obj;
			//clean up the old array
			delete [] unpacked_values_[bin];
			//swap in the new array
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

template<typename T> bool JaggedArray<T>::isBinValid(unsigned int bin) const {
	return bin < numBins_;
}

template<typename T> void JaggedArray<T>::print() {
	std::cout << "print not implemented" << std::endl;
}


#endif
