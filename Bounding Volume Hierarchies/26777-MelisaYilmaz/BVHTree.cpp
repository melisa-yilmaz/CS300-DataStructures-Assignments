//Melisa Yilmaz 26777
#include "BVHTree.h"
#include <stack>

//constructor
BVHTree::BVHTree()
{
    root = nullptr;
    std::unordered_map<std::string, BVHTreeNode *> m;
    map = m;
}

//destuctor
BVHTree::~BVHTree()
{
    makeEmpty(root);
}

//make empty all the nodes in the tree
void BVHTree::makeEmpty(BVHTreeNode *&node)
{
    if (node == nullptr) {
        return;
    }
    
    makeEmpty(node->leftChild);
    makeEmpty(node->rightChild);
    
    delete node;
    node = nullptr;
}

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level)
{
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}

BVHTreeNode* findExistingLeaf(BVHTreeNode *branchNode,BVHTreeNode *newNode)
{
    //start the root to search t
    while(branchNode->isLeaf==false)
    {
        int increaseInRightTreeSize = AABB::unionArea(newNode->aabb,branchNode->rightChild->aabb)- branchNode->rightChild->aabb.getArea();
        int increaseInLeftTreeSize = AABB::unionArea(newNode->aabb,branchNode->leftChild->aabb) - branchNode->leftChild->aabb.getArea();
        if (increaseInRightTreeSize < increaseInLeftTreeSize)
            branchNode= branchNode->rightChild;
        else
            branchNode = branchNode->leftChild;
 
    }
    return branchNode;
   

    
}
void updateTree(BVHTreeNode *&branchNode)
{
    BVHTreeNode *parentPtr = branchNode->parent;
    while(parentPtr != 0)
    {
        parentPtr->aabb = parentPtr->rightChild->aabb + parentPtr->leftChild->aabb;
        parentPtr = parentPtr->parent;
    }
}

//add a node to the tree
void BVHTree::addBVHMember(AABB objectArea, std::string name)
{
   
    //if tree is empty
        //When the node is added, it becomes the root of the tree, and the hashmap “map” will map its name to its address.
    if (map.empty())
    {
        BVHTreeNode*branchNode = new BVHTreeNode(objectArea,name,true);
        root = branchNode;
        map[name]=branchNode;
    
    }
    //there is exactly one node in the tree
    else if (map.size() == 1)
    {
        BVHTreeNode *oldRoot = root;
        BVHTreeNode *newNode = new BVHTreeNode(objectArea,name,true);
        map[name] = newNode;
        
        BVHTreeNode *branchNode =  new BVHTreeNode(oldRoot->aabb + newNode->aabb,"branch",false);
        root = branchNode;
        branchNode->parent = 0;
        
        branchNode->leftChild = newNode;
        branchNode->rightChild = oldRoot;
        newNode->parent=branchNode;
        oldRoot->parent = branchNode;
    }
    //there is more than one node in the tree
    else
    {
        BVHTreeNode *branchNode = root;
        
        BVHTreeNode *newNode = new BVHTreeNode(objectArea,name,true);
        map[name] = newNode;
    
       
        BVHTreeNode *existingLeaf = findExistingLeaf(branchNode,newNode);
        BVHTreeNode *newBranchNode = new BVHTreeNode(existingLeaf->aabb + newNode->aabb,"branch",false);
        
        newBranchNode->leftChild = newNode;
        newBranchNode->rightChild = existingLeaf;
        
        if(existingLeaf->parent->rightChild == existingLeaf)
            existingLeaf->parent->rightChild = newBranchNode;

        else if (existingLeaf->parent->leftChild == existingLeaf)
            existingLeaf->parent->leftChild = newBranchNode;
   
        
        newBranchNode->parent = existingLeaf->parent;
        existingLeaf->parent = newBranchNode;
        newNode->parent=newBranchNode;

        updateTree(newBranchNode);

    }
}

//change/move the location of the node inside of the tree
void BVHTree::moveBVHMember(std::string name, AABB newLocation)
{
    BVHTreeNode *movingNode;
    movingNode = map[name];
    
    BVHTreeNode *newNodeParent;
    newNodeParent=movingNode->parent;
    if(newNodeParent==0)
    {
        movingNode->aabb = newLocation;
    }
    else
    {
        if(newNodeParent->leftChild == movingNode)
        {
            AABB parentUnionArea =newNodeParent->leftChild->aabb + newNodeParent->rightChild->aabb;
            AABB newUnionArea = newLocation + newNodeParent->rightChild->aabb;
            //if moving the box does not make it outside of its parent
            if (parentUnionArea != newUnionArea)
            {
                removeBVHMember(movingNode->name);
                addBVHMember(newLocation, name);
            }
            //if moving the box will make it outside of its parent
            else
            {
                newNodeParent->leftChild->aabb = newLocation;
            }
        }
        else if (newNodeParent->rightChild == movingNode)
        {
            AABB parentUnionArea =newNodeParent->leftChild->aabb + newNodeParent->rightChild->aabb;
            AABB newUnionArea = newLocation + newNodeParent->leftChild->aabb;
            
            if (parentUnionArea != newUnionArea)
            {
                removeBVHMember(movingNode->name);
                addBVHMember(newLocation, name);
              
            }
            //if moving the box will make it outside of its parent
            else
            {
                newNodeParent->rightChild->aabb = newLocation;
            }
        }
        
    }
   
}


//remove the giving name of the node from the tree
void BVHTree::removeBVHMember(std::string name)
{
    //go to hash map and find the location of the node that we want to remove
    BVHTreeNode *toRemove;
    toRemove = map[name];
    //erase the node from the unordered map
    map.erase(name);
    
    BVHTreeNode *branchNode;
    branchNode = toRemove->parent;
    BVHTreeNode *branchNodeParent = branchNode->parent;

    //if the removing node is the left child of its parent, sibling of it will be the right side of the its parent
    if(branchNode->leftChild==toRemove)
    {
        
        BVHTreeNode *sibling = branchNode->rightChild;
        //make the sibling parent grand-parent of the removing node
        sibling->parent = branchNodeParent;
        
        if(branchNodeParent->leftChild==branchNode)
            branchNodeParent->leftChild = sibling;
        else
        {
            sibling->parent = branchNodeParent;
            branchNodeParent->rightChild = sibling;
        }
        updateTree(sibling);
    }
    else
    {
        BVHTreeNode *sibling = branchNode->leftChild;
        BVHTreeNode *branchNodeParent = branchNode->parent;
        if(branchNodeParent->leftChild==branchNode)
        {
            sibling->parent = branchNodeParent;
            branchNodeParent->leftChild = sibling;
        }
        else
        {
            sibling->parent = branchNodeParent;
            branchNodeParent->rightChild = sibling;
        }
        updateTree(sibling);
    }

   
    delete toRemove;
    toRemove = nullptr;
 
    
    
}
std::vector<std::string> Collide( std::vector<std::string> &collidingVector,BVHTreeNode *branchNode,AABB object)
{
    // with inorder traversal method search all node of the tree and check them if they collide with given object or node
    // if they collide with the given object add them to the vector
    if(branchNode->aabb.collide(object) == false)
        return collidingVector;
    if(branchNode->leftChild==0)
    {
        collidingVector.push_back(branchNode->name);
        return collidingVector;
    }
    Collide(collidingVector, branchNode->leftChild,object);
    Collide(collidingVector, branchNode->rightChild,object);
    return collidingVector;

    
}
//add all the colliding objects to the newly created vector and return it
std::vector<std::string> BVHTree::getCollidingObjects(AABB object)
{
    std::vector<std::string> collidingVector;
    BVHTreeNode *branchNode = root;
    Collide(collidingVector, branchNode, object);
    
    return collidingVector;
}


std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
    tree.printNode(out, tree.root, 0);
    return out;
}
