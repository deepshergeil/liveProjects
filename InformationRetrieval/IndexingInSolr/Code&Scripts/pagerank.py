import networkx as nx

G = nx.read_edgelist("edgeList.txt",create_using=nx.DiGraph())
pr = nx.pagerank(G, alpha=0.85, personalization=None, max_iter=30, tol=1e-06, nstart=None, weight='weight', dangling=None)

outputFileObject = open("external_pageRankFile.txt","w")

for key in pr:
	outputFileObject.write("C:\Users\Deepak Shergeil\Documents\UbuntuShare\BG\BG\BG" + key + "=" + str( pr[key]))
	outputFileObject.write("\n")

outputFileObject.close()


