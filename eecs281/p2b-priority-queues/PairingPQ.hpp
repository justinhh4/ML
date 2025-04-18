// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include <deque>
#include <utility>

#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
    public:
        // Description: Custom constructor that creates a node containing
        //              the given value.
        explicit Node(const TYPE &val)
            : elt { val } {}

        // Description: Allows access to the element at that Node's position.
        //              There are two versions, getElt() and a dereference
        //              operator, use whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        // The following line allows you to access any private data
        // members of this Node class from within the PairingPQ class.
        // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
        // function).
        friend PairingPQ;

    private:
        TYPE elt;
        Node *child = nullptr;
        Node *sibling = nullptr;
        Node *previous = nullptr; 
        // TODO: Add and initialize one extra pointer (parent or previous) as desired.
    };  // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp }, root(nullptr), num_nodes(0) {
        // TODO: Implement this function.
    }  // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp }, root(nullptr), num_nodes(0) {
        // TODO: Implement this function.
        while (start != end){
            push(*start); 
            start++; 
        }
    }  // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other)
        : BaseClass { other.compare }, root(nullptr), num_nodes(0) {
        // TODO: Implement this function.
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
        if (other.root == nullptr) return; 
        std::deque<Node*> nodes; 
        nodes.push_back(other.root);
        while (!nodes.empty()){
            Node* temp = nodes.front(); 
            nodes.pop_front(); 
            push(temp->elt); 
            if (temp->child != nullptr){
                nodes.push_back(temp->child); 
            }
            if (temp->sibling != nullptr){
                nodes.push_back(temp->sibling); 
            }
        }
    }  // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        // lecture.
        PairingPQ copy(rhs); 
        Node * temp = root; 
        root = copy.root;
        num_nodes = copy.num_nodes; 
        copy.root = temp; 
        return *this; 
    }  // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        if (root == nullptr) return; 
        std::deque<Node*> nodes; 
        nodes.push_back(root);
        while (!nodes.empty()){
            Node* temp = nodes.front(); 
            nodes.pop_front(); 
            if (temp->child != nullptr){
                nodes.push_back(temp->child); 
            }
            if (temp->sibling != nullptr){
                nodes.push_back(temp->sibling); 
            }
            delete temp; 
        }        
    }  // ~PairingPQ()


    // Description: Move constructor and assignment operators don't need any
    //              code, the members will be reused automatically.
    PairingPQ(PairingPQ &&) noexcept = default;
    PairingPQ &operator=(PairingPQ &&) noexcept = default;


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        if (root == nullptr) return; 
        std::deque<Node*> nodes; 
        if (root->child != nullptr){
            nodes.push_back(root->child); 
        }
        if (root->sibling != nullptr){
            nodes.push_back(root->sibling); 
        }
        root->sibling = nullptr; 
        root->child = nullptr; 
        while (!nodes.empty()){
            Node* temp = nodes.front(); 
            nodes.pop_front(); 
            if (temp->child != nullptr){
                nodes.push_back(temp->child); 
            }
            if (temp->sibling != nullptr){
                nodes.push_back(temp->sibling); 
            }
            temp->previous = nullptr; 
            temp->child = nullptr;
            temp->sibling = nullptr; 
            root = meld(root, temp);             
        }        
        // TODO: Implement this function.
    }  // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) { addNode(val); }  // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        num_nodes--; 
        Node* temp = root; 
        root = root->child; 
        delete temp; 
        if (root == nullptr){
            return; 
        }
        root->previous = nullptr; 
        std::deque<Node*> nodes; 
        temp = root; 
        while (temp != nullptr){
            nodes.push_back(temp);
            temp = temp->sibling; 
        }
        while (nodes.size() != 1){
            Node * node1 = nodes.front();
            nodes.pop_front(); 
            Node * node2 = nodes.front();
            nodes.pop_front(); 
            node1->previous = nullptr; 
            node1->sibling = nullptr; 
            node2->previous = nullptr; 
            node2->sibling = nullptr; 
            node1 = meld(node1, node2); 
            nodes.push_back(node1); 
        }
        root = nodes.front(); 

    }  // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function
        return root->elt; 
    }  // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    [[nodiscard]] virtual std::size_t size() const {
        // TODO: Implement this function
        return num_nodes; 
    }  // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    [[nodiscard]] virtual bool empty() const {
        // TODO: Implement this function
        return num_nodes == 0; 
    }  // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node *node, const TYPE &new_value) {
        // TODO: Implement this function
        node->elt = new_value; 
        if (node->previous == nullptr){
            return; 
        }
        else if (node->sibling == nullptr && node->previous->sibling != node){
            node->previous->child = nullptr; 
            node->previous = nullptr; 
            root = meld(root, node); 
        }
        else if (node->sibling == nullptr && node->previous->sibling == node){
            node->previous->sibling = nullptr; 
            node->previous = nullptr; 
            root = meld(root, node); 
        }
        else if (node->sibling != nullptr && node->previous->sibling != node){
            node->sibling->previous = node->previous; 
            node->previous->child = node->sibling; 
            node->sibling = nullptr; 
            node->previous = nullptr; 
            root = meld(root, node); 
        }
        else {
            node->sibling->previous = node->previous; 
            node->previous->sibling = node->sibling; 
            node->previous = nullptr; 
            node->sibling = nullptr; 
            root = meld(root, node); 
        }
    }  // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node *addNode(const TYPE &val) {
        // TODO: Implement this function
        Node* new_node = new Node(val); 
        root = meld(root, new_node);
        num_nodes++; 
        return new_node; 
    }  // addNode()


private:
    // TODO: Add any additional member variables or member functions you
    // require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    // papers).

    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
    Node* root; 
    size_t num_nodes; 

    Node* meld(Node* r1, Node* r2){
        if (r1 == nullptr) return r2; 
        if (r2 == nullptr) return r1; 
        if (this->compare(r1->elt, r2->elt)){
            r1->previous = r2; 
            if (r2->child != nullptr){
                Node* temp = r2->child; 
                r2->child = r1; 
                r1->sibling = temp; 
                temp->previous = r1; 
            }
            else {
                r2->child = r1; 
            }
            return r2; 
        }
        else {
            r2->previous = r1; 
            if (r1->child != nullptr){
                Node* temp = r1->child; 
                r1->child = r2; 
                r2->sibling = temp; 
                temp->previous = r2; 
            }
            else {
                r1->child = r2; 
            }
            return r1;
        }
    }
};


#endif  // PAIRINGPQ_H
