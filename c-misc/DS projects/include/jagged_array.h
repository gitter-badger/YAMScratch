

template <typedef T>
class JaggedArray
{
public:
	JaggedArray(arguments); //default constructor
	JaggedArray(const JaggedArray& j){copy(j); } //copy constructor
	JaggedArray& JaggedArray(JaggedArray&); //move constructor
	~JaggedArray(); //destructor

	JaggedArray& operator = (const JaggedArray);

	//GETTERS
	int numElements();
	int numBins();
	int numElementsInBin(int bin);
	int getElement(int bin, int element);
	bool isPacked();
	void clear();
	void addElement(int bin, T element);
	void removeElement();
	void pack();
	void unpack();
	void print();

private:
	int numElements_;
	int numBins_;
	int *offsets_;
	T *packed_values_;
	T *unpacked_values;
	int *counts_;


};