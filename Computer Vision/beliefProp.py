def get0Values(key, holdVar, col, cCostTable, varNodesOfKey, xToCDict, xToCMessages):
    varValues = []
    sumValues = []
    sVal = 0.0

    for i in range(0, len(varNodesOfKey)):
        if holdVar is not varNodesOfKey[i]:
            varIndex = varNodesOfKey[i]
            keyValueArray = xToCDict[varIndex]
            count = 0
            for j in keyValueArray:
                if j is key:
                    valArr = xToCMessages[varIndex]
                    varValues.append(valArr[count])
                count = count + 1
    for row in cCostTable:
        sVal = 0.0
        if row[col] == 0:
            for value in varValues:
                sVal += row[-1] + value[0]
            sumValues.append(sVal)

    return min(sumValues)


def get1Values(key, holdVar, col, cCostTable, varNodesOfKey, xToCDict, xToCMessages):
    varValues = []
    sumValues = []
    sVal = 0.0

    for i in range(0, len(varNodesOfKey)):
        if holdVar is not varNodesOfKey[i]:
            varIndex = varNodesOfKey[i]
            keyValueArray = xToCDict[varIndex]
            count = 0
            for j in keyValueArray:
                if j is key:
                    valArr = xToCMessages[varIndex]
                    varValues.append(valArr[count])
                count = count + 1
    for row in cCostTable:
        sVal = 0.0
        if row[col] == 1:
            for value in varValues:
                sVal += row[-1] + value[1]
            sumValues.append(sVal)

    return min(sumValues)


def findVarNodeIndex(fNode, cToXDict, varNode):
    i = 0
    varArray = cToXDict[fNode]
    for node in varArray:
        if node is varNode:
            return i
        else:
            i = i + 1

def findFactNodeIndex(varNode, xToCDict, factorNode):
    i = 0
    varArray = xToCDict[varNode]
    for node in varArray:
        if node is factorNode:
            return i
        else:
            i = i + 1

MAX_ITERATIONS = 10

cCostTable = [[0, 0, 0, 0, 0, ],
              [0, 0, 0, 1, 1000],
              [0, 0, 1, 0, 1000],
              [0, 1, 0, 0, 1000],
              [0, 0, 1, 1, 0],
              [0, 1, 1, 0, 0],
              [0, 1, 1, 1, 1000],
              [0, 1, 0, 1, 0],
              [1, 0, 0, 0, 1000],
              [1, 0, 0, 1, 0],
              [1, 0, 1, 0, 0],
              [1, 1, 0, 0, 0],
              [1, 0, 1, 1, 1000],
              [1, 1, 1, 0, 1000],
              [1, 1, 1, 1, 0],
              [1, 1, 0, 1, 1000]]

# These are the messages getting passed from the variables to the factor nodes
xToCDict = {'x1': ['c1', 'c8', 'c9', 'c10'], 'x2': ['c2', 'c8', 'c9'], 'x3': ['c3', 'c8', 'c10'],
            'x4': ['c4', 'c9', 'c10'], 'x5': ['c5', 'c8'], 'x6': ['c6', 'c9'], 'x7': ['c7', 'c10']}
xToCMessages = {'x1': [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0]], 'x2': [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0]],
                'x3': [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0]],
                'x4': [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0]], 'x5': [[0.0, 0.0], [0.0, 0.0]],
                'x6': [[0.0, 0.0], [0.0, 0.0]], 'x7': [[0.0, 0.0], [0.0, 0.0]]}

# These are the costs tables
cToXDict = {'c1': ['x1'], 'c2': ['x2'], 'c3': ['x3'], 'c4': ['x4'], 'c5': ['x5'], 'c6': ['x6'], 'c7': ['x7'],
            'c8': ['x1', 'x2', 'x3', 'x5'],
            'c9': ['x1', 'x2', 'x4', 'x6'], 'c10': ['x1', 'x3', 'x4', 'x7']}
cToXMessages = {'c8': [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0]],
                'c9': [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0]],
                'c10': [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0]], 'c1': [[0.0, 3.0]], 'c2': [[0.0, 2.0]],
                'c3': [[0.0, 2.5]], 'c4': [[0.0, 5.4]], 'c5': [[0.0, 4.0]], 'c6': [[0.2, 0.0]], 'c7': [[0.7, 0.0]]}

varBeliefs = {'x1': [0.0, 0.0], 'x2': [0.0, 0.0], 'x3': [0.0, 0.0], 'x4': [0.0, 0.0], 'x5': [0.0, 0.0], 'x6': [0.0, 0.0], 'x7': [0.0, 0.0]}
varValues = {'x1': 0, 'x2': 0, 'x3': 0, 'x4': 0, 'x5': 0, 'x6': 0, 'x7': 0}
oldVarValues = {'x1': 0, 'x2': 0, 'x3': 0, 'x4': 0, 'x5': 0, 'x6': 0, 'x7': 0}
count = 0
sameValues = True
firstIteration = True

while count < MAX_ITERATIONS:
    sameValues = True
    # Step 1 pass messages from variables to factor nodes
    if not firstIteration:
        for var in xToCMessages:

            currentBelief = varBeliefs[var]
            currentVarMessages = xToCMessages[var]
            fNodesOfVariable = xToCDict[var]

            for node in fNodesOfVariable:
                vIndex = findVarNodeIndex(node, cToXDict, var)
                fNodeIndex = findFactNodeIndex(var, xToCDict, node)
                xToCMessages[var][fNodeIndex] = [currentBelief[0] - cToXMessages[node][vIndex][0],
                                                                         currentBelief[1] - cToXMessages[node][vIndex][1]]
        # for messages in currentVarMessages:
        #     xToCMessages[var][messageIndex] = [currentBelief[0] - messages[0], currentBelief[1] - messages[1]]
        #     messageIndex = messageIndex + 1

    # step 2 send the message back from the factor nodes 1 - 7 to the variables
    # for factorNode in cToXMessages:
    firstIteration = False
    keys = cToXMessages.keys()
    for key in keys:
        varNodesOfKey = cToXDict[key]

        zeroValues = []
        oneValues = []
        for i in range(0, len(varNodesOfKey)):
            # Get the zero values for the messages to send from the factor to variable nodes
            zeroValues.append(
                get0Values(key, varNodesOfKey[i], i, cCostTable, varNodesOfKey, xToCDict, xToCMessages))

            # Get the one values for the messages to send from the factor to variable nodes
            oneValues.append(
                get1Values(key, varNodesOfKey[i], i, cCostTable, varNodesOfKey, xToCDict, xToCMessages))

            smallest0Num = min(zeroValues)
            if len(cToXMessages[key]) > 1:
                cToXMessages[key][i][0] = smallest0Num

            smallest1Num = min(oneValues)
            if len(cToXMessages[key]) > 1:
                cToXMessages[key][i][1] = smallest1Num

    # step 3 add up all the incoming messages that each variable node has to decide each var node's belief
    varNodeKeys = xToCDict.keys()
    for key in varNodeKeys:
        sumOfVarNodeMessages = [0.0, 0.0]
        factorNodesOfVarNode = xToCDict[key]

        for fNode in factorNodesOfVarNode:
            varNodeArray = cToXDict[fNode]

            if len(varNodeArray) == 1:
                value = cToXMessages[fNode]
                sumOfVarNodeMessages = [sumOfVarNodeMessages[0] + value[0][0], sumOfVarNodeMessages[1] + value[0][1]]
            else:
                index = findVarNodeIndex(fNode, cToXDict, key)
                value2 = cToXMessages[fNode][index]
                sumOfVarNodeMessages = [sumOfVarNodeMessages[0] + value2[0], sumOfVarNodeMessages[1] + value2[1]]

        varBeliefs[key] = [sumOfVarNodeMessages[0], sumOfVarNodeMessages[1]]
    # step 4
    for key in varBeliefs:
        beliefs = varBeliefs[key]
        if beliefs[0] < beliefs[1]:
            varValues[key] = 0
        else:
            varValues[key] = 1

    for key in varValues:
        if varValues[key] != oldVarValues[key]:
            oldVarValues[key] = varValues[key]
            sameValues = False

    if sameValues:
        print "\n"
        print "Successful Convergence on Threshold Count: ", count
        print "Variable Values ", varValues
        break

    count = count + 1
