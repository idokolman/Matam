#include "mtm_map/map.h"
#include "areaVotes.h"
#include "utilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define INITIAL_ID -1

/**
 * MAP_TO_AREAVOTES_MEMORY_RESULT: Macro to change MAP_OUT_OF_MEMORY to AREAVOTES_OUT_OF_MEMORY. 
 */
#define MAP_TO_AREAVOTES_MEMORY_RESULT(map_result)\
if (map_result==MAP_OUT_OF_MEMORY)\
{\
	return (AREAVOTES_OUT_OF_MEMORY);\
}

/**
* areaVotesDestroy: Deallocates an AreaVote and it's votes map 
*
* @param to_delete - AreaVote to destroy
*/
static void areaVotesDestroy(AreaVotes to_delete);

/**
* areaVotesGetMap: Searches for the votes of a specific area.
*
* @param area_dummy -
* The AreaVotes list were all election votes are kept
* @param area_id_check -
* The id of the area whose map of votes we want to return.
* @return
* Map of votes of the requested area if found.
* NULL - if received NULL or if the area does not exist in the AreaVotes list.
*/
static Map areaVotesGetMap(AreaVotes areavote_dummy_head, int area_id_check);

/**
* addToNewArea: Adds votes to the tribe in the specific area.
* The area never voted before.
*
* @param dummy -
* The head of the linked list of the areavotes.
* @param area_id -
* The id of the area which voted.
* @param str_tribe_id -
* The id of the tribe which we need to add votes to in the area.
* @param str_votes -
* The number of votes we need to add to tribe in the area.
* @return
* 	AREAVOTES_OUT_OF_MEMORY if an allocation failed
* 	AREAVOTES_SUCCESS the votes had been inserted successfully
*/
static AreaVotesResult addToNewArea(AreaVotes dummy, int area_id, char* str_tribe_id, char* str_votes);

/**
* addToExistTribe: Updates the votes to the tribe in the specific area.
* The area already voted before to this tribe.
*
* @param votes -
* The map of the votes for the specific area
* @param input_votes -
* The number of new votes we need to add/remove for the tribe.
* @param tribe_id -
* The id of the tribe which we need to add/remove votes to in the area.
* @return
* 	AREAVOTES_OUT_OF_MEMORY if an allocation failed
* 	AREAVOTES_SUCCESS the votes had been inserted/removed successfully
*/
static AreaVotesResult addToExistTribe(Map votes, int input_votes, const char* tribe_id);

/**
* areaVoteComputeWinnerAux: Compute who is the winning tribe in spesific area.
* in case of a tie, lower tribe_id tribe is the winner.
*
* @param votes -
* Map of votes of the area
* @return
* char* with the winning tribe in the area
* NULL otherwise
*/
static char* areaVoteComputeWinnerAux(Map votes,const char* min_tribe_id);

struct AreaVotes_t{
    int area_id;
    Map votes;
    struct AreaVotes_t* next;
};

AreaVotes areaVotesCreate()
{
    AreaVotes new_area_votes = malloc (sizeof(*new_area_votes));
    VALIDATE_ARGUMENT(new_area_votes,NULL); // if memory allocation failed
    new_area_votes->area_id=INITIAL_ID; //initializing the dummy
    new_area_votes->votes=mapCreate(); 
    if (new_area_votes->votes==NULL)
    {
        free (new_area_votes);
        return NULL;
    }
    new_area_votes->next=NULL; //initializing the dummy
    return new_area_votes;
}

static void areaVotesDestroy(AreaVotes area_to_delete)
{
	if (!area_to_delete)
	{
		return;
	}
    mapDestroy(area_to_delete->votes);
    free(area_to_delete);
}

void areaVotesDestroyList(AreaVotes run_areas)
{
    while (run_areas) //frees until reached the end (==NULL)
	{
		AreaVotes area_to_delete=run_areas;
		run_areas=run_areas->next;
		areaVotesDestroy(area_to_delete);//delete area 
	}
}

AreaVotesResult areaVotesRemoveArea(AreaVotes areavote_dummy_head, int area_id)
{
    VALIDATE_ARGUMENT(areavote_dummy_head,AREAVOTES_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(areavote_dummy_head->next,AREAVOTES_SUCCESS); //no areas means like it was deleted
	AreaVotes current=areavote_dummy_head->next;
	AreaVotes previous=areavote_dummy_head;
	while (current!=NULL && current->area_id!=area_id) //searches for the area, saves previous and checks next
	{
		previous=current;
		current=current->next;
	}
    VALIDATE_ARGUMENT(current, AREAVOTES_SUCCESS); //if area not found, like it was already removed
	previous->next=current->next; //connects previous to next, removes current
	current->next=NULL;
	areaVotesDestroy(current);
	return AREAVOTES_SUCCESS;
}

AreaVotesResult areaVotesRemoveTribe(AreaVotes run_areas, const char* tribe_id)
{
    VALIDATE_ARGUMENT(run_areas,AREAVOTES_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(tribe_id,AREAVOTES_NULL_ARGUMENT);
    while (run_areas)
    { // go over all the areas
        AreaVotes current_area=run_areas;
		run_areas=run_areas->next;
        mapRemove(current_area->votes, tribe_id); // will remove tribe_id from area's votes map
    }
    return AREAVOTES_SUCCESS;
}

static Map areaVotesGetMap(AreaVotes areavote_dummy_head, int area_id_check) 
{
    VALIDATE_ARGUMENT(areavote_dummy_head,NULL);
    AreaVotes current=areavote_dummy_head->next; 
    while (current!=NULL) 
    {
        if (current->area_id==area_id_check)
        {
            return current->votes;
        }
        current=current->next;
    }
    return NULL;
}

bool areaVotesContain(AreaVotes areavote_dummy_head, int area_id_check) 
{
    // If the map returned is different then NULL it means the area exists
    // NULL means false, if the map exists then it's true
    return areaVotesGetMap(areavote_dummy_head, area_id_check);
}

static AreaVotesResult addToNewArea(AreaVotes dummy_head, int area_id, char* str_tribe_id, char* str_votes)
{
    AreaVotes new_area_votes = areaVotesCreate();
    if (new_area_votes==NULL)    {
        return AREAVOTES_OUT_OF_MEMORY;
    }
    new_area_votes->next=dummy_head->next; //connects the AreaVotes to other AreaVotes
    dummy_head->next=new_area_votes; //connects the head to the new AreaVotes
    new_area_votes->area_id=area_id;
    MapResult result_check=mapPut(new_area_votes->votes, str_tribe_id, str_votes); //puts the new votes
    MAP_TO_AREAVOTES_MEMORY_RESULT(result_check);
    return AREAVOTES_SUCCESS;
}

static AreaVotesResult addToNewTribe(Map votes, int num_of_votes, char* str_tribe_id, char* str_votes)
{
    if (num_of_votes<0)
    {   //instead of removing votes from a tribe that didn't receive votes, just returns success
        return AREAVOTES_SUCCESS; 
    }
    MapResult result_check=mapPut(votes, str_tribe_id, str_votes); //adds new votes to the tribe
    MAP_TO_AREAVOTES_MEMORY_RESULT(result_check);
    return AREAVOTES_SUCCESS;   
}

static AreaVotesResult addToExistTribe(Map votes, int input_votes, const char* tribe_id)
{
    char* str_old_votes=mapGet(votes, tribe_id); //gets current num of votes
    VALIDATE_ARGUMENT(str_old_votes, AREAVOTES_OUT_OF_MEMORY);
    int old_votes=strToInt(str_old_votes);
    if (old_votes<0){
        return AREAVOTES_OUT_OF_MEMORY;
    }
    int sum_votes=old_votes+input_votes; //sum the new votes and old votes
    if (sum_votes<0){ //checks if vote sum is lower than 0
        sum_votes=0;
    }
    char* str_sum_votes=intToStr(sum_votes); 
    VALIDATE_ARGUMENT(str_sum_votes, AREAVOTES_OUT_OF_MEMORY);
    MapResult result_check=mapPut(votes, tribe_id, str_sum_votes);//puts the new sum votes, success or memory result
    free(str_sum_votes);
    MAP_TO_AREAVOTES_MEMORY_RESULT(result_check);
    return AREAVOTES_SUCCESS;
}

AreaVotesResult areaVotesPut(AreaVotes areavote_dummy_head, int area_id, int tribe_id, int num_of_votes)
{
    VALIDATE_ARGUMENT(areavote_dummy_head, AREAVOTES_NULL_ARGUMENT);
    char* str_tribe_id=intToStr(tribe_id);
    VALIDATE_ARGUMENT(str_tribe_id, AREAVOTES_OUT_OF_MEMORY);
    char* str_votes=intToStr(num_of_votes);
    if (str_votes==NULL)
    {
        free(str_tribe_id);
        return AREAVOTES_OUT_OF_MEMORY;
    }
    if (!areaVotesContain(areavote_dummy_head, area_id))  //if its a new area with no prior votes to any tribe
    {
        AreaVotesResult result_check=addToNewArea(areavote_dummy_head, area_id, str_tribe_id, str_votes);
        FREE_TWO_STRINGS(str_tribe_id, str_votes);
        return result_check;  //either OUT_OF_MEMORY or SUCCESS
    }
    else 
    {   //since it reached here it is not new area, searches for the correct area's votes:
        Map current_votes=areaVotesGetMap(areavote_dummy_head, area_id); 
        assert(current_votes);
        if (!mapContains(current_votes, str_tribe_id)) 
        {   //if the tribe didn't receive votes before in this area:
            AreaVotesResult result_check=addToNewTribe(current_votes, num_of_votes, str_tribe_id, str_votes);
            FREE_TWO_STRINGS(str_tribe_id, str_votes);
            return result_check;     //either OUT_OF_MEMORY or SUCCESS    
        }
        else 
        {   //the tribe received votes in this area already:
            AreaVotesResult result_check=addToExistTribe(current_votes, num_of_votes, str_tribe_id);
            FREE_TWO_STRINGS(str_tribe_id, str_votes);
            return result_check;     //either OUT_OF_MEMORY or SUCCESS
        }
   }
} 

static char* areaVoteComputeWinnerAux(Map votes,const char* min_tribe_id)
{
    assert(votes); // this function handles areas with non empty vote map
    char* winner_tribe=mapGetFirst(votes);
    int max_votes=strToInt(mapGet(votes,winner_tribe));
    MAP_FOREACH(iterator,votes) // go over all the tribes
    {
        int temp_votes=strToInt(mapGet(votes,iterator)); // num of votes for tribe
        if (temp_votes>max_votes) // if current tribe has more votes
        {
            winner_tribe=iterator; // change current to be max
            max_votes=temp_votes;
        }
        else if (temp_votes==max_votes)  // if current tribe has same votes
        {
           if (strToInt(iterator)<strToInt(winner_tribe)) // smaller tribe_id winns
           {
                winner_tribe=iterator;
                max_votes=temp_votes;  
           }            
        }        
    }
    if (max_votes==0) // remains of removes votes for tribe
    {
        winner_tribe=(char*)min_tribe_id;
    }
    return winner_tribe;
}

Map areaVoteComputeWinner(AreaVotes area_votes, Map winner_map,const char* min_tribe_id)
{
    VALIDATE_ARGUMENT(area_votes,NULL);
    VALIDATE_ARGUMENT(winner_map,NULL);
    VALIDATE_ARGUMENT(min_tribe_id,NULL);
    MAP_FOREACH(iterator,winner_map)
    {
        int current_area_id=strToInt(iterator);
        if(areaVotesContain(area_votes,current_area_id)==false ||
        mapGetSize(areaVotesGetMap(area_votes,current_area_id))==0) //if there are no votes in this area
        {
           if (mapPut(winner_map,iterator,min_tribe_id)!=MAP_SUCCESS){ // put min_tribe as the winner
                return NULL;
            }
        }
        else //there are votes in this area
        {
            char* winner_tribe=areaVoteComputeWinnerAux(areaVotesGetMap(area_votes,current_area_id),min_tribe_id);
            if (mapPut(winner_map,iterator,winner_tribe)!=MAP_SUCCESS){ // put winner tribe
                return NULL;
            }            
        }
    }
    return winner_map;
}