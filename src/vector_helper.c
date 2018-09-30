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

#include "vector_helper.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void static inline __vector_helper_block_element_assignment(const char* element_destination,
	const void * element_source, const size_t* element_assignment_size)
{
	*(((VectorBlockBuffer*)element_destination) + 0) = *(((VectorBlockBuffer*)element_source) + 0);
	if ((*element_assignment_size) > 4)
	{
		*(((VectorBlockBuffer*)element_destination) + 1) = *(((VectorBlockBuffer*)element_source) + 1);
		if ((*element_assignment_size) > 8)
		{
			*(((VectorBlockBuffer*)element_destination) + 2) = *(((VectorBlockBuffer*)element_source) + 2);
			if ((*element_assignment_size) > 12)
				*(((VectorBlockBuffer*)element_destination) + 3) = *(((VectorBlockBuffer*)element_source) + 3);
		}
	}
}

bool static inline __vector_helper_block_element_equal(const char* element_value1, const char* element_value2,
	size_t equal_value_size)
{
	size_t equal_need_count = equal_value_size / 4;
	for (size_t equal_count = 0; equal_count < equal_need_count; equal_count++)
	{
		if (!(*(element_value1 + (equal_count * 4)) == *(element_value2 + (equal_count * 4))
			&& *(element_value1 + (equal_count * 4) + 1) == *(element_value2 + (equal_count * 4) + 1)
			&& *(element_value1 + (equal_count * 4) + 2) == *(element_value2 + (equal_count * 4) + 2)
			&& *(element_value1 + (equal_count * 4) + 3) == *(element_value2 + (equal_count * 4) + 3)))
		{
			return false;
		}
	}
	return true;
}

VectorContext * vector_helper_create_object(size_t single_element_size, size_t init_element_capacity,
	size_t max_element_count)
{
	VectorContext* vector_context_result = NULL;
	if (single_element_size > 0 && init_element_capacity > 0 && max_element_count >= 0
		&& (max_element_count > 0) ? (init_element_capacity <= max_element_count) : true)
	{
		vector_context_result = (VectorContext*)malloc(sizeof(VectorContext));
		if (vector_context_result)
		{
			bool operation_success = false;
			memset(vector_context_result, 0, sizeof(VectorContext));
			vector_context_result->single_element_size = single_element_size;
			vector_context_result->current_element_count = 0;
			vector_context_result->max_element_count = max_element_count;
			vector_context_result->current_alloc_count = 1;
			vector_context_result->current_alloc_memory_size = (single_element_size * init_element_capacity);
			vector_context_result->element = (void**)malloc(vector_context_result->current_alloc_memory_size);
			if (vector_context_result->element)
			{
				memset(vector_context_result->element, 0, vector_context_result->current_alloc_memory_size);
				operation_success = true;
			}
			if (!operation_success)
			{
				vector_helper_destroy_object(vector_context_result);
				vector_context_result = NULL;
			}
		}
	}
	return vector_context_result;
}

bool vector_helper_destroy_object(VectorContext * vector_object_context)
{
	bool free_result = false;
	if (vector_object_context)
	{
		if (vector_object_context->element)
		{
			memset(vector_object_context->element, 0, 
				vector_object_context->current_alloc_memory_size);
			free(vector_object_context->element);
		}
		memset(vector_object_context, 0, sizeof(VectorContext));
		free(vector_object_context);
		free_result = true;
	}
	return free_result;
}

size_t vector_helper_get_count(VectorContext * vector_object_context, VectorCountType count_type)
{
	size_t element_count_result = 0;
	if (vector_object_context && count_type >= 0)
	{
		switch (count_type)
		{
		case VCT_CURRENT_ELEMENT_COUNT:
			element_count_result = vector_object_context->current_element_count; break;
		case VCT_MAX_ELEMENT_COUNT:
			element_count_result = vector_object_context->max_element_count; break;
		case VCT_CURRENT_ALLOC_MEMORY_COUNT:
			element_count_result = vector_object_context->current_alloc_count; break;
		}
	}
	return element_count_result;
}

size_t vector_helper_get_size(VectorContext * vector_object_context, VectorSizeType size_type)
{
	size_t size_result = 0;
	if (vector_object_context && size_type >= 0)
	{
		switch (size_type)
		{
		case VST_CURRENT_ALLOC_MEMORY_SIZE:
			size_result = vector_object_context->current_alloc_memory_size; break;
		case VST_SINGLE_ELEMENT_SIZE:
			size_result = vector_object_context->single_element_size; break;
		case VST_AVAILABLE_MEMORY_SIZE:
			size_result = vector_object_context->current_alloc_memory_size -
				(vector_object_context->single_element_size * vector_object_context->current_element_count); break;
		case VST_USED_MEMORY_SIZE:
			size_result = vector_object_context->single_element_size * 
				vector_object_context->current_element_count; break;
		}
	}
	return size_result;
}

size_t vector_helper_get_capacity(VectorContext * vector_object_context)
{
	return (vector_object_context && vector_object_context->current_alloc_memory_size > 0
		&& vector_object_context->single_element_size > 0) ? (vector_object_context->current_alloc_memory_size /
			vector_object_context->single_element_size) : 0;
}

/*
	message: this function code has been deprecated because it contains potential performance and security issues.
	--------
	size_t vector_helper_push_element(VectorContext * vector_object_context, const void * element_value, 
		size_t element_value_size)
	{
		size_t current_element_count_result = 0;
		if (vector_object_context && vector_object_context->element && element_value
			&& element_value_size <= vector_object_context->single_element_size)
		{
			if (!(vector_object_context->max_element_count > 0
				&& vector_object_context->current_element_count >=
				vector_object_context->max_element_count))
			{
				size_t available_memory_size = vector_object_context->current_alloc_memory_size - 
					(vector_object_context->single_element_size * vector_object_context->current_element_count);
				char* offset_element = NULL;
				bool realloc_error = false;
				if (available_memory_size < vector_object_context->single_element_size)
				{
					size_t current_alloc_memory_size = vector_object_context->current_alloc_memory_size;
					offset_element = (char*)vector_object_context->element;
					vector_object_context->element = (void**)realloc(vector_object_context->element, 
						vector_object_context->current_alloc_memory_size * VECTOR_ELEMENT_EXPANSION_FACTOR);
					if (!vector_object_context->element)
					{
						vector_object_context->element = (void**)offset_element;
						offset_element = NULL;
						realloc_error = true;
					}
					else vector_object_context->current_alloc_memory_size += current_alloc_memory_size;
				}
				if (!realloc_error)
				{
					offset_element = ((char*)(vector_object_context->element)) +
						(vector_object_context->single_element_size * vector_object_context->current_element_count);
					memcpy(offset_element, element_value, element_value_size);
					vector_object_context->last_reserved_element_count = 0;
					current_element_count_result = ++vector_object_context->current_element_count;
				}
			}
		}
		return current_element_count_result;
	}
*/

bool vector_helper_push_element(VectorContext * vector_object_context, const void * element_value, 
	size_t element_value_size, size_t* push_element_index)
{
	bool push_element_result = false;
	if (vector_object_context && element_value && element_value_size > 0 
		&& element_value_size <= vector_object_context->single_element_size)
	{
		size_t reserve_element_index = 0;
		if (vector_helper_try_reserve_element(vector_object_context, 1, &reserve_element_index))
		{
			char* offset_start_element = ((char*)(vector_object_context->element)) + 
				(vector_object_context->single_element_size * reserve_element_index);
			if (element_value_size <= 16 && (element_value_size % 4) == 0)
			{
				__vector_helper_block_element_assignment(offset_start_element, element_value, 
					&element_value_size);
			}
			else memcpy(offset_start_element, element_value, element_value_size);
			vector_object_context->last_reserved_element_count = 0;
			if (push_element_index) *push_element_index = reserve_element_index;
			push_element_result = true;
		}
	}
	return push_element_result;
}

bool vector_helper_push_multiple_element(VectorContext* vector_object_context, const void* multiple_element_value, 
	size_t single_element_value_size, size_t push_element_count, size_t* push_start_element_index)
{
	bool push_multiple_element_result = false;
	if (vector_object_context && multiple_element_value && single_element_value_size > 0
		&& single_element_value_size <= vector_object_context->single_element_size && push_element_count > 0)
	{
		size_t reserve_element_index = 0;
		if (vector_helper_try_reserve_element(vector_object_context, push_element_count, &reserve_element_index))
		{
			if (single_element_value_size == vector_object_context->single_element_size) // align push
			{
				memcpy(((char*)(vector_object_context->element)) + (vector_object_context->single_element_size *
					reserve_element_index), multiple_element_value, single_element_value_size * push_element_count);
			}
			else // split push
			{
				char* offset_start_element = ((char*)(vector_object_context->element)) +
					(vector_object_context->single_element_size * reserve_element_index);
				for (size_t loop_count = 0; loop_count < push_element_count; loop_count++)
				{
					memcpy(offset_start_element + (vector_object_context->single_element_size * loop_count),
						((char*)multiple_element_value) + single_element_value_size * loop_count, single_element_value_size);
				}
			}
			vector_object_context->last_reserved_element_count = 0;
			if (push_start_element_index) *push_start_element_index = reserve_element_index;
			push_multiple_element_result = true;
		}
	}
	return push_multiple_element_result;
}

bool vector_helper_pop_element(VectorContext * vector_object_context, void * element_value,
	size_t element_value_size)
{
	bool pop_element_result = false;
	if (vector_object_context && vector_object_context->element
		&& vector_object_context->current_element_count >= 1)
	{
		if (vector_helper_get_element(vector_object_context, 0, element_value, element_value_size) > 0)
			vector_helper_remove_element(vector_object_context, 0, 1, &pop_element_result);
	}
	return pop_element_result;
}

bool vector_helper_set_element(VectorContext * vector_object_context, size_t element_index,
	const void * element_value, size_t element_value_size)
{
	bool set_element_result = false;
	if (vector_object_context && vector_object_context->element && element_index >= 0
		&& ((int)element_index) < vector_object_context->current_element_count && element_value
		&& element_value_size <= vector_object_context->single_element_size)
	{
		char* offset_start_element = ((char*)(vector_object_context->element)) +
			(vector_object_context->single_element_size * element_index);
		if (element_value_size <= 16 && (element_value_size % 4) == 0)
		{
			__vector_helper_block_element_assignment(offset_start_element, element_value, 
				&element_value_size);
		}
		else memcpy(offset_start_element, element_value, vector_object_context->single_element_size);
		set_element_result = true;
	}
	return set_element_result;
}

size_t vector_helper_get_element(VectorContext * vector_object_context, size_t element_index,
	void * element_value, size_t element_value_size)
{
	size_t element_value_size_result = 0;
	if (vector_object_context && vector_object_context->element && element_index >= 0
		&& element_value && element_value_size >= vector_object_context->single_element_size
		&& ((int)element_index) < vector_object_context->current_element_count)
	{
		char* offset_start_element = ((char*)(vector_object_context->element)) +
			(vector_object_context->single_element_size * element_index);
		if (vector_object_context->single_element_size <= 16 
			&& (vector_object_context->single_element_size % 4) == 0)
		{
			__vector_helper_block_element_assignment((char*)element_value, offset_start_element, 
				&vector_object_context->single_element_size);
		}
		else memcpy(element_value, offset_start_element, vector_object_context->single_element_size);
		element_value_size_result = vector_object_context->single_element_size;
	}
	return element_value_size_result;
}

void * vector_helper_get_element_value(VectorContext * vector_object_context, size_t element_index) // unsafe
{
	void* element_value_result = NULL;
	if (vector_object_context && vector_object_context->element && element_index >= 0
		&& ((int)element_index) < vector_object_context->current_element_count)
	{
		element_value_result = ((char*)(vector_object_context->element)) +
			(vector_object_context->single_element_size * element_index);
	}
	return element_value_result;
}

size_t vector_helper_remove_element(VectorContext * vector_object_context, size_t element_start_index,
	size_t remove_count, bool* remove_result)
{
	size_t current_element_count_result = 0;
	if (vector_object_context && vector_object_context->element
		&& remove_count > 0 && ((int)element_start_index) >= 0
		&& (element_start_index + remove_count) <= vector_object_context->current_element_count)
	{
		size_t remove_memory_size = vector_object_context->single_element_size * remove_count;
		size_t start_memory_size = vector_object_context->single_element_size * element_start_index;
		size_t used_memory_size = vector_object_context->single_element_size * vector_object_context->current_element_count;
		size_t end_memory_size = used_memory_size - (start_memory_size + remove_memory_size);
		if (remove_memory_size > 0 && used_memory_size > 0)
		{
			char* offset_element = ((char*)(vector_object_context->element)) + start_memory_size;
			char* end_element = offset_element + remove_memory_size;
			if (offset_element >= ((char*)(vector_object_context->element)) && end_element)
			{
				memset(offset_element, 0, remove_memory_size);
				if(end_memory_size > 0) memmove(offset_element, end_element, end_memory_size);
				current_element_count_result = (vector_object_context->current_element_count -= remove_count);
				vector_object_context->last_reserved_element_count = 0;
				memset(((char*)(vector_object_context->element)) +
					(vector_object_context->single_element_size * vector_object_context->current_element_count)
					, 0, remove_memory_size);
				if (remove_result) *remove_result = true;
			}
		}
	}
	return current_element_count_result;
}

bool vector_helper_remove_all_element(VectorContext * vector_object_context)
{
	bool remove_all_element_result = false;
	if (vector_object_context && vector_object_context->current_element_count > 0)
	{
		size_t used_memory_size = vector_object_context->single_element_size * 
			vector_object_context->current_element_count;
		if (used_memory_size > 0) memset(vector_object_context->element, 0, used_memory_size);
		vector_object_context->last_reserved_element_count = 0;
		vector_object_context->current_element_count = 0;
		remove_all_element_result = true;
	}
	return remove_all_element_result;
}

bool vector_helper_free_available_memory(VectorContext * vector_object_context)
{
	bool free_result = false;
	if (vector_object_context && vector_object_context->element)
	{
		size_t used_memory_size = vector_object_context->single_element_size * vector_object_context->current_element_count;
		if (used_memory_size > 0)
		{
			memset(((char*)(vector_object_context->element)) + (vector_object_context->single_element_size * 
				(vector_object_context->current_element_count + 1)), 0, vector_object_context->current_alloc_memory_size - used_memory_size);
			void** realloc_element = (void**)realloc(vector_object_context->element, used_memory_size);
			if (realloc_element)
			{
				vector_object_context->current_alloc_memory_size = used_memory_size;
				vector_object_context->element = realloc_element;
				free_result = true;
			}
		}
	}
	return free_result;
}

bool vector_helper_reset_all_element(VectorContext * vector_object_context)
{
	bool reset_result = false;
	if (vector_object_context && vector_object_context->element
		&& vector_object_context->current_element_count > 0)
	{
		memset(vector_object_context->element, 0, 
			vector_object_context->single_element_size * vector_object_context->current_element_count);
		reset_result = true;
	}
	return reset_result;
}

#ifdef VECTOR_ALLOW_DISCARD_FUNCTION
/* message: this function has a known performance issue and has been replaced by the function vector_helper_enum_element */

bool vector_helper_query_element(VectorContext * vector_object_context, size_t * element_index,
	void * element_value, size_t element_value_size)
{
	bool query_result = false;
	if (vector_object_context && vector_object_context->element && element_index >= 0
		&& element_value && element_value_size >= vector_object_context->single_element_size
		&& ((int)*element_index) < vector_object_context->current_element_count)
	{
		for (;*element_index < vector_object_context->current_element_count; (*element_index)++)
		{
			if (memcmp(((char*)(vector_object_context->element)) + (vector_object_context->single_element_size * 
				(*element_index)), element_value,  vector_object_context->single_element_size) == 0)
			{
				query_result = true;
				break;
			}
		}
	}
	return query_result;
}

#endif

bool vector_helper_is_empty(VectorContext * vector_object_context)
{
	return vector_object_context ? (vector_object_context->current_element_count <= 0) : false;
}

bool vector_helper_try_reserve_element(VectorContext* vector_object_context, size_t reserve_element_count,
	size_t* reserve_element_start_index)
{
	bool try_reserve_result = false;
	if (vector_object_context && vector_object_context->element && reserve_element_count > 0)
	{
		if (!(vector_object_context->max_element_count > 0
			&& vector_object_context->current_element_count >=
			vector_object_context->max_element_count))
		{
			bool verify_memory_size = false;
			size_t current_memory_size = vector_object_context->current_alloc_memory_size;
			size_t reserve_memory_size = vector_object_context->single_element_size * reserve_element_count;
			if ((current_memory_size - (vector_object_context->single_element_size *
				vector_object_context->current_element_count)) < reserve_memory_size)
			{
				size_t realloc_memory_size = reserve_memory_size + (current_memory_size * VECTOR_ELEMENT_EXPANSION_FACTOR);
				size_t max_memory_size = vector_object_context->single_element_size * vector_object_context->max_element_count;
				if (vector_object_context->max_element_count > 0 && realloc_memory_size > max_memory_size)
					realloc_memory_size = max_memory_size;
				void** realloc_element = (void**)realloc(vector_object_context->element, realloc_memory_size);
				if (realloc_element)
				{
					vector_object_context->element = realloc_element;
					vector_object_context->current_alloc_memory_size = realloc_memory_size;
					memset(((char*)vector_object_context->element) + current_memory_size, 0, realloc_memory_size - current_memory_size);
					if (vector_object_context->current_alloc_count < INT32_MAX)
						vector_object_context->current_alloc_count++;
					else vector_object_context->current_alloc_count = 1;
					verify_memory_size = true;
				}
			}
			else verify_memory_size = true;
			if (verify_memory_size)
			{
				vector_object_context->current_element_count += reserve_element_count;
				vector_object_context->last_reserved_element_count = reserve_element_count;
				if (reserve_element_start_index)
					*reserve_element_start_index = vector_object_context->current_element_count - reserve_element_count;
				try_reserve_result = true;
			}
		}
	}
	return try_reserve_result;
}

bool vector_helper_handover_element(VectorContext* vector_object_context, size_t reserve_element_start_index,
	size_t handover_element_count, bool used_reserve_element)
{
	bool handover_result = false;
	if (vector_object_context && vector_object_context->element && vector_object_context->current_element_count > 0
		&& ((int)reserve_element_start_index) >= 0 && handover_element_count >= 0)
	{
		if (vector_object_context->last_reserved_element_count > 0
			&& reserve_element_start_index == 0 && handover_element_count == 0)
		{
			vector_object_context->current_element_count -= vector_object_context->last_reserved_element_count;
			if (used_reserve_element)
			{
				size_t handover_memory_size = vector_object_context->single_element_size * vector_object_context->last_reserved_element_count;
				if (handover_memory_size > 0)
				{
					memset(((char*)vector_object_context->element) + (vector_object_context->single_element_size *
						vector_object_context->current_element_count), 0, handover_memory_size);
				}
			}
			handover_result = true;
		}
		else vector_helper_remove_element(vector_object_context, reserve_element_start_index, 
			handover_element_count, &handover_result);
		if(handover_result) vector_object_context->last_reserved_element_count = 0;
	}
	return handover_result;
}

bool vector_helper_used_reserve_element(VectorContext * vector_object_context, size_t used_reserve_element_count)
{
	bool used_reserve_result = false;
	if (vector_object_context && vector_object_context->last_reserved_element_count > 0
		&& used_reserve_element_count <= vector_object_context->last_reserved_element_count)
	{
		if (used_reserve_element_count > 0)
		{
			size_t unused_reserve_element_count = vector_object_context->last_reserved_element_count - used_reserve_element_count;
			if (unused_reserve_element_count > 0)
			{
				vector_object_context->current_element_count -= unused_reserve_element_count;
				memset(((char*)vector_object_context->element) + (vector_object_context->single_element_size *
					vector_object_context->current_element_count), 0, 
					(vector_object_context->single_element_size * unused_reserve_element_count));
			}
		}
		vector_object_context->last_reserved_element_count = 0;
		used_reserve_result = true;
	}
	return used_reserve_result;
}

bool vector_helper_get_last_element_index(VectorContext * vector_object_context, size_t * last_element_index)
{
	bool get_result = false;
	if (vector_object_context && last_element_index && vector_object_context->current_element_count > 0)
	{
		*last_element_index = vector_object_context->current_element_count - 1;
		get_result = true;
	}
	return get_result;
}

bool vector_helper_is_valid_element_index(VectorContext * vector_object_context, size_t element_index)
{
	return (vector_object_context && element_index >= 0) ? 
		(((int)element_index) < vector_object_context->current_element_count) : false;
}

bool vector_helper_enum_element(VectorContext * vector_object_context, size_t * element_start_index, 
	const void * element_value, size_t element_value_size)
{
	bool enum_result = false;
	if (vector_object_context && element_start_index && element_value && element_value_size > 0 
		&& element_value_size <= vector_object_context->single_element_size && (*element_start_index) >= 0 
		&& (int)(*element_start_index) < vector_object_context->current_element_count)
	{
		if (element_value_size <= 16 && (element_value_size % 4) == 0)
		{
			for (; (*element_start_index) < vector_object_context->current_element_count; (*element_start_index)++)
			{
				if (__vector_helper_block_element_equal(((char*)vector_object_context->element) +
					(vector_object_context->single_element_size * (*element_start_index)), (char*)element_value, element_value_size))
				{
					enum_result = true;
					break;
				}
			}
		}
		else
		{
			for (; (*element_start_index) < vector_object_context->current_element_count; (*element_start_index)++)
			{
				if (memcmp(((char*)vector_object_context->element) + (vector_object_context->single_element_size *
					(*element_start_index)), element_value, element_value_size) == 0)
				{
					enum_result = true;
					break;
				}
			}
		}
	}
	return enum_result;
}
