#include <iostream>
#include <vector>

#define leftChild childPtr(false)
#define rightChild childPtr(true)

using namespace std;

const int infit = 1e9;
const long long infll = 1e18;
const double infdb = 1e14;

/*
Tree argument to change
Here we define more frequent options
*/
int mod = 998244353;
long long infval = infit;
bool modOp = false;

template <class S, class T>
inline auto addOp(S a, T b) // For different uses
{
    if(modOp)
        return (a + b) % mod;
    return a + b;
}

template <class S, class T>
inline auto mulOp(S a, T b)
{
    if(modOp)
        return a * b % mod;
    return a * b;
}

/*
The abstract class "Node" is for the base unit of some binary trees
It is designed to be a part of segment tree, binary tree or something else
*/
template <class T=int>
class Node
{
protected:
public:
    T valSum, valMin, valMax;
    T lazyAdd, lazyMul;
    Node() : valSum(0), valMax(-infval), valMin(infval),
                    lazyAdd(0), lazyMul(1) {}
    Node(T val) : valSum(val), valMax(val), valMin(val),
                    lazyAdd(0), lazyMul(1) {}
    virtual inline void initializeNode(T val) = 0;
    virtual inline Node* childPtr(bool r) = 0;
    virtual inline void createChild(bool r) = 0;
    virtual inline void refreshNode() = 0;
    //friend inline auto mulOp(auto, auto);
    //friend inline auto addOp(auto, auto);
};

/*
Class simpleNode is not allowed to be changed
The simpleNode "EndNode" is used to prevent something unexpected
*/
template <class T=int>
class imaginaryNode : public Node<T>
{
public:
    imaginaryNode() : Node<T>() {}
    imaginaryNode(T val) : Node<T>(val) {}
    inline void initializeNode(T val) {}
    inline Node<T>* childPtr(bool r) { return this; }
    inline void createChild(bool r) {}
    inline void refreshNode() {}
};
imaginaryNode<long long> nullLikeNode;
Node<long long> *EndNode = &nullLikeNode;

/*
Class realNode is the real unit in those trees
It is in charge of maintaining the information of subtrees
*/
template <class T=int>
class realNode : public Node<T>
{
protected:
    Node<T>* child[2];
public:
    realNode() : Node<T>() { child[0] = EndNode; child[1] = EndNode; }
    realNode(T val) : Node<T>(val) { child[0] = EndNode; child[1] = EndNode; }
    ~realNode();
    inline void initializeNode(T val) { Node<T>::valSum = Node<T>::valMin = Node<T>::valMax = val; }
    inline Node<T>* childPtr(bool r) { return child[r]; }
    inline void createChild(bool r) { child[r] = new realNode<T>; }
    inline void refreshNode();
};

template <class T>
realNode<T>::~realNode()
{
    if(child[0] != EndNode)
        delete child[0];
    if(child[1] != EndNode)
        delete child[1];
}

template <class T>
inline void realNode<T>::refreshNode()
{
    Node<T>::valSum = addOp(leftChild->valSum, rightChild->valSum);
    Node<T>::valMin = min(leftChild->valMin, rightChild->valMin);
    Node<T>::valMax = max(leftChild->valMax, rightChild->valMax);
}

template <class T=int>
class SegmentTree : public realNode<T>
{
private:
    Node<T>* treeRoot;
    int leftBorder, rightBorder, querylRange, queryrRange, updateVal;
    void buildfromArray(Node<T>*, int, int, const T*&);
    void buildfromVector(Node<T>*, int, int, const vector<T>&);
    void lazyPushdown(Node<T>*, int, int);
    void updateMul(Node<T>*, int, int);
    void updateAdd(Node<T>*, int, int);
    T querySum(Node<T>*, int, int);
    T queryMin(Node<T>*, int, int);
    T queryMax(Node<T>*, int, int);
public:
    SegmentTree() : treeRoot(EndNode), leftBorder(1), rightBorder(1),
                    querylRange(1), queryrRange(1), updateVal(0) {}
    SegmentTree(const T*, const T*); // Build tree using array
    SegmentTree(const vector<T>&); // Build tree using vector
    ~SegmentTree() { if(treeRoot!=EndNode) delete treeRoot; }
    void UpdateValue(int, int, T, int); // Integrated update function
    T QueryValue(int, int, int); // Integrated query function
};

template <class T>
SegmentTree<T>::SegmentTree(const T* BeginArray, const T* EndArray)
{
    treeRoot = new realNode<T>;
    querylRange = queryrRange = 1;
    updateVal = 0;
    leftBorder = 1;
    rightBorder = EndArray-BeginArray;
    buildfromArray(treeRoot, leftBorder, rightBorder, BeginArray);
}

template <class T>
SegmentTree<T>::SegmentTree(const vector<T>& FromVector)
{
    treeRoot = new Node<T>;
    querylRange = queryrRange = 1;
    updateVal = 0;
    leftBorder = 1;
    rightBorder = FromVector.size();
    buildfromArray(treeRoot, leftBorder, rightBorder, FromVector);
}

template <class T>
void SegmentTree<T>::buildfromArray(Node<T>* subtreeRoot, int leftRange, int rightRange, const T* &fromArray)
{
    if(leftRange == rightRange)
    {
        subtreeRoot->initializeNode(fromArray[leftRange-1]);
        // Notice here we used array[0...n), but range is [1...n]
        return;
    }
    int midRange = leftRange + rightRange >> 1;
    if(subtreeRoot->leftChild == EndNode)
        subtreeRoot->createChild(false);
    buildfromArray(subtreeRoot->leftChild, leftRange, midRange, fromArray);
    if(subtreeRoot->rightChild == EndNode)
        subtreeRoot->createChild(true);
    buildfromArray(subtreeRoot->rightChild, midRange+1, rightRange, fromArray);
    subtreeRoot->refreshNode();
}

template <class T>
void SegmentTree<T>::buildfromVector(Node<T>* subtreeRoot, int leftRange, int rightRange, const vector<T> &fromVector)
{
    if(leftRange == rightRange)
    {
        subtreeRoot->initializeNode(fromVector[leftRange-1]);
        // Notice here we used vector[0...n), but range is [1...n]
        return;
    }
    int midRange = leftRange + rightRange >> 1;
    if(subtreeRoot->leftChild == EndNode)
        subtreeRoot->createChild(false);
    buildfromArray(subtreeRoot->leftChild, leftRange, midRange, fromVector);
    if(subtreeRoot->rightChild == EndNode)
        subtreeRoot->createChild(true);
    buildfromArray(subtreeRoot->rightChild, midRange+1, rightRange, fromVector);
    subtreeRoot->refreshNode();
}

template <class T>
void SegmentTree<T>::lazyPushdown(Node<T>* subtreeRoot, int leftRange, int rightRange)
{
    int midRange = leftRange + rightRange >> 1;
    if(subtreeRoot->leftChild != EndNode)
    {
        Node<T>* sonRoot = subtreeRoot->leftChild;
        sonRoot->valSum = addOp(mulOp(sonRoot->valSum, subtreeRoot->lazyMul),
                        mulOp(subtreeRoot->lazyAdd, midRange-leftRange+1ll));
        if(sonRoot->valMax > -infval)
            sonRoot->valMax = addOp(mulOp(sonRoot->valMax, subtreeRoot->lazyMul), subtreeRoot->lazyAdd);
        if(sonRoot->valMin < infval)
            sonRoot->valMin = addOp(mulOp(sonRoot->valMin, subtreeRoot->lazyMul), subtreeRoot->lazyAdd);
        sonRoot->lazyMul = mulOp(sonRoot->lazyMul, subtreeRoot->lazyMul);
        sonRoot->lazyAdd = addOp(mulOp(sonRoot->lazyAdd, subtreeRoot->lazyMul), subtreeRoot->lazyAdd);
    }
    if(subtreeRoot->rightChild != EndNode)
    {
        Node<T>* sonRoot = subtreeRoot->rightChild;
        sonRoot->valSum = addOp(mulOp(sonRoot->valSum, subtreeRoot->lazyMul),
                        mulOp(subtreeRoot->lazyAdd, 1ll*rightRange-midRange));
        if(sonRoot->valMax > -infval)
            sonRoot->valMax = addOp(mulOp(sonRoot->valMax, subtreeRoot->lazyMul), subtreeRoot->lazyAdd);
        if(sonRoot->valMin < infval)
            sonRoot->valMin = addOp(mulOp(sonRoot->valMin, subtreeRoot->lazyMul), subtreeRoot->lazyAdd);
        sonRoot->lazyMul = mulOp(sonRoot->lazyMul, subtreeRoot->lazyMul);
        sonRoot->lazyAdd = addOp(mulOp(sonRoot->lazyAdd, subtreeRoot->lazyMul), subtreeRoot->lazyAdd);
    }
    subtreeRoot->lazyMul = 1;
    subtreeRoot->lazyAdd = 0;
}

template <class T>
void SegmentTree<T>::updateMul(Node<T>* subtreeRoot, int leftRange, int rightRange)
{
    if(leftRange > queryrRange || rightRange < querylRange) return;
    if(querylRange <= leftRange && rightRange <= queryrRange)
    {
        subtreeRoot->valSum = mulOp(subtreeRoot->valSum, updateVal);
        if(subtreeRoot->valMax > -infval)
            subtreeRoot->valMax = mulOp(subtreeRoot->valMax, updateVal);
        if(subtreeRoot->valMin < infval)
            subtreeRoot->valMin = mulOp(subtreeRoot->valMin, updateVal);
        subtreeRoot->lazyMul = mulOp(subtreeRoot->lazyMul, updateVal);
        subtreeRoot->lazyAdd = mulOp(subtreeRoot->lazyAdd, updateVal);
        return;
    }
    int midRange = leftRange + rightRange >> 1;
    lazyPushdown(subtreeRoot, leftRange, rightRange);
    updateMul(subtreeRoot->leftChild, leftRange, midRange);
    updateMul(subtreeRoot->rightChild, midRange+1, rightRange);
    subtreeRoot->refreshNode();
}

template <class T>
void SegmentTree<T>::updateAdd(Node<T>* subtreeRoot, int leftRange, int rightRange)
{
    if(leftRange > queryrRange || rightRange < querylRange) return;
    if(querylRange <= leftRange && rightRange <= queryrRange)
    {
        subtreeRoot->valSum = addOp(subtreeRoot->valSum, mulOp(updateVal, rightRange-leftRange+1ll));
        if(subtreeRoot->valMax > -infval)
            subtreeRoot->valMax = addOp(subtreeRoot->valMax, mulOp(updateVal, rightRange-leftRange+1ll));
        if(subtreeRoot->valMin < infval)
            subtreeRoot->valMin = addOp(subtreeRoot->valMin, mulOp(updateVal, rightRange-leftRange+1ll));
        subtreeRoot->lazyAdd = addOp(subtreeRoot->lazyAdd, updateVal);
        return;
    }
    int midRange = leftRange + rightRange >> 1;
    lazyPushdown(subtreeRoot, leftRange, rightRange);
    updateAdd(subtreeRoot->leftChild, leftRange, midRange);
    updateAdd(subtreeRoot->rightChild, midRange+1, rightRange);
    subtreeRoot->refreshNode();
}

template <class T>
T SegmentTree<T>::querySum(Node<T>* subtreeRoot, int leftRange, int rightRange)
{
    if(leftRange > queryrRange || rightRange < querylRange)
        return 0;
    if(querylRange <= leftRange && rightRange <= queryrRange)
        return subtreeRoot->valSum;
    int midRange = leftRange + rightRange >> 1;
    lazyPushdown(subtreeRoot, leftRange, rightRange);
    return addOp(querySum(subtreeRoot->leftChild, leftRange, midRange),
            querySum(subtreeRoot->rightChild, midRange+1, rightRange));
}

template <class T>
T SegmentTree<T>::queryMin(Node<T>* subtreeRoot, int leftRange, int rightRange)
{
    if(leftRange > queryrRange || rightRange < querylRange)
        return infval;
    if(querylRange <= leftRange && rightRange <= queryrRange)
        return subtreeRoot->valMin;
    int midRange = leftRange + rightRange >> 1;
    lazyPushdown(subtreeRoot, leftRange, rightRange);
    return min(queryMin(subtreeRoot->leftChild, leftRange, midRange),
            queryMin(subtreeRoot->rightChild, midRange+1, rightRange));
}

template <class T>
T SegmentTree<T>::queryMax(Node<T>* subtreeRoot, int leftRange, int rightRange)
{
    if(leftRange > queryrRange || rightRange < querylRange)
        return -infval;
    if(querylRange <= leftRange && rightRange <= queryrRange)
        return subtreeRoot->valMax;
    int midRange = leftRange + rightRange >> 1;
    lazyPushdown(subtreeRoot, leftRange, rightRange);
    return max(queryMax(subtreeRoot->leftChild, leftRange, midRange),
            queryMax(subtreeRoot->rightChild, midRange+1, rightRange));
}

template <class T>
void SegmentTree<T>::UpdateValue(int RangeLeft, int RangeRight, T ArgValue, int Option)
{
    if(RangeLeft < leftBorder || RangeRight > rightBorder || RangeLeft > RangeRight)
        return; // Request resist
    querylRange = RangeLeft;
    queryrRange = RangeRight;
    updateVal = ArgValue;
    switch(Option)
    {
        case 1:
            updateAdd(treeRoot, leftBorder, rightBorder);
            break;
        case 2:
            updateMul(treeRoot, leftBorder, rightBorder);
            break;
    }
}

template <class T>
T SegmentTree<T>::QueryValue(int RangeLeft, int RangeRight, int Option)
{
    if(RangeLeft < leftBorder || RangeRight > rightBorder || RangeLeft > RangeRight)
        return infval+1; // Request resist
    querylRange = RangeLeft;
    queryrRange = RangeRight;
    switch(Option)
    {
        case 1:
            return querySum(treeRoot, leftBorder, rightBorder);
        case 2:
            return queryMin(treeRoot, leftBorder, rightBorder);
        case 3:
            return queryMax(treeRoot, leftBorder, rightBorder);
    }
    return infval+2; // Error query
}

long long a[100005];

int main()
{
    int n, m;
    scanf("%d%d%*d", &n, &m);
    for(int i = 1; i <= n; i += 1)
        scanf("%lld", &a[i]);
    SegmentTree<long long> Seg1(a+1, a+n+1);
    int op, x, y;
    long long z;
    for(int i = 1; i <= m; i += 1)
    {
        scanf("%d", &op);
        if(op == 1)
        {
            scanf("%d%d%lld", &x, &y, &z);
            //updateMul(rootSeg, 1, n, x, y, z);
            Seg1.UpdateValue(x, y, z, 2);
        }
        else if(op == 2)
        {
            scanf("%d%d%lld", &x, &y, &z);
            Seg1.UpdateValue(x, y, z, 1);
        }
        else
        {
            scanf("%d%d", &x, &y);
            printf("%lld\n", Seg1.QueryValue(x, y, 1));
        }
    }
    return 0;
}
