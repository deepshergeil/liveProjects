# Image Classification using===( K-NN and PCA)

### Dataset : CIFAR-10 (https://www.cs.toronto.edu/~kriz/cifar.html)
The CIFAR-10 dataset has labeled tiny images (32 × 32 each) with 10 classes. There are in total 50000 images for training and 10000 images for testing. It is a widely-used dataset for benchmarking image classification models.”


### K-Nearest Neighbors (K-NN)
“K-nearest neighbors algorithm (K-NN) is a non-parametric method used for classification. A query object is classified by a majority vote of the K closest training examples (i.e. its neighbors) in the feature space. In this problem, the objective is to implement a K-NN classifier to perform image classification given the image features obtained by PCA.”

### Principal Component Analysis (PCA)
“Instead of classifying images in the pixel domain, we usually first project them into a feature space since raw input data is often too large, noisy and redundant for analysis. Here is where dimensionality reduction techniques come into play. Dimensionality reduc- tion is the process of reducing the number of dimensions of each data point while preserv- ing as much essential information as possible. PCA is one of the main techniques of di- mensionality reduction. It performs a linear mapping of the data to a lower-dimensional space in such a way that the variance of the data in the lower-dimensional representation is maximized. In this problem, the objective is to use the scikit-learn PCA package to perform dimensionality reduction on images extracted from the CIFAR-10 dataset.”



Note-->>I have not used any built in function to use K-NN,rather implemented it myself in python script.:)

Write a Python3 program to solve the aforementioned problems. Your program should use sys.argv[] for taking four argument inputs including K, N, D, and PATH TO DATA. Make sure your program does not download the dataset during execution.


Sample Command : python 4916525888.py 5 100 10 ./cifar-10-batches-py/data batch 1

Sample Output :
                                                                                                                                                         
    3 6
    8 9
    9 9
    6 4
    0 1
    1 1
    2 2
    0 7
    6 8
    0 3                                               
