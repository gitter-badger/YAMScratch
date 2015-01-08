

template <typedef T>
class JaggedArray
{
public:
	JaggedArray(){create(); } //default constructor
	JaggedArray(int bins){create(int bins); } //bin constructor
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
	create();
	create(int bins);

	copy(const JaggedArray& j);

	int numElements_;
	int numBins_;
	int *offsets_;
	T *packed_values_;
	T *unpacked_values;
	int *counts_;


};