/*

	Vector Helper
	--------
	Version: 1.0.2 Modify 2018/9/30 14:50:47
	Website: https://www.xiaoyy.org/
	Docsite: https://smallso.gitbook.io/vector/
	License: MIT (From https://opensource.org/licenses/MIT)
	--------
	The MIT License (MIT)
	Copyright 2018 SmallSO Studios.

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the "Software"), 
	to deal in the Software without restriction, including without limitation the 
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
	sell copies of the Software, and to permit persons to whom the Software 
	is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in 
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

*/

#pragma once

#ifndef VECTOR_HELPER_H_
#define VECTOR_HELPER_H_

/* define precompilation options */

#define VECTOR_ALLOW_UNSAFE_FUNCTION
// #define VECTOR_ALLOW_DISCARD_FUNCTION

/* include the necessary header files */

#include <stddef.h>
#include <stdlib.h>

/* define a basic type other than cplsuplus */

#ifndef __cplusplus

typedef _Bool bool;

#define true 1
#define false 0

#endif // __cplusplus

/* define the C statement under cplsuplus (start) */

#ifdef __cplusplus

extern "C"
{

#endif // __cplusplus

/* define related data structures */

typedef struct _VectorContext
{
	size_t current_element_count;
	size_t last_reserved_element_count;
	size_t max_element_count;
	size_t current_alloc_count;
	size_t current_alloc_memory_size;
	size_t single_element_size;
	void** element;
} VectorContext, *Vector;

typedef struct _VectorBlockBuffer
{
	char element_block_buffer[4];
} VectorBlockBuffer;

typedef enum _VectorCountType
{
	kCurrentElementCount,
	kMaxElementCount,
	kCurrentMemoryAllocCount
} VectorCountType;

typedef enum _VectorSizeType
{
	kCurrentAllocMemorySize,
	kSingleElementSize,
	kAvailableMemorySize,
	kUsedMemorySize
} VectorSizeType;

/* define the macro corresponding to the enumerator */

#ifdef __cplusplus

#define VST_CURRENT_ALLOC_MEMORY_SIZE VectorSizeType::kCurrentAllocMemorySize
#define VST_SINGLE_ELEMENT_SIZE VectorSizeType::kSingleElementSize
#define VST_AVAILABLE_MEMORY_SIZE VectorSizeType::kAvailableMemorySize
#define VST_USED_MEMORY_SIZE VectorSizeType::kUsedMemorySize

#define VCT_CURRENT_ELEMENT_COUNT VectorCountType::kCurrentElementCount
#define VCT_MAX_ELEMENT_COUNT VectorCountType::kMaxElementCount
#define VCT_CURRENT_ALLOC_MEMORY_COUNT VectorCountType::kCurrentMemoryAllocCount

#else

#define VST_CURRENT_ALLOC_MEMORY_SIZE 0 // VectorSizeType::kCurrentAllocMemorySize
#define VST_SINGLE_ELEMENT_SIZE 1 // VectorSizeType::kSingleElementSize
#define VST_AVAILABLE_MEMORY_SIZE 2 // VectorSizeType::kAvailableMemorySize
#define VST_USED_MEMORY_SIZE 3 // VectorSizeType::kUsedMemorySize

#define VCT_CURRENT_ELEMENT_COUNT 0 // VectorCountType::kCurrentElementCount
#define VCT_MAX_ELEMENT_COUNT 1 // VectorCountType::kMaxElementCount
#define VCT_CURRENT_ALLOC_MEMORY_COUNT 2 // VectorCountType::kCurrentMemoryAllocCount

#endif // __cplusplus

/* element buffer expansion factor, defaults to 2 times the current buffer size */

#define VECTOR_ELEMENT_EXPANSION_FACTOR 2

/* define safe function */

#define VECTOR_NULL 0
#define VECTOR_NO_MAX_ELEMENT 0

VectorContext* vector_helper_create_object(size_t single_element_size, size_t init_element_capacity,
	size_t max_element_count);

bool vector_helper_destroy_object(VectorContext* vector_object_context);

size_t vector_helper_get_count(VectorContext* vector_object_context, VectorCountType count_type);

size_t vector_helper_get_size(VectorContext* vector_object_context, VectorSizeType size_type);

size_t vector_helper_get_capacity(VectorContext* vector_object_context);

/*
	message: this function code has been deprecated because it contains potential performance and security issues.
	--------
	size_t vector_helper_push_element(VectorContext* vector_object_context, const void* element_value,
		size_t element_value_size);
*/

bool vector_helper_push_element(VectorContext* vector_object_context, const void* element_value,
	size_t element_value_size, size_t* push_element_index);

bool vector_helper_push_multiple_element(VectorContext* vector_object_context, 
	const void* multiple_element_value, size_t single_element_value_size, size_t push_element_count, 
	size_t* push_start_element_index);

bool vector_helper_pop_element(VectorContext* vector_object_context, void* element_value,
	size_t element_value_size);

bool vector_helper_set_element(VectorContext* vector_object_context, size_t element_index,
	const void* element_value, size_t element_value_size);

size_t vector_helper_get_element(VectorContext* vector_object_context, size_t element_index,
	void* element_value, size_t element_value_size);

size_t vector_helper_remove_element(VectorContext* vector_object_context, size_t element_start_index,
	size_t remove_count, bool* remove_result);

bool vector_helper_remove_all_element(VectorContext* vector_object_context);

bool vector_helper_free_available_memory(VectorContext* vector_object_context);

bool vector_helper_reset_all_element(VectorContext* vector_object_context);

#ifdef VECTOR_ALLOW_DISCARD_FUNCTION
// message: this function has a known performance issue and has been replaced by the function vector_helper_enum_element

bool vector_helper_query_element(VectorContext* vector_object_context, size_t* element_index,
	void* element_value, size_t element_value_size);

#endif

bool vector_helper_is_empty(VectorContext* vector_object_context);

bool vector_helper_try_reserve_element(VectorContext* vector_object_context, size_t reserve_element_count, 
	size_t* reserve_element_start_index);

bool vector_helper_handover_element(VectorContext* vector_object_context, size_t reserve_element_start_index,
	size_t handover_element_count, bool used_reserve_element);

#define VECTOR_USED_ALL_RESERVED_ELEMENT 0

bool vector_helper_used_reserve_element(VectorContext* vector_object_context, size_t used_reserve_element_count);

bool vector_helper_get_last_element_index(VectorContext* vector_object_context, size_t* last_element_index);

bool vector_helper_is_valid_element_index(VectorContext* vector_object_context, size_t element_index);

bool vector_helper_enum_element(VectorContext* vector_object_context, size_t* element_start_index,
	const void* element_value, size_t element_value_size);

/* define unsafe function */

#ifdef VECTOR_ALLOW_UNSAFE_FUNCTION

/* warning: unable to ensure element and buffer integrity when using the vector_helper_get_element_value function */

void* vector_helper_get_element_value(VectorContext* vector_object_context, size_t element_index); // unsafe

#endif // VECTOR_ALLOW_UNSAFE_FUNCTION

/* define the C statement under cplsuplus (end) */

#ifdef __cplusplus

} // __cplusplus

#endif // __cplusplus

#endif // VECTOR_HELPER_H_
