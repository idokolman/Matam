#include "map.h"
#include "../utilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef struct Node_t {
	char* key;
	char* value;
	struct Node_t* next;
}*MapEntry;

struct Map_t {
	MapEntry dummy;
	MapEntry internal_iterator; 
	MapEntry iterator;
};

/**
* mapEntryDestroy: Deallocates an entry and it's key and value
*
* @param to_free - MapEntry to destroy
*/
static void mapEntryDestroy(MapEntry to_free)
{
	if (!to_free)
	{
		return;
	}
	free(to_free->key);
	free(to_free->value);
    free (to_free); 
}

/**
* mapEntryDestroyAll: Deallocates a list of entries - thier key and value and entry itself
*
* @param run_ptr - head of MapEntry list to destroy
*/
static void mapEntryDestroyAll(MapEntry run_ptr)
{
    while (run_ptr) //frees until reached the end (==NULL)
	{
		MapEntry to_delete=run_ptr;
		run_ptr=run_ptr->next;
		mapEntryDestroy(to_delete); //delete the curr entry (key+value)
	}
}

/**
* mapEntryCreate: Allocates a new entry with it's key and value parameters to the top of a list of entries
*
* @param head - first entry of entries list
* @param input_key - key identifier in the map
* @param input_value - the maped value to the given key
* @return
* 	NULL - if one of the allocations failed.
* 	A new MapEntry in case of success.
*/
static MapEntry mapEntryCreate(const char* input_key, const char* input_value)
{
	MapEntry new_entry = malloc(sizeof(*new_entry));
	VALIDATE_ARGUMENT(new_entry,NULL);
	new_entry->next = NULL; //a first dummy entry to be initialized
	new_entry->key = NULL;
	new_entry->value = NULL;
	if (input_key!=NULL && input_value!=NULL){ //not a first dummy entry
		new_entry->key = duplicateData(input_key); // allocate key
		if (!new_entry->key)
		{
			mapEntryDestroy(new_entry);
			return NULL;
		}
		new_entry->value = duplicateData(input_value); // allocate value
		if (!new_entry->value)
		{
			mapEntryDestroy(new_entry); 
			return NULL;
		}
		new_entry->next = NULL;
	}
	return new_entry;
}

Map mapCreate()
{
    Map new_map = malloc(sizeof(*new_map));
	VALIDATE_ARGUMENT(new_map,NULL);
	new_map->dummy = mapEntryCreate(NULL, NULL); //dummy head for the linked list
    if (!new_map->dummy) {
		mapDestroy(new_map); 
        return NULL;
    }
	new_map->internal_iterator=new_map->dummy;
    new_map->iterator=NULL; 
    return new_map;
}

void mapDestroy(Map map)
{
	if(!map)
	{
		return;
	}
	mapClear(map); //first clear the whole map (it's MapEntries)
    free(map->dummy);
	free(map);
}

int mapGetSize(Map map)
{
	VALIDATE_ARGUMENT(map, -1);
	int counter=0;
	for (map->internal_iterator=map->dummy->next; map->internal_iterator !=NULL;
	 	 map->internal_iterator=map->internal_iterator->next) //runs on the whole map and counts
	{
		counter++;
	}
	return counter;
}

bool mapContains(Map map, const char* key)
{
	VALIDATE_ARGUMENT(map,false);
	VALIDATE_ARGUMENT(key,false);
	VALIDATE_ARGUMENT(map->dummy,false);
	for (map->internal_iterator=map->dummy->next; map->internal_iterator !=NULL;
	 	 map->internal_iterator=map->internal_iterator->next)//runs on the whole map
	{
		if (strcmp(map->internal_iterator->key,key)==0){
			return true; // internal iterator points to the entry that matches this key
		}
	} 
	return false;
}

char* mapGet(Map map, const char* key)
{
	VALIDATE_ARGUMENT(map,NULL);
	VALIDATE_ARGUMENT(key,NULL);
	if (mapContains(map,key)==false){ //internal iterator will point to the right key if true
		return NULL;
	}
	return (map->internal_iterator)->value; 
}

MapResult mapPut(Map map, const char* key, const char* data)
{
	VALIDATE_ARGUMENT(map,MAP_NULL_ARGUMENT);
	VALIDATE_ARGUMENT(key,MAP_NULL_ARGUMENT);
	VALIDATE_ARGUMENT(data,MAP_NULL_ARGUMENT);
	if (mapContains(map,key)==false){ // if its a new key
		MapEntry new_entry=mapEntryCreate(key,data);
		VALIDATE_ARGUMENT(new_entry,MAP_OUT_OF_MEMORY);		
		if (map->dummy->next == NULL) { // first real value (after dummy)
			map->dummy->next = new_entry; // make new_entry to be first after dummy 
			return MAP_SUCCESS;
		}
		else { //not the first value
			assert(map->dummy->next != NULL);
        	new_entry->next = map->dummy->next; // connect new_entry to the first real value
        	map->dummy->next = new_entry; // make new_entry to be first after dummy
        	return MAP_SUCCESS;
		}
	}
	else // existing key
	{
		assert(strcmp(map->internal_iterator->key, key)==0);//internal iterator after mapContains is on the right key
		free(map->internal_iterator->value); 
		map->internal_iterator->value=duplicateData(data); 
		if (map->internal_iterator->value == NULL) {
                return MAP_OUT_OF_MEMORY;
        } 
	}
	return MAP_SUCCESS;
}

Map mapCopy(Map map)
{
	VALIDATE_ARGUMENT(map,NULL);
    Map copy_map=mapCreate();
	VALIDATE_ARGUMENT(copy_map,NULL);
	for (map->internal_iterator=map->dummy->next; map->internal_iterator !=NULL;
	 	 map->internal_iterator=map->internal_iterator->next)
		{
			if (mapPut(copy_map,map->internal_iterator->key,map->internal_iterator->value)!=MAP_SUCCESS)
			{
				mapDestroy(copy_map); // if put did not succeed
				return NULL;
			}	  	
		}
    return copy_map;
}

MapResult mapRemove(Map map, const char* key)
{
	VALIDATE_ARGUMENT(key, MAP_NULL_ARGUMENT);
	VALIDATE_ARGUMENT(map, MAP_NULL_ARGUMENT);
	VALIDATE_ARGUMENT(map->dummy->next, MAP_ITEM_DOES_NOT_EXIST); // if list is empty
	if (strcmp(map->dummy->next->key,key)==0){ //if the key to remove is the first
		MapEntry temp=map->dummy->next; 
		map->dummy->next=map->dummy->next->next;
		mapEntryDestroy (temp);
		return MAP_SUCCESS;
	}
	map->internal_iterator=map->dummy->next;
	MapEntry previous=map->internal_iterator;
	while (map->internal_iterator!=NULL && strcmp(map->internal_iterator->key,key)!=0) //searches for the key
	{
		previous=map->internal_iterator; // save previous 
		map->internal_iterator=map->internal_iterator->next; // check next
	}
	VALIDATE_ARGUMENT(map->internal_iterator, MAP_ITEM_DOES_NOT_EXIST); //if reached end of list and key was not found
	previous->next=map->internal_iterator->next; //key was found - remove it from middle
	map->internal_iterator->next=NULL;
	mapEntryDestroy(map->internal_iterator);
	return MAP_SUCCESS;
}

char* mapGetFirst(Map map)
{
	VALIDATE_ARGUMENT(map, NULL);
	VALIDATE_ARGUMENT(map->dummy->next, NULL);// empty map
	map->iterator=map->dummy->next; //iterator is now head
	return map->iterator->key; 
}

char* mapGetNext(Map map)
{
	VALIDATE_ARGUMENT(map, NULL);
	VALIDATE_ARGUMENT(map->iterator, NULL);
	map->iterator=map->iterator->next; //iterator moves one forward
	VALIDATE_ARGUMENT(map->iterator, NULL); //checks if reached the end, if so returns NULL
	return map->iterator->key; //returns current iterator's key
}

MapResult mapClear(Map map)
{
	VALIDATE_ARGUMENT(map, MAP_NULL_ARGUMENT); //checks if map is NULL
	VALIDATE_ARGUMENT(map->dummy, MAP_NULL_ARGUMENT);
	VALIDATE_ARGUMENT(map->dummy->next, MAP_SUCCESS); //if map is empty returns SUCCESS
	MapEntry ptr = map->dummy->next; //pointer to the first key
    mapEntryDestroyAll(ptr);
	map->dummy->next=NULL;
    return MAP_SUCCESS;
}