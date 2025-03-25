#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_set>
#include <queue>
#include <stack>
#include <sstream>
using namespace std;


//current->addCHild(newNode) in bool allvisted = true shit
bool isInteger(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;  // If any character is not a digit, return false
        }
    }
    return true;  // All characters are digits, return true
}

bool validateInput(int argc, char* argv[]) 
{
    int fal = 0;
    if (argc != 7) {
        cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
        return false;
    }
    for(int i = 1; i <= 3; i++)
    {
        if (!isInteger(argv[i]) || stoi(argv[i]) <= 0)
        {
            cout << "Error: Invalid capacity '" << argv[i] << "' for jug " << static_cast<char>(64+i) << ".\n";
            fal++;
        }
    }
    for(int i = 4; i <=6; i++)
    {
        if (!isInteger(argv[i]) || stoi(argv[i]) < 0)
        {
            cout << "Error: Invalid goal '" << argv[i] << "' for jug " << static_cast<char>(64+(i-3)) << ".\n";
            fal++;
        }
    }
    if(fal>0) return false;

    int capA = stoi(argv[1]), capB = stoi(argv[2]), capC = stoi(argv[3]);
    int goalA = stoi(argv[4]), goalB = stoi(argv[5]), goalC = stoi(argv[6]);
    if (goalA > capA)
    {
        cout << "Error: Goal cannot exceed capacity of jug A.";
        return false;
    }
    else if (goalB > capB)
    {
        cout << "Error: Goal cannot exceed capacity of jug B.";
        return false;
    }
    else if (goalC > capC)
    {
        cout << "Error: Goal cannot exceed capacity of jug C.";
        return false;
    }
    else if (goalA + goalB + goalC > capC)
    {
        cout << "Error: Total gallons in goal state must be equal to the capacity of jug C.";
        return false;
    }
    else if(goalA + goalB + goalC < capC)
    {
        cout << "Error: Total gallons in goal state must be equal to the capacity of jug C.";
        return false;
    }
   return true;

}
struct Node {
    tuple<int, int, int> data;  // The data for each node is a tuple<int, int, int>
    Node* prev;  // Pointer to the previous node
    Node* next;  // Pointer to the next node
    vector<Node*> children;  // Vector to hold the children of the node
    string action;  // The action taken to reach this state

    // Constructor to initialize the node with a tuple and action
    Node(tuple<int, int, int> t, string a) : data(t), prev(nullptr), next(nullptr), action(a) {}

    // Add a child to the current node
    void addChild(Node* child) {
        children.push_back(child);
    }
    
    ~Node() {
        for (Node* child : children) {
            delete child;  // Recursively delete children
        }
    }
};

// Convert tuple to string for easy comparison in the visited set
string tupleToString(const tuple<int, int, int>& state) {
    stringstream ss;
    ss << get<0>(state) << "," << get<1>(state) << "," << get<2>(state);
    return ss.str();
}

// Check if a state has already been visited
bool isVisited(const unordered_set<string>& visited, const tuple<int, int, int>& state) {
    return visited.find(tupleToString(state)) != visited.end();
}

// Add a state to the visited set
void addVisited(unordered_set<string>& visited, const tuple<int, int, int>& state) {
    visited.insert(tupleToString(state));
}

// Get all valid neighboring states based on pouring between jugs
vector<tuple<int, int, int>> getNeighbors(const tuple<int, int, int>& state, int capA, int capB, int capC, vector<string>& actions) {
    vector<tuple<int, int, int>> neighbors;
    int a = get<0>(state), b = get<1>(state), c = get<2>(state);

    // Try all possible moves between the jugs

     // C -> A
     if (c > 0 && a < capA && c!=0) {
        int pour = min(c, capA - a);
        neighbors.push_back(make_tuple(a + pour, b, c - pour));
        // Check if it's 1 gallon or more
        string gallonString = (pour == 1) ? "gallon" : "gallons";
        actions.push_back("Pour " + to_string(pour) + " " + gallonString + " from C to A. (" + to_string(a + pour) + ", " + to_string(b) + ", " + to_string(c - pour) + ")");
    }

    // B -> A
    if (b > 0 && a < capA && b!=0) {
        int pour = min(b, capA - a);
        neighbors.push_back(make_tuple(a + pour, b - pour, c));
        // Check if it's 1 gallon or more
        string gallonString = (pour == 1) ? "gallon" : "gallons";
        actions.push_back("Pour " + to_string(pour) + " " + gallonString + " from B to A. (" + to_string(a + pour) + ", " + to_string(b - pour) + ", " + to_string(c) + ")");
    }

    // C -> B
    if (c > 0 && b < capB) {
        int pour = min(c, capB - b);
        neighbors.push_back(make_tuple(a, b + pour, c - pour));
        // Check if it's 1 gallon or more
        string gallonString = (pour == 1) ? "gallon" : "gallons";
        actions.push_back("Pour " + to_string(pour) + " " + gallonString + " from C to B. (" + to_string(a) + ", " + to_string(b + pour) + ", " + to_string(c - pour) + ")");
    }

    // A -> B
    if (a > 0 && b < capB) {
        int pour = min(a, capB - b);
        neighbors.push_back(make_tuple(a - pour, b + pour, c));
        // Check if it's 1 gallon or more
        string gallonString = (pour == 1) ? "gallon" : "gallons";
        actions.push_back("Pour " + to_string(pour) + " " + gallonString + " from A to B. (" + to_string(a - pour) + ", " + to_string(b + pour) + ", " + to_string(c) + ")");
    }

    // B -> C
    if (b > 0 && c < capC) {
        int pour = min(b, capC - c);
        neighbors.push_back(make_tuple(a, b - pour, c + pour));
        // Check if it's 1 gallon or more
        string gallonString = (pour == 1) ? "gallon" : "gallons";
        actions.push_back("Pour " + to_string(pour) + " " + gallonString + " from B to C. (" + to_string(a) + ", " + to_string(b - pour) + ", " + to_string(c + pour) + ")");
    }

    // A -> C
    if (a > 0 && c < capC) {
        int pour = min(a, capC - c);
        neighbors.push_back(make_tuple(a - pour, b, c + pour));
        // Check if it's 1 gallon or more
        string gallonString = (pour == 1) ? "gallon" : "gallons";
        actions.push_back("Pour " + to_string(pour) + " " + gallonString + " from A to C. (" + to_string(a - pour) + ", " + to_string(b) + ", " + to_string(c + pour) + ")");
    }

    return neighbors;
}


vector<Node*> nodes;
// Find the solution using BFS
int find(tuple<int, int, int> target, tuple<int, int, int> start, int capA, int capB, int capC, vector<string>& actions) {
    // Queue for BFS
    queue<Node*> q;
    unordered_set<string> visited;

    // Create the start node and add to visited
    Node* startNode = new Node(start, "Initial state. (" + to_string(get<0>(start)) + ", " + to_string(get<1>(start)) + ", " + to_string(get<2>(start)) + ")");
    nodes.push_back(startNode);
    q.push(startNode);
    addVisited(visited, start);
    
    while (!q.empty()) 
    {
        Node* current = q.front();
        q.pop();
        

        // If we reached the target state, reconstruct the path using a stack
        if (current->data == target) {
            stack<Node*> path;
            Node* temp = current;
            while (temp != nullptr) {
                path.push(temp);
                temp = temp->prev;
            }

            // Print the path from the start to the target in LIFO order
            while (!path.empty()) {
                cout << path.top()->action << endl;
                path.pop();
            }

            //delete startNode;
            //deleteAllNodes(startNode);
            return 0;
        }

        // Generate neighbors and explore them
        vector<string> actionsTemp;
        vector<tuple<int, int, int>> neighbors = getNeighbors(current->data, capA, capB, capC, actionsTemp);

        //bool allVisited = true;
        for (size_t i = 0; i < neighbors.size(); ++i) {
            if (!isVisited(visited, neighbors[i])) {
                // Create a new node for each valid state
                Node* newNode = new Node(neighbors[i], actionsTemp[i]);
                nodes.push_back(newNode);
                newNode->prev = current;
                q.push(newNode);
                addVisited(visited, neighbors[i]);
                //allVisited = false;
            }
        }

        actionsTemp.clear();
        neighbors.clear();
        
        //if (allVisited)
            //delete current;
    }

    // If no solution is found
    return -1;
}




int main(int argc, char* argv[]) 
{
    if(validateInput(argc, argv))
    {
        // Get the capacities and target state from the command line arguments
        int capA = stoi(argv[1]), capB = stoi(argv[2]), capC = stoi(argv[3]);
        int targetA = stoi(argv[4]), targetB = stoi(argv[5]), targetC = stoi(argv[6]);

        tuple<int, int, int> target = make_tuple(targetA, targetB, targetC);
        tuple<int, int, int> start = make_tuple(0, 0, capC);  // Start with Jug C full and the other two empty

        vector<string> actions;
        // Find the solution and print the actions taken
        if (find(target, start, capA, capB, capC, actions) == 0) 
        {
        // Solution found, actions printed during path reconstruction
        } 
        else 
            cout << "No solution." << endl;

        for(auto it : nodes)
        {
            delete it;
        }
        return 0;
    }

    return -1;
    
}
