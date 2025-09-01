/*
 * ============================================
 * file: main.c
 * @Author John Malliotakis (jmal@csd.uoc.gr)
 * @Version 24/10/2023
 *
 * @e-mail hy240@csd.uoc.gr
 *
 * @brief Main function
 *        for CS240 Project Phase 1,
 *        Winter Semester 2024-2025
 * @see   Compile using supplied Makefile by running: make
 * ============================================
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "elections.h"

/* Maximum input line size */
#define MAX_LINE 1024

/*
 * Uncomment the following line to
 * enable debugging prints
 * or comment to disable it
 */
/* #define DEBUG */
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else
#define DPRINT(...)
#endif /* DEBUG */

/*
 * Globals:
 * you may add some here for certain events
 * (such as D and P)
 */
struct district Districts[56];
struct party Parties[5];
struct parliament Parliament;

int count = 0;		 /* Made it global so it can be used by create_stations to check ITS FOR DISTRICTS how many districts are currently in Districts array */
int count_party = 0; /* How many parties are the in my Parties array */

/* These functions return the whole District or Party with a certain did or pid */
struct district *ReturnDistrictOfDid(int did)
{
	for (int i = 0; i < 56; i++)
	{
		if (Districts[i].did == did)
		{
			return &Districts[i];
		}
	}
	return NULL;
}

struct party *ReturnPartyOfPid(int pid)
{
	for (int i = 0; i < count_party; i++)
	{
		if (Parties[i].pid == pid)
		{
			return &Parties[i];
		}
	}
	return NULL;
}

/* These functions return the index in my array that a specific district or a party resides on */
int ReturnCandidateNewDid(int did)
{
	for (int i = 0; i < count; i++)
	{
		if (Districts[i].did == did)
		{
			return i;
		}
	}
	return -1; // Doesn't exist
}

int ReturnCandidateNewPid(int pid)
{
	for (int i = 0; i < count_party; i++)
	{
		if (Parties[i].pid == pid)
		{
			return i;
		}
	}
	return -1; // Doesn't exist
}
// ------------------------------------------------- A ---------------------------------------------
void announce_elections(void)
{
	printf("A\n");

	/* Initializing the Districts array with the values wanted
	(integers get -1)
	(pointers get NULL) */
	for (int i = 0; i < 56; i++)
	{
		Districts[i].did = -1;
		Districts[i].seats = -1;
		Districts[i].allotted = -1;
		Districts[i].blanks = -1;
		Districts[i].voids = -1;
		Districts[i].stations = NULL;
		Districts[i].candidates = NULL;
	}
	/* Initializing the Parties array with the values wanted
	(integers get -1)
	(pointers get NULL) */
	for (int i = 0; i < 5; i++)
	{
		Parties[i].pid = -1;
		Parties[i].nelected = -1;
		Parties[i].elected = NULL;
	}
	/* Initializing the Parliament struct with the values wanted (pointers get NULL) */
	Parliament.members = NULL;
	printf("DONE\n");
}
// ------------------------------------------------- A ---------------------------------------------
// ------------------------------------------------- D ---------------------------------------------
void print_event_create_district(int did, int seats)
{
	printf("D <%d> <%d>\n", did, seats);
	printf(" Districts = ");
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1) // last district
		{
			printf("<%d> ", Districts[i].did);
		}
		else
		{
			printf("<%d>, ", Districts[i].did);
		}
	}
	printf("\nDONE\n");
}
int create_district(int did, int seats)
{
	/* Placing it in the Districts array checking if it fits incrementing the index */
	if (count < 56)
	{
		Districts[count].did = did; // in O(1)
		Districts[count].seats = seats;
		Districts[count].allotted = 0;
		Districts[count].blanks = 0;
		Districts[count].voids = 0;
		Districts[count].stations = NULL;
		Districts[count].candidates = NULL;
		// -- Printing --
		count++;
		print_event_create_district(did, seats);
		// -- Printing --
		return 0; /* success */
	}
	else
	{
		printf("Array is full\n");
		return 1; /* fail (cant fit) */
	}
}
// ------------------------------------------------- D ---------------------------------------------
// ------------------------------------------------- S ---------------------------------------------
int create_station(int sid, int did)
{
	/* Check if the district exists */
	struct district *district_did = ReturnDistrictOfDid(did);
	if (district_did == NULL)
	{
		printf("There is no district with that did of %d\n", did);
		return 1; // failure there is no district with that did
	}
	/* Creating a new station object with sid of the did */
	struct station *new_station = (struct station *)malloc(sizeof(struct station));
	new_station->sid = sid;
	new_station->registered = 0;
	// Setup the sentinel now
	struct voter *new_vsentinel = (struct voter *)malloc(sizeof(struct voter));
	new_vsentinel->vid = -1;
	new_vsentinel->voted = 1; // Assume he has voted
	new_vsentinel->next = NULL;

	new_station->vsentinel = new_vsentinel;
	// Only node of the voters list is the sentinel
	new_station->voters = new_station->vsentinel;

	/* Add it to the rest of the stations in the front O(1) */
	new_station->next = district_did->stations; /* New node points to the current head O(1) time cause it is put on the beginning of the list */
	district_did->stations = new_station;		/* New head is the new station */
	// -- Printing --
	printf("S <%d> <%d>\n", sid, did);
	printf(" Stations = ");
	/* Indexing starts from 0 */
	struct station *temp = district_did->stations;
	while (temp->next != NULL)
	{
		printf("<%d>, ", temp->sid);
		temp = temp->next;
	}
	printf("<%d> ", temp->sid);
	printf("\nDONE\n");
	// -- Printing --
	return 0; /* Success */
}
// ------------------------------------------------- S ---------------------------------------------

// ------------------------------------------------- P ---------------------------------------------
void print_event_create_party(int pid)
{
	printf("P <%d>\n", pid);
	printf(" Parties = ");
	for (int i = 0; i < count_party; i++)
	{
		if (i == count_party - 1) // last party
		{
			printf("<%d> ", Parties[i].pid);
		}
		else
		{
			printf("<%d>, ", Parties[i].pid);
		}
	}
	printf("\nDONE\n");
}
void create_party(int pid)
{
	/* Placing it in the Parties array checking if it fits incrementing the index */
	if (count_party < 5)
	{
		Parties[count_party].pid = pid;
		Parties[count_party].nelected = 0;
		Parties[count_party].elected = NULL;
		count_party++;
		// -- Printing --
		print_event_create_party(pid);
		// -- Printing --
	}
	else
	{
		printf("Array is full\n");
		return; /* fail (cant fit) */
	}
}
// ------------------------------------------------- P ---------------------------------------------
int register_candidate(int cid, int did, int pid)
{
	/* Check if the district exists */
	struct district *district_did = ReturnDistrictOfDid(did);
	if (district_did == NULL)
	{
		printf("There is no district with that did of %d\n", did);
		return 1; // failure there is no district with that did
	}
	/* Check if the party exists */
	struct party *party_pid = ReturnPartyOfPid(pid);
	if (party_pid == NULL)
	{
		printf("There is no party with that pid of %d\n", pid);
		return 1; // failure there is no party with that pid
	}
	if (cid != 0 && cid != 1)
	{
		/* New candidate */
		struct candidate *new_candidate = (struct candidate *)malloc(sizeof(struct candidate));
		new_candidate->votes = 0;
		new_candidate->cid = cid;
		new_candidate->pid = pid;
		new_candidate->elected = 0;
		new_candidate->next = NULL;
		new_candidate->prev = NULL;
		/* Add it to the doubly linked list of Districts doesnt need to sort now because everyone has 0 votes */
		/* Adding a node to a doubly linked list (Districts[i].candidates) */
		if (district_did->candidates == NULL) // case where doubly linked list is empty
		{
			new_candidate->next = NULL;
			new_candidate->prev = NULL;
			district_did->candidates = new_candidate; // new head
		}
		else
		{
			new_candidate->next = district_did->candidates;
			district_did->candidates->prev = new_candidate;
			district_did->candidates = new_candidate; // new head
		}
		// -- Printing --
		printf("C <%d> <%d> <%d>\n", cid, did, pid);
		printf(" Candidates = ");
		struct candidate *temp = district_did->candidates;
		while (temp->next != NULL)
		{
			printf("<%d>, ", temp->cid);
			temp = temp->next;
		}
		printf("<%d>", temp->cid);
		printf("\nDONE\n");
		// -- Printing --
		return 0; // success
	}
	return 1; // failure
}
// ------------------------------------------------- P ---------------------------------------------
// ------------------------------------------------- R ---------------------------------------------
int register_voter(int vid, int did, int sid)
{
	/* Check if the district exists */
	struct district *district_did = ReturnDistrictOfDid(did);
	if (district_did == NULL)
	{
		printf("There is no district with that did of %d\n", did);
		return 1; // failure there is no district with that did
	}
	/* Check if the station exists */
	struct station *station_traversal = district_did->stations;
	while (station_traversal != NULL)
	{
		if (station_traversal->sid == sid)
		{
			/* Adding a node to a singly linked list (station_traversal->voters) */
			if (station_traversal->voters == NULL) // case where singly linked list is empty it will not because it contains vsentinel already but added it to be clear
			{
				struct voter *new_voter = (struct voter *)malloc(sizeof(struct voter));
				new_voter->vid = vid;
				new_voter->voted = 0;
				new_voter->next = station_traversal->vsentinel; // vsentinel is already allocated
				station_traversal->voters = new_voter;	// new head
			}
			else
			{
				struct voter *new_voter = (struct voter *)malloc(sizeof(struct voter));
				new_voter->vid = vid;
				new_voter->voted = 0;
				new_voter->next = station_traversal->voters;
				station_traversal->voters = new_voter; // new head
			}
			/* Increment the registered voters of the station */
			station_traversal->registered = station_traversal->registered + 1;
			// -- Printing --
			printf("R <%d> <%d> <%d>\n", vid, did, sid);
			printf(" Voters = ");
			struct voter *temp = station_traversal->voters;
			while (temp->next != NULL && temp->next->vid != -1) // avoid printing sentinel node
			{
				printf("<%d>, ", temp->vid);
				temp = temp->next;
			}
			if (temp->vid != -1)
			{
				printf("<%d>", temp->vid);
			}
			else
			{
				printf("<NONE>");
			}
			printf("\nDONE\n");
			return 0; // success
			break;
		}
		station_traversal = station_traversal->next;
	}
	if (station_traversal == NULL)
	{
		printf("There is no station with that sid of %d\n", sid);
		return 1; // failure there is no station with that sid
	}
	return 1; // failure CHECK
}
// ------------------------------------------------- R ---------------------------------------------
// ------------------------------------------------- U ---------------------------------------------
int unregister_voter(int vid)
{
	/* We will search the voter with certain vid and remove him the traversal will happen for
	every district for every station and every voter also need to decrement the registered voters of the station */
	for (int i = 0; i < count; i++)
	{
		struct station *station_traversal = Districts[i].stations;
		while (station_traversal != NULL)
		{
			struct voter *voter_traversal_prev = NULL;
			// check to see if its really a valid station (has voters)
			if (station_traversal->voters != NULL)
			{
				struct voter *voter_traversal = station_traversal->voters;
				station_traversal->vsentinel->vid = vid;
				/* Traverse the voters list to find the vid we are looking for using vsentinel */
				while (voter_traversal->vid != station_traversal->vsentinel->vid)
				{
					voter_traversal_prev = voter_traversal;
					voter_traversal = voter_traversal->next;
				}
				/* SOS NEED TO CHANGE BACK THE SENTINEL VID TO -1 SO IT IS NOT MISTAKEN FOR AN ACTUAL NODE */
				station_traversal->vsentinel->vid = -1;
				/* If this is true we found the vid in a voter node not a sentinel so we found the voter */
				if (voter_traversal != station_traversal->vsentinel)
				{
					if (voter_traversal_prev == NULL)
					{													   // different handling for if its the first node
						station_traversal->voters = voter_traversal->next; // new head
					}
					else
					{
						voter_traversal_prev->next = voter_traversal->next; // prev->next points to current->next
					}
					free(voter_traversal);	// free the node
					station_traversal->registered = station_traversal->registered - 1; // we dont fear its going to be negative sentinel protects it
					// -- Printing --
					printf("U <%d>\n", vid);
					printf(" <%d> <%d>\n", Districts[i].did, station_traversal->sid);
					printf(" Voters = ");
					// station_traversal because it contains the station where we found the vid we were looking for
					struct voter *temp_voter = station_traversal->voters;
					while (temp_voter->next != NULL && temp_voter->next->vid != -1)
					{ // avoid printing sentinel node
						printf("<%d>, ", temp_voter->vid);
						temp_voter = temp_voter->next;
					}
					if (temp_voter->vid != -1)
					{
						printf("<%d>", temp_voter->vid);
					}
					else
					{
						printf("<NONE>");
					}
					printf("\nDONE\n"); // only correct case
					// -- Printing --
					return 0; // success
				}
			}
			station_traversal = station_traversal->next;
		}
	}
	/* We never found the vid we were looking for */
	return 1; // failed
}
// ------------------------------------------------- U ---------------------------------------------
// ------------------------------------------------- E ---------------------------------------------
void delete_empty_stations()
{
	/* Iterate through every district and every station and remove the empty ones (that dont have any voters registered) */
	printf("E\n");
	for (int i = 0; i < count; i++)
	{
		struct station *station_traversal = Districts[i].stations;
		struct station *station_traversal_prev = NULL;
		while (station_traversal != NULL)
		{
			if (station_traversal->registered == 0)
			{
				printf(" <%d> <%d>\n", station_traversal->sid, Districts[i].did);
				/* First of all free the sentinel of the station */
				free(station_traversal->vsentinel);
				/* Removing a node from a signly linked list first checking if it is the first node and then vanilla removal */
				if (station_traversal_prev == NULL)
				{													 // different handling for if its the first node station
					Districts[i].stations = station_traversal->next; // new head
					free(station_traversal);
					station_traversal = Districts[i].stations; // update the traversal with the new head which is the next node
				}
				else
				{
					station_traversal_prev->next = station_traversal->next;
					free(station_traversal);
					station_traversal = station_traversal_prev->next; // update the traversal again with the next node (the next station) traversal doesnt start from the beginnning again stays O(n)
				}
			}
			else
			{
				station_traversal_prev = station_traversal;
				station_traversal = station_traversal->next;
			}
		}
	}
	printf("DONE\n"); // only correct case
}
// ------------------------------------------------- E ---------------------------------------------
// ------------------------------------------------- V ---------------------------------------------
void AddCandidateVoteSorted(struct candidate *candidate_traversal)
{
	/* Shifting the voters to sort them in descending order */
	while (candidate_traversal->prev != NULL && candidate_traversal->votes > candidate_traversal->prev->votes)
	{
		/* Current traversal data added to temps */
		int temp_cid = candidate_traversal->cid;
		int temp_pid = candidate_traversal->pid;
		int temp_votes = candidate_traversal->votes;
		int temp_elected = candidate_traversal->elected;

		/* Update current node data with previous node data */
		candidate_traversal->cid = candidate_traversal->prev->cid;
		candidate_traversal->pid = candidate_traversal->prev->pid;
		candidate_traversal->votes = candidate_traversal->prev->votes;
		candidate_traversal->elected = candidate_traversal->prev->elected;

		/* Previous node contains the data of the node that was next */
		candidate_traversal->prev->cid = temp_cid;
		candidate_traversal->prev->pid = temp_pid;
		candidate_traversal->prev->votes = temp_votes;
		candidate_traversal->prev->elected = temp_elected;

		candidate_traversal = candidate_traversal->prev;
	}
}
int vote(int vid, int sid, int cid)
{
	/* Traverse all the districts till we find the station we want with sid and find the vid in the station and then the cid  */
	printf("V <%d> <%d> <%d>\n", vid, sid, cid);
	for (int i = 0; i < count; i++)
	{
		struct station *station_traversal = Districts[i].stations;
		while (station_traversal != NULL)
		{
			if (station_traversal->sid == sid)
			{
				// Now lets find the voter with vid
				struct voter *voter_traversal = station_traversal->voters;
				// use the vsentinel to find the voter
				station_traversal->vsentinel->vid = vid;

				while (voter_traversal->vid != station_traversal->vsentinel->vid)
				{
					voter_traversal = voter_traversal->next;
				}
				// SOS NEED TO CHANGE BACK THE SENTINEL VID TO -1 SO IT IS NOT MISTAKEN FOR AN ACTUAL NODE
				station_traversal->vsentinel->vid = -1;
				if (voter_traversal != station_traversal->vsentinel)
				{
					if (!voter_traversal->voted) // check has not voted again
					{
						voter_traversal->voted = 1; /* Update that his has votes so he cant vote again */
						if (cid == 0)
						{
							Districts[i].blanks = Districts[i].blanks + 1; // increment blanks
						}
						else if (cid == 1)
						{
							Districts[i].voids = Districts[i].voids + 1; // increment voids
						}
						else
						{
							// vote is valid now spot the candidate to put him his votes
							struct candidate *candidate_traversal = Districts[i].candidates;
							while (candidate_traversal != NULL)
							{
								if (candidate_traversal->cid == cid)
								{
									// add to sorted
									// make sure everything stays sorted
									// check if the previous candidate has less votes than the current one
									candidate_traversal->votes = candidate_traversal->votes + 1;
									AddCandidateVoteSorted(candidate_traversal);
								}
								candidate_traversal = candidate_traversal->next;
							}
						}
						// -- Printing --
						printf(" District = <%d>\n", i + 1);
						printf(" Candidate votes = ");
						struct candidate *new_candidate_traversal = Districts[i].candidates;
						while (new_candidate_traversal->next != NULL)
						{
							printf("(<%d>, <%d>), ", new_candidate_traversal->cid, new_candidate_traversal->votes);
							new_candidate_traversal = new_candidate_traversal->next;
						}
						printf("(<%d>, <%d>)\n", new_candidate_traversal->cid, new_candidate_traversal->votes);
						printf(" Blanks = <%d>\n", Districts[i].blanks);
						printf(" Voids = <%d>\n", Districts[i].voids);
						printf("DONE\n");
						// -- Printing --
						return 0; // success
					}
					else
					{
						printf("Voter with vid %d has already voted\n", vid);
						return 1; // failure
					}
				}
			}

			station_traversal = station_traversal->next;
		}
	}
	return 1; // failure
}
// ------------------------------------------------- V ---------------------------------------------
// ------------------------------------------------- M ---------------------------------------------
void AddCandidateToElectedSorted(struct candidate **elected, struct candidate *new_candidate)
{
	struct candidate *new_node = malloc(sizeof(struct candidate));
	*new_node = *new_candidate; 
	new_node->next = NULL;		

	struct candidate *current = *elected;
	struct candidate *prev = NULL;

	/* While the new node has less votes than the current one move on */
	while (current != NULL && current->votes > new_node->votes)
	{
		prev = current;
		current = current->next;
	}

	new_node->next = current;
	if (prev == NULL)
	{
		*elected = new_node;
	}
	else
	{
		prev->next = new_node;
	}
}

int party_seats[5] = {0};
int total_party_votes_each_did[56][5] = {0};

void count_votes(int did)
{
	struct district *district_did = ReturnDistrictOfDid(did);
	int party_votes_for_did[5] = {0};

	printf("M <%d>\n", did);
	printf(" Seats = \n");
	int total_valid_votes_for_did = 0;

	if (district_did == NULL)
	{
		printf("There is no district with the did of %d\n", did);
		return;
	}
	int index_did = ReturnCandidateNewDid(did); // this did flactuates from 1-56 and is used for indexing inside my total_party_votes_each_did array because still have the original arrays with (-1)
	if (index_did == -1)
	{
		printf("There is no district with the did of %d\n", did);
		return;
	}
	struct candidate *candidate_traversal = district_did->candidates;
	while (candidate_traversal != NULL)
	{
		int index_pid = ReturnCandidateNewPid(candidate_traversal->pid); // this pid flactuates from 0-4 and is used for indexing inside my party_votes_for_did array because still have the original arrays
		if (index_pid == -1)
		{
			printf("There is no party with the pid of %d\n", candidate_traversal->pid);
			return;
		}
		total_valid_votes_for_did = total_valid_votes_for_did + candidate_traversal->votes; // Increment the total valid votes for this specific district
		party_votes_for_did[index_pid] = party_votes_for_did[index_pid] + candidate_traversal->votes; // Increment the party votes for this specific district
		total_party_votes_each_did[index_did - 1][index_pid] = total_party_votes_each_did[index_did - 1][index_pid] + candidate_traversal->votes; // also calculate the total votes as M events happen [did][pid]
		candidate_traversal = candidate_traversal->next;
	}

	// different for each district
	if (district_did->seats == 0)
	{
		printf("DONE\n"); // nothing to add
		return;
	}
	double eklogiko_metro = total_valid_votes_for_did / (double)district_did->seats;
	if (eklogiko_metro == 0)
	{
		printf("DONE\n"); // nothing to add
		return;
	}

	/* These are the party seats for each party */
	for (int i = 0; i < count_party; i++)
	{
		party_seats[i] = party_seats[i] + (int)party_votes_for_did[i] / (double)eklogiko_metro;
	}

	candidate_traversal = district_did->candidates; // check restart the traversal
	while (candidate_traversal != NULL)
	{
		int index_pid = ReturnCandidateNewPid(candidate_traversal->pid); // this pid flactuates from 0-4 and is used for indexing inside my party_votes_for_did array because candidates still have the original arrays
		if (index_pid == -1)
		{
			printf("There is no party with the pid of %d\n", candidate_traversal->pid);
			return;
		}
		// check if the candidate is not elected and if the party has any seats left
		if (party_seats[index_pid] > 0 && district_did->allotted < district_did->seats && candidate_traversal->votes > 0)
		{
			// its sorted so first has the most votes
			// ALL THE PEOPLE HERE ARE GOING TO BE ELECTED
			candidate_traversal->elected = 1;
			Parties[index_pid].nelected = Parties[index_pid].nelected + 1;
			district_did->allotted = district_did->allotted + 1;
			// -- Printing --
			printf("  <%d> <%d> <%d>\n", candidate_traversal->cid, candidate_traversal->pid, candidate_traversal->votes);
			// -- Printing --
			// add to the list of parties[candidate_traversal->pid].elected
			AddCandidateToElectedSorted(&Parties[index_pid].elected, candidate_traversal);
			// renew data
			party_seats[index_pid] = party_seats[index_pid] - 1; // decrement how many of the seats of the party are left
			candidate_traversal = candidate_traversal->next;
		}
		else
		{
			candidate_traversal = candidate_traversal->next;
		}
	}
	printf("DONE\n");
}
// ------------------------------------------------- M ---------------------------------------------
// ------------------------------------------------- G ---------------------------------------------
int party_seats_left[5] = {0};
int FindPartyWithMostVotesInCoutry()
{
	// first find the party with the most votes in the country
	int max_votes = 0;
	int max_votes_pid = 0;
	for (int j = 0; j < count_party; j++)
	{
		int district_votes_for_party = 0;
		for (int i = 0; i < count; i++)
		{
			district_votes_for_party = district_votes_for_party + total_party_votes_each_did[i][j];
		}
		if (district_votes_for_party > max_votes)
		{
			max_votes = district_votes_for_party;
			max_votes_pid = j; // good
		}
	}
	return max_votes_pid;
}
// only adds to the party with the max votes
void AddPartySeatsLeft(int pid)
{
	// start iterating through every district
	pid = ReturnCandidateNewPid(pid); // this pid flactuates from 0-4 and is used for indexing inside my party_votes_for_did array because candidates still have the original arrays
	for (int i = 0; i < count; i++)
	{
		// check if the district has any seats left
		int seats_left = Districts[i].seats - Districts[i].allotted;
		party_seats_left[pid] = party_seats_left[pid] + seats_left; // add the seats left of every district in the party_seats_left arrays
	}
}
void form_government() // check not elected
{
	printf("G\n");
	printf(" Seats = \n");
	int max_votes_pid = FindPartyWithMostVotesInCoutry();
	// Add the seats left of every district inseide the party with most votes
	AddPartySeatsLeft(max_votes_pid);

	// need to both check if the party has any seats left and if the total party seats are enough
	for (int i = 0; i < count; i++)
	{
		struct candidate *candidate_traversal = Districts[i].candidates;
		while (candidate_traversal != NULL)
		{
			int index_pid = ReturnCandidateNewPid(candidate_traversal->pid); // this pid flactuates from 0-4 and is used for indexing inside my party_votes_for_did array because candidates still have the original arrays
			if (index_pid == -1)
			{
				printf("There is no party with the pid of %d\n", candidate_traversal->pid);
				return;
			}
			// give the seats to the party with the most votes and check not elected EDW DEN MPAINEI POTE SSTO SMALL
			if (candidate_traversal->elected == 0 && candidate_traversal->pid == Parties[ReturnCandidateNewPid(max_votes_pid)].pid && Districts[i].allotted < Districts[i].seats)
			{
				// its sorted so first has the most votes
				// ALL THE PEOPLE HERE ARE GOING TO BE ELECTED
				candidate_traversal->elected = 1;
				Parties[ReturnCandidateNewPid(max_votes_pid)].nelected = Parties[ReturnCandidateNewPid(max_votes_pid)].nelected + 1; // Update the number of elected in the party array of the specific party
				Districts[i].allotted = Districts[i].allotted + 1;
				// -- Printing --
				printf("  <%d> <%d> <%d>\n", Districts[i].did, candidate_traversal->cid, candidate_traversal->votes);
				// -- Printing --
				// add to the list of parties[candidate_traversal->pid].elected
				AddCandidateToElectedSorted(&Parties[ReturnCandidateNewPid(max_votes_pid)].elected, candidate_traversal);
				// renew data
				party_seats_left[ReturnCandidateNewPid(max_votes_pid)] = party_seats_left[ReturnCandidateNewPid(max_votes_pid)] - 1; // decrement how many of the seats of the party are left
				party_seats[ReturnCandidateNewPid(max_votes_pid)] = party_seats[ReturnCandidateNewPid(max_votes_pid)] - 1;
			}
			candidate_traversal = candidate_traversal->next;
		}
		// check if the district has more seats left
		if (Districts[i].allotted < Districts[i].seats)
		{
			struct candidate *candidate_traversal = Districts[i].candidates;
			while (candidate_traversal != NULL && Districts[i].allotted < Districts[i].seats)
			{
				int index_pid = ReturnCandidateNewPid(candidate_traversal->pid); // this pid flactuates from 0-4 and is used for indexing inside my party_votes_for_did array because candidates still have the original arrays
				if (index_pid == -1)
				{
					printf("There is no party with the pid of %d\n", candidate_traversal->pid);
					return;
				}
				if (candidate_traversal->elected == 0 && candidate_traversal->pid != Parties[ReturnCandidateNewPid(max_votes_pid)].pid)
				{
					// its sorted so first has the most votes
					// ALL THE PEOPLE HERE ARE GOING TO BE ELECTED
					candidate_traversal->elected = 1;
					Parties[index_pid].nelected = Parties[index_pid].nelected + 1;
					Districts[i].allotted = Districts[i].allotted + 1;
					// -- Printing --
					printf("  <%d> <%d> <%d>\n", Districts[i].did, candidate_traversal->cid, candidate_traversal->votes);
					// -- Printing --
					// add to the list of parties[candidate_traversal->pid].elected
					AddCandidateToElectedSorted(&Parties[index_pid].elected, candidate_traversal);
					party_seats[index_pid] = party_seats[index_pid] - 1; // decrement how many of the seats of the party are left
				}
				candidate_traversal = candidate_traversal->next;
			}
			if (Districts[i].allotted < Districts[i].seats) {
				printf("There are still seats left in district %d\n", Districts[i].did); // Corner case mentioned on webmail
			}
		}
	}
	printf("DONE\n");
}
// ------------------------------------------------- G ---------------------------------------------
// ------------------------------------------------- N ---------------------------------------------
void form_parliament()
{
	printf("N\n");
	printf(" Members = \n");
	int count_parliament = 0;

	struct candidate *parliament_start = NULL;
	struct candidate *parliament_end = NULL;
	struct candidate *elected_all[5] = {Parties[0].elected, Parties[1].elected, Parties[2].elected, Parties[3].elected, Parties[4].elected}; // this array holds all the heads of all the elected party lists
	while (count_parliament < 300)
	{
		struct candidate *elected_most_votes = NULL;
		int index_to_push = -1;

		// IF I PLACE THEM UPSIDE DOWN THEN ALSO IT WILL BE UPSIDE DOWN
		if (elected_all[4] != NULL) /* 4,3,2,1 */
		{
			elected_most_votes = elected_all[4];
			index_to_push = 4;
		}
		if (elected_all[3] != NULL && (elected_most_votes == NULL || elected_all[3]->votes > elected_most_votes->votes)) // if the party3 now has more votes set it as max votes value for now
		{
			elected_most_votes = elected_all[3];
			index_to_push = 3;
		}
		if (elected_all[2] != NULL && (elected_most_votes == NULL || elected_all[2]->votes > elected_most_votes->votes))
		{
			elected_most_votes = elected_all[2];
			index_to_push = 2;
		}
		if (elected_all[1] != NULL && (elected_most_votes == NULL || elected_all[1]->votes > elected_most_votes->votes))
		{
			elected_most_votes = elected_all[1];
			index_to_push = 1;
		}
		if (elected_all[0] != NULL && (elected_most_votes == NULL || elected_all[0]->votes > elected_most_votes->votes))
		{
			elected_most_votes = elected_all[0];
			index_to_push = 0;
		}

		if (elected_most_votes == NULL)
		{
			break; // noone got more votes either all lists are empty
		}

		// add to the parliament
		struct candidate *new_candidate = (struct candidate *)malloc(sizeof(struct candidate));
		new_candidate->cid = elected_most_votes->cid;
		new_candidate->pid = elected_most_votes->pid;
		new_candidate->votes = elected_most_votes->votes;
		new_candidate->elected = 1;
		new_candidate->next = NULL;
		new_candidate->prev = NULL;

		/* Adding a node the a linked list but by having the assist of a pointer */
		if (parliament_start == NULL)
		{
			parliament_start = new_candidate;
			parliament_end = new_candidate;
		}
		else
		{
			parliament_end->next = new_candidate;
			parliament_end = new_candidate;
		}
		printf("  <%d> <%d> <%d>\n", new_candidate->cid, new_candidate->pid, new_candidate->votes);

		// push the pointer to the next elected candidate of the index_to_push party
		elected_all[index_to_push] = elected_all[index_to_push]->next;
		count_parliament++;
	}
	printf("DONE\n");
	Parliament.members = parliament_start; /* Make the parliament list point to the new list */
}
// ------------------------------------------------- N ---------------------------------------------
// ------------------------------------------------- I ---------------------------------------------
void print_district(int did)
{
	printf("I <%d>\n", did);
	struct district *district_did = ReturnDistrictOfDid(did);
	if (district_did == NULL)
	{
		printf("There is no district with the did of %d\n", did);
		return;
	}
	printf(" Seats = <%d>\n", district_did->seats);
	printf(" Blanks = <%d>\n", district_did->blanks);
	printf(" Voids = <%d>\n", district_did->voids);
	printf(" Candidates = \n");
	struct candidate *candidate_traversal = district_did->candidates;
	while (candidate_traversal != NULL)
	{
		printf("  <%d> <%d> <%d>\n", candidate_traversal->cid, candidate_traversal->pid, candidate_traversal->votes);
		candidate_traversal = candidate_traversal->next;
	}
	printf(" Stations = ");
	struct station *station_traversal = district_did->stations;
	while (station_traversal != NULL)
	{
		// last iteration no comma needed
		if (station_traversal->next == NULL)
		{
			printf("<%d>\n", station_traversal->sid);
		}
		else
		{
			printf("<%d>, ", station_traversal->sid);
		}
		station_traversal = station_traversal->next;
	}
	printf("DONE\n");
}
// ------------------------------------------------- I ---------------------------------------------
// ------------------------------------------------- J ---------------------------------------------
void print_station(int sid, int did)
{
	printf("J <%d>\n", sid);
	struct district *district_did = ReturnDistrictOfDid(did);

	if (district_did == NULL)
	{
		printf("There is no district with the did of %d\n", did);
		return;
	}
	struct station *station_traversal = district_did->stations;
	while (station_traversal != NULL && station_traversal->sid != sid)
	{
		station_traversal = station_traversal->next;
	}
	/* Take advantage of short circuit evaluation wont move to the next station */
	if (station_traversal != NULL && station_traversal->sid == sid)
	{
		printf(" Registered = <%d>\n", station_traversal->registered);
		printf(" Voters = \n");
		struct voter *voter_traversal = station_traversal->voters;
		station_traversal->vsentinel->vid = -1; // add it there so its contents are not printed
		while (voter_traversal != NULL)
		{
			if (voter_traversal->vid == -1)
			{
				voter_traversal = voter_traversal->next;
				break;
			}
			printf("  <%d> <%d>\n", voter_traversal->vid, voter_traversal->voted);
			voter_traversal = voter_traversal->next;
		}
	}
	else
	{
		printf("There is no station with that sid of %d\n", sid);
	}
	printf("DONE\n");
}
// ------------------------------------------------- J ---------------------------------------------
// ------------------------------------------------- K ---------------------------------------------
void print_party(int pid)
{
	// static int party_elected_counter = 1;
	printf("K <%d>\n", pid);
	printf(" Elected = \n");
	struct candidate *elected_traversal = ReturnPartyOfPid(pid)->elected;
	while (elected_traversal != NULL)
	{
		// printf("%d.", party_elected_counter++);
		printf("  <%d> <%d>\n", elected_traversal->cid, elected_traversal->votes);
		elected_traversal = elected_traversal->next;
	}
	printf("DONE\n");
}
// ------------------------------------------------- K ---------------------------------------------
// ------------------------------------------------- L ---------------------------------------------
void print_parliament()
{
	printf("L\n");
	printf(" Members = \n");
	struct candidate *parliament_traversal = Parliament.members;
	// int p_counter = 1; // check
	while (parliament_traversal != NULL)
	{
		// printf("%d.", p_counter++);
		printf("  <%d> <%d> <%d>\n", parliament_traversal->cid, parliament_traversal->pid, parliament_traversal->votes);
		parliament_traversal = parliament_traversal->next;
	}
	printf("DONE\n");
}
// ------------------------------------------------- L ---------------------------------------------
void destroy_structures(void)
{
	for (int i = 0; i < count; i++)
	{
		// first free the stations
		struct station *station_traversal = Districts[i].stations;
		while (station_traversal != NULL)
		{
			struct station *temp = station_traversal; // temp has current station
			// inside here free the voters for this station
			struct voter *voters_traversal = station_traversal->voters;
			// free(temp->vsentinel); // free the sentinel
			while (voters_traversal != NULL)
			{
				struct voter *temp_v = voters_traversal;
				voters_traversal = temp_v->next;
				free(temp_v);
				temp_v = NULL;
			}
			station_traversal = temp->next; // keep the next pointer alive
			free(temp);
			temp = NULL; // good practice
						 // Also free the sentinel CHECK
		}
		// now free the candidates
		struct candidate *candidate_traversal = Districts[i].candidates;
		while (candidate_traversal != NULL)
		{
			struct candidate *temp_c = candidate_traversal;
			candidate_traversal = temp_c->next;
			free(temp_c);
			temp_c = NULL;
		}
	}
	// do the same for parties
	for (int j = 0; j < count_party; j++)
	{
		struct candidate *elected_traversal = Parties[j].elected;
		while (elected_traversal != NULL)
		{
			struct candidate *temp_e = elected_traversal;
			elected_traversal = temp_e->next;
			free(temp_e);
			temp_e = NULL;
		}
	}
	// free the parliament list
	struct candidate *parliament_traversal = Parliament.members;
	while (parliament_traversal != NULL)
	{
		struct candidate *temp_p = parliament_traversal;
		parliament_traversal = temp_p->next;
		free(temp_p);
		temp_p = NULL;
	}
}
int main(int argc, char *argv[])
{
	FILE *event_file;
	char line_buffer[MAX_LINE];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input_file>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	event_file = fopen(argv[1], "r");
	if (!event_file)
	{
		perror("fopen error for event file open");
		exit(EXIT_FAILURE);
	}

	while (fgets(line_buffer, MAX_LINE, event_file))
	{
		char *trimmed_line;
		char event;
		int did, sid, vid, cid, pid, seats;
		int ret = 0;
		/*
		 * First trim any whitespace
		 * leading the line.
		 */
		trimmed_line = line_buffer;
		while (trimmed_line && isspace(*trimmed_line))
			trimmed_line++;
		if (!trimmed_line)
			continue;
		/*
		 * Find the event,
		 * or comment starting with #
		 */
		if (sscanf(trimmed_line, "%c", &event) != 1)
		{
			fprintf(stderr, "Could not parse event type out of input line:\n\t%s",
					trimmed_line);
			fclose(event_file);
			exit(EXIT_FAILURE);
		}

		switch (event)
		{
		/* Comment, ignore this line */
		case '#':
			break;
		case 'A':
			announce_elections();
			break;
		case 'D':
			if (sscanf(trimmed_line, "D %d %d", &did, &seats) != 2)
			{
				fprintf(stderr, "Event D parsing error\n");
				ret = 1;
				break;
			}
			ret = create_district(did, seats);
			break;
		case 'S':
			if (sscanf(trimmed_line, "S %d %d", &sid, &did) != 2)
			{
				fprintf(stderr, "Event S parsing error\n");
				ret = 1;
				break;
			}
			ret = create_station(sid, did);
			break;
		case 'P':
			if (sscanf(trimmed_line, "P %d", &pid) != 1)
			{
				fprintf(stderr, "Event P parsing error\n");
				ret = 1;
				break;
			}
			create_party(pid);
			break;
		case 'C':
			if (sscanf(trimmed_line, "C %d %d %d", &cid, &did, &pid) != 3)
			{
				fprintf(stderr, "Event C parsing error\n");
				ret = 1;
				break;
			}
			ret = register_candidate(cid, did, pid);
			break;
		case 'R':
			if (sscanf(trimmed_line, "R %d %d %d", &vid, &did, &sid) != 3)
			{
				fprintf(stderr, "Event R parsing error\n");
				ret = 1;
				break;
			}
			ret = register_voter(vid, did, sid);
			break;
		case 'U':
			if (sscanf(trimmed_line, "U %d", &vid) != 1)
			{
				fprintf(stderr, "Event U parsing error\n");
				ret = 1;
				break;
			}
			ret = unregister_voter(vid);
			break;
		case 'E':
			delete_empty_stations();
			break;
		case 'V':
			if (sscanf(trimmed_line, "V %d %d %d", &vid, &sid, &cid) != 3)
			{
				fprintf(stderr, "Event V parsing error\n");
				ret = 1;
				break;
			}
			ret = vote(vid, sid, cid);
			break;
		case 'M':
			if (sscanf(trimmed_line, "M %d", &did) != 1)
			{
				fprintf(stderr, "Event M parsing error\n");
				ret = 1;
				break;
			}
			count_votes(did);
			break;
		case 'G':
			form_government();
			break;
		case 'N':
			form_parliament();
			break;
		case 'I':
			if (sscanf(trimmed_line, "I %d", &did) != 1)
			{
				fprintf(stderr, "Event I parsing error\n");
				ret = 1;
				break;
			}
			print_district(did);
			break;
		case 'J':
			if (sscanf(trimmed_line, "J %d %d", &sid, &did) != 2)
			{
				fprintf(stderr, "Event J parsing error\n");
				ret = 1;
				break;
			}
			print_station(sid, did);
			break;
		case 'K':
			if (sscanf(trimmed_line, "K %d", &pid) != 1)
			{
				fprintf(stderr, "Event I parsing error\n");
				ret = 1;
				break;
			}
			print_party(pid);
			break;
		case 'L':
			print_parliament();
			break;
		default:
			fprintf(stderr, "WARNING: Unrecognized event %c. Continuing...\n",
					event);
			ret = 1;
			break;
		}
		fprintf(stderr, "Event %c %s.\n", event,
				ret ? "failed" : "succeeded");
	}
	fclose(event_file);
	destroy_structures();
	return 0;
}
