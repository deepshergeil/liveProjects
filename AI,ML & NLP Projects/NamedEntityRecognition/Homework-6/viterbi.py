import numpy as np

def run_viterbi(emission_scores, trans_scores, start_scores, end_scores):
    """Run the Viterbi algorithm.

    N - number of tokens (length of sentence)
    L - number of labels

    As an input, you are given:
    - Emission scores, as an NxL array
    - Transition scores (Yp -> Yc), as an LxL array
    - Start transition scores (S -> Y), as an Lx1 array
    - End transition scores (Y -> E), as an Lx1 array

    You have to return a tuple (s,y), where:
    - s is the score of the best sequence
    - y is a size N array of integers representing the best sequence.
    """





    L = start_scores.shape[0]
    assert end_scores.shape[0] == L
    assert trans_scores.shape[0] == L
    assert trans_scores.shape[1] == L
    assert emission_scores.shape[1] == L
    N = emission_scores.shape[0]

    #print N,L
    #print "emission",emission_scores
    #print "trans",trans_scores
    y = []

    trellis = []
    trellis = np.empty((L,N))
    trellis.fill(-999999)
    # for i in range(L):x
    #     trellis.append([])
    #     for j in range(N):
    #         trellis[i].append(-999999)

    #backpointer look up
    backpointer_lookup = []
    for i in range(L):
         backpointer_lookup.append([])
         for j in range(N):
             backpointer_lookup[i].append(-999999)
    #backpointer_lookup = np.empty((L, N))
    #backpointer_lookup.fill(-999999)

    #intialisation step

    for i in range(L):
        trellis[i][0] = start_scores[i]+emission_scores[0][i]




    bclist = []
    for i in range(1,N):
        for t in range(L):
            trellis[t][i] = -9999999
            for tsharp in range(0,L):
                tmp = trellis[tsharp][i-1] + trans_scores[tsharp][t]
                if(tmp > trellis[t][i]):
                    trellis[t][i] = tmp

                    backpointer_lookup[t][i] = tsharp
            trellis[t][i] += emission_scores[i][t]
    #print trellis

    t_max = 0
    vit_max = -999999
    for t in range(L):
        if(trellis[t][N-1]+end_scores[t]>vit_max):
            t_max = t
            vit_max = trellis[t][N-1] + end_scores[t]
            fptr = t_max
    #print vit_max
    #print trellis

    lookup = N - 1
    y = [fptr]
    while (lookup > 0):
        y = [backpointer_lookup[fptr][lookup]] + y
        fptr = backpointer_lookup[fptr][lookup]
        lookup -= 1
    # score set to 0
    return (vit_max, y)


# emission_scores = [[0.98048664, -1.05400224, 0.9757591, -0.42101348],
#                        [0.4493018, -0.15166035, 0.71797169, 0.84359061],
#                        [-0.42743567, -1.23153515, 1.27599586, 2.40114231]]
# emission_scores = np.asarray(emission_scores)
# trans_scores = [[0.97377485, 0.84401151, -1.33471054, 0.61136348],
#                     [-0.34217604, -0.00214455, -0.86955931, -0.45247103],
#                     [-1.02864579, 1.78405202, -0.34863648, -0.07168477],
#                     [0.28655519, 0.0767991, 0.28627151, -0.15665304]]
# trans_scores = np.asarray(trans_scores)
# start_scores = [0.90601404, 1.40207137, -1.26996332, -1.51748341]
# start_scores = np.asarray(start_scores)
# end_scores = [0.65992413, 0.94548428, 0.5690676, -1.11417573]
# end_scores = np.asarray(end_scores)
# run_viterbi(emission_scores, trans_scores, start_scores, end_scores)