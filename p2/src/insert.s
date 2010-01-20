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
# int insert( Node **root, Node *new, int list );  

# Inserts the Node pointed at by new into a linked list at the appropriate position; 
# returns 1 if the insert succeeds, and 0 if the node is a duplicate 
# (same artist and title as an existing entry). 
.globl insert

# Constants for which list to insert into.

ARTIST_LIST = 0
TITLE_LIST  = 1

SUCCESS     = 1 
FAILURE     = 0 

insert:
	  enter   $0, $0 
		pushal                 # Push all registers to stack to save them.

		je			setfailure     # Duplicate node

setsuccess:                # Set return value for success
		movl    $SUCCESS, %eax 
		jmp     return

setfailure:                # Set return value for failure
 	  movl    $FAILURE, %eax 

return:
    popal 							# restore all registers 
    leave 							# discared stack frame
    ret  								# return

