////////////////////////////////////////////////////////////////////////////////////////////////
// LEAK DETECTION
////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
using namespace std;
#include "DynArray.h"		// Include the DynArray header.

////////////////////////////////////////////////////////////////////////////////////////////////
// An example data structure, data stored in a c++ structure
////////////////////////////////////////////////////////////////////////////////////////////////
class DS
{
	string theString;
	int theInt;
	double theDouble;

public:
	DS(const string s = "bad", const int i = 0xBADF00D, const double d = 38.00) : theString(s), theInt(i), theDouble(d){}
	friend ostream& operator<< (ostream& out, const DS& v);
};

ostream& operator<< (ostream& out, const DS& v)
{
	//out << v.theString << ' ' << v.theInt << ' ' << v.theDouble << '\n';
	// for this little test, let's just print out the double
	out << v.theDouble;
	return out;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	printDynArray
// Parameters : array - the array to print
// Return :		void
////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Type>
void printDynArray(const DynArray<Type>& array)
{
	cout << "{ ";
	for (unsigned int i = 0; i < array.size(); ++i)
		cout << array[i] << ' ';
	cout << "} ";
	cout << "Size " << array.size();
	cout << " Capacity " << array.capacity() << '\n';
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	main
// Parameters : argc - the number of command line arguments
//				argv - the array of command line arguments
// return :		int - 0 for success
////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// this function call will set a breakpoint at the location of a leaked block
	// set the parameter to the identifier for a leaked block
	//	_CrtSetBreakAlloc(187);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Declarations
	////////////////////////////////////////////////////////////////////////////////////////////
	DynArray<DS> dsTest1;
	DynArray<DS> dsTest2;
	DynArray<int> testArray;
	DynArray<int>* testPointer;
	DynArray<int> testCopy;
	DynArray<string> stringArray;
	string vals[] = { "0", "4", "8", "12", "16", "20", "24" };
	int i;

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 1 - test the append function
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "*** Test 1 - test the append function ***\n";
	testArray.clear();
	printDynArray(testArray);
	for (i = 0; i < 28; i += 4)
	{
		testArray.append(i);
		printDynArray(testArray);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 2 - test the reserve function - default behavior & attempt to make it smaller
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 2 - test the reserve function - default behavior & attempt to make it smaller ***\n";
	testArray.reserve();
	printDynArray(testArray);
	testArray.reserve(2);
	printDynArray(testArray);
	stringArray.reserve(4);
	printDynArray(stringArray);
	for (i = 0; i < 7; ++i)
	{
		stringArray.append(vals[i].c_str());
		printDynArray(stringArray);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 3 - test the reserve function
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 3 - test the reserve function ***\n";
	testArray.clear();
	testArray.clear();
	testArray.reserve(10);
	printDynArray(testArray);
	testArray.clear();

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 4 - test overwrite behavior
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 4 - test overwrite behavior ***\n";
	testArray.clear();
	testArray.append(1);
	testArray.append(2);
	testArray.append(3);
	testArray[3] = 4;
	testArray.append(5);
	printDynArray(testArray);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 5 - test assignment and copy constructors
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 5 - test assignment and copy constructors ***\n";
	testPointer = new DynArray<int>;
	for (i = 0; i < 28; i += 4)
		testPointer->append(i);
	testCopy = *testPointer;
	delete testPointer;
	printDynArray(testCopy);
	testCopy.clear();
	printDynArray(testCopy);

	testPointer = new DynArray<int>;
	for (i = 0; i < 28; i += 4)
		testPointer->append(i);
	DynArray<int> testCpyCtr(*testPointer);
	delete testPointer;
	printDynArray(testCpyCtr);
	testCpyCtr.clear();
	printDynArray(testCpyCtr);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 6 - test default reserve behavior starting with an empty array
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 6 - test default reserve behavior starting with an empty array ***\n";
	testArray.clear();
	for (i = 0; i < 3; ++i)
	{
		printDynArray(testArray);
		testArray.reserve();
	}
	printDynArray(testArray);
	testArray.append(0);
	testArray.append(1);
	printDynArray(testArray);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 7 - test reserve behavior - append, reserve, then call append to add items
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 7 - test reserve behavior - append, reserve, then call append to add items ***\n";
	testArray.clear();
	testArray.append(0);
	testArray.append(1);
	testArray.reserve();
	testArray.append(2);
	testArray.append(3);
	printDynArray(testArray);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 8 - more assignment stuff
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 8 - more assignment stuff ***\n";
	testCopy.clear();
	testArray.clear();
	for (i = 0; i < 28; i += 4)
		testArray.append(i);
	testCopy.append(i);
	printDynArray(testArray);
	printDynArray(testCopy);
	testArray = testCopy;
	printDynArray(testArray);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 9 - testing random stuff
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 9 - testing random stuff ***\n";
	dsTest1.append(DS("one", 1, 1.1));
	dsTest1.append(DS("two", 2, 2.2));
	dsTest1.append(DS("three", 3, 3.3));
	dsTest1.append(DS("four", 4, 4.4));
	dsTest1.append(DS("five", 5, 5.5));
	dsTest2 = dsTest1;
	printDynArray(dsTest2);
	dsTest1.clear();
	printDynArray(DynArray<DS>(dsTest2));

	////////////////////////////////////////////////////////////////////////////////////////////
	// Test 10 - self-Assignment
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** Test 10 - self-Assignment ***\n";
	testArray.clear();
	for (i = 0; i < 28; i += 4)
		testArray.append(i);
	testPointer = &testArray;
	*testPointer = testArray;
	printDynArray(testArray);
	system("pause");

	////////////////////////////////////////////////////////////////////////////////////////////
	// Complete
	////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}