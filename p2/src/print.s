# Author: Brian Gianforcaro (bjg1955@rit.edu)
# 
# Node struct layout in memory 
# 
# Contains        |  Size (bytes)  | Description
# ------------------------------------------
# pointer to node |			4		 | Link to next entry in artist list
# pointer to node |			4		 | Link to next entry in title list
# pointer to char |			4		 | Pointer to artist name string
# pointer to char |			4		 | Pointer to title string
# unsigned short  |			2		 | Time (in seconds)
# unsigned char   |			1		 | Track count
# irrelevant      |			1		 | Other stuff
# 
# void print( Node **root, int list, char *target );
# Prints the database, using the required format. 


A_ARTIST     = 0
A_TITLE      = 1
ALL_TITLES   = 4
ALL_ARTISTS  = 5

ROOT_ARG     = 8
LIST_ARG     = 12
TARG_ARG     = 16

next_a = 0                 # The next artist field of the node
next_t = 4                 # The next title field of the node
artist = 8                 # The artist field of the node
title  = 12                # The title field of the node 
time   = 16                # The time field of the node
tracks = 18                # The track count field of the node


.text

arthdr: 
	.string "Name of Artist            CD Title                                 Trk Time \n"
artbar: 
	.string "========================= ======================================== === =====\n"
artfmt: 
	.string "%-25s %-40s %3i %02i:%02i\n" 

ttlhdr: 
	.string "CD Title                                 Name of Artist            Trk Time \n"
ttlbar: 
	.string "======================================== ========================= === =====\n"
ttlfmt: 
  .string "%-40s %-25s %3i %02i:%02i\n"

# Simple macro for printing the cd title header
.macro prtth 
			pushl $ttlhdr
			call  printf
			popl  %ebx

			pushl $ttlbar
			call  printf
			popl  %ebx
.endm

# Simple macro for printing the artist header
.macro prtah 
			pushl $arthdr
			call  printf
			popl  %ebx

			pushl $artbar
			call  printf
			popl  %ebx
.endm

.macro prtart node
			movl  time(\node), %eax
			movl  $60, %ebx
			idivl %ebx
			pushl %edx         # push the seconds onto the stack
			pushl %eax         # push the minutes onto the stack
			pushl tracks(\node)
			pushl title(\node) 
			pushl artist(\node) 
			pushl $artfmt
			call  printf
.endm


#.macro prtttl node
#			movw  $60, %bx
#			movw  time(\node), %ax
#			idivw %bx
#			movw  %dx, %edx
#			movw  %ax, %eax
#			pushl %edx         # push the seconds onto the stack
#			pushl %eax         # push the minutes onto the stack
#			movb  tracks(\node), %ebx
#			pushl %ebx 
#			pushl artist(\node) 
#			pushl title(\node) 
#			pushl $ttlfmt
#			call  printf
#			popl  %edx
#			popl  %edx
#			popl  %edx
#			popl  %edx
#			popl  %edx
#			popl  %edx
#.endm

.globl print
print: 
				enter  $0, $0
				movl   LIST_ARG(%ebp), %ecx      # list argument

				cmpl   $A_ARTIST, %ecx
				je     print_a
				cmpl   $A_TITLE, %ecx
				je     print_t
				cmpl   $ALL_TITLES, %ecx
				je     printall_t
        cmpl   $ALL_ARTISTS, %ecx
        je     printall_a


printall_t:															 # print all titles
			 prtth
       movl    ROOT_ARG(%ebp), %ecx      # grab node argument
			 movl    (%ecx), %ecx

printall_t_loop:												 # print all titles
			 #prtttl  %ecx										 # print this node
				movw  $60, %bx
				movw  time(%ecx), %ax
				idivw %bx
				pushl %edx         # push the seconds onto the stack
				pushl %eax         # push the minutes onto the stack
				xorl  %ebx, %ebx
				movb  tracks(%ecx), %bl
				pushl %ebx 
				pushl artist(%ecx) 
				pushl title(%ecx) 
				pushl $ttlfmt
				call  printf
				popl  %edx
				popl  %edx
				popl  %edx
				popl  %edx
				popl  %edx
				popl  %edx

			 movl    next_t(%ecx), %ebx
			 movl    %ebx, %ecx
			 cmpl    $0, %ecx
			 jnz     printall_t_loop
			 jmp     return

printall_a:															 # print all titles
			 prtah
       movl    ROOT_ARG(%ebp), %ecx      # grab node argument
			 movl    (%ecx), %ecx

printall_a_loop:												 # print all titles
			 #prtttl  %ecx										 # print this node
			 movl    next_a(%ecx), %ecx
			 cmpl    $0, %ecx
			 jne     printall_a_loop

			 jmp		 return

print_a:																 # print a artist
			 prtah

			 jmp		 return
			
print_t:																 # print a title
			 prtth

return:
       leave														 # discared stack frame
       ret															 # return


