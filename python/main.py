import maze as mz
import node
import score
import interface
import time


import random
import numpy as np
import pandas
import time
import sys
import os

def main():
    directory = "python/data/mid_maze.csv"
    print(directory)
    maze = mz.Maze(directory)
    # point = score.Scoreboard("python/data/UID.csv", "team_942136")
    interf = interface.interface()
    
    # TODO : Initialize necessary variables
    car_dir = node.Direction(2)
    nd_begin = maze.nodes[0]
    nd_current = nd_begin
    dir_list = []
    if nd_begin in maze.dead_end:
        maze.dead_end.remove(nd_begin)
    
    while len(maze.dead_end)>0:
        if len(maze.dead_end) == 1:
            print('reach the end')
            # interf.end_process() 
            break
        if nd_current in maze.dead_end:
            maze.dead_end.remove(nd_current)
        print('beginning this turn from', nd_current)
        route = maze.BFS(nd_current)
        print(route)
        for i in range (0,len(route)-1):
            nd_from = route[i]
            nd_to = route[i+1]
            next_action, car_dir = maze.getAction(car_dir, nd_from, nd_to)
            print('From %s to %s, action is %s (%s), next direction is %s'%(nd_from, nd_to, next_action, int(next_action) ,car_dir))
            dir_list.append(int(next_action))
        nd_current = route[-1]
        # print('remain dead', maze.dead_end)
    # print(dir_list)
    dir_list.pop(0)
    while True:
        if dir_list == []:
            interf.end_process()
            print('end of function')
            break
        if interf.ser.SerialReadString()=="N":
            interf.send_action(dir_list[0])
            dir_list.pop(0)
            print("remaining directions: ", dir_list)
        elif interf.ser.SerialReadString() == "T":
            point.add_UID(interf.get_UID())
            


if __name__ == '__main__':
    main()
