#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <string>

using namespace std;

int dimension, upLimit, downLimit, d8, d15, l1, l2, l3, l4, r1, r2, r3, r4;                  //This was the most straightforward way to implement the limits of the puzzle 

void setDimensionInfo(int length){
    dimension=length;
    if(dimension == 3){
        upLimit=2;
        downLimit=6;
        l1=0;
        l2=3;
        l3=6;
        l4=-99;
        r1=2;
        r2=5;
        r3=8;
        r4=-99;
    }
    else{
        upLimit=3;
        downLimit=12;
        l1=0;
        l2=4;
        l3=8;
        l4=12;
        r1=3;
        r2=7;
        r3=11;
        r4=15;
    }
}

struct nodule{
    string state;
    char prev;
};

struct noduleWithIntData{                                       //Int data used for either heuristics or depth
    int data;
    string state;
    char prev;
    bool operator<(const noduleWithIntData &rhs) const{       //this allows this program to use a priority queue for this structure
        return data>rhs.data;
    }
};

string swap(string input, int index1, int index2){
    string ret=input;
    ret.at(index1) = input.at(index2);
    ret.at(index2) = input.at(index1);
    return ret;
}

bool isVisited(vector<nodule> &visited, string node){
    for(int i=0; i<visited.size(); ++i)
        if(visited.at(i).state == node)
            return true;
    return false;
}

int isVisited(vector<noduleWithIntData> &visited, string node){
    for(int i=0; i<visited.size(); ++i)
        if(visited.at(i).state == node)
            return true;
    return false;
}

int isVisitedAStar(vector<noduleWithIntData> &visited, string node){
    for(int i=0; i<visited.size(); ++i)
        if(visited.at(i).state == node)
            return i;
    return -1;
}

int manhattanDistance(string input, string goal){
    int h=0;
    int inputT, goalT;
    for(int i=1; i<input.size(); ++i){
        inputT=input.find('0'+i);
        goalT=goal.find('0'+i);
        h+=(abs((inputT%dimension)-(goalT%dimension)) + abs((inputT/dimension)-(goalT/dimension)));
    }
    return h;
}

int outOfPlace(string input, string goal){
    int h=0;
    for(int i=0; i<input.size(); ++i)
        if(input.at(i) != '0')
            if(input.at(i) != goal.at(i))
                h++;
    return h;
}

void constructSolution(vector<nodule> &visited, nodule final, string initial){
    string solution;
    nodule tracker=final;
    int moveIndex;
    
    while(tracker.state != initial){
        moveIndex=tracker.state.find('0');
        if(tracker.prev=='u'){
            solution.push_back('u');
            tracker.state=swap(tracker.state, moveIndex, moveIndex+dimension);
        }
        else if(tracker.prev=='d'){
            solution.push_back('d');
            tracker.state=swap(tracker.state, moveIndex, moveIndex-dimension);
        }
        else if(tracker.prev=='l'){
            solution.push_back('l');
            tracker.state=swap(tracker.state, moveIndex, moveIndex+1);
        }
        else{
            solution.push_back('r');
            tracker.state=swap(tracker.state, moveIndex, moveIndex-1);
        }
        for(int i=0; i<visited.size(); ++i)
            if(visited.at(i).state == tracker.state)
                tracker=visited.at(i);
    }
    cout<<endl<<"Solution: "<<endl;
    for(int i=solution.size()-1; i>=0; --i)
        cout<<solution.at(i);
}

void constructSolutionIterative(vector<noduleWithIntData> &visited, noduleWithIntData final, string initial){
    string solution;
    noduleWithIntData tracker=final;
    int moveIndex;
    
    while(tracker.state != initial){
        moveIndex=tracker.state.find('0');
        if(tracker.prev=='u'){
            solution.push_back('u');
            tracker.state=swap(tracker.state, moveIndex, moveIndex+dimension);
        }
        else if(tracker.prev=='d'){
            solution.push_back('d');
            tracker.state=swap(tracker.state, moveIndex, moveIndex-dimension);
        }
        else if(tracker.prev=='l'){
            solution.push_back('l');
            tracker.state=swap(tracker.state, moveIndex, moveIndex+1);
        }
        else{
            solution.push_back('r');
            tracker.state=swap(tracker.state, moveIndex, moveIndex-1);
        }
        for(int i=0; i<visited.size(); ++i)
            if(visited.at(i).state == tracker.state)
                tracker=visited.at(i);
    }
    cout<<endl<<"Solution: "<<endl;
    for(int i=solution.size()-1; i>=0; --i)
        cout<<solution.at(i);
}

void constructSolutionAStar(vector<noduleWithIntData> &visited, noduleWithIntData finalIndex, string initial){
    string solution;
    noduleWithIntData tracker=finalIndex;
    int moveIndex;
    
    while(tracker.state != initial){
        moveIndex=tracker.state.find('0');
        if(tracker.prev=='u'){
            solution.push_back('u');
            tracker.state=swap(tracker.state, moveIndex, moveIndex+dimension);
        }
        else if(tracker.prev=='d'){
            solution.push_back('d');
            tracker.state=swap(tracker.state, moveIndex, moveIndex-dimension);
        }
        else if(tracker.prev=='l'){
            solution.push_back('l');
            tracker.state=swap(tracker.state, moveIndex, moveIndex+1);
        }
        else{
            solution.push_back('r');
            tracker.state=swap(tracker.state, moveIndex, moveIndex-1);
        }
        for(int i=0; i<visited.size(); ++i)
            if(visited.at(i).state == tracker.state)
                tracker=visited.at(i);
    }
    cout<<endl<<"Solution: "<<endl;
    for(int i=solution.size()-1; i>=0; --i)
        cout<<solution.at(i);
}

void moveOptionsQueue(vector<nodule> &visited, nodule &node, queue<nodule> &q){
    visited.push_back(node);
    nodule temp;

    int moveIndex = node.state.find('0');                       //used for checking if moving up is possible
    if(moveIndex > upLimit){                                          //pushes state from moving up into queue
        temp.state = swap(node.state,moveIndex,moveIndex-dimension);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'u';
            q.push(temp);
        }
    }
    if(moveIndex < downLimit){                                          //move down
        temp.state = swap(node.state,moveIndex,moveIndex+dimension);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'd';
            q.push(temp);
        }
    }
    if(moveIndex != l1 && moveIndex != l2 && moveIndex != l3 && moveIndex != l4){     //move left
        temp.state = swap(node.state,moveIndex,moveIndex-1);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'l';
            q.push(temp);
        }
    }
    if(moveIndex != r1 && moveIndex != r2 && moveIndex != r3 && moveIndex != r4){     //move right
        temp.state = swap(node.state,moveIndex,moveIndex+1);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'r';
            q.push(temp);
        }
    }    
}

void moveOptionsStack(vector<nodule> &visited, nodule &node, stack<nodule> &stk){
    visited.push_back(node);
    nodule temp;

    int moveIndex = node.state.find('0');                       //used for checking if moving up is possible
    if(moveIndex > upLimit){                                          //pushes state from moving up into queue
        temp.state = swap(node.state,moveIndex,moveIndex-dimension);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'u';
            stk.push(temp);
        }
    }
    if(moveIndex < downLimit){                                          //move down
        temp.state = swap(node.state,moveIndex,moveIndex+dimension);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'd';
            stk.push(temp);
        }
    }
    if(moveIndex != l1 && moveIndex != l2 && moveIndex != l3 && moveIndex != l4){     //move left
        temp.state = swap(node.state,moveIndex,moveIndex-1);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'l';
            stk.push(temp);
        }
    }
    if(moveIndex != r1 && moveIndex != r2 && moveIndex != r3 && moveIndex != r4){     //move right
        temp.state = swap(node.state,moveIndex,moveIndex+1);
        if(!isVisited(visited,temp.state)){                     //if new state has not been visited
            temp.prev = 'r';
            stk.push(temp);
        }
    }  
}

void moveOptionsPQ(vector<noduleWithIntData> &visited, noduleWithIntData &node, priority_queue<noduleWithIntData> &pq, string goal, int prevIndex){
    visited.push_back(node);
    noduleWithIntData temp;

    int moveIndex = node.state.find('0');                       //used for checking if moving up is possible
    if(moveIndex > upLimit){                                          //pushes state from moving up into queue
        temp.state = swap(node.state,moveIndex,moveIndex-dimension);
        temp.prev = 'u';
        temp.data=manhattanDistance(temp.state,goal)+outOfPlace(temp.state,goal)+node.data;
        prevIndex=isVisitedAStar(visited,temp.state);
        if(prevIndex == -1)                                     //if new state has not been visited
            pq.push(temp);
        else
            if(temp.data < visited.at(prevIndex).data){
                pq.push(temp);
                visited.at(prevIndex)=temp;
            }
    }
    if(moveIndex < downLimit){                                          //move down
        temp.state = swap(node.state,moveIndex,moveIndex+dimension);
        temp.prev = 'd';
        temp.data=manhattanDistance(temp.state,goal)+outOfPlace(temp.state,goal)+node.data;
        prevIndex=isVisitedAStar(visited,temp.state);
        if(prevIndex == -1)                                     //if new state has not been visited
            pq.push(temp);
        else
            if(temp.data < visited.at(prevIndex).data){
                pq.push(temp);
                visited.at(prevIndex)=temp;
            }
    }
    if(moveIndex != l1 && moveIndex != l2 && moveIndex != l3 && moveIndex != l4){     //move left
        temp.state = swap(node.state,moveIndex,moveIndex-1);
        temp.prev = 'l';
        temp.data=manhattanDistance(temp.state,goal)+outOfPlace(temp.state,goal)+node.data;
        prevIndex=isVisitedAStar(visited,temp.state);
        if(prevIndex == -1)                                     //if new state has not been visited
            pq.push(temp);
        else
            if(temp.data < visited.at(prevIndex).data){
                pq.push(temp);
                visited.at(prevIndex)=temp;
            }
    }
    if(moveIndex != r1 && moveIndex != r2 && moveIndex != r3 && moveIndex != r4){     //move right
        temp.state = swap(node.state,moveIndex,moveIndex+1);
        temp.prev = 'r';
        temp.data=manhattanDistance(temp.state,goal)+outOfPlace(temp.state,goal)+node.data;
        prevIndex=isVisitedAStar(visited,temp.state);
        if(prevIndex == -1)                                     //if new state has not been visited
            pq.push(temp);
        else
            if(temp.data < visited.at(prevIndex).data){
                pq.push(temp);
                visited.at(prevIndex)=temp;
            }
    }
}

void bfs(string input, string goal, int length){
    setDimensionInfo(length);
    int nodesExpanded=0;
    vector<nodule> visited;
    queue<nodule> q;
    
    nodule node;
    node.state=input;
    q.push(node);
    
    while(!q.empty()){
        node=q.front();
        q.pop();

        if(node.state == goal)
            break;

        if(!isVisited(visited,node.state)){                             //if node has not been visited 
            nodesExpanded++;
            moveOptionsQueue(visited,node,q);
        }
    }
    if(node.state == goal)                       //covers final case/constructing the solution
        constructSolution(visited,node,input);
    cout<<endl<<"Nodes Expanded by Breadth-First Search: "<<nodesExpanded<<endl;
    return;
}

void dfs(string input, string goal, int length){
    setDimensionInfo(length);
    int nodesExpanded=0;
    vector<nodule> visited;
    stack<nodule> stk;
    
    nodule node;
    node.state=input;
    stk.push(node);
    
    while(!stk.empty()){
        node=stk.top();
        stk.pop();

        if(node.state == goal)
            break;

        if(!isVisited(visited,node.state)){                             //if node has not been visited 
            nodesExpanded++;
            moveOptionsStack(visited,node,stk);
        }
    }
    if(node.state == goal)                       //covers final case/constructing the solution
        constructSolution(visited,node,input);
    cout<<endl<<"Nodes Expanded by Depth-First Search: "<<nodesExpanded<<endl;
    return;
}

void iddfs(string input, string goal, int length){
    setDimensionInfo(length);
    int nodesExpanded=0;
    int limit=0;
    int stopper=0;

    while(stopper==0 && limit<999){
        vector<noduleWithIntData> visited;
        stack<noduleWithIntData> stk;
        noduleWithIntData node;
        node.state=input;
        node.data=0;
        stk.push(node);
        while(!stk.empty()){
            node=stk.top();
            stk.pop();

            if(node.state == goal){
                stopper=1;
                break;
            }            

            if(!isVisited(visited,node.state) && node.data != limit){                             //if node has not been visited 
                nodesExpanded++;
                visited.push_back(node);
                noduleWithIntData temp;

                int moveIndex = node.state.find('0');                       //used for checking if moving up is possible
                if(moveIndex > upLimit){                                          //pushes state from moving up into queue
                    temp.state = swap(node.state,moveIndex,moveIndex-dimension);
                    temp.data = node.data+1;
                    if(!isVisited(visited,temp.state)){                     //if new state has not been visited
                        temp.prev = 'u';
                        stk.push(temp);
                    }
                }
                if(moveIndex < downLimit){                                          //move down
                    temp.state = swap(node.state,moveIndex,moveIndex+dimension);
                    temp.data = node.data+1;
                    if(!isVisited(visited,temp.state)){                     //if new state has not been visited
                        temp.prev = 'd';
                        stk.push(temp);
                    }
                }
                if(moveIndex != l1 && moveIndex != l2 && moveIndex != l3 && moveIndex != l4){     //move left
                    temp.state = swap(node.state,moveIndex,moveIndex-1);
                    temp.data = node.data+1;
                    if(!isVisited(visited,temp.state)){                     //if new state has not been visited
                        temp.prev = 'l';
                        stk.push(temp);
                    }
                }
                if(moveIndex != r1 && moveIndex != r2 && moveIndex != r3 && moveIndex != r4){     //move right
                    temp.state = swap(node.state,moveIndex,moveIndex+1);
                    temp.data = node.data+1;
                    if(!isVisited(visited,temp.state)){                     //if new state has not been visited
                        temp.prev = 'r';
                        stk.push(temp);
                    }
                }
            }
        }
        if(node.state == goal)                       //covers final case/constructing the solution
            constructSolutionIterative(visited,node,input);
        limit++;
    }
    cout<<endl<<"Nodes Expanded by Iterative-Deepening Depth-First Search: "<<nodesExpanded<<endl;
    return;
}

void astar(string input, string goal, int length){
    setDimensionInfo(length);
    int nodesExpanded=0;
    vector<noduleWithIntData> visited;
    priority_queue<noduleWithIntData> pq;
    
    noduleWithIntData node={manhattanDistance(input,goal)+outOfPlace(input,goal),input,'\0'};
    pq.push(node);
    
    while(!pq.empty()){
        node=pq.top();
        pq.pop();

        if(node.state == goal)
            break;
        int prevIndex=isVisitedAStar(visited,node.state);
        if(prevIndex == -1){                             //if node has not been visited 
            nodesExpanded++;
            moveOptionsPQ(visited,node,pq,goal,prevIndex);
        }
    }
    if(node.state == goal)
        constructSolutionAStar(visited,node,input);
    cout<<endl<<"Nodes Expanded by A* Search: "<<nodesExpanded<<endl;
    return;
}

void astarPROPER(string input, string goal, int length){        //DOES NOT STOP UNTIL IT HAS SEARCHED EVERY POSSIBLE STATE TO ENSURE AN OPTIMAL SOLUTION
    setDimensionInfo(length);
    int nodesExpanded=0;
    vector<noduleWithIntData> visited;
    priority_queue<noduleWithIntData> pq;
    
    noduleWithIntData node={manhattanDistance(input,goal)+outOfPlace(input,goal),input,'\0'};
    pq.push(node);
    
    while(!pq.empty()){
        node=pq.top();
        pq.pop();

        int prevIndex=isVisitedAStar(visited,node.state);
        if(prevIndex == -1){                             //if node has not been visited 
            nodesExpanded++;
            moveOptionsPQ(visited,node,pq,goal,prevIndex);
        }
    }
    int goalIndex=isVisitedAStar(visited,goal);
    if(goalIndex != -1)                       //covers final case/constructing the solution
        constructSolutionAStar(visited,visited.at(goalIndex),input);
    cout<<endl<<"Nodes Expanded by A* Search: "<<nodesExpanded<<endl;
    return;
}

void astarIterative(string input, string goal, int length){
    setDimensionInfo(length);
    int nodesExpanded=0;
    int limit=0;
    int stopper=0;

    while(stopper==0 && limit<999){
        vector<noduleWithIntData> visited;
        priority_queue<noduleWithIntData> pq;
        priority_queue<int> threshold;
        noduleWithIntData node;
        node.state=input;
        node.data=0;
        pq.push(node);

        while(!pq.empty()){
            node=pq.top();
            threshold.push(pq.top().data);
            pq.pop();

            if(node.state == goal){
                stopper=1;
                break;
            }

            int prevIndex=isVisitedAStar(visited,node.state);
            if(prevIndex == -1 && node.data <= limit){                             //if node has not been visited 
                nodesExpanded++;
                moveOptionsPQ(visited,node,pq,goal,prevIndex);
            }
        }
        if(node.state == goal){                       //covers final case/constructing the solution
            constructSolutionAStar(visited,node,input);
            cout<<endl<<"Nodes Expanded by Iterative-Deepening A* Search: "<<nodesExpanded<<endl;
            return;
        }
        
        int dupe=threshold.top();
        threshold.pop();
        while(dupe == threshold.top()){
            dupe = threshold.top();
            threshold.pop();
        }
        limit=dupe;
    }
}

void followSequence(string startState, string moveSequence, int length){
    string tracker=startState;
    int moveIndex;
    while(!moveSequence.empty()){
        moveIndex=tracker.find('0');
        if(moveSequence.at(0) == 'u')
            tracker=swap(tracker, moveIndex, moveIndex-length);
        else if(moveSequence.at(0) =='d')
            tracker=swap(tracker, moveIndex, moveIndex+length);
        else if(moveSequence.at(0) == 'l')
            tracker=swap(tracker, moveIndex, moveIndex-1);
        else
            tracker=swap(tracker, moveIndex, moveIndex+1);
        moveSequence.erase(0,1);
    }
    cout<<endl<<"End state: "<<endl;
    for(int i=0; i<(length*length); i+=length){
        cout<<"  ";
        for(int j=0; j<length; ++j){
            cout<<tracker.at(j+i)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}