import sys
#import time
from Queue import PriorityQueue

player1Value={}
player2Value={}
initNodes=[]
adjacencyList={}
inf= float("inf")
bestValue=-inf
bestState=""
bestColor=""

colored= []
initNodes = PriorityQueue()
playerUtility=0
class Node:
    def __init__(self, name, color, depth,minmax,alpha,beta,player):
        self.name = name
        self.color = color
        self.depth = depth
        self.alpha = alpha
        self.beta = beta
        self.minmax = minmax
        self.player = player

        return

    def __cmp__(self, other):
        val= cmp(self.name, other.name)
        if(val == 0):
                return cmp(self.color, other.color)
        else:
                return val


# read input file
#f = open("t5.txt", 'r+b')
#colorArray
with open(sys.argv[2]) as f:
    # colorArray
    colorArray = map(lambda x: x.strip(), f.readline().strip().split(","))
    # print colorArray
    # Initial State Assignment
    initAssignment = f.readline().strip().split(",")
    for item in initAssignment:
        nodeName = item.split(":")[0].strip()
        color = item.split(":")[1].split("-")[0].strip()
        player = item.split(":")[1].split("-")[1].strip()
        # colored nodeNames list
        colored.append(nodeName)
        if (player == "1"):
            myNode = Node(nodeName, color, 0, inf, -inf, inf, player)
        if (player == "2"):
            myNode = Node(nodeName, color, 0, -inf, -inf, inf, player)

        initNodes.put(myNode)

    root = colored[-1]
    colorRoot = " "
    colored.remove(root)
    initNodesList = []
    while not initNodes.empty():
        a = initNodes.get()
        initNodesList.append(a)


    for item in initNodesList:
        if (root == item.name):
            colorRoot = item.color
            initNodesList.remove(item)

    # print colorRoot
    # print root

    # Depth
    maxDepth = int(f.readline().strip())
    # ColorScoring
    valueColorP1 = f.readline().strip().split(",")
    for item in valueColorP1:
        color1 = item.strip().split(":")[0]
        value1 = item.strip().split(":")[1]
        player1Value[color1] = value1

    # print player1Value

    valueColorP2 = f.readline().strip().split(",")
    for item in valueColorP2:
        color2 = item.strip().split(":")[0]
        value2 = item.strip().split(":")[1]
        player2Value[color2] = value2

    # print player2Value

    for item in initNodesList:
        if (item.name != root):
            if (item.player == "1"):
                playerUtility = playerUtility + int(player1Value[item.color])
            if (item.player == "2"):
                playerUtility = playerUtility + (-int(player2Value[item.color]))
    # print playerUtility

    # AdjacencyList
    for index, item in enumerate(f, start=0):
        lineComponents = item.split(': ')
        mainNode = lineComponents[0]  # origin node
        finalNodes = lineComponents[1].strip().split(', ')
        adjacentNode = []
        for finalNode in finalNodes:
            adjacentNode.append(finalNode)
        adjacencyList[mainNode] = adjacentNode

        # print adjacencyList
fo = open("output.txt", "w")

#ArcConsistency
def arcConsistency(colored,initNodesList):
    childlist = []
    my_set=set()
    children = PriorityQueue()
    for item in colored:
        for i in adjacencyList[item]:
            my_set.add(i)
    #unique adjacent of all the colored neighbours
    #my_set = set(childlist)
    a = list(my_set)

    #removing already colored states
    for i in colored:
        for j in a:
                if(i==j):
                    a.remove(j)


    #coloring B,G,R to all the adjacents
    for item in a:
        for color in colorArray:
            myNode = Node(item, color, 0, 0, 0, 0, 0)
            children.put(myNode)

    childList = []
    while not children.empty():
        a = children.get()
        childList.append(a)

    #searching all the invalid adjacent states
    invalidstates = []
    for item in initNodesList:
        for adj in adjacencyList[item.name]:
                    myNode = Node(adj,item.color, 0, 0, 0, 0, 0)
                    invalidstates.append(myNode)


    #satisfying arc Consistency
    for item in invalidstates:
        for child in childList:
            if (item.name == child.name and item.color == child.color):
                childList.remove(item)



    return childList



#MIN-NODE
def minV(nodeName,colorRoot,depth,minimax,alpha,beta,playerUtility,colored,initNodesList):
    if depth==maxDepth:

        colored.append(nodeName)
        myNode = Node(nodeName, colorRoot, 0, inf, -inf, -inf, 2)
        initNodesList.append(myNode)

        playerUtility = int(playerUtility) + int(player1Value[colorRoot])
        minimax = playerUtility
        my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
        my_string = ", ".join(my_ans)
        fo.write(my_string + '\n')
        # s=nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str( beta)
        # fo.write(s + '\n')
        # print playerUtility
        # print "terminalminV"
        return minimax

        # UPDATING STACK
    colored.append(nodeName)
    myNode = Node(nodeName, colorRoot, 0, inf, -inf, -inf, 2)
    initNodesList.append(myNode)
    # performing Arc Consistency
    actions = arcConsistency(colored, initNodesList)

    if(len(actions)== 0):
        playerUtility = int(playerUtility) + int(player1Value[colorRoot])
        minimax = playerUtility
        my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
        my_string = ", ".join(my_ans)
        fo.write(my_string + '\n')
        # s=nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str(beta)
        # fo.write(s + '\n')
        # print playerUtility
        # print "terminalminV"
        return minimax

    minimax = inf

    my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
    my_string = ", ".join(my_ans)
    fo.write(my_string + '\n')
    # s= nodeName+", " +colorRoot+", "+str(depth)+", "+str(minimax)+", "+str(alpha)+", "+str(beta)
    # fo.write(s + '\n')
    playerUtility = int(playerUtility) + int(player1Value[colorRoot])
    # print playerUtility



    for items in actions:
        minimax=min(minimax,maxV(items.name,items.color,depth+1,minimax,alpha,beta,playerUtility,colored,initNodesList))
        colored.pop()
        initNodesList.pop()
        if (minimax <= alpha):
            my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
            my_string = ", ".join(my_ans)
            fo.write(my_string + '\n')
            # s= nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str(beta)
            # fo.write(s + '\n')
            return minimax
        beta=min(beta,minimax)
        my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
        my_string = ", ".join(my_ans)
        fo.write(my_string + '\n')
        # s= nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str(beta)
        # fo.write(s + '\n')
    return minimax

#MAX-NODE
def maxV(nodeName,colorRoot,depth,minimax,alpha,beta,playerUtility,colored,initNodesList):
    global bestValue,bestState,bestColor
    if depth == maxDepth:
        colored.append(nodeName)
        myNode = Node(nodeName, colorRoot, 0, inf, -inf, -inf, 2)
        initNodesList.append(myNode)

        playerUtility = int(playerUtility) - (int(player2Value[colorRoot]))
        minimax=playerUtility
        my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
        my_string=", ".join(my_ans)
        fo.write(my_string + '\n')
        # s=nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str(beta)
        # fo.write(s + '\n')
        # print minimax
        # print "terminalmaxV"
        return minimax


    #UPDATING STACK
    colored.append(nodeName)
    myNode = Node(nodeName, colorRoot, 0, -inf, -inf, inf, 1)
    initNodesList.append(myNode)
    #performing Arc Consistency
    actions=arcConsistency(colored,initNodesList)

    if(len(actions)==0):
        playerUtility = int(playerUtility) - (int(player2Value[colorRoot]))
        minimax = playerUtility
        my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
        my_string = ", ".join(my_ans)
        fo.write(my_string + '\n')
        # s=nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str(beta)
        # fo.write(s + '\n')
        # print minimax
        # print "terminalmaxV"
        return minimax


    minimax = -inf
    my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
    my_string = ", ".join(my_ans)
    fo.write(my_string + '\n')
    # s= nodeName+", " +colorRoot+", "+str(depth)+", "+str(minimax)+", "+str(alpha)+", "+str(beta)
    # fo.write(s + '\n')
    # print playerUtility
    # print player2Value[colorRoot]
    playerUtility=int(playerUtility)-(int(player2Value[colorRoot]))
    # print playerUtility

    for items in actions:
        minimax=max(minimax,minV(items.name,items.color,depth+1,minimax,alpha,beta,playerUtility,colored,initNodesList))
        colored.pop()
        initNodesList.pop()
        if (minimax >= beta):
            my_ans = [nodeName, colorRoot, str(depth), str(minimax), str(alpha), str(beta)]
            my_string = ", ".join(my_ans)
            fo.write(my_string + '\n')
            # s= nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str(beta)
            # fo.write(s + '\n')
            return minimax
        alpha=max(alpha,minimax)
        if(nodeName== root):
            if(bestValue<minimax):
                bestValue=minimax
                bestState=items.name
                bestColor=items.color

        my_ans = [nodeName, colorRoot,str(depth), str(minimax), str(alpha), str(beta)]
        my_string = ", ".join(my_ans)
        fo.write(my_string+'\n')
        # s= nodeName + ", " + colorRoot + ", " + str(depth) + ", " + str(minimax) + ", " + str(alpha) + ", " + str(beta)
        # fo.write(s + '\n')
    return minimax

#ALPHA-BETA SEARCH
def alphabeta(nodeName,colorRoot):
    v=maxV(nodeName,colorRoot,0,-inf,-inf,inf,playerUtility,colored,initNodesList)
    # print v
    my_ans = [bestState, bestColor, str(bestValue)]
    my_string = ", ".join(my_ans)
    fo.write(my_string)
    # s= bestState+", "+str(bestColor)+", "+str(bestValue)
    # fo.write(s)
    fo.close()
    return v
#time1=time.time()
alphabeta(root,colorRoot)
#time2=time.time()
#print(str(time2-time1))