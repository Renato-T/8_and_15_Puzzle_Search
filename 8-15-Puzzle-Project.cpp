#include <iostream>
#include "PuzzleFunctions.h"

using namespace std;

int main(){
    string input;
    string goal;
    int choice;
    int dimension;

    cout<<"Please enter choice:"<<endl;
    cout<<"1. Give start and goal state to return move solution"<<endl;
    cout<<"2. Give start state and move sequence to return end state"<<endl;
    cin>>choice;

    if(choice == 1){
        cout<<"Both the input and goal string must not contain duplicates! Both must contain the numbers 0-8 or 0-F! Both must be solvable!"<<endl;
        cout<<"Please enter input string:"<<endl;
        cin>>input;
        cout<<"Please enter goal string:"<<endl;
        cin>>goal;
    
        //verifying dimensions
        if(input.size() == 9 && goal.size() == 9)
            dimension=3;
        else if(input.size() == 16 && goal.size() == 16)
            dimension=4;
        else{
            cout<<"Incorrect inputs for puzzle"<<endl;
            return 1;
        }
    
        cout<<endl<<"Please enter solution method."<<endl;
        cout<<"1. Breadth-First Search"<<endl;
        cout<<"2. Depth-First Search"<<endl;
        cout<<"3. Iterative-Deepening Depth-First Search"<<endl;
        cout<<"4. A* w/ Out-of-Place and Manhattan Distance Heuristics"<<endl;
        cout<<"5. A* w/ Out-of-Place and Manhattan Distance Heuristics (Stops when first solution is found. Not gauranteed to be optimal)"<<endl;
        cout<<"6. Iterative Deepening A* w/ Out-of-Place and Manhattan Distance Heuristics"<<endl;
        cin>>choice;

        if(choice==1)
            bfs(input,goal,dimension);
        else if(choice==2)
            dfs(input,goal,dimension);
        else if(choice==3)
            iddfs(input,goal,dimension);
        else if(choice==4)
            astarPROPER(input,goal,dimension);
        else if(choice==5)
            astar(input,goal,dimension);
        else if(choice==6)
            astarIterative(input,goal,dimension);
    }
    else if(choice == 2){
        cout<<"Start state string must not contain duplicates and must contain the numbers 0-8 or 0-F!"<<endl;
        cout<<"Please enter start state:"<<endl;
        cin>>input;
        cout<<"Please enter move sequence:"<<endl;
        cin>>goal;

        //verifying dimensions
        if(input.size() == 9)
            dimension=3;
        else if(input.size() == 16)
            dimension=4;
        else{
            cout<<"Incorrect inputs for puzzle"<<endl;
            return 1;
        }

        followSequence(input,goal,dimension);
    }

    //create code to verify solution
    system("pause");
    return 0;
}