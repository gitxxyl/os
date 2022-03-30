#include <lib/ordered_array.h>
#include <lib/assert.h>

bool standard_lessthan_predicate(void* a, void* b){
    return (a<b)?true:false;
}

ordered_array_t create_ordered_array(uint64_t max_size, lessthan_predicate_t less_than){
   ordered_array_t ar;
   ar.array = (void*)kmalloc(max_size*sizeof(void*));
   memset(ar.array, 0, max_size*sizeof(void*));
   ar.size = 0;
   ar.max_size = max_size;
   ar.less_than = less_than;
   return ar;
}

ordered_array_t place_ordered_array(void *addr, uint64_t max_size, lessthan_predicate_t less_than){
   ordered_array_t to_ret;
   to_ret.array = (void**)addr;
   memset(to_ret.array, 0, max_size*sizeof(void*));
   to_ret.size = 0;
   to_ret.max_size = max_size;
   to_ret.less_than = less_than;
   return to_ret;
}

void destroy_ordered_array(ordered_array_t *array){
// kfree(array->array);
}
void insert_ordered_array(void* item, ordered_array_t *array){
   assert(array->less_than);
   uint64_t i = 0;
   while (i < array->size && array->less_than(array->array[i], item))
       i++;
   if (i == array->size) // just add at the end of the array.
       array->array[array->size++] = item;
   else
   {
       void* tmp = array->array[i];
       array->array[i] = item;
       while (i < array->size)
       {
           i++;
           void* tmp2 = array->array[i];
           array->array[i] = tmp;
           tmp = tmp2;
       }
       array->size++;
   }
}

void* lookup_ordered_array(uint64_t i, ordered_array_t *array){
   assert(i < array->size);
   return array->array[i];
}
void remove_ordered_array(uint64_t i, ordered_array_t *array){
   while (i < array->size){
       array->array[i] = array->array[i+1];
       i++;
   }
   array->size--;
} 




