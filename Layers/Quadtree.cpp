#include "Quadtree.h"

/**
 * @brief This constructor builds the Quadtree data structure
 * @param nodes A list of Node objects to be included in the Quadtree
 * @param size The maximum number of Node objects allowed in each leaf
 * @param minX The lower bound x-value
 * @param maxX The upper bound x-value
 * @param minY The lower bound y-value
 * @param maxY The upper bound y-value
 */
Quadtree::Quadtree(std::vector<Node> nodes, int size, float minX, float maxX, float minY, float maxY)
{
	nodeList = nodes;
	binSize = size;

	// Create the root branch
	root = newBranch(minX, maxX, minY, maxY);

	if (binSize > 0)
		for (unsigned int i=0; i<nodeList.size(); i++)
			addNode(&nodeList[i], root);
}


Quadtree::~Quadtree()
{
	for (unsigned int i=0; i<leafList.size(); i++)
		if (leafList[i] != 0)
			delete leafList[i];
	for (unsigned int i=0; i<branchList.size(); i++)
		if (branchList[i] != 0)
			delete branchList[i];
}


/**
 * @brief This function is called by the user to find the Node closest to the provided x-y coordinates
 *
 * This function is called by the user to find the Node closest to the provided x-y coordinates.
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return A pointer to the Quadtree's copy of the Node closest to (x, y)
 * @return 0 if the point is outside the bounds of the Quadtree
 */
Node* Quadtree::FindNode(float x, float y)
{
	return FindNode(x, y, root);
}


/**
 * @brief The actual recursive function called when the user requests a Node
 *
 * This is the actual recursive function used to traverse the tree and find the Node closest
 * to (x, y).
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @param currBranch Pass in root to begin tree traversal
 * @return A pointer to the Quadtree's copy of the Node closest to (x, y)
 */
Node* Quadtree::FindNode(float x, float y, branch *currBranch)
{
	// If the point falls into a sub-branch, start recursion by calling this function
	// on that sub-branch.
	for (int i=0; i<4; i++)
		if (currBranch->branches[i] != 0 && pointIsInside(currBranch->branches[i], x, y))
			return FindNode(x, y, currBranch->branches[i]);

	// If the point falls into a leaf, find the Node closest to the provided point,
	// and return that Node
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0 && pointIsInside(currBranch->leaves[i], x, y) && currBranch->leaves[i]->nodes.size() > 0)
		{
			Node *currClosest = currBranch->leaves[i]->nodes[0];
			float newDistance, currDistance = distance(currBranch->leaves[i]->nodes[0], x, y);
			for (unsigned int j=1; j<currBranch->leaves[i]->nodes.size(); j++)
			{
				newDistance = distance(currBranch->leaves[i]->nodes[j], x, y);
				if (newDistance < currDistance)
				{
					currClosest = currBranch->leaves[i]->nodes[j];
					currDistance = newDistance;
				}
			}
			return currClosest;
		}
	}

	// The point is not inside the quadtree bounds or there are no nodes in the
	// deepest level reached (possible improvements -- continue searching other
	// leaves/branches if the parser reaches a leaf with no nodes
	return 0;
}


/**
 * @brief Creates a new leaf with the specified boundaries
 *
 * This function creates a new leaf on the heap with the specified boundaries
 *
 * @param l The lower bound x-value
 * @param r The upper bound x-value
 * @param b The lower bound y-value
 * @param t The upper bound y-value
 * @return A pointer to the new leaf object
 */
leaf* Quadtree::newLeaf(float l, float r, float b, float t)
{
	// Create leaf in heap and put pointer into leaf list
	leaf *currLeaf = new leaf;
	leafList.push_back(currLeaf);

	// Set the boundaries of the leaf
	currLeaf->bounds[0] = l;
	currLeaf->bounds[1] = r;
	currLeaf->bounds[2] = b;
	currLeaf->bounds[3] = t;

	return currLeaf;
}


/**
 * @brief Creates a new branch with the specified boundaries
 *
 * This function creates a new branch on the heap with the specified boundaries
 *
 * @param l The lower bound x-value
 * @param r The upper bound x-value
 * @param b The lower bound y-value
 * @param t The upper bound y-value
 * @return A pointer to the new branch object
 */
branch* Quadtree::newBranch(float l, float r, float b, float t)
{
	// Create branch in heap and put pointer into branch list
	branch *currBranch = new branch;
	branchList.push_back(currBranch);

	// Set the boundaries of the branch
	currBranch->bounds[0] = l;
	currBranch->bounds[1] = r;
	currBranch->bounds[2] = b;
	currBranch->bounds[3] = t;

	// Set all branch pointers in the branch to 0
	for (int i=0; i<4; i++)
		currBranch->branches[i] = 0;

	// Create all four leaves in the branch
	float x = l+(r-l)/2.0;
	float y = b+(t-b)/2.0;
	currBranch->leaves[0] = newLeaf(x, r, y, t);
	currBranch->leaves[1] = newLeaf(l, x, y, t);
	currBranch->leaves[2] = newLeaf(l, x, b, y);
	currBranch->leaves[3] = newLeaf(x, r, b, y);

	return currBranch;
}


/**
 * @brief Converts a leaf into a branch
 *
 * This function is used to turn a leaf into a branch when the leaf needs to add more nodes
 * but has reached its maximum capacity. All of the nodes that were in the old leaf are added
 * to the new branch and the old leaf is permanently deleted.
 *
 * @param currLeaf A pointer to the leaf that will be turned into a branch
 * @return  A pointer to the new branch object
 */
branch* Quadtree::leafToBranch(leaf *currLeaf)
{
	// Create new branch with same bounds as the old leaf
	branch *currBranch = newBranch(currLeaf->bounds[0], currLeaf->bounds[1], currLeaf->bounds[2], currLeaf->bounds[3]);

	// Add all of the nodes that were in the old leaf to the new branch
	for (unsigned int i=0; i<currLeaf->nodes.size(); i++)
		addNode(currLeaf->nodes[i], currBranch);

	// Remove the old leaf pointer from the leaf list (just change to 0, removing is inefficient) and delete it
	for (unsigned int i=0; i<leafList.size(); i++)
	{
		if (leafList[i] == currLeaf && leafList[i] != 0)
		{
			leafList[i] = 0;
			delete currLeaf;
		}
	}
	return currBranch;
}


/**
 * @brief Adds a Node to a leaf
 *
 * This function is used to add a Node to a leaf. If the leaf is full, it will be turned
 * into a branch and a pointer to the new branch object will be returned.
 *
 * @param currNode A pointer to the Node object that will be added to the leaf
 * @param currLeaf A pointer to the leaf that the Node will be added to
 * @return A pointer to a new branch if the leaf was full and a new branch was created
 * @return 0 if the Node was added successfully to the leaf
 */
branch* Quadtree::addNode(Node *currNode, leaf *currLeaf)
{
	// Make sure leaf has room for the new node. If so, add it. Otherwise,
	// turn the leaf into a branch and add the node to the new branch.
	if ((int)currLeaf->nodes.size() < binSize)
	{
		currLeaf->nodes.push_back(currNode);
		return 0;
	} else {
		branch *currBranch = leafToBranch(currLeaf);
		addNode(currNode, currBranch);
		return currBranch;
	}
}


/**
 * @brief Adds a Node to a branch
 *
 * This function is used to add a Node to a branch. It is called recursively until
 * a leaf is found that the Node will fit into.
 *
 * @param currNode A pointer to the Node object that will be added to the branch
 * @param currBranch A pointer to the branch that the Node will be added to
 */
void Quadtree::addNode(Node *currNode, branch *currBranch)
{
	// Loop through the branches to find a fit
	for (int i=0; i<4; i++)
	{
		if (currBranch->branches[i] != 0)
		{
			// Found a fit, add the node to this branch
			if (nodeIsInside(currNode, currBranch->branches[i]))
			{
				addNode(currNode, currBranch->branches[i]);
				return;
			}
		}
	}

	// Couldn't find a branch to go into, now look for a leaf to go into
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
		{
			// Found a fit, add the node to this leaf
			if (nodeIsInside(currNode, currBranch->leaves[i]))
			{
				branch *result = addNode(currNode, currBranch->leaves[i]);

				// Result is 0, so leaf still exists
				if (result == 0)
					return;

				// Leaf was turned into a branch, so update the current branch to include it
				else
				{
					// Leaf gets deleted by the leafToBranch() function
					currBranch->leaves[i] = 0;
					currBranch->branches[i] = result;
					return;
				}
			}
		}
	}

	// This node doesn't fit anywhere, let the user know
	DEBUG("Error adding Node to Quadtree, node picking will not work for node number %i", currNode->nodeNumber);
}


/**
 * @brief A helper function that determines if the Node is inside of the leaf
 *
 * A helper function that determines if the Node is inside of the leaf.
 *
 * @param currNode A pointer to the Node being tested
 * @param currLeaf A pointer to the leaf being tested
 * @return true if the Node is inside of the leaf
 * @return false if the Node is not inside of the leaf
 */
bool Quadtree::nodeIsInside(Node *currNode, leaf *currLeaf)
{
	if (currNode->x <= currLeaf->bounds[1] && currNode->x >= currLeaf->bounds[0])
		if (currNode->y <= currLeaf->bounds[3] && currNode->y >= currLeaf->bounds[2])
			return true;
	return false;
}


/**
 * @brief A helper function that determines if the Node is inside of the branch
 *
 * A helper function that determines if the Node is inside of the branch.
 *
 * @param currNode A pointer to the Node being tested
 * @param currBranch A pointer to the branch being tested
 * @return true if the Node is inside of the branch
 * @return false if the Node is not inside of the branch
 */
bool Quadtree::nodeIsInside(Node *currNode, branch *currBranch)
{
	if (currNode->x <= currBranch->bounds[1] && currNode->x >= currBranch->bounds[0])
		if (currNode->y <= currBranch->bounds[3] && currNode->y >= currBranch->bounds[2])
			return true;
	return false;
}


/**
 * @brief A helper function that determines the distance between the Node and the given x-y coordinates
 *
 * A helper function that determines the distance between the Node and the given x-y coordinates.
 * Distance is calculated using a^2 = b^2 + c^2.
 *
 * @param currNode A pointer to the Node we need to find the distance to
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return The distance between the Node and (x, y)
 */
float Quadtree::distance(Node *currNode, float x, float y)
{
	return sqrt(pow(y-currNode->y, 2)+pow(x-currNode->x, 2));
}


/**
 * @brief A helper function that determines if the point is inside of the leaf
 *
 * A helper function that determines if the point is inside of the leaf. Similar
 * to nodeIsInside, but used for picking instead of building the Quadtree.
 *
 * @param currLeaf A pointer to the leaf being tested
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return true if the point is inside of the leaf
 * @return false if the point is not inside of the leaf
 */
bool Quadtree::pointIsInside(leaf *currLeaf, float x, float y)
{
	if (x <= currLeaf->bounds[1] && x >= currLeaf->bounds[0])
		if (y <= currLeaf->bounds[3] && y >= currLeaf->bounds[2])
			return true;
	return false;
}


/**
 * @brief A helper function that determines if the point is inside of the branch
 *
 * A helper function that determines if the point is inside of the branch. Similar
 * to nodeIsInside, but used for picking instead of building the Quadtree.
 *
 * @param currBranch A pointer to the branch being tested
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return true if the point is inside of the branch
 * @return false if the point is not inside of the branch
 */
bool Quadtree::pointIsInside(branch *currBranch, float x, float y)
{
	if (x <= currBranch->bounds[1] && x >= currBranch->bounds[0])
		if (y <= currBranch->bounds[3] && y >= currBranch->bounds[2])
			return true;
	return false;
}
