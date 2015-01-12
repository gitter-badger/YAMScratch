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
#include <iomanip>

//error reporting macros
#define PACKED_ERROR {\
    std::cerr << "Cannot edit JaggedArray while it is in packed state" << std::endl; \
    std::cerr << "you should call JaggedArray::unpack() before attempting edit" << std::endl; }
#define BIN_INDEX_ERROR(ask_bin,max_bin) { std::cerr << "Bin index error, bin index is an unsigned int" << std::endl; \
                            std::cerr << "you asked for bin index = " << (ask_bin) << std::endl; \
                            std::cerr << "maximum bin was bin index = " << (max_bin) << std::endl; }
#define SLOT_INDEX_ERROR { std::cerr << "Slot index error" << std::endl; }
#define NULL_POINTER_EXCEPTION {std::cerr << "Null pointer exception" << std::endl;}

//default constructor
template<typename T> JaggedArray<T>::JaggedArray() \
    : isPacked_(false), numElements_(0), numBins_(0), offsets_(NULL), \
    packed_values_(NULL), unpacked_values_(NULL), counts_(0) {}

//bin constructor
template<typename T> JaggedArray<T>::JaggedArray(unsigned int bins) \
    : isPacked_(false), numElements_(0), numBins_(bins), offsets_(NULL), \
    packed_values_(NULL), unpacked_values_(NULL), counts_(NULL) 
{
    //initialize the array of bin counts
    counts_ = new unsigned int[numBins_];
    for(unsigned int i = 0; i < numBins_; i++) {
        counts_[i] = 0;
    }
    unpacked_values_ = new T*[numBins_];
    for(unsigned int i = 0; i < numBins_; i++) {
        unpacked_values_[i] = NULL;
    }
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
    this->isPacked_ = j.isPacked_;
}
//=========================================================================
//GETTERS
template<typename T> unsigned int JaggedArray<T>::numElementsInBin(unsigned int bin) const {
    if(bin < numBins_){
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
    unsigned int num_bin_slots;
    if(isPacked_) {
        //allocate a new array to hold the elements
        unpacked_values_ = new T*[numBins_];
        counts_ = new unsigned int[numBins_];
        if(unpacked_values_ == NULL || counts_ == NULL) {NULL_POINTER_EXCEPTION; exit(1); }
        //proceed once each
        for(unsigned int i = 0; i < numBins_; i++) {
            if(i == numBins_ - 1) { //for the last one we calculate differenly
                num_bin_slots = numElements_ - offsets_[i];
            } else {
                num_bin_slots = offsets_[i+1] - offsets_[i];
            }
            //now unpack into each bin
            if(num_bin_slots) { //check for zero elements
                T* temp = new T[num_bin_slots];
                if(temp == NULL) { NULL_POINTER_EXCEPTION; exit(1);}
                //copy over the new values
                for(unsigned int j = 0; j < num_bin_slots; j++) {
                    temp[j] = packed_values_[ ( j + offsets_[i] ) ];
                }
                //swap the new array in
                unpacked_values_[i] = temp;
                //update the bin counts
            } else { //there are no values to copy
                unpacked_values_[i] = NULL;
            }
            //update the number of counts in each bin
            counts_[i] = num_bin_slots;

        }
        //release the packed values
        delete [] packed_values_;
        delete [] offsets_;
        packed_values_ = NULL;
        offsets_ = NULL;
        //change the state to unpacked
        isPacked_ = false;
    }
    // if it is already unpacked nothing to be done
}

//=========================================================================
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
//=========================================================================
//MODIFIERS
//exits if we try to edit in packed mode
//also does usual checks for indices
template<typename T> void JaggedArray<T>::removeElement(unsigned int bin, unsigned int slot) {
    if(isPacked_) {PACKED_ERROR; exit(1);}
    //otherwise we continue and test for validity
    if(bin < this->numBins_) {
        //now test the slots number
        if(slot < counts_[bin]) {
            if(counts_[bin] == 1) {
                //removing this element will result in zero size array
                delete [] unpacked_values_[bin];
                unpacked_values_[bin] = NULL;
                counts_[bin] = 0;
                numElements_ -= 1;
                return;
            }
            //create a temp to hold new
            T* temp = new T[(counts_[bin]-1)];
            if(temp == NULL) { NULL_POINTER_EXCEPTION; exit(1); } //checking for null
            //walk up list and copy over
            for(unsigned int i = 0; i < slot; i++) {
                temp[i] = unpacked_values_[bin][i];
            }
            //now copy over the remaining
            for(unsigned int i = slot+1; i < counts_[bin]; i++) {
                temp[i-1] = unpacked_values_[bin][i];
            }
            //clean up the old array
            delete [] unpacked_values_[bin];
            //swap in the new array
            unpacked_values_[bin] = temp;
            //update the number of elements
            counts_[bin] -= 1;
            numElements_ -= 1;
        } else {SLOT_INDEX_ERROR; exit(1);}
    } else { BIN_INDEX_ERROR(bin,this->numBins_); exit(1);}
}

template<typename T> void JaggedArray<T>::addElement(unsigned int bin, T obj)
{
    if(!isPacked_) {
        if(bin < numBins_) {
            //allocate new storage for that bin
            int num_new_elements = counts_[bin] + 1;
            T* temp = new T[num_new_elements];
            if(temp == NULL) { NULL_POINTER_EXCEPTION; exit(1); } //checking for null
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
            //update the number in bins
            counts_[bin] += 1;
            numElements_ += 1;
        }
    } else {PACKED_ERROR; exit(1);}
}

template<typename T> void JaggedArray<T>::clear() {
    if(!isPacked_) {
        //free the unpacked values
        for(unsigned int i = 0; i < numBins_ ; i++) {
            if(unpacked_values_[i] != NULL) {
                delete [] unpacked_values_[i];
                unpacked_values_[i] = NULL; //zero out the array
                counts_[i] = 0; //clear the number of elements in each bin
            }
        }
        //this occurs only after the array is all deleted
        numElements_ = 0; //zero out the number of elements
    } else { PACKED_ERROR; exit(1); }
}

//=========================================================================
//Utility functions
#define FILL ' '
template<typename T> void JaggedArray<T>::print() {
    // find the max field width for num of slots in bin
    int mfw = (numElements_ /10 )+2;
    if(isPacked_) { //easy to represent

    } else {
        // determine the max number of slots in a bin as we display them
        //we only have to call setfill once in this case
        std::cout << std::setw(10) << std::setfill(FILL) << std::left << "counts: ";
        unsigned int maximum = 0;
        for(unsigned int i = 0; i < numBins_; i ++) {
            maximum = (counts_[i] > maximum) ? counts_[i] : maximum;
            std::cout << std::setw(mfw) << std::right << counts_[i];
        }
        std::cout << std::endl;
        //now print the  bins
        for(unsigned int i = 0; i < maximum; i ++) {
            if(!i) { //only print the header the first time
                std::cout << std::setw(10)<< std::left << "contents: ";
            } else {
                std::cout << std::setw(10) << FILL;
            }
            //print out each column
            for(unsigned int j = 0; j < numBins_; j++) {
                std::cout << std::setw(mfw) << std::right;
                if( i < counts_[j]) { //only print if there is something 
                    std::cout << unpacked_values_[j][i];
                } else {
                    std::cout << FILL;
                }
            }
            //finish the printing
            std::cout << std::endl;
        }
    }
}

#endif
