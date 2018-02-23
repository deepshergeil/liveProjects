Documentation for Warmup Assignment 1
=====================================

+------------------------+
| BUILD & RUN (Required) |
+------------------------+



To compile your code, the grader should type: 

make warmup1 (for sort functionality,part B)
make listtest (to check part A)

+--------------+
| SELF-GRADING |
+--------------+

Replace each "?" below with a numeric value:

(A) Doubly-linked Circular List : 40 out of 40 pts

(B.1) Sort (file) : 30 out of 30 pts
(B.2) Sort (stdin) : 30 out of 30 pts

Missing required section(s) in README file : -0 pts
Cannot compile : -0 pts
Compiler warnings : -0 pts
"make clean" : -0 pts
Segmentation faults : -0 pts
Separate compilation : -0 pts
Malformed input : -0 pts
Too slow : -0 pts
Bad commandline : -0 pts
Did not use My402List and My402ListElem to implement "sort" in (B) : -0 pts

+----------------------+
| BUGS / TESTS TO SKIP |
+----------------------+



Please skip the following tests: none

+-----------------------------------------------+
| OTHER (Optional) - Not considered for grading |
+-----------------------------------------------+

Comments on design decisions: 

1.All the Arguements validation are checked in main() function,(Check for Valid No. of arguements passed/Check valid file pointer/Valid command format)

2.Control passed to parseData() function,where we parse the data from file line by line.
    -Used strtok() to separate the fields(\t) of the input.
	-errorhandler() function ,check for all the possible malformed field data in the input and exit the program.
	-handleAmount() function,check the amount data validation(Numeric/Not Numeric/Large) and exits accordingly.
3.After sorting().printList() function prints thge sorted List.

4.processTime(),processDesc(),processBalance(),processAmt() functions are used to manipulate and output the data according to specs given.
