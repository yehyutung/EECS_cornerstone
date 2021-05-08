import node
import numpy as np
import csv
import pandas
from enum import IntEnum
import math


class Action(IntEnum):
    ADVANCE = 1
    U_TURN = 2
    TURN_RIGHT = 3
    TURN_LEFT = 4
    HALT = 5


class Maze:
    def __init__(self, filepath):
        # TODO : read file and implement a data structure you like
		# For example, when parsing raw_data, you may create several Node objects.  
		# Then you can store these objects into self.nodes.  
		# Finally, add to nd_dictionary by {key(index): value(corresponding node)}
        self.raw_data = pandas.read_csv(filepath).values
        # self.node_count = len(self.raw_data.index)
        self.nodes = []   # node indexes are stored here
        self.nd_dict = dict()  # key: index, value: the correspond node
        self.dead_end = []
        
        for row in self.raw_data:
            node_index = int(row[0])
            new_node = node.Node(node_index)
            self.nodes.append(node_index)
            for i in range(1,5):
                if not math.isnan(row[i]):
                    new_node.setSuccessor(int(row[i]),i,row[i+4])
            self.nd_dict[node_index] = new_node
        # print(self.nodes)
        # print(self.nd_dict)
        
        # self.current_node = self.nodes[0]
        for n in self.nodes:
            if len(self.nd_dict[n].Successors)<2:
                self.dead_end.append(n)
        # print(self.dead_end)
    
    def getStartPoint(self):
        if (len(self.nd_dict) < 2):
            print("Error: the start point is not included.")
            return 0
        return self.nd_dict[1]

    def getNodeDict(self):
        return self.nd_dict


    def getAction(self, car_dir, nd_from, nd_to):
        # TODO : get the car action
        # Tips : return an action and the next direction of the car if the nd_to is the Successor of nd_to
		# If not, print error message and return 0
        if self.nd_dict[nd_from].isSuccessor(nd_to) == False:
            print("You cannot get to this node in one step")
            return Action(5), car_dir 
        next_dir = self.nd_dict[nd_from].getDirection(nd_to)
        if car_dir == next_dir:
            return Action(1), next_dir
        # if car_dir*next_dir == 12 or 2:
        if (car_dir == 1 and next_dir == 2) or (car_dir == 4 and next_dir == 3) or \
            (car_dir == 2 and next_dir == 1) or (car_dir == 3 and next_dir == 4):

            return Action(2), next_dir
        if (car_dir == 1 and next_dir == 4) or (car_dir == 4 and next_dir == 2) or \
            (car_dir == 2 and next_dir == 3) or (car_dir == 3 and next_dir == 1):
            return Action(3), next_dir
        else:
            return Action(4), next_dir
        # return 0, 0
        # return action, next_direction



    def BFS(self, nd):
        # TODO : design your data structure here for your algorithm
        # Tips : return a sequence of nodes from the node to the nearest unexplored deadend
 
        dead_list = []
        for dead_end_node in self.dead_end:
            if nd == dead_end_node:
                print('dead end here')
                break
            dead_list.append(self.BFS_2(nd, dead_end_node))
        shortest_length = dead_list[0][0]
        shortest_route = dead_list[0][1]
        record_dead = 0
        for i in range (0, len(dead_list)):
            if dead_list[i][0]<shortest_length:
                shortest_length = dead_list[i][0]
                shortest_route = dead_list[i][1]
                # record_dead = i
        # dead_list.pop(i)
        return shortest_route

    # route之外還有回傳距離！
    def BFS_2(self, nd_from, nd_to):
        # TODO : similar to BFS but with fixed start point and end point
        # Tips : return a sequence of nodes of the shortest path
        if nd_from == nd_to:
            return 0, [nd_to]
        queue = [nd_from]
        # total_nodes = (len(nd_dict)-1)/2
        m_function=[nd_from]
        d_function = dict()
        d_function[nd_from] = 0
        pi_function = dict()
        pi_function[nd_from] = None
        result_list = []
        my_result = []
        least_step = 0
        while (queue!=[]):
            u = queue[0]
            queue.remove(u)
            i = 0

            for ad_nd in self.nd_dict[u].Successors:
                queue.append(ad_nd[0])
                for a in m_function:
                    if a == ad_nd[0] and ad_nd[0] != nd_to:
                        queue.remove(ad_nd[0])
                        break
                else:
                    m_function.append(ad_nd[0])
                    # print(ad_nd, self.nd_dict[u].getSuccessorNum(i))
                    d_function[ad_nd[0]] = d_function[u] + self.nd_dict[u].getSuccessorNum(i)
                    # print(d_function[ad_nd[0]])
                    pi_function[ad_nd[0]] = u
                    i += 1
                    if ad_nd[0] == nd_to:
                        nd_pt = nd_to
                        result = []
                        while (nd_pt != nd_from):
                            result.insert(0,nd_pt)
                            nd_pt = pi_function[nd_pt]
                        result.insert(0,nd_from)
                        result_list.append([d_function[ad_nd[0]], result])
                    # print(result_list)
        # print("All possible routes: ", result_list)
        best_result = result_list[0][0]
        my_result = result_list[0][1]
        for re in result_list:
            if re[0] < best_result:
                best_result = re[0]
                my_result = re[1]
        return best_result, my_result



    def strategy(self, nd):
        return self.BFS(nd)

    def strategy_2(self, nd_from, nd_to):
        return self.BFS_2(nd_from, nd_to)

    def get_number_of_nodes(self):
        return (len(self.nd_dict)-1)/2