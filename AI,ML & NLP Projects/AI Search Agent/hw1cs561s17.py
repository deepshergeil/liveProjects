import sys
from Queue import PriorityQueue

lines=[]
# read input file
#f = open("t8.txt", 'r+b')
with open(sys.argv[2]) as f:
    search_type = f.readline().rstrip('\n')
    travel_fuel= int(f.readline().rstrip('\n'))
    start_node = f.readline().rstrip('\n')
    end_node = f.readline().rstrip('\n')

    #Nodename list
    nodes_name=list()

    #Graph for Input adjacency List
    Graph={}
    for line in f:
        splitline=line.split(":")
        node1=splitline[0].split(",")[0]
        Graph[node1]=[]
        branches=splitline[1].strip().rstrip("\n").split(",")
        for i in range(len(branches)):
            branch=tuple(branches[i].strip().split('-'))
            Graph[node1].append(branch)



#print Graph

class Node:
    def __init__(self,nodeName,parent,cost,visited=False):
        self.nodeName=nodeName
        self.parent=parent
        self.cost=cost
        self.visited=visited
    def __cmp__(self, other):
        if cmp(self.cost, other.cost)==0:
            return cmp(self.nodeName,other.nodeName)
        else:
            return cmp(self.cost,other.cost)


#get the nodes_name extract
nodes_name = Graph.keys()
#sort the nodes_name
nodes_name.sort()
#print nodes_name

if int(len(nodes_name))<=0:
    exit(0)

if int(len(nodes_name))==1:
    print "No Path"
    exit(0)

#making dictionary to sort
dic={}
for i in range (len(nodes_name)):
    dic[nodes_name[i]]=i

#print dic
#print nodes_name

graph_matrix = []

#Initialising Matrix to zero
for x in nodes_name:
    rows=[0]*len(nodes_name)
    graph_matrix.append(rows)

#updating Matrix
for x in nodes_name:
    branch=Graph[x]
    row=dic[x]
    for y in branch:
        col=dic[y[0]]
        graph_matrix[row][col]=int(y[1])


#print graph_matrix

def bfs(graph_matrix,start_node,end_node,nodes_name,travel_fuel):
    #initial setup
    start_no = nodes_name.index(start_node)
    #Frontier
    bfs_queue = list()
    #add the start node in queue

    names_visited = []
    for x in nodes_name:
        names_visited.append(Node(x,0,0,False))
    #print len(names_visited)
    bfs_queue.append(names_visited[start_no])

    #names_visited[start_no].visited=True
    bfs_path =list()

    #Dequeue node 0
    start_no = nodes_name.index(start_node)
    node = bfs_queue.pop(0)
    node.visited=True


    bfs_path.append(node)

    while True:
        for x in range (0,len(names_visited)):
                #check if route exists and that node isnt visited plus fuel limitation
                if graph_matrix[start_no][x] > 0 and (names_visited[x].visited == False and ( names_visited[start_no].cost+int(graph_matrix[start_no][x])) <= travel_fuel):

                     #visit node
                        #names_visited[x].visited=True
                        names_visited[x].parent = nodes_name[start_no]

                        names_visited[x].cost = names_visited[start_no].cost+(graph_matrix[start_no][x])
                        if names_visited[x].nodeName == end_node:
                                rem_fuel=travel_fuel-names_visited[x].cost
                                bfs_queue.append(names_visited[x])
                                bfs_path.append(names_visited[x])
                                #for p in bfs_path:
                                     #print p.nodeName
                                #print rem_fuel
                                print_path=path_find(bfs_path)
                                #print  print_path,rem_fuel
                                return print_path, rem_fuel
                        else:

                              #enqueue element
                               bfs_queue.append(names_visited[x])
                               bfs_path.append(names_visited[x])


        #when queue is empty ,break
        if len(bfs_queue) == 0:
            break;

        else:

                node=bfs_queue.pop(0)
                name=node.nodeName
                start_no=nodes_name.index(name)
                node.visited=True


def dfs(graph_matrix, start_node, end_node, nodes_name, travel_fuel):
    # initial setup
    names_visited=[]
    for x in nodes_name:
        names_visited.append(Node(x, 0, 0, False))
    #names_visited[start_no].visited = True
    dfs_path = list()
    Answer=dfs_util(graph_matrix,start_node,end_node,nodes_name,names_visited,travel_fuel,dfs_path)
    #print Answer
    return Answer

    #return Answer

def dfs_util(graph_matrix,start_node,end_node,nodes_name,names_visited,travel_fuel,dfs_path):
    # Dequeue node 0
    start_node=start_node.rstrip("\r")
    end_node=end_node.rstrip("\r")
    start_no = nodes_name.index(start_node)

    names_visited[start_no].visited=True
    dfs_path.append(names_visited[start_no])
    if start_node == end_node:
        rem_Cost = travel_fuel-int(names_visited[start_no].cost)
        # for p in dfs_path:
        #     print p.nodeName
        # print rem_Cost

        print_path = path_find(dfs_path)
        #print print_path, rem_Cost
        return print_path, rem_Cost

    # names_visited[start_no].cost
    for x in range(0, len(names_visited)):
            if graph_matrix[start_no][x] > 0 and (names_visited[x].visited == False and (names_visited[start_no].cost + int(graph_matrix[start_no][x])) <= travel_fuel):
                # visit node
                #names_visited[x].visited = True
                names_visited[x].parent = nodes_name[start_no]
                names_visited[x].cost = names_visited[start_no].cost + (graph_matrix[start_no][x])
                #print names_visited[x].cost
                # DFS Again
                answer = dfs_util(graph_matrix,names_visited[x].nodeName,end_node,nodes_name,names_visited,travel_fuel,dfs_path)
                if answer is not None:
                    return answer




def ucs(graph_matrix,start_node,end_node,nodes_name,travel_fuel):
    start_node=start_node.rstrip("\r")
    end_node= end_node.rstrip("\r")
    start_no = nodes_name.index(start_node)
    # Frontier
    ucs_queue = PriorityQueue()
    # add the start node in queue
    #ucs_queue.put(nodes_name[start_no])
    names_visited = []
    for x in nodes_name:
        names_visited.append(Node(x, 0, 0, False))

    ucs_queue.put(names_visited[start_no])
    #names_visited[start_no].visited = True
    ucs_path = list()

    # Dequeue node 0
    #start_no = nodes_name.index(start_node)
    node = ucs_queue.get()
    node.visited=True
    ucs_path.append(node)
    while True:
        for x in range(0, len(names_visited)):
            # check if route exists and that node isnt visited plus fuel limitation
            if graph_matrix[start_no][x] > 0 and (names_visited[x].visited == False and (names_visited[start_no].cost + int(graph_matrix[start_no][x])) <= travel_fuel):


                if names_visited[x].cost > 0:
                        if names_visited[x].cost > names_visited[start_no].cost + (graph_matrix[start_no][x]):
                            Newnode=names_visited[x]

                            Newnode.cost=names_visited[start_no].cost + (graph_matrix[start_no][x])
                            #CostUpdate and change parent
                            Newnode.parent = nodes_name[start_no]
                            ucs_queue.put(Newnode)
                            ucs_path.append(Newnode)
                            #print (names_visited[x].nodeName+" "+str(names_visited[x].cost))
                else:
                    names_visited[x].cost = names_visited[start_no].cost + (graph_matrix[start_no][x])
                    names_visited[x].parent = nodes_name[start_no]
                    ucs_queue.put(names_visited[x])
                    ucs_path.append(names_visited[x])


        # when queue is empty ,break
        if ucs_queue.qsize() == 0:
            break;

        else:
            size= ucs_queue.qsize()
            copy=list()
            #print ("Qsize "+str(size) )
            while not ucs_queue.empty():
                a= ucs_queue.get()
                #print a.nodeName,a.cost,a.parent
                copy.append(a)
            for i in copy:

                ucs_queue.put(i)

            node = ucs_queue.get()
            name=node.nodeName
            #print ('pop '+name)
            start_no = nodes_name.index(name)
            node.visited=True
            if node.nodeName == end_node:
                rem_fuel = travel_fuel - node.cost
                ucs_queue.put(names_visited[start_no])
                #ucs_path.append(nodes_name[start_no])

                #print rem_fuel
                Answer=path_find(ucs_path)
                return Answer,rem_fuel
                exit(0)



def path_find(algo_path):
    size=len(algo_path)
    final_path=list()
    algo_path.reverse()
    node=algo_path.pop(0)
    name= node.nodeName
    #print name
    #goal added
    final_path.append(name)
    while node.parent!= 0:
        parent_name = node.parent
        #print parent_name
        final_path.append(parent_name)
        for p in algo_path:
            if p.nodeName == parent_name:
                node=p
                break
    final_path.reverse()
    #print final_path
    #print final_path
    return final_path
    #print final_path
#output work
if search_type.upper() == "BFS":
    ans_tuple = bfs(graph_matrix, start_node, end_node, nodes_name,travel_fuel)

if search_type.upper() == "DFS":
    ans_tuple = dfs(graph_matrix, start_node, end_node, nodes_name,travel_fuel)

if search_type.upper() == "UCS":
    ans_tuple = ucs(graph_matrix, start_node, end_node, nodes_name,travel_fuel)

if ans_tuple != None:
    path=ans_tuple[0]
    #print path
    rem=ans_tuple[1]
    f = open("output.txt", "w")
    path_string=""
    for word in path:
        path_string += str(word).rstrip('^%') + "-"
    path_string = path_string[:-1]
    f.write(path_string.replace('\r', '') +' '+ str(rem))
    f.close()


else:
    f = open("output.txt", "w")
    f.write("No Path")
    f.close()
