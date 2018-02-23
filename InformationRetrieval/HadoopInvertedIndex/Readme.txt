Exercidse 3 (Creating an Inverted Index Using Hadoop--Map-Reduce).
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

1)In this Exercise,I have created an Inverted Indexof words occuring in a set of English books.
  --Given a collection of 3,036 English books written by 142 authors.
  --This collection is a small subset of the Project Gutenberg corpus that was further cleaned for the purpose of this assignment.
  
Steps Followed:
 1.Uploading the input data into the bucket.
 2.After Unzipping Contents,we find two folders inside named ‘development’ and ‘full data’. Each of the folders contains the actual data(books) and a mapper file to map the docID to the file name.
 
 3.Now we have cluster and the books in place, I have written the  actual code for the job.
 4.As of now, Google Cloud allows us to submit jobs via the UI, only if they are packaged as a jar file.
 5.Hadoop word count implementation in detail. It takes one text file as input and returns the word count for every word in the file.
 6.The reducer takes this as input, aggregates the word counts using a Hashmap and creates the Inverted index.
 
 7.I have created a jar file for the code,and place this jar file in the default cloud bucket of the cluster.
 8.The output.txt file in the bucket contains the full Inverted Index for all the books.