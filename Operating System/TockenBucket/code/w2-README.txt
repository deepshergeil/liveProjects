Documentation for Warmup Assignment 2
=====================================

+-------+
| BUILD |
+-------+

Comments: use Make command

+---------+
| GRADING |
+---------+

Basic running of the code : 100 out of 100 pts

Missing required section(s) in README file : N/A, completed
Cannot compile : Successful
Compiler warnings : No
"make clean" : Successful
Segmentation faults : None
Separate compilation : Successful
Using busy-wait : No
Handling of commandline arguments:
    1) -n : Successful
    2) -lambda : Successful
    3) -mu : Successful
    4) -r : Successful
    5) -B : Successful
    6) -P : Successful
Trace output :
    1) regular packets: Successful
    2) dropped packets: Successful
    3) removed packets: Successful
    4) token arrival (dropped or not dropped): Successful
	5) monotonically non-decreasing timestamps: Yes
Statistics output :
    1) inter-arrival time : Successful
    2) service time : Successful
    3) number of customers in Q1 : Successful
    4) number of customers in Q2 : Successful
    5) number of customers at a server : Successful
    6) time in system : Successful
    7) standard deviation for time in system : Successful
    8) drop probability : Successful
	
Output bad format : Successful
Output wrong precision for statistics (should be 6-8 significant digits) : Successful
Large service time test : Successful
Large inter-arrival time test : Successful
Tiny inter-arrival time test : Successful
Tiny service time test : Successful
Large total number of customers test : Successful
Large total number of customers with high arrival rate test : Successful
Dropped tokens test : Successful
Cannot handle <Cntrl+C> at all (ignored or no statistics) : Successful
Can handle <Cntrl+C> but statistics way off : Successful
Not using condition variables and do some kind of busy-wait : Successful
Synchronization check : Successful
Deadlocks : Not occured
Bad commandline or command : Handled

+----------------------+
| BUGS / TESTS TO SKIP |
+----------------------+

Is there are any tests in the standard test suite that you know that it's not
working and you don't want the grader to run it at all so you won't get extra
deductions, please list them here.  (Of course, if the grader won't run these
tests, you will not get plus points for them.)

Comments: None

+------------------+
| OTHER (Optional) |
+------------------+

None