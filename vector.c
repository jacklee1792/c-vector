#include <stdlib.h>
#include <string.h>

/*
Return types of non-void functions
*/
#define BAD_ALLOC 0
#define OK 1

/*
Pseudo-C++ style template parameter
*/
#define T int

/*
Other constants
*/
#define INIT_CAPACITY 8
#define SCALING_FACTOR 2

/*
Class declaration
*/
typedef struct {
	T* contents;
	size_t size;
	size_t capacity;
} vector;

/*
Forward declarations
*/
int vector_init(vector* vec);
void vector_clear(vector* vec);
int vector_reserve(vector* vec, size_t count);
int vector_resize(vector* vec, size_t count);
int vector_assign(vector* vec, size_t count, T value);
int vector_push_back(vector* vec, T value);
int vector_pop_back(vector* vec);
int vector_insert(vector* vec, size_t index, T value);
int vector_erase_range(vector* vec, size_t leftmost, size_t rightmost);
int vector_erase(vector* vec, size_t index);

/*
Initailizes the vector with size INIT_CAPACITY.

Complexity: O(1)
*/
int vector_init(vector* vec) {
	vec->contents = NULL;
	vec->size = 0;
	vec->capacity = 0;
	return vector_reserve(vec, INIT_CAPACITY);
}

/*
Removes all existing elements in vector.
Resets size and capacity.

Complexity: O(N)
*/
void vector_clear(vector* vec) {
	free(vec->contents);
	vec->size = 0;
	vec->capacity = 0;
}

/*
Reserves new memory block of size count using realloc to ensure that
vector has at enough capacity to hold at least count elements.
If count is not greater than current capacity, call is ignored.
Otherwise, contents are reallocated into new block of memory using realloc.

Complexity: O(N)
*/
int vector_reserve(vector* vec, size_t count) {
	if(count <= vec->capacity) {
		return OK;
	}
	T* ptr = realloc(vec->contents, count * sizeof(T));
	if(ptr == NULL) {
		return BAD_ALLOC;
	}
	vec->contents = ptr;
	vec->capacity = count;
	return OK;
}

/*
Container is forcibly resized to new size using realloc.
If count is smaller than current size, then contents are truncated.
Otherwise, new elements are set to 0.

Complexity: O(N)
*/
int vector_resize(vector* vec, size_t count) {
	T* ptr = realloc(vec->contents, count * sizeof(T));
	if(ptr == NULL && count != 0) {
		return BAD_ALLOC;
	}
	if(count > vec->size) {
		memset(vec->contents + count, 0, (count - vec->size) * sizeof(T));
	}
	vec->contents = ptr;
	vec->size = count;
	vec->capacity = count;
	return OK;
}

/*
Calls vector_resize to resize vector, and assigns all elements.

Complexity: O(N)
*/
int vector_assign(vector* vec, size_t count, T value) {
	if(vector_resize(vec, count)) {
		for(T* ptr = vec->contents; ptr < vec->contents + count; ++ptr) {
			*ptr = value;
		}
		return OK;
	}
	return BAD_ALLOC;
}

/*
Dynamically scales container with vector_reserve, then appends element.

Complexity: Amortized O(1)
*/
int vector_push_back(vector* vec, T value) {
	if(vec->size == vec->capacity && !vector_reserve(vec, SCALING_FACTOR * vec->size + 1)) {
		return BAD_ALLOC;
	}
	vec->contents[vec->size] = value;
	++(vec->size);
	return OK;
}

/*
Pops last element in container, reducing its size by 1.

Complexity: O(1)
*/
int vector_pop_back(vector* vec) {
	if(vec->size == 0) {
		return BAD_ALLOC;
	}
	--(vec->size);
	return OK;
}

/*
Inserts an element before the given index.

Complexity: O(N)
*/
int vector_insert(vector* vec, size_t index, T value) {
	if(index < 0 || index >= vec->size) {
		return BAD_ALLOC;
	}
	if(vec->size == vec->capacity && !vector_reserve(vec, SCALING_FACTOR * vec->size + 1)) {
		return BAD_ALLOC;
	}
	memmove(vec->contents + index + 1, vec->contents + index, (vec->size - index) * sizeof(T));
	vec->contents[index] = value;
	++(vec->size);
	return OK;
}

/*
Erases range [leftmost, righmost].

Complexity: O(N)
*/
int vector_erase_range(vector* vec, size_t leftmost, size_t rightmost) {
	if(leftmost > rightmost || leftmost < 0 || rightmost >= vec->size) {
		return BAD_ALLOC;
	}
	memmove(vec->contents + leftmost, vec->contents + rightmost + 1, (vec->size - rightmost - 1) * sizeof(T));
	vec->size -= rightmost - leftmost + 1;
	return OK;
}

/*
Erases single element at index.

Complexity: O(N)
*/
int vector_erase(vector* vec, size_t index) {
	return vector_erase_range(vec, index, index);
}