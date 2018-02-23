#!/bin/python
import nltk
from gensim.models import Word2Vec
import os
import collections
import string
from collections import defaultdict
import numpy as np
from nltk import word_tokenize
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from sklearn.cluster import KMeans
from sklearn.feature_extraction.text import TfidfVectorizer
import re
import sklearn
import gensim

clusters = collections.defaultdict(list)
nclusters =20
sportsdict ={}
sents =[]
cp = defaultdict(list)
clusterdict = defaultdict(list)
stops_words = []
wordcount ={}
bigramdict = defaultdict(list)
trigramdict = defaultdict(list)
c = {}
listofcategories = ["company", "facility", "location", "movie", "musicartist", "person", "product", "sportsteam",
                    "tvshow", "company", "facility"]

def Shape(chars):
    if chars.isupper():
        return 'X'
    elif chars.islower():
        return 'x'
    elif chars.isdigit():
        return 'd'
    else:
        return chars


def wordShape(word):
    stri = ""
    for index in range(len(word) - 2):
        if index < 2:
            stri += Shape(word[index])
        elif index >= 2:
            if stri[-1] != Shape(word[index]):
                stri += Shape(word[index])
    if len(word) > 2:
        stri += Shape(word[-2])
    if len(word) > 3:
        stri += Shape(word[-1])
    return stri


def cosinesimilarity(v1,v2):
    return np.dot(v1, v2) / (np.sqrt(np.dot(v1, v1)) * np.sqrt(np.dot(v2, v2)))

def preprocess_corpus(train_sents):
    global listoftags
    global clusters
    global sents
    global nclusters
    global cp
    global stops_words
    global bigramdict

    """Use the sentences to do whatever preprocessing you think is suitable,
    such as counts, keeping track of rare features/words to remove, matches to lexicons,
    loading files, and so on. Avoid doing any of this in token2features, since
    that will be called on every token of every sentence.

    Of course, this is an optional function.

    Note that you can also call token2features here to aggregate feature counts, etc.
    """





    # f = open("/Users/RishabhTyagi/PycharmProjects/NLP/Homework-6/data/lexicon/english.stop")
    # for name in f:
    #     name = name.strip()
    #     stops_words.append(name)
    # f.close()



    #b = Word2Vec(train_sents,min_count=1,window=5)





    # for x in train_sents:
    #     for j in x:
    #
    #         lista =[]
    #         for k in (b.most_similar(j, topn=10)):
    #             lista.append(k[0])
    #             y = j.strip().lower()
    #             y = ''.join(ch for ch in y if ch not in string.punctuation)
    #
    #         c[y] = lista

    from sklearn.cluster import KMeans
    import numpy as np

    # listofallwords =[]
    # for x in train_sents:
    #     for j in x:
    #         listofallwords.append(j)



    #data/lexicon/
    #"data/lexicon/"
    for filename in os.listdir("/Users/DeepakSingh/PycharmProjects/NLP/Homework-6/data/lexicon/"):
        f = open("data/lexicon/" + filename)
        if not(filename == "internet.website" or filename == "automotive.model" or filename == "dictionaries.conf"  or filename == "venues"):
            for x in f.readlines():
                y = x.strip().lower()
                y = ''.join(ch for ch in y if ch not in string.punctuation)

                listy = y.split()
                for xy in listy:
                    if xy != "":
                        cp[xy].append(filename)
    pass


def token2features(sent, i, add_neighs = True):
    global lisoftags
    global clusters
    global sents
    global nclusters
    global cp
    global stops_words




    """Compute the features of a token.

    All the features are boolean, i.e. they appear or they do not. For the token,
    you have to return a set of strings that represent the features that *fire*
    for the token. See the code below.

    The token is at position i, and the rest of the sentence is provided as well.
    Try to make this efficient, since it is called on every token.

    One thing to note is that it is only called once per token, i.e. we do not call
    this function in the inner loops of training. So if your training is slow, it's
    not because of how long it's taking to run this code. That said, if your number
    of features is quite large, that will cause slowdowns for sure.

    add_neighs is a parameter that allows us to use this function itself in order to
    recursively add the same features, as computed for the neighbors. Of course, we do
    not want to recurse on the neighbors again, and then it is set to False (see code).
    """


    ftrs = []
    # bias
    ftrs.append("BIAS")



    # position features
    if i == 0:
        ftrs.append("SENT_BEGIN")
    if i == len(sent)-1:
        ftrs.append("SENT_END")


    #the word itself
    word = unicode(sent[i])

    ##rohanrishabhfeature



    #Adding top 10 similar words
    # y = word.strip().lower()
    # y = ''.join(ch for ch in y if ch not in string.punctuation)
    #
    # if y in c:
    #     for x in c[y]:
    #         ftrs.append(x)

    ## Vowel count of each word by length of sentence
    vowel_count = 0
    vowels = "aeiouAEIOU"


    for x in word:
        if x in vowels:
            vowel_count+=1
    lenofsent = 0
    lenofsent = len(sent)

    ftrs.append("lengthofSent"+str((vowel_count/lenofsent)))

   ## Vowel count

    ##Word Shape

    stri = wordShape(word)
    ftrs.append(word + stri + ": wordshape")




    #PREFIXES AND SUFFICES

    # if (len(word) >= 4):
    #     ftrs.append("prefix=%s" % word[0:1].lower())
    #     ftrs.append("prefix=%s" % word[0:2].lower())
    #     ftrs.append("prefix=%s" % word[0:3].lower())
    #     ftrs.append("suffix=%s" % word[len(word) - 1:len(word)].lower())
    #     ftrs.append("suffix=%s" % word[len(word) - 2:len(word)].lower())
    #     ftrs.append("suffix=%s" % word[len(word) - 3:len(word)].lower())




    #End Rishubs's

    y = nltk.pos_tag(sent)
    y = y[i]

    ftrs.append(y[1])

    # countvow = 0
    # for bn in word:
    #     if bn == "a" or bn == "e" or bn == "i" or bn == "o" or bn == "u":
    #         countvow += 1
    # ftrs.append("Vowel count: "+str(countvow))

    #Month as a faeture
    #ListMonth = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]



    #Gazeteer
    y = word.strip().lower()
    tmepstr = ''.join(ch for ch in y if ch not in string.punctuation)

    if tmepstr in cp:
       if tmepstr != "":
            for some in cp[tmepstr]:
                if some not in ftrs:
                    ftrs.append(some)
    ## End Gazeeter


    ftrs.append("WORD=" + word)
    ftrs.append("LCASE=" + word.lower())
    # some features of the word





    # Capital First
    if word[0].isupper():
        ftrs.append("Capital_first")
    # End Capital First
    if word.isalnum():
        ftrs.append("IS_ALNUM")
    if word.isnumeric():
        ftrs.append("IS_NUMERIC")
    if word.isdigit():
        ftrs.append("IS_DIGIT")
    if word.isupper():
        ftrs.append("IS_UPPER")
    if word.islower():
        ftrs.append("IS_LOWER")

    # previous/next word feats
    if add_neighs:
        if i > 0:
            for pf in token2features(sent, i-1, add_neighs = False):
                ftrs.append("PREV_" + pf)
        if i < len(sent)-1:
            for pf in token2features(sent, i+1, add_neighs = False):
                ftrs.append("NEXT_" + pf)

    # return it!

    return ftrs

if __name__ == "__main__":
    # sents = [
    # ["My", "name", "is", "above", "ALPHA"],
    # ["My", "name", "is", "Pushkal", "Batra"],
    # ["What", "is", "your", "hobby"]
    # ]
    sents = [["I","am","Rohan"],["I","am","Batman"],["I","am","Superman"],["University"]]
    preprocess_corpus(sents)
    for sent in sents:
        for i in xrange(len(sent)):
            print sent[i], ":", len(token2features(sent, i))