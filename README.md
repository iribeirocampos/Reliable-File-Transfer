# <p align="center">School Project</p>

## School and Course
<img src="https://epg.ulisboa.pt/sites/ulisboa.pt/files/styles/logos_80px_vert/public/uo/logos/logo_ist.jpg?itok=2NCqbcIP" width="100" height="50">

[Instituto Superior Técnico](https://tecnico.ulisboa.pt/)

[Engenharia Informática e de Computadores](https://tecnico.ulisboa.pt/en/education/courses/undergraduate-programmes/computer-science-and-engineering/)

## Class Subject and Goals
### Class: [RC](https://fenix.tecnico.ulisboa.pt/cursos/leic-t/disciplina-curricular/1971853845332790) - Computer Networks
### Goals

1. **Understand reliable data transfer concepts**  
   Learn the fundamentals of reliable data transfer by implementing a simplified **Automatic Repeat Request (ARQ)** mechanism.

2. **Develop a simple file transfer system**  
   Build a file transfer application that operates over **unreliable UDP** messages, ensuring data integrity despite potential packet loss.

3. **Implement a client–server architecture**  
   - **Client:** Reads a file, splits it into chunks, and sends them over the network.  
   - **Server:** Receives these chunks and reconstructs the original file.

4. **Upgrade the provided basic implementation**  
   Modify the starter code—which assumes no packet loss or reordering—to handle **packet loss** and **reordering** using a **sliding window ARQ algorithm**.

5. **Implement send and receive windows**  
   - **Send window:** Tracks unacknowledged chunks on the sender side.  
   - **Receive window:** Tracks received chunks and identifies missing ones on the receiver side.

6. **Incorporate selective acknowledgments and timeouts**  
   Use **Selective Acknowledgments (SACKs)** and **timeout-based retransmissions** to efficiently manage lost or delayed packets.

7. **Support multiple ARQ modes**  
   Implement configurable ARQ behavior through adjustable window sizes:  
   - **Stop-and-Wait:** Send and receive window size = 1  
   - **Go-Back-N:** Send window > 1, receive window = 1  
   - **Selective Repeat:** Both send and receive window sizes > 1

 
### Grade: 18/20 ![Grade](https://img.shields.io/badge/Grade-A-brightgreen)


## Problem Specification

Please read [Problem Specification](project-reliable-xfer.md)


## Setup
The project should be built using GNU Make and the corresponding Makefile should be submitted alongside the code accordingly.
Though how you organize your code and build the project is up to you, both the sender and the receiver should be built with a simple call to `make` without any arguments.
Once `make` is run in this fashion, both the `file-sender` and `file-receiver` binaries should be available within the same directory as the Makefile.
The following Makefile does a basic build as specified above:

~~~ {.make}
TARGETS = file-sender file-receiver

CC = gcc
CFLAGS = -Wall -O0 -g

default: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
~~~



## Run

~~~ {.make}
bash run.sh
~~~


<h2>Credits</h2>

- Author: <a href="https://github.com/iribeirocampos" target="_blank">Iuri Campos</a>

<h2>Copyright</h2>
This project is licensed under the terms of the MIT license and protected by IST Honor Code and Community Code of Conduct. <br>
**Disclaimer:**  
A portion of the source code was provided as starter material by the course instructors.  
