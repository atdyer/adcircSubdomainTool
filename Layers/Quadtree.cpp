#include "Quadtree.h"

/**
 * @brief This constructor builds the Quadtree data structure from a list of Nodes
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

	hasElements = false;
}


/**
 * @brief This constructor builds the Quadtree data structure from a list of Nodes and
 * a list Elements
 * @param nodes
 * @param elements
 * @param size
 * @param minX
 * @param maxX
 * @param minY
 * @param maxY
 */
Quadtree::Quadtree(std::vector<Node> nodes, std::vector<Element> elements, int size, float minX, float maxX, float minY, float maxY)
{
	nodeList = nodes;
	elementList = elements;
	binSize = size;

	// Create the root branch
	root = newBranch(minX, maxX, minY, maxY);

	if (binSize > 0)
	{
		for (unsigned int i=0; i<nodeList.size(); i++)
			addNode(&nodeList[i], root);

		for (unsigned int i=0; i<elementList.size(); i++)
			addElement(&elementList[i], root);
	}

	hasElements = true;
	DEBUG(elements.size() << " elements added to quadtree");
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
 * @brief This function is called by the user to find all Nodes that fall inside of the provided circle
 *
 * This function is called by the user to find all Nodes that fall inside of the provided circle
 *
 * @param x The x-coordinate of the circle center
 * @param y The y-coordinate of the circle center
 * @param radius The radius of the circle
 * @return A vector of pointers to the Nodes that fall inside of the circle
 */
std::vector<Node*> Quadtree::FindNodesInCircle(float x, float y, float radius)
{
	// Make three lists:
	// - List of Nodes that fall inside of the circle
	// - List of leaves that fall completely inside of the circle
	// - List of leaves that fall partially inside of the circle
	//
	// Add all Nodes that are in the leaves completely inside the circle list
	// to the list of Nodes
	//
	// Search the list of leaves that fall partially inside of the circle
	// for Nodes that fall inside of the circle and add them to the list of
	// Nodes
	//
	// Return the list of Nodes

	std::vector<Node*> circleNodes;
	std::vector<leaf*> fullLeaves;
	std::vector<leaf*> partialLeaves;

	FindLeavesInCircle(x, y, radius, root, &fullLeaves, &partialLeaves);
	AddFullNodes(&fullLeaves, &circleNodes);
	AddPartialNodes(x, y, radius, &partialLeaves, &circleNodes);

	return circleNodes;
}


std::vector<Element*> Quadtree::FindElementsInCircle(float x, float y, float radius)
{
	// Make three lists:
	// - List of Elements that fall inside of the circle
	// - List of leaves that fall completely inside of the circle
	// - List of leaves that fall partially inside of the circle
	//
	// Add all Elements that are in the leaves completely inside the circle list
	// to the list of Elements
	//
	// Search the list of leaves that fall partially inside of the circle
	// for Elements that fall inside of the circle and add them to the list of
	// Elements
	//
	// Return the list of Elements

	std::vector<Element*> circleElements;

	if (!hasElements)
		return circleElements;

	std::vector<leaf*> fullLeaves;
	std::vector<leaf*> partialLeaves;

	FindLeavesInCircle(x, y, radius, root, &fullLeaves, &partialLeaves);
	AddFullElements(&fullLeaves, &circleElements);
	AddPartialElements(x, y, radius, &partialLeaves, &circleElements);

	return circleElements;
}


std::vector<std::vector<Element*>*> Quadtree::GetElementsThroughDepth(int depth)
{
	std::vector< std::vector<Element*> *> elements;
	if (hasElements && depth >= 0)
	{
		RetrieveElements(root, depth, &elements);
	}
	return elements;
}


std::vector<std::vector<Element*>*> Quadtree::GetElementsThroughDepth(int depth, float l, float r, float b, float t)
{
	std::vector< std::vector<Element*> *> elements;
	if (hasElements && depth >= 0)
	{
		RetrieveElements(root, depth, &elements, l, r, b, t);
	}
	return elements;
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
 * @brief The recursive function that finds all of the leaves that fall within the circle
 * @param x The circle center x-coordinate
 * @param y The circle center y-coordinate
 * @param radius The circle radius
 * @param currBranch The branch to search
 * @param full Pointer to the vector of leaves that fall full within the circle
 * @param partial Pointer to the vector of leaves that fall partially within the circle
 */
void Quadtree::FindLeavesInCircle(float x, float y, float radius, branch *currBranch, std::vector<leaf *> *full, std::vector<leaf *> *partial)
{

	// Make a list of the x-y coordinates of all four corners of the current branch
	int cornersInsideCircle = 0;
	float corners[8] = {currBranch->bounds[0], currBranch->bounds[3],
			    currBranch->bounds[1], currBranch->bounds[3],
			    currBranch->bounds[0], currBranch->bounds[2],
			    currBranch->bounds[1], currBranch->bounds[2]};

	// See how many of the corners of the branch are inside of the circle
	for (int i=0; i<8; i+=2)
		if (pointIsInsideCircle(corners[i], corners[i+1], x, y, radius))
			cornersInsideCircle++;

	// If all four are inside, just add all of the leaves below this branch
	if (cornersInsideCircle == 4)
		AddAllLeaves(currBranch, full);

	// If some of them are inside, recurse
	else if (cornersInsideCircle > 0)
	{
		for (int i=0; i<4; i++)
		{
			// Recurse on branches
			if (currBranch->branches[i] != 0)
				FindLeavesInCircle(x, y, radius, currBranch->branches[i], full, partial);
			// Add leaves to the list of leaves that are partially inside of the circle
			if (currBranch->leaves[i] != 0)
				partial->push_back(currBranch->leaves[i]);	// Possible optimization here -- check leaf bounds
		}
	}

	// Do one last check to see if the circle intersects with the branch but not any of its corners
	else {
		bool cRight = pointIsInside(currBranch, x+radius, y);
		bool cLeft = pointIsInside(currBranch, x-radius, y);
		bool cTop = pointIsInside(currBranch, x, y+radius);
		bool cBottom = pointIsInside(currBranch, x, y-radius);

		// First check to see if the entire circle is inside of branch
		if (cRight || cLeft || cTop || cBottom)
		{
			for (int i=0; i<4; i++)
			{
				if (currBranch->branches[i] != 0)
					FindLeavesInCircle(x, y, radius, currBranch->branches[i], full, partial);
				if (currBranch->leaves[i] != 0)
					partial->push_back(currBranch->leaves[i]);
			}
		}
	}
}


/**
 * @brief Adds all leaves that fall below the given branch to the given list
 * @param currBranch The top branch containing all desired leaves
 * @param full The list of leaves to be appended to
 */
void Quadtree::AddAllLeaves(branch *currBranch, std::vector<leaf *> *full)
{
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
			full->push_back(currBranch->leaves[i]);
		if (currBranch->branches[i] != 0)
			AddAllLeaves(currBranch->branches[i], full);
	}
}


/**
 * @brief Adds all of the Nodes from the leaf list to the nodes list
 * @param full The list of leaves to get Nodes from
 * @param nodes The list of nodes to append Nodes to
 */
void Quadtree::AddFullNodes(std::vector<leaf *> *full, std::vector<Node *> *nodes)
{
	for (unsigned int i=0; i<full->size(); i++)
	{
		leaf *currLeaf = (*full)[i];
		for (unsigned int j=0; j<currLeaf->nodes.size(); j++)
		{
			nodes->push_back(currLeaf->nodes[j]);
		}
	}
}


/**
 * @brief Adds the Nodes that fall within the circle to the nodes list
 * @param x The circle center x-coordinate
 * @param y The circle center y-coordinate
 * @param radius The circle radius
 * @param partial The list of leaves that fall partially within the circle
 * @param nodes The list of nodes to append Nodes to
 */
void Quadtree::AddPartialNodes(float x, float y, float radius, std::vector<leaf *> *partial, std::vector<Node *> *nodes)
{
	for (unsigned int i=0; i<partial->size(); i++)
	{
		leaf *currLeaf = (*partial)[i];
		for (unsigned int j=0; j<currLeaf->nodes.size(); j++)
		{
			if (pointIsInsideCircle(currLeaf->nodes[j]->normX, currLeaf->nodes[j]->normY, x, y, radius))
			{
				nodes->push_back(currLeaf->nodes[j]);
			}
		}
	}
}


void Quadtree::AddFullElements(std::vector<leaf *> *full, std::vector<Element *> *elements)
{
	for (unsigned int i=0; i<full->size(); i++)
	{
		leaf *currLeaf = (*full)[i];
		for (unsigned int j=0; j<currLeaf->elements.size(); j++)
		{
			elements->push_back(currLeaf->elements[j]);
		}
	}
}


void Quadtree::AddPartialElements(float x, float y, float radius, std::vector<leaf *> *partial, std::vector<Element *> *elements)
{
	for (unsigned int i=0; i<partial->size(); i++)
	{
		leaf *currLeaf = (*partial)[i];
		for (unsigned int j=0; j<currLeaf->elements.size(); j++)
		{
			if (pointIsInsideCircle(currLeaf->elements[j]->n1->normX, currLeaf->elements[j]->n1->normY, x, y, radius) ||
			    pointIsInsideCircle(currLeaf->elements[j]->n2->normX, currLeaf->elements[j]->n2->normY, x, y, radius) ||
			    pointIsInsideCircle(currLeaf->elements[j]->n3->normX, currLeaf->elements[j]->n3->normY, x, y, radius))
				elements->push_back(currLeaf->elements[j]);

		}
	}
}


void Quadtree::RetrieveElements(branch *currBranch, int depth, std::vector< std::vector<Element*>* >*list)
{
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
		{
			list->push_back(&currBranch->leaves[i]->elements);
//			list->reserve(list->size()+ currBranch->leaves[i]->elements.size());
//			list->insert(list->end(), currBranch->leaves[i]->elements.begin(), currBranch->leaves[i]->elements.end());
		}
	}

	if (depth <= 0)
	{
		return;
	} else {
		depth--;
		for (int i=0; i<4; i++)
		{
			if (currBranch->branches[i] != 0)
			{
				RetrieveElements(currBranch->branches[i], depth, list);
			}
		}
	}
}


void Quadtree::RetrieveElements(branch *currBranch, int depth, std::vector<std::vector<Element *> *> *list, float l, float r, float b, float t)
{
	/* Check starting with root if branch intersects with window, searching recursively */
	/* Once an intersection has been found, retrieve elements through depth value */

	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
		{
			if (rectangleIntersection(currBranch->leaves[i], l, r, b, t))
			{
				list->push_back(&currBranch->leaves[i]->elements);
			} else {
				DEBUG("No leaf intersection");
			}
		} else {
			DEBUG("No leaf");
		}
	}

	if (depth <= 0)
	{
		return;
	} else {
		depth--;
		for (int i=0; i<4; i++)
		{
			if (currBranch->branches[i] != 0)
			{
				if (rectangleIntersection(currBranch->branches[i], l, r, b, t))
				{
					RetrieveElements(currBranch->branches[i], depth, list);
				} else {
					DEBUG("No branch intersection");
				}
			} else {
				DEBUG("No branch");
			}
		}
	}
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
	DEBUG("Error adding Node to Quadtree, node picking will not work for node number " << currNode->nodeNumber);
}


/**
 * @brief Adds an Element to the quadtree
 *
 * Adds an Element to the quadtree. An element is considered to be inside of a branch
 * or leaf if its node n1 is inside of the branch or leaf. Adding the Element is done
 * by recursively going down the tree based on n1 location.
 *
 * @param currElement The Element being added
 * @param currBranch The current branch we'd like to add the Element to
 */
void Quadtree::addElement(Element *currElement, branch *currBranch)
{
	// See if any of the Nodes fall into any of the branches
	for (int i=0; i<4; i++)
	{
		if (currBranch->branches[i] != 0)
		{
			if (nodeIsInside(currElement->n1, currBranch->branches[i]))
			{
				addElement(currElement, currBranch->branches[i]);
				return;
			}
		}
	}

	// See if any of the Nodes fall into any of the leaves
	for (int i=0; i<4; i++)
	{
		if (currBranch->leaves[i] != 0)
		{
			if (nodeIsInside(currElement->n1, currBranch->leaves[i]))
			{
				currBranch->leaves[i]->elements.push_back(currElement);
				return;
			}
		}
	}
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
	if (currNode->normX <= currLeaf->bounds[1] && currNode->normX >= currLeaf->bounds[0])
		if (currNode->normY <= currLeaf->bounds[3] && currNode->normY >= currLeaf->bounds[2])
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
	if (currNode->normX <= currBranch->bounds[1] && currNode->normX >= currBranch->bounds[0])
		if (currNode->normY <= currBranch->bounds[3] && currNode->normY >= currBranch->bounds[2])
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
	return sqrt(pow(y-currNode->normY, 2)+pow(x-currNode->normX, 2));
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


bool Quadtree::pointIsInside(float l, float r, float b, float t, float x, float y)
{
	if (l <= x && x <= r)
		if (b <= y && y <= t)
			return true;
	return false;
}


/**
 * @brief A helper function that determines if the point is inside of the circle
 *
 * A helper function that determines if the point is inside of the circle.
 *
 * @param x The point x-coordinate
 * @param y The point y-coordinate
 * @param circleX The circle center x-coordinate
 * @param circleY The circle center y-coordinate
 * @param radius The circle radius
 * @return
 */
bool Quadtree::pointIsInsideCircle(float x, float y, float circleX, float circleY, float radius)
{
	return (pow(x - circleX, 2.0) + (pow(y - circleY, 2.0))) < pow(radius, 2.0);
}


bool Quadtree::rectangleIntersection(leaf *currLeaf, float l, float r, float b, float t)
{
	if (pointIsInside(currLeaf, l, t) ||
	    pointIsInside(currLeaf, r, t) ||
	    pointIsInside(currLeaf, l, b) ||
	    pointIsInside(currLeaf, r, b) ||
	    pointIsInside(l, r, b, t, currLeaf->bounds[0], currLeaf->bounds[3]) ||
	    pointIsInside(l, r, b, t, currLeaf->bounds[0], currLeaf->bounds[2]) ||
	    pointIsInside(l, r, b, t, currLeaf->bounds[1], currLeaf->bounds[3]) ||
	    pointIsInside(l, r, b, t, currLeaf->bounds[1], currLeaf->bounds[2]))
		return true;
	return false;
}


bool Quadtree::rectangleIntersection(branch *currBranch, float l, float r, float b, float t)
{
	if (pointIsInside(currBranch, l, t) ||
	    pointIsInside(currBranch, r, t) ||
	    pointIsInside(currBranch, l, b) ||
	    pointIsInside(currBranch, r, b) ||
	    pointIsInside(l, r, b, t, currBranch->bounds[0], currBranch->bounds[3]) ||
	    pointIsInside(l, r, b, t, currBranch->bounds[0], currBranch->bounds[2]) ||
	    pointIsInside(l, r, b, t, currBranch->bounds[1], currBranch->bounds[3]) ||
	    pointIsInside(l, r, b, t, currBranch->bounds[1], currBranch->bounds[2]))
		return true;
	return false;
}
