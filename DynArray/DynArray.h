#pragma once

template<typename Type>
class DynArray
{
private:
	Type* t;

	// Another slight optimization, create the variable once and use it for making a new array when we run out of memory
	Type* t2;

	unsigned int arrayCapacity;
	unsigned int arraySize;

public:
	// Very minor optimization, by creating the variable once for loop indexing rather than every time
	unsigned int i = 0;

	// Accessors
	int GetArraySize() const { return arraySize; }
	int GetArrayCapacity() const { return arrayCapacity; }

	// Mutators
	void SetArraySize(int newArraySize) { arraySize = newArraySize; }
	void SetArrayCapacity(int newArrayCapacity) { arrayCapacity = newArrayCapacity; }

	/////////////////////////////////////////////////////////////////////////////
	// Function :	Constructor
	// Notes : Constructs an empty array (Size 0 Capacity 0)
	/////////////////////////////////////////////////////////////////////////////
	DynArray()
	{
		t = nullptr;
		t2 = nullptr;
		arraySize = 0;
		arrayCapacity = 0;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function :	Destructor
	// Notes : cleans up any dynamic memory
	/////////////////////////////////////////////////////////////////////////////
	~DynArray()
	{
		if (t != nullptr)
		{
			delete[] t;
			t = nullptr;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function :	Copy Constructor
	/////////////////////////////////////////////////////////////////////////////
	DynArray(const DynArray<Type>& that)
	{
		t = nullptr;
		*this = that;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function :	Assignment Operator
	/////////////////////////////////////////////////////////////////////////////
	DynArray<Type>& operator=(const DynArray<Type>& that)
	{
		if (this != &that)
		{
			arraySize = that.arraySize;
			arrayCapacity = that.arrayCapacity;

			delete[] t;
			t = new Type[arrayCapacity];
			for (i = 0; i < arraySize; ++i)
				t[i] = that.t[i];

		}
		return *this;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : operator[]
	// Parameters : index - the index to access
	// Return : Type & - returns the array element at [index]
	// Notes : performs no error checking. user should ensure index is 
	//		valid with the size() method
	/////////////////////////////////////////////////////////////////////////////
	Type& operator[](const unsigned int index)
	{
		return t[index];
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : operator[]
	// Parameters : index - the index to access
	// Return : const Type & - returns the array element at [index]
	// Notes : performs no error checking. user should ensure index is 
	//		valid with the size() method
	/////////////////////////////////////////////////////////////////////////////
	const Type& operator[](const unsigned int index) const
	{
		return t[index];
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function :	size
	// Returns : int - returns the number of items being stored
	/////////////////////////////////////////////////////////////////////////////
	unsigned int size() const
	{
		return arraySize;
	}


	void decSize()
	{
		--arraySize;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : capacity
	// Returns : int - returns the number of items the array can store before 
	//		the next resize
	// Notes : this function returns the number of items the array can store, 
	//		not the number of bytes
	/////////////////////////////////////////////////////////////////////////////
	unsigned int capacity() const
	{
		return arrayCapacity;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function :	clear
	// Notes : cleans up any dynamic memory and resets size and capacity to 0
	/////////////////////////////////////////////////////////////////////////////
	void clear()
	{
		delete[] t;
		t = nullptr;
		arraySize = 0;
		arrayCapacity = 0;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : append
	// Parameters : item - the item to be appended to the next open spot
	// Notes : If no room exists, the array's capacity will be doubled and
	//			then the item will be added
	//
	/////////////////////////////////////////////////////////////////////////////
	void append(const Type& item)
	{
		// Check if we are over our capacity
		if (arraySize < arrayCapacity)
			t[arraySize] = item;
		// Since there is no room we need to make some
		else if (arraySize >= arrayCapacity && arraySize != 0)
		{
			// Allocate a new array that is twice as big as the original
			t2 = new Type[arrayCapacity * 2];
			// Copy each element from the original array
			for (i = 0; i < arraySize; ++i)
				t2[i] = t[i];
			// Add the new element we are trying to append
			t2[arraySize] = item;

			// Update our new capacity
			arrayCapacity *= 2;
			// Clean up original array
			delete[] t;
			// Copy the new one
			t = t2;
		}
		// If this is the first element to be added
		else
		{
			// Allocate the memory
			t = new Type[1];
			// Assign the element
			t[0] = item;
			// Update the capacity
			arrayCapacity = 1;
		}
		// Increment the size
		++arraySize;
	}
	/////////////////////////////////////////////////////////////////////////////
	// Function : reserve
	// Parameters : newCap - the new capacity
	// Notes : 	- default parameter - reserve more space in the array, based on 
	//		the expansion rate (100%, 1 minimum).
	//		- non-default parameter, expand to the specified capacity
	//		- if newCap is LESS than the current capacity, do nothing. 
	//		This function should NOT make the array smaller.
	/////////////////////////////////////////////////////////////////////////////
	void reserve(const unsigned int & newCap = 0)
	{
		if (newCap == arrayCapacity || newCap == 0)
		{
			if (arrayCapacity != 0)
			{
				// Allocate a new array that is twice as big as the original
				t2 = new Type[arrayCapacity * 2];
				// Copy over every element
				for (i = 0; i < arraySize; ++i)
					t2[i] = t[i];

				// Update the capacity
				arrayCapacity *= 2;
				// Clean up the original array
				delete[] t;
				// Copy the new one
				t = t2;
			}
			// If capacity was 0
			else
			{
				// Allocate memory for the array
				t = new Type[1];
				// Update the capacity
				arrayCapacity = 1;
			}
		}
		// If trying to make the capacity smaller than it is now
		else if (newCap < arrayCapacity)
			return;
		else
		{
			// Allocate a new array that is the capacity of what was passed in
			t2 = new Type[newCap];
			// Copy over every element
			for (i = 0; i < arraySize; ++i)
				t2[i] = t[i];

			// Update the capacity
			arrayCapacity = newCap;
			// Clean up the original array
			delete[] t;
			// Copy the new one
			t = t2;
		}
	}
};