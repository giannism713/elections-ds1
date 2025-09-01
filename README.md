
## Project Overview

This project is an implementation of an election management system for the CS240 course (Winter Semester 2024-2025). It simulates the organization and operation of parliamentary elections, including the management of districts, parties, candidates, voting stations, voters, and the election process itself.

The system supports the following core features:

- **District Management:** Create and manage electoral districts, each with a specified number of seats.
- **Station Management:** Add and manage voting stations within districts.
- **Party and Candidate Management:** Register parties and candidates, and assign candidates to districts and parties.
- **Voter Registration:** Register and unregister voters at specific stations.
- **Voting Process:** Allow registered voters to cast votes for candidates at their assigned stations.
- **Vote Counting and Seat Allocation:** Count votes per district, allocate seats to parties and candidates, and handle the formation of government and parliament.
- **Reporting:** Print information about districts, stations, parties, and the final parliament composition.

## How It Works

The project is written in C and is organized around a set of data structures representing districts, parties, candidates, stations, and voters. The main logic is implemented in `main.c`, with function and structure declarations in `elections.h`.

The system processes a series of events (commands), each corresponding to a specific operation (e.g., create district, register voter, cast vote, count votes, etc.). These events are handled by dedicated functions, which update the global data structures accordingly.

## How to Build and Run

1. **Build the project:**
	- Use the provided `Makefile` by running:
	  ```bash
	  make
	  ```
	- This will compile the source code and produce an executable.

2. **Run the program:**
	- Execute the compiled program (e.g., `./main`) and provide input commands as required by the assignment specification.

## Notes

- The project is intended for educational purposes and follows the requirements of the CS240 course project.
- For more details on the available commands and their usage, refer to the comments in `elections.h` and `main.c`.