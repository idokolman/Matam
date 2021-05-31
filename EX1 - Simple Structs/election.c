#include "election.h"
#include "areaVotes.h"
#include "utilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define CHAR_BOUNDARY_START 'a'
#define CHAR_BOUNDARY_END 'z'
#define VALID_CHAR ' '

/**
 * VALIDATE_ID: Checks if ID input is negetive
 * If input is negetive, returns the return_value.
 */
#define VALIDATE_ID(input,return_value)\
if (input<0)\
{\
    return (return_value);\
}

/**
 * VALIDATE_VOTES: Checks if votes input is non-positive
 * If input is non-pisitive, returns the return_value.
 */
#define VALIDATE_VOTES(input,return_value)\
if (input<=0)\
{\
    return (return_value);\
}

/**
 * AREAVOTES_TO_ELECTION_RESULT: Changes AreaVotesResult to ElectionResult
 * Returns the ElectionResult according to the areavotes_result received
 */
#define AREAVOTES_TO_ELECTION_RESULT(areavotes_result)\
{\
    if (areavotes_result==AREAVOTES_SUCCESS){\
        return ELECTION_SUCCESS;\
    }\
    if (areavotes_result==AREAVOTES_OUT_OF_MEMORY){\
        return ELECTION_OUT_OF_MEMORY;\
    }\
    if (areavotes_result==AREAVOTES_NULL_ARGUMENT){\
        return ELECTION_NULL_ARGUMENT;\
    }\
}

/**
 * MAP_TO_ELECTION_MEMORY_RESULT: Changes MAP_OUT_OF_MEMORY to ELECTION_OUT_OF_MEMORY. 
 */
#define MAP_TO_ELECTION_MEMORY_RESULT(map_result)\
if (map_result==MAP_OUT_OF_MEMORY)\
{\
	return (ELECTION_OUT_OF_MEMORY);\
}

/**
* validateName: Checks if the name received is valid according to predetermined standards, 
* which is lowercase English letters and spaces only in this exercise.
*
* @param name - string to check
* @return
* 	False - if the string has characters that are not lowercase letters or spaces.
* 	True - if the string only consists of lowercase letters or spaces.
*/
static bool validateName(const char* name);

/**
* electionAddAux: Adds an area or tribe to the election system
*
* @param election -
* The election system to add the area/tribe to
* @param areas_or_tribes
* So the function would add the area/tribe to the correct map
* @param id
* The id of the area/tribe that needs to be added to the election. 
* @param name
* The name of the area/tribe that needs to be added to the election.
* @return
* ELECTION_NULL_ARGUMENT if a NULL was sent to the function
* ELECTION_ID_INVALID if area_id or tribe_id are a negative number
* ELECTION_AREA_ALREADY_EXIST if the area already exists in the election
* ELECTION_TRIBE_ALREADY_EXIST if the tribe already exists in the election
* ELECTION_OUT_OF_MEMORY if an allocation failed
* ELECTION_SUCCESS if encountered no problems and validated everything
*/
static ElectionResult electionAddAux(Election election, Map areas_or_tribes, int id, const char* name);

/**
* electionValidateVotes: Checks if all parameters received for vote functions are eligible.
*
* @param election -
* The election system to add/remove votes to
* @param area_id
* The area_id to add/remove votes to in the election.
* @param tribe_id
* The tribe_id that received votes in the election. 
* @param num_of_votes
* The number of votes received for the corresponding area and tribe.
* @return
* ELECTION_NULL_ARGUMENT if a NULL was sent to the function
* ELECTION_ID_INVALID if area_id or tribe_id are a negative number
* ELECTION_INVALID_VOTES if non-positive number of votes was received
* ELECTION_AREA_NOT_EXIST if the area does not exist in the election
* ELECTION_TRIBE_NOT_EXIST if the tribe does not exist in the election
* ELECTION_OUT_OF_MEMORY if an allocation failed
* ELECTION_SUCCESS if encountered no problems and validated everything
*/
static ElectionResult electionValidateVotes(Election election, int area_id, int tribe_id, int num_of_votes);

/**
* findMinTribe: findes the tribe with the minimal tribe id
*
* @param tribes -
* map of all the tribes
* @return
* char* with the minimal tribe id
* NULL otherwise
*/
static char* findMinTribe(Map tribes);

struct election_t {
	Map areas;
	Map tribes; 
	AreaVotes votes;
};

static bool validateName(const char* name)
{
    VALIDATE_ARGUMENT (name,false); //if name is NULL return false
    int i=0;
    for (i=0; name[i]!='\0'; i++)
    {
        if ((name[i]>=CHAR_BOUNDARY_START && name[i]<=CHAR_BOUNDARY_END) || (name[i]==VALID_CHAR))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

Election electionCreate()
{
    Election new_election=malloc(sizeof(*new_election));
    VALIDATE_ARGUMENT(new_election,NULL);
    new_election->areas=mapCreate();
    if (new_election->areas==NULL)
    {
        mapDestroy(new_election->areas);
        free(new_election);
        return NULL;
    }
    new_election->tribes=mapCreate();
    if (new_election->tribes==NULL)
    {
        mapDestroy(new_election->areas);
        mapDestroy(new_election->tribes);
        free(new_election);
        return NULL;
    }
    new_election->votes=areaVotesCreate();
    if (new_election->votes==NULL)
    {
        mapDestroy(new_election->areas);
        mapDestroy(new_election->tribes);
        areaVotesDestroyList(new_election->votes);
        free(new_election);
        return NULL;
    }
    return new_election;
}

void electionDestroy(Election election)
{
    if (!election)
	{
		return;
	}
    mapDestroy(election->tribes);
    mapDestroy(election->areas);
    areaVotesDestroyList(election->votes);
    free(election);
}

static ElectionResult electionAddAux(Election election, Map areas_or_tribes, int id, const char* name)
{
    //already checked election and map is not NULL
    VALIDATE_ARGUMENT(name, ELECTION_NULL_ARGUMENT);
    VALIDATE_ID(id, ELECTION_INVALID_ID);
    char* str_id=intToStr(id);
    VALIDATE_ARGUMENT(str_id, ELECTION_OUT_OF_MEMORY);
    if (mapContains(areas_or_tribes, str_id))
    {
        free (str_id);
        if (areas_or_tribes==election->tribes){
        return ELECTION_TRIBE_ALREADY_EXIST;
        }
        if (areas_or_tribes==election->areas){
        return ELECTION_AREA_ALREADY_EXIST;
        }
    }
    if (!validateName(name))
    {
        free (str_id);
        return ELECTION_INVALID_NAME;
    }
    MapResult result=mapPut(areas_or_tribes, str_id, name); //already checked if any are NULL
    free (str_id);
    //could only be success or out of memory, therefore only checks for memory problem
    MAP_TO_ELECTION_MEMORY_RESULT(result);
    return ELECTION_SUCCESS;
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name)
{
    VALIDATE_ARGUMENT(election, ELECTION_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(election->tribes, ELECTION_NULL_ARGUMENT);
    return (electionAddAux(election, election->tribes, tribe_id, tribe_name));
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name)
{
    VALIDATE_ARGUMENT(election, ELECTION_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(election->areas, ELECTION_NULL_ARGUMENT);
    return (electionAddAux(election, election->areas, area_id, area_name));
}

char* electionGetTribeName (Election election, int tribe_id)
{
    VALIDATE_ARGUMENT(election, NULL);
    VALIDATE_ARGUMENT(election->tribes, NULL);
    VALIDATE_ID(tribe_id,NULL);
    char* str_tribe_id=intToStr(tribe_id);
    VALIDATE_ARGUMENT(str_tribe_id, NULL);
    char* tribe_name=duplicateData(mapGet(election->tribes,str_tribe_id));
    free (str_tribe_id);
    return tribe_name;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name)
{
    VALIDATE_ARGUMENT(election, ELECTION_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(tribe_name, ELECTION_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(election->tribes, ELECTION_NULL_ARGUMENT);
    VALIDATE_ID(tribe_id, ELECTION_INVALID_ID);
    char* str_tribe_id=intToStr(tribe_id);
    VALIDATE_ARGUMENT(str_tribe_id, ELECTION_OUT_OF_MEMORY);
    if (!mapContains(election->tribes, str_tribe_id))
    {
        free (str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (!validateName(tribe_name))
    {
        free (str_tribe_id);
        return ELECTION_INVALID_NAME;
    }
    MapResult result=mapPut(election->tribes, str_tribe_id, tribe_name);
    free (str_tribe_id);
    MAP_TO_ELECTION_MEMORY_RESULT(result); //map result can't be NULL, only memory error or success
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveTribe (Election election, int tribe_id)
{
    VALIDATE_ARGUMENT(election, ELECTION_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(election->tribes, ELECTION_NULL_ARGUMENT);
    VALIDATE_ID(tribe_id, ELECTION_INVALID_ID);
    char* str_tribe_id=intToStr(tribe_id);
    VALIDATE_ARGUMENT(str_tribe_id, ELECTION_OUT_OF_MEMORY);
    if (mapRemove(election->tribes, str_tribe_id)==MAP_ITEM_DOES_NOT_EXIST) // remove from tribe map
    {
        free (str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (!election->votes)
    {
        free (str_tribe_id);
        return ELECTION_SUCCESS; // if no votes - done
    }
    areaVotesRemoveTribe(election->votes,str_tribe_id); // remove votes for tribe, input can't be null
    free (str_tribe_id);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area)
{
    VALIDATE_ARGUMENT(election, ELECTION_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(should_delete_area, ELECTION_NULL_ARGUMENT);
    char* iterator = mapGetFirst(election->areas); // run over the areas
    while (iterator)
    {
       int area_id_num = strToInt(iterator);
       if (area_id_num==EOF){
           return ELECTION_ERROR;
       }
       char* to_remove=iterator;
       iterator = mapGetNext(election->areas); // move to the next area before currunt might be deleted
       if (should_delete_area(area_id_num)==true)
       {
           mapRemove(election->areas,to_remove); // remove from area map
           areaVotesRemoveArea(election->votes,area_id_num); // remove votes of area, input can't be null 
       }
    }
    return ELECTION_SUCCESS;
}

static ElectionResult electionValidateVotes(Election election, int area_id, int tribe_id, int num_of_votes)
{
    VALIDATE_ARGUMENT(election, ELECTION_NULL_ARGUMENT);
    VALIDATE_ARGUMENT(election->votes, ELECTION_NULL_ARGUMENT);
    VALIDATE_ID(area_id, ELECTION_INVALID_ID);
    VALIDATE_ID(tribe_id, ELECTION_INVALID_ID);
    VALIDATE_VOTES(num_of_votes, ELECTION_INVALID_VOTES);
    char* str_area_id=intToStr(area_id);
    VALIDATE_ARGUMENT(str_area_id, ELECTION_OUT_OF_MEMORY);
    if (!mapContains(election->areas, str_area_id))
    {
        free (str_area_id);
        return ELECTION_AREA_NOT_EXIST;
    }
    char* str_tribe_id=intToStr(tribe_id);
    if (str_tribe_id==NULL)
    {
        free (str_area_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    if (!mapContains(election->tribes, str_tribe_id))
    {
        FREE_TWO_STRINGS(str_area_id, str_tribe_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    FREE_TWO_STRINGS(str_area_id, str_tribe_id);
    return ELECTION_SUCCESS;
}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes)
{
    ElectionResult check_validation=electionValidateVotes(election, area_id, tribe_id, num_of_votes);
    if (check_validation!=ELECTION_SUCCESS){
        return check_validation;
    }
    AreaVotesResult result=areaVotesPut(election->votes, area_id, tribe_id, num_of_votes);
    AREAVOTES_TO_ELECTION_RESULT(result);
    //the function should not get here, all possible results are returned one line above
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes)
{
    ElectionResult check_validation=electionValidateVotes(election, area_id, tribe_id, num_of_votes);
    if (check_validation!=ELECTION_SUCCESS){
        return check_validation;
    }
    if (!areaVotesContain(election->votes, area_id)){
        return ELECTION_SUCCESS; //if the area didn't vote already, no need to remove votes
    }
    AreaVotesResult result=areaVotesPut(election->votes, area_id, tribe_id, -(num_of_votes)); //adds minus votes
    AREAVOTES_TO_ELECTION_RESULT(result);
    //the function should not get here, all possible results are returned one line above
    return ELECTION_SUCCESS;
}

static char* findMinTribe(Map tribes)
{
    assert(tribes);
    char* min_tribe_id=mapGetFirst(tribes);
    MAP_FOREACH(iterator,tribes)
    {
        if (strToInt(iterator)<=strToInt(min_tribe_id))
        {
            min_tribe_id=iterator;            
        }
    }
    return min_tribe_id;
}

Map electionComputeAreasToTribesMapping (Election election)
{
    VALIDATE_ARGUMENT(election,NULL);
    if (mapGetSize(election->tribes)==0 || mapGetSize(election->areas)==0)
    {
       Map winner_map=mapCreate(); // if there are no tribes or areas
       return winner_map; // return empty map or null if alloation haven't succeeded 
    }
    Map winner_map=mapCopy(election->areas); // otherwise, compute area winners
    VALIDATE_ARGUMENT(winner_map,NULL);
    char* min_tribe_id=findMinTribe(election->tribes);
    Map result = areaVoteComputeWinner(election->votes,winner_map,min_tribe_id); 
    if(!result)
    {
        mapDestroy(winner_map);
        return NULL;
    }
    return winner_map;
}