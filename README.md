# Monte Carlo Polygon Area Estimation


This program is designed to estimate the area of a polygon using the Monte Carlo method. It employs multiple child processes to generate random points and determine whether they fall within the given polygon.

## Requirements


A. Command Line Arguments: The program accepts command line arguments for the input polygon file, the number of child processes, and the total number of random points to consider.
Example usage: ./monteCarlo poligon.txt 3 900

B. Child Processes: Multiple child processes are created, each responsible for finding points falling within the polygon. They terminate by writing the number of points within the polygon to a shared text file.

C. Inter-Process Communication (IPC): The child processes can alternatively communicate with the parent process through pipes. The parent process estimates the polygon area based on the received data and prints the result to the terminal. Two modes of writing to the pipe are supported: normal and verbose.

D. Progress Indicator: The parent process periodically prints and updates a progress indicator, showing the percentage of points verified.
E. Unix Domain Sockets: Modification of requirement C to support inter-process communication using Unix Domain Sockets. The parent process acts as a server, waiting for connections from child processes (clients), and prints the estimated area of the polygon to the terminal.

## Usage

### Compile the program:

gcc monteCarlo.c -o monteCarlo -lpthread

### Run the program with command line arguments:

./monteCarlo poligon.txt 3 900
Follow the on-screen instructions and observe the progress indicator and final result.

## Files

monteCarlo.c: Main source code file containing the implementation of the Monte Carlo algorithm and process management.
poligon.txt: Example input file containing the polygon coordinates.


## Implementation Details

The program utilizes multiple processes to efficiently verify random points within the given polygon.
Inter-process communication is implemented using pipes or Unix Domain Sockets, depending on the specified requirement.
Progress indication and area estimation are handled by the parent process, ensuring a smooth user experience.

## Notes

Ensure that the input polygon file follows the specified format.
Depending on the system configuration, additional libraries or permissions may be required for certain functionalities.
