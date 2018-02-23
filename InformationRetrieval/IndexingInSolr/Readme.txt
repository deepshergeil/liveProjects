Exercise 4 (Comparing search EngineRanking Algorithms)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

1)I n this Exercide,we have used Apache Solr software to import a set of web pages and investigate different ranking strategies.
2)Solr can be installed on Unix or Windows machines. However, it is much easier to run Solr on a Unix computer.
3)After Solr is installed,we index the saved webpages in Solr.
4)The user’s query will be processed by a program at your web server which formats the query and sends it to Solr. Solr will process the query and return some results in JSON format. A program on your web server will re-format the results and present them to the user as any search engine would do.
5)Clients use Solr's five fundamental operations to work with Solr. The operations are query, index, delete, commit, and optimize.
++++++++++++++++++++++++++
Comparison with page rank

======================

1)Solr uses Lucene to facilitate ranking.
2)Lucene uses a combination of the Vector Space Model and the Boolean model to determine how relevant a given document is to a user's query.
3)Solr permits us to change the ranking algorithm. This gives us an opportunity to use the PageRank algorithm and see how the results differ
4)There are several ways to manipulate the ranking of a document in Solr. Here, we explain the method which uses a field that refers to an External File that stores the PageRank scores. This external file basically contains a mapping from a key field to the field value.
5) I have used python library to create the page rank, https://pypi.python.org/pypi/networkx/


+++++++++++++++++++++++++++++++++

Note:= I have a file "steps" in documentation folder,which clearly provides the step by step process of the project along with the snapshots for better understanding.
