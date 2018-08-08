# multiAgents.py
# --------------
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


from util import manhattanDistance
import random, util

from game import Agent
from math import sqrt, pow
from util import Stack


class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """

    state_set = set([])

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]

        chosenIndex = random.choice(bestIndices)  # Pick randomly among the best
        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.
        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.
        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.
        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        successor_game_state = currentGameState.generatePacmanSuccessor(action)
        next_pac_position = successor_game_state.getPacmanPosition()
        new_ghost_states = successor_game_state.getGhostStates()

        # don't go towards the ghost if neighbors
        # if action results in same position as a ghost, return -999
        for ghostState in new_ghost_states:
            if ghostState.getPosition() == next_pac_position:
                return float("-inf")

        # if action results in getting closer to ghost(s) position
        for ghostState in currentGameState.getGhostStates():
            if isNextTo(next_pac_position, ghostState.getPosition()):
                return float("-inf")

        # if get me closer to food
        current_pac_position = currentGameState.getPacmanPosition()
        if len(successor_game_state.getFood().asList()) is 0:
            return float("inf")
        new_closest_food = findClosestCoordsToFood(successor_game_state.getFood().asList(), next_pac_position)
        old_closest_food = findClosestCoordsToFood(currentGameState.getFood().asList(), current_pac_position)
        old_food_distance = manhattanDistance(current_pac_position, old_closest_food[0])
        new_food_distance = manhattanDistance(next_pac_position, new_closest_food[0])
        food_distance_array = [new_food_distance]
        smallest_dist = min(food_distance_array)

        # get closest closest ghost and its distance
        closest_ghost_dist = 1000
        for ghostState in currentGameState.getGhostStates():
            if closest_ghost_dist < ghostState.getPosition():
                closest_ghost_dist = manhattanDistance(next_pac_position, ghostState.getPosition())

        # add ghost distance to linear equation so i don't loop back and forth
        smallest_index = food_distance_array.index(smallest_dist)
        closest_food_or_capsule_dist = food_distance_array[smallest_index]

        if closest_ghost_dist < 3:
            if closest_ghost_dist is 0:
                return 2/float((closest_food_or_capsule_dist + 4))
            return 2/float((closest_food_or_capsule_dist + 4)) * 20 - 2/closest_ghost_dist
        else:
            # closest_ghost_dist = 20
            if new_food_distance is old_food_distance:
                return 2/float((closest_food_or_capsule_dist + 4)) * 20 - 1/closest_ghost_dist + successor_game_state.getScore()
            return 4/float((closest_food_or_capsule_dist + 4)) * 20 + 4/closest_ghost_dist + (successor_game_state.getScore())


def findClosestCoordsToFood(currentFoodList, newPacmanPos):
    closestFood = [currentFoodList[0], manhattanDistance(currentFoodList[0], newPacmanPos)]

    for food in currentFoodList:
        temp_dist = manhattanDistance(food, newPacmanPos)
        if temp_dist < closestFood[1]:
            closestFood = [food, temp_dist]

    return closestFood


def isNextTo(position_1, position_2):
    # if North of
    if position_2[0] == position_1[0] and position_2[1] == position_1[1] + 1:
        return True
    # if South of
    elif position_2[0] == position_1[0] and position_2[1] == position_1[1] - 1:
        return True
    # if East of
    elif position_2[0] == position_1[0] + 1 and position_2[1] == position_1[1]:
        return True
    # if West of
    elif position_2[0] == position_1[0] - 1 and position_2[1] == position_1[1]:
        return True
    else:
        return False


def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()


class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.
      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.
      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn='scoreEvaluationFunction', depth='2'):
        self.index = 0  # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)
        self.notFirstIteration = False
        self.currentPlayer = 0


class MinimaxAgent(MultiAgentSearchAgent):

    def getAction(self, gameState):
        return minimax(self, gameState, 'max', 0, 0)


def minimax(self, gameState, currentState, currentPlayer, depthCount):
    if gameState.isWin() or gameState.isLose() or depthCount == self.depth:
        return self.evaluationFunction(gameState)

    legalMoves = gameState.getLegalActions(currentPlayer)

    scores = []
    for action in legalMoves:
        nextState = gameState.generateSuccessor(currentPlayer, action)
        # if end of ply
        if currentPlayer % gameState.getNumAgents() == 0:
            scores.append(minimax(self, nextState, 'min', currentPlayer + 1, depthCount))
        # if mid-ply
        elif currentPlayer == (gameState.getNumAgents() - 1):
            scores.append(minimax(self, nextState, 'max', 0, depthCount + 1))
        # if max
        else:
            scores.append(minimax(self, nextState, 'min', currentPlayer + 1, depthCount))

    if depthCount == 0 and currentPlayer == 0:
        index = scores.index(max(scores))
        return legalMoves[index]
    elif currentState == 'max':
        maxMove = max(scores)
        return maxMove
    else:
        minMove = min(scores)
        return minMove


class AlphaBetaAgent(MultiAgentSearchAgent):

    def getAction(self, gameState):
        alpha = -99999
        beta = 99999
        alpha_or_beta = 'alpha'
        opponentCounter = 0
        depthCount = 0
        return alphaBeta(self, gameState, alpha_or_beta, alpha, beta, opponentCounter, depthCount)


def alphaBeta(self, gameState, alpha_or_beta, alpha, beta, currentPlayer, depthCount):
    if gameState.isWin() or gameState.isLose() or depthCount == self.depth:
        return self.evaluationFunction(gameState)

    legalMoves = gameState.getLegalActions(currentPlayer)

    scores = []
    for action in legalMoves:
        nextState = gameState.generateSuccessor(currentPlayer, action)
        if currentPlayer % gameState.getNumAgents() == 0:
            scores.append(alphaBeta(self, nextState, 'beta', alpha, beta, currentPlayer + 1, depthCount))
        elif currentPlayer == (gameState.getNumAgents() - 1):
            scores.append(alphaBeta(self, nextState, 'alpha', alpha, beta, 0, depthCount + 1))
        else:
            scores.append(alphaBeta(self, nextState, 'beta', alpha, beta, currentPlayer + 1, depthCount))
        if alpha_or_beta == 'alpha':
            if scores[-1] > alpha:
                alpha = scores[-1]
        elif alpha_or_beta == 'beta':
            if scores[-1] < beta:
                beta = scores[-1]
        if alpha_or_beta == 'alpha' and scores[-1] > beta:
            break
        elif alpha_or_beta == 'beta' and scores[-1] < alpha:
            break

    if depthCount == 0 and currentPlayer == 0:
        index = scores.index(max(scores))
        return legalMoves[index]
    elif alpha_or_beta == 'alpha':
        maxMove = max(scores)
        return maxMove
    else:
        minMove = min(scores)
        return minMove


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """
    def getAction(self, gameState):
        result = expecti(self, gameState, 'max', 0, 0)
        return result


def expecti(self, gameState, currentState, currentPlayer, depthCount):
    if gameState.isWin() or gameState.isLose() or depthCount == self.depth:
        result = self.evaluationFunction(gameState)
        return result

    legalMoves = gameState.getLegalActions(currentPlayer)

    scores = []
    for action in legalMoves:
        nextState = gameState.generateSuccessor(currentPlayer, action)
        if currentPlayer % gameState.getNumAgents() == 0:
            scores.append(expecti(self, nextState, 'min', currentPlayer + 1, depthCount))
        elif currentPlayer == (gameState.getNumAgents() - 1):
            scores.append(expecti(self, nextState, 'max', 0, depthCount + 1))
        else:
            scores.append(expecti(self, nextState, 'min', currentPlayer + 1, depthCount))

    if depthCount == 0 and currentPlayer == 0:
        index = scores.index(max(scores))
        return legalMoves[index]
    elif currentState == 'max':
        maxMove = max(scores)
        return maxMove
    else:
        return getExpectimaxValue(scores)

def getExpectimaxValue(scores):
    expectivalue = 0.0
    for value in scores:
        expectivalue = expectivalue + value
    return expectivalue / len(scores)


def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).
      DESCRIPTION: Using native numbers is similar to using the inverse. The closer a food pellet is the
      less negative effect is has on the choice. I think I could have gotten more points here if I incorporated
      ghost timers.
    """
    if currentGameState.isWin():
        if currentGameState.getNumFood() is 0:
            return 999.0
        return 800.0
    elif currentGameState.isLose():
        return -999.0

    for ghostState in currentGameState.getGhostStates():
        if isNextTo(currentGameState.getPacmanPosition(), ghostState.getPosition()):
            return -999

    current_position = currentGameState.getPacmanPosition()
    closest_food = findClosestCoordsToFood(currentGameState.getFood().asList(), current_position)
    closest_food_distance = manhattanDistance(currentGameState.getPacmanPosition(), closest_food[0])

    closest_ghost_dist = float("inf")
    for ghostState in currentGameState.getGhostStates():
        if closest_ghost_dist < ghostState.getPosition():
            closest_ghost_dist = manhattanDistance(currentGameState.getPacmanPosition(), ghostState.getPosition())

    return (-0.3 * closest_food_distance) + (-15 * len(currentGameState.getCapsules())) + (-5 * closest_ghost_dist) \
           + scoreEvaluationFunction(currentGameState) + (-2 * len(currentGameState.getFood().asList()))


# Abbreviation
better = betterEvaluationFunction
