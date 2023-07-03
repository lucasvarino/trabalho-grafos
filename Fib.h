// Struct used for each Fibonacci heap node
struct FibonacciNode
{
    int degree;
    FibonacciNode *parent;
    FibonacciNode *child;
    FibonacciNode *left;
    FibonacciNode *right;
    bool mark;
    float key;
    int nodeIndex;
};

// Fibonacci heap class
class MaxFibonacciHeap
{
private:
    FibonacciNode *maxNode; // Renomear minNode para maxNode
    int numNodes;
    vector<FibonacciNode *> degTable;
    vector<FibonacciNode *> nodePtrs;

public:
    MaxFibonacciHeap(int n)
    {
        // Constructor function
        this->numNodes = 0;
        this->maxNode = NULL; // Renomear minNode para maxNode
        this->degTable = {};
        this->nodePtrs.resize(n);
    }

    ~MaxFibonacciHeap()
    {
        // Destructor function
        this->numNodes = 0;
        this->maxNode = NULL; // Renomear minNode para maxNode
        this->degTable.clear();
        this->nodePtrs.clear();
    }

    int size()
    {
        // Number of nodes in the heap
        return this->numNodes;
    }

    bool empty()
    {
        // Is the heap empty?
        return this->numNodes == 0;
    }

    void insert(int u, float key)
    {
        // Insert the vertex u with the specified key (value for L(u)) into the Fibonacci heap. O(1) operation
        this->nodePtrs[u] = new FibonacciNode;
        this->nodePtrs[u]->nodeIndex = u;
        FibonacciNode *node = this->nodePtrs[u];
        node->key = key;
        node->degree = 0;
        node->parent = NULL;
        node->child = NULL;
        node->left = node;
        node->right = node;
        node->mark = false;
        FibonacciNode *maxN = this->maxNode; // Renomear minNode para maxNode
        if (maxN != NULL)
        {
            FibonacciNode *maxLeft = maxN->left; // Renomear minLeft para maxLeft
            maxN->left = node;
            node->right = maxN;
            node->left = maxLeft;
            maxLeft->right = node;
        }
        if (maxN == NULL || maxN->key < node->key)
        {                         // Inverter a comparação para < em vez de >
            this->maxNode = node; // Renomear minNode para maxNode
        }
        this->numNodes++;
    }

    FibonacciNode *extractMax()
    { // Renomear extractMin para extractMax
        // Extract the node with the maximum key from the heap. O(log n) operation, where n is the number of nodes in the heap
        FibonacciNode *maxN = this->maxNode; // Renomear minNode para maxNode
        if (maxN != NULL)
        {
            int deg = maxN->degree;
            FibonacciNode *currChild = maxN->child;
            FibonacciNode *remChild;
            for (int i = 0; i < deg; i++)
            {
                remChild = currChild;
                currChild = currChild->right;
                _existingToRoot(remChild);
            }
            _removeNodeFromRoot(maxN); // Renomear _removeNodeFromRoot(minN) para _removeNodeFromRoot(maxN)
            this->numNodes--;
            if (this->numNodes == 0)
            {
                this->maxNode = NULL; // Renomear minNode para maxNode
            }
            else
            {
                this->maxNode = maxN->right;          // Renomear minNode para maxNode
                FibonacciNode *maxNLeft = maxN->left; // Renomear minNLeft para maxNLeft
                this->maxNode->left = maxNLeft;       // Renomear minNode para maxNode
                maxNLeft->right = this->maxNode;      // Renomear minNLeft para maxNLeft
                _consolidate();
            }
        }
        return maxN; // Renomear minN para maxN
    }

    void increaseKey(int u, int newKey)
    { // Renomear decreaseKey para increaseKey
        // Increase the key of the node in the Fibonacci heap that has index u. O(1) operation
        FibonacciNode *node = this->nodePtrs[u];
        if (newKey < node->key)
        { // Inverter a comparação para < em vez de >
            return;
        }
        node->key = newKey;
        if (node->parent != NULL)
        {
            if (node->key > node->parent->key)
            { // Inverter a comparação para > em vez de <
                FibonacciNode *parentNode = node->parent;
                _cut(node);
                _cascadingCut(parentNode);
            }
        }
        if (node->key > this->maxNode->key)
        {                         // Inverter a comparação para > em vez de <
            this->maxNode = node; // Renomear minNode para maxNode
        }
    }

private:
    // The following are private functions used by the public methods above
    void _existingToRoot(FibonacciNode *newNode)
    {
        FibonacciNode *maxN = this->maxNode; // Renomear minNode para maxNode
        newNode->parent = NULL;
        newNode->mark = false;
        if (maxN != NULL)
        {
            FibonacciNode *maxLeft = maxN->left; // Renomear minLeft para maxLeft
            maxN->left = newNode;
            newNode->right = maxN;
            newNode->left = maxLeft;
            maxLeft->right = newNode;
            if (maxN->key < newNode->key)
            {                            // Inverter a comparação para < em vez de >
                this->maxNode = newNode; // Renomear minNode para maxNode
            }
        }
        else
        {
            this->maxNode = newNode; // Renomear minNode para maxNode
            newNode->right = newNode;
            newNode->left = newNode;
        }
    }

    void _removeNodeFromRoot(FibonacciNode *node)
    {
        if (node->right != node)
        {
            node->right->left = node->left;
            node->left->right = node->right;
        }
        if (node->parent != NULL)
        {
            if (node->parent->degree == 1)
            {
                node->parent->child = NULL;
            }
            else
            {
                node->parent->child = node->right;
            }
            node->parent->degree--;
        }
    }

    void _cut(FibonacciNode *node)
    {
        _removeNodeFromRoot(node);
        _existingToRoot(node);
    }

    void _addChild(FibonacciNode *parentNode, FibonacciNode *newChildNode)
    {
        if (parentNode->degree == 0)
        {
            parentNode->child = newChildNode;
            newChildNode->right = newChildNode;
            newChildNode->left = newChildNode;
            newChildNode->parent = parentNode;
        }
        else
        {
            FibonacciNode *child1 = parentNode->child;
            FibonacciNode *child1Left = child1->left;
            child1->left = newChildNode;
            newChildNode->right = child1;
            newChildNode->left = child1Left;
            child1Left->right = newChildNode;
        }
        newChildNode->parent = parentNode;
        parentNode->degree++;
    }

    void _cascadingCut(FibonacciNode *node)
    {
        FibonacciNode *parentNode = node->parent;
        if (parentNode != NULL)
        {
            if (node->mark == false)
            {
                node->mark = true;
            }
            else
            {
                _cut(node);
                _cascadingCut(parentNode);
            }
        }
    }

    void _link(FibonacciNode *highNode, FibonacciNode *lowNode)
    {
        _removeNodeFromRoot(highNode);
        _addChild(lowNode, highNode);
        highNode->mark = false;
    }

    void _consolidate()
    {
        int deg, rootCnt = 0;
        if (this->numNodes > 1)
        {
            this->degTable.clear();
            FibonacciNode *currNode = this->maxNode; // Renomear minNode para maxNode
            FibonacciNode *currDeg, *currConsolNode;
            FibonacciNode *temp = this->maxNode;   // Renomear minNode para maxNode
            FibonacciNode *itNode = this->maxNode; // Renomear minNode para maxNode
            do
            {
                rootCnt++;
                itNode = itNode->right;
            } while (itNode != temp);
            for (int cnt = 0; cnt < rootCnt; cnt++)
            {
                currConsolNode = currNode;
                currNode = currNode->right;
                deg = currConsolNode->degree;
                while (true)
                {
                    while (deg >= int(this->degTable.size()))
                    {
                        this->degTable.push_back(NULL);
                    }
                    if (this->degTable[deg] == NULL)
                    {
                        this->degTable[deg] = currConsolNode;
                        break;
                    }
                    else
                    {
                        currDeg = this->degTable[deg];
                        if (currConsolNode->key < currDeg->key)
                        { // Inverter a comparação para < em vez de >
                            swap(currConsolNode, currDeg);
                        }
                        if (currDeg == currConsolNode)
                        {
                            break;
                        }
                        _link(currDeg, currConsolNode);
                        this->degTable[deg] = NULL;
                        deg++;
                    }
                }
            }
            this->maxNode = NULL; // Renomear minNode para maxNode
            for (size_t i = 0; i < this->degTable.size(); i++)
            {
                if (this->degTable[i] != NULL)
                {
                    _existingToRoot(this->degTable[i]);
                }
            }
        }
    }
};
