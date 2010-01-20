# Author: Brian Gianforcaro (bjg1955@rit.edu)
# 
# Node struct layout in memory 
# 
# Contains        |  Size (bytes)  | Description
# ------------------------------------------
# pointer to node |	    4	   | Link to next entry in artist list
# pointer to node |	    4	   | Link to next entry in title list
# pointer to char |	    4	   | Pointer to artist name string
# pointer to char |	    4	   | Pointer to title string
# unsigned short  |	    2	   | Time (in seconds)
# unsigned char	  |	    1	   | Track count
# irrelevant      |	    1	   | Other stuff
# 
# Node *mknode( char *title, char *artist, int tracks, int seconds );
# 
# 1) Allocates a new Node using the allocate() function; 
# 2) Fill in the fields of the new node by copying the arguments into the appropriate fields; 
# 3) Clears all other fields to 0; returns the pointer to the new node. If the allocation fails, returns 0. 

.globl mknode

NODESZ = 20      # Size of one node struct
NULL   = 0       # Representation of NULL

# Argument Globals
TITARG = 8
ARTARG = 12
TRKARG = 16
SECARG = 20

# Struct Feilds
next_a = 0          # The artist field of the node
next_t = 4          # The title field of the node 
artist = 8          # The artist field of the node
title  = 12         # The title field of the node 
time   = 16         # The time field of the node
tracks = 18         # The track count field of the node
other  = 19         # The other bit of useless info


mknode:
		enter  $0, $0              # Save stack frame

		pushl  $NODESZ             # Push argument to allocate
		call   allocate
		popl   %ebx
   
	  cmp    $NULL, %eax         # If allocate failed ( == 0 ) exit
	  jz     exit       

    # Construct node from arguments
	  movl   TITARG(%ebp), %ebx
	  movl   %ebx, title(%eax)

	  movl   ARTARG(%ebp), %ebx
	  movl   %ebx, artist(%eax)

	  movb   TRKARG(%ebp), %bl
	  movb   %bl, tracks(%eax)

	  movw   SECARG(%ebp), %bx 
	  movw   %bx, time(%eax)

    # Clear remaining feilds
		movl   $NULL,  next_a(%eax) 
		movl   $NULL,  next_t(%eax)
		movb   $NULL,  other(%eax)

exit:
    leave 							# discared stack frame
    ret  								# return

