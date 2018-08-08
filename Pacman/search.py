# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from __future__ import print_function
from util import Queue
from game import Configuration

"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util


class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return [s, s, w, s, w, w, s, w]


def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    """
    from game import Stack
    start_state = problem.getStartState()
    path_stack = Stack()
    visited_coordinates = set()
    visited_coordinates.add(start_state)
    path_stack = myDFS(problem, start_state, path_stack, visited_coordinates)
    path_stack.pop()
    path = []
    while not path_stack.isEmpty():
        item = path_stack.pop()
        path.append(item)
    path.reverse()
    return path


def myDFS(problem, current_position, path_stack, visited_coordinates):
    # if at goal
    if problem.isGoalState(current_position) is True:
        path_stack.push(True)
        return path_stack

    # create stack of neighbors; using stack due to hw specification
    from game import Stack
    successors = problem.getSuccessors(current_position)
    child_stack = Stack()
    for item in successors:
        child_stack.push(item)

    # visit each children
    while not child_stack.isEmpty():
        child = child_stack.pop()
        if child[0] not in visited_coordinates:
            visited_coordinates.add(child[0])
            path_stack.push(child[1])
            path_stack = myDFS(problem, child[0], path_stack, visited_coordinates)
            found = path_stack.pop()
            if found is not True:
                continue
            else:
                path_stack.push(True)
                break
    return path_stack


def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    # start_state = (problem.getStartState(), 'Initial', 0)
    start_state = (problem.getStartState(),)
    # print(start_state)
    # visited_coords = {(start_state[0], start_state[2]): "visited"}
    visited_coords = {}
    parent_nodes = {start_state: start_state}
    direction_nodes = {start_state: "Initial"}
    goalNode = myBFS(problem, start_state, visited_coords, parent_nodes, direction_nodes)
    path = getShortestPath(goalNode, problem, parent_nodes, direction_nodes)
    return path


def myBFS(problem, current_position, visited_coords, parent_nodes, direction_nodes):
    nodes_Queue = Queue()

    while problem.isGoalState(current_position[0]) is not True:
        if (current_position[0]) in visited_coords:
            if nodes_Queue.isEmpty():
                break
            current_position = (nodes_Queue.pop())
            continue
        else:
            # print(current_position)
            nodes_to_expand = problem.getSuccessors(current_position[0])
            visited_coords[current_position[0]] = 'visited'

        for node in nodes_to_expand:
            # print(node[0])
            if visited_coords.get(node[0]) is None:
                nodes_Queue.push(node)
                # visited_coords[(node[0], node[2])] = 'visited'
                parent_nodes[node] = current_position
                direction_nodes[node] = node[1]
            if problem.isGoalState(node[0]):
                # print('visited:', visited_coords)
                # current_position = node
                visited_coords[node[0]] = 'visited'
                #
                parent_nodes[node] = current_position
                direction_nodes[node] = node[1]
                continue

        # if problem.isGoalState(current_position[0]):
        #     break
        else:
            if nodes_Queue.isEmpty():
                break
            current_position = (nodes_Queue.pop())

    return current_position


def getShortestPath(current_node, problem, parent_nodes, direction_nodes):
    path = []
    tempnode = current_node

    while tempnode[0][0] is not problem.getStartState()[0]:
        path.append(direction_nodes.get(tempnode))
        tempnode = parent_nodes.get(tempnode)

    path.reverse()
    # path.remove('Initial')
    return path


def uniformCostSearch(problem):
    from game import PriorityQueue
    start_state = problem.getStartState()
    frontier = PriorityQueue()
    parents_dictionary = {}
    actions = []
    visited = set()
    visited.add(start_state)
    actions = myUCS(problem, start_state, frontier, actions, visited, parents_dictionary)
    actions.pop()
    return actions


def myUCS(problem, current_position, frontier, actions, visited, parents_dictionary):
    # if at goal
    if problem.isGoalState(current_position) is True:
        actions.append("found")
        return actions

    # push children into priority queue
    successors = problem.getSuccessors(current_position)
    for item in successors:
        if item[0] not in visited:
            # check if current path is better than saved path
            if item[0] not in parents_dictionary:
                parents_dictionary[item[0]] = (current_position, item)
            else:
                if findShortestPath(problem, parents_dictionary, current_position, item):
                    parents_dictionary[item[0]] = (current_position, item)
            # calculate f(n) = g(n) + h(n)
            new_actions = findPath(parents_dictionary, problem.getStartState(), item[0])
            p_cost = problem.getCostOfActions(new_actions)
            f_cost = p_cost
            frontier.update(item, f_cost)

    # go to next node from priority queue (frontier)
    while not frontier.isEmpty():
        next_position = frontier.pop()
        if next_position[0] not in visited:
            visited.add(next_position[0])
            actions = findPath(parents_dictionary, problem.getStartState(), next_position[0])
            actions = myUCS(problem, next_position[0], frontier, actions, visited, parents_dictionary)
            result = actions.pop()
            if result is "found":
                actions.append("found")
                break
            else:
                continue
    return actions


def findShortestPath(problem, dictionary, current_position, next_position):
    # current path
    potential_path = findPath(dictionary, problem.getStartState(), current_position)
    potential_path.append(next_position[1])
    potential_p_cost = problem.getCostOfActions(potential_path)
    potential_f_cost = potential_p_cost
    # saved path
    new_actions = findPath(dictionary, problem.getStartState(), next_position[0])
    p_cost = problem.getCostOfActions(new_actions)
    f_cost = p_cost
    old_cost = f_cost
    if potential_f_cost > old_cost:
        return False
    else:
        return True


def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0


def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    from game import PriorityQueue
    start_state = problem.getStartState()
    frontier = PriorityQueue()
    parents_dictionary = {}
    actions = []
    visited = set()
    visited.add(start_state)
    actions = myAStar(problem, start_state, frontier, actions, visited, parents_dictionary, heuristic)
    return actions


def myAStar(problem, current_position, frontier, actions, visited, parents_dictionary, heuristic):

    while not problem.isGoalState(current_position):

        successors = problem.getSuccessors(current_position)

        for item in successors:
            if item[0] not in visited:
                if item[0] not in parents_dictionary:
                    parents_dictionary[item[0]] = (current_position, item)
                else:
                    if betterPath(problem, parents_dictionary, current_position, item, heuristic):
                        parents_dictionary[item[0]] = (current_position, item)

                new_actions = findPath(parents_dictionary, problem.getStartState(), item[0])
                p_cost = problem.getCostOfActions(new_actions)
                h_cost = heuristic(item[0], problem)
                f_cost = p_cost + h_cost
                frontier.update(item[0], f_cost)

        current_position = frontier.pop()
        visited.add(current_position)
        actions = findPath(parents_dictionary, problem.getStartState(), current_position)
        if problem.isGoalState(current_position):
            break

    return actions

# def myAStar(problem, current_position, frontier, actions, visited, parents_dictionary, heuristic):
#     # if at goal
#     if problem.isGoalState(current_position) is True:
#         actions.append("found")
#         return actions
#
#     # push children into priority queue
#     successors = problem.getSuccessors(current_position)
#     for item in successors:
#         if item[0] not in visited:
#             # check if current path is better than saved path
#             if item[0] not in parents_dictionary:
#                 parents_dictionary[item[0]] = (current_position, item)
#             else:
#                 if betterPath(problem, parents_dictionary, current_position, item, heuristic):
#                     parents_dictionary[item[0]] = (current_position, item)
#             # calculate f(n) = g(n) + h(n)
#             new_actions = findPath(parents_dictionary, problem.getStartState(), item[0])
#             p_cost = problem.getCostOfActions(new_actions)
#             h_cost = heuristic(item[0], problem)
#             f_cost = p_cost + h_cost
#             frontier.update(item, f_cost)
#
#     # go to next node from priority queue (frontier)
#     while not frontier.isEmpty():
#         next_position = frontier.pop()
#         if next_position[0] not in visited:
#             visited.add(next_position[0])
#             actions = findPath(parents_dictionary, problem.getStartState(), next_position[0])
#             actions = myAStar(problem, next_position[0], frontier, actions, visited, parents_dictionary, heuristic)
#             result = actions.pop()
#             if result is "found":
#                 actions.append("found")
#                 break
#             else:
#                 continue
#     return actions


# determines if current path is better than saved path of dictionary


def betterPath(problem, dictionary, current_position, next_position, heuristic):
    # current path
    potential_path = findPath(dictionary, problem.getStartState(), current_position)
    potential_path.append(next_position[1])
    potential_p_cost = problem.getCostOfActions(potential_path)
    potential_h_cost = heuristic(next_position[0], problem)
    potential_f_cost = potential_p_cost + potential_h_cost
    # saved path
    new_actions = findPath(dictionary, problem.getStartState(), next_position[0])
    p_cost = problem.getCostOfActions(new_actions)
    h_cost = heuristic(next_position[0], problem)
    f_cost = p_cost + h_cost
    old_cost = f_cost
    if potential_f_cost > old_cost:
        return False
    else:
        return True


# finds the shortest path from goal_state to start_state from dictionary
def findPath(dictionary, start_state, goal_state):
    actions = []
    current = goal_state
    while True:
        previous = dictionary[current]
        actions.insert(0, previous[1][1])
        current = previous[0]
        if previous[0] is start_state:
            break
    return actions


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
