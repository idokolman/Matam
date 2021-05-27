#ifndef AREAVOTES_H_
#define AREAVOTES_H_

#include <stdbool.h>
#include <string.h>

/** Type for defining the AreaVotes */
typedef struct AreaVotes_t* AreaVotes;

typedef enum AreaVotesResult_t {
    AREAVOTES_OUT_OF_MEMORY,
    AREAVOTES_NULL_ARGUMENT,
    AREAVOTES_SUCCESS
} AreaVotesResult;

/**
* areaVotesCreate: Allocates a new area votes.
*
* @return
* 	NULL - if allocations failed.
* 	A new AreaVotes in case of success.
*/
AreaVotes areaVotesCreate();

/**
* areaVotesDestroyList: Deallocates a list of areaVotes and their votes map 
*
* @param run_vote - head of areaVotes list to destroy
*/
void areaVotesDestroyList(AreaVotes run_vote);

/**
* areaVotesPut: Adds votes to the tribe in the specific area.
* The function separates between:
* 1. The area receives votes for the first time, sends it to addToNewArea.
* 2. The area received votes before but this is first time for this specific tribe.
* 3. The tribe already received votes in this area before,
* which means we need to update the votes, sends it to addToExistTribe.
*
* @param areavote_dummy -
* The head of the linked list of the areavotes.
* @param area_id -
* The id of the area which voted.
* @param tribe_id -
* The id of the tribe which we need to add/remove votes to in the area.
* @param num_of_votes -
* The number of votes we need to add/remove to tribe in the area.
* @return
* 	AREAVOTES_NULL_ARGUMENT if one of the params is NULL
* 	AREAVOTES_OUT_OF_MEMORY if an allocation failed
* 	AREAVOTES_SUCCESS the votes had been inserted/removed successfully
*/
AreaVotesResult areaVotesPut(AreaVotes area_vote_head, int area_id, int tribe_id, int num_of_votes);

/**
* areaVotesRemoveArea: remove an area vote and it's votes map given it's id
* @param area_vote_head -
* The areaVotes list to remove input_area_id from
* @param input_area_id -
* The area_id to find and destroy
* @return
* AREAVOTES_NULL_ARGUMENT if a NULL was sent to the function
* AREAVOTES_SUCCESS the area had been removed successfully
*/
AreaVotesResult areaVotesRemoveArea(AreaVotes area_vote_head, int input_area_id);

/**
* areaVotesRemoveTribe: remove tribe from votes map of all AreaVotes
* @param area_vote_head -
* The areaVotes list to remove tribe_id from
* @param tribe_id -
* The tribe to find and remove from all map votes
* @return
* AREAVOTES_NULL_ARGUMENT if a NULL was sent to the function
* AREAVOTES_SUCCESS the area had been removed successfully
*/
AreaVotesResult areaVotesRemoveTribe(AreaVotes area_vote_head,const char* tribe_id);

/**
* areaVotesContain: Checks if the area's id exists in the AreaVotes. The area_id element will be
* considered in the areaVotes if one of the area_id elements in the areaVotes is determined equal.
*
* @param area_vote_head -
* The head of the AreaVotes (linked list) to search in
* @param area_id_check -
* The area_id to look for.
* @return
* 	false - if one or more of the inputs is null, or if the area_id was not found.
* 	true - if the area_id element was found in the AreaVotes.
*/
bool areaVotesContain(AreaVotes area_vote_head, int area_id_check); 

/**
* areaVoteComputeWinner: Compute who is the winning tribe in every area.
* in case of a tie, lower tribe_id tribe is the winner.
* @param area_votes -
* The AreaVotes list were all election votes are kept
* @param winner_map -
* A map with all areas to fill in their winning tribe
* @param min_tribe_id -
* The tribe with the minimal tribe id - default winner for area with no votes
* @return
* Map of area to winner tribe in the area - in case of success
* NULL - Otherwise.
*/
Map areaVoteComputeWinner(AreaVotes area_votes, Map winner_map,const char* min_tribe_id);

#endif //AREAVOTES_H_