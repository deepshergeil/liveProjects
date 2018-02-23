#author@DeepakSingh,Copyright Reserved.
import sys
import numpy as np
import math
import operator 
from sklearn.decomposition import PCA

def outputToFile(fname, res, label):
	l = len(res)-1
	with open(fname, 'w+') as file:
		for i in range(l):
			file.write(str(res[i])+' '+str(label[i])+'\n')
		file.write(str(res[l])+' '+str(label[l]))

def getNeighbors(training, testing):
	distanceList = []
	for x in range(len(training)):
		dist = np.linalg.norm(testing - training[x])
		distanceList.append(dist)
	return distanceList

def unpickle(file):
    import pickle
    with open(file, 'rb') as fo:
        dict = pickle.load(fo, encoding='bytes')
    return dict

dict = unpickle(sys.argv[4])
myDict = []
l = np.array(dict[b'labels'])
dt = np.array(dict[b'data'])

labels = l[0:1000]
data = dt[0:1000]

#extracting the values of k,n and d
k = int(sys.argv[1])
d = int(sys.argv[2])
n = int(sys.argv[3])

#L(Grayscale) = 0.299R + 0.587G + 0.114B
grayScale = data[:,0:1024] * 0.299 + data[:, 1024:2048] * 0.587 + data[:, 2048:3072] * 0.114

testing = grayScale[0:n, :]
training = grayScale[n:1000, :]

pca = PCA(n_components=d, svd_solver='full')
pca.fit(training)

training_val = pca.transform(training)
testing_val = pca.transform(testing)

length_testing = len(testing_val)
arr = []
for i in range(0, length_testing):
	arr.append(getNeighbors(training_val, testing_val[i]))
neighbors = np.array(arr)

res = []
for b in range(n):
	sortedIndices = neighbors[b].argsort()
	weighted_sum = [0 for zz in range(10)]
	for c in range(k):
		index = sortedIndices[c]
		lbl = labels[n+index] 
		val = 1/neighbors[b][index]
		weighted_sum[lbl] += val
	res.append(weighted_sum.index(max(weighted_sum)))

outputToFile('output.txt', res, labels[:n])
