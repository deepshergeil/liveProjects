Exercise-5(Implementing Spell check,AutoComplete & Snippets to your search engine)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
--To implement the spelling correction feature, I used the Peter Norvig’s Spelling corrector code in PHP to find out the candidates for the erroneous words. 
1.	To implement the autocomplete method, I first made the changes required in the solrconfig.xml as mentioned in the tutorials given under assignment five. 
2.	I used ajax xmlhttpresponse in order to communicate between my php file and the javascript. 
3.	The php file name getSuggestions.php (included in the zip) is called using the onkeyup event handler. 

Snippets\+++++++++++
+++++++++++++++++
1.	I have used simple_html_dom parser to generate the plaintext out of the html pages given to us. 
2.	File_get_contents can be used to get the contents of the file. 

Note:=For Step by step details,please go through doc file "Steps&Report" in the Documentation Folder.

