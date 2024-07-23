#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <set>
#include <queue>
#include <array>
typedef std::vector<std::vector<int>> STATE;

struct Node{
std::vector<std::vector<int>> state;
int cost;
int heurestics;
int f_value;

bool operator<(const Node &other) const{
    return f_value<other.f_value;
}
bool operator>(const Node &other) const{
    return f_value>other.f_value;
}
bool operator==(const Node &other) const{
 for(int x =0;x<3;++x){
           if(state[x]!=other.state[x]){
            return false;
           }
        }
        return true;
}
};
struct StateHash {
    std::size_t operator()(const STATE& state) const {
        std::size_t seed = 0;
        for (const auto& row : state) {
            for (const auto& elem : row) {
                seed ^= std::hash<int>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
        }
        return seed;
    }
};
STATE moveDown(STATE temp,int blankX,int blankY){
    temp[blankX][blankY] = temp[blankX+1][blankY];
            temp[blankX+1][blankY] = 0;
            return temp;
}
STATE moveUp(STATE temp,int blankX,int blankY){
    temp[blankX][blankY] = temp[blankX-1][blankY];
            temp[blankX-1][blankY] = 0;
            return temp;
}
STATE moveRight(STATE temp,int blankX,int blankY){
   temp[blankX][blankY] = temp[blankX][blankY+1];
            temp[blankX][blankY+1] = 0;
            return temp;
}
STATE moveLeft(STATE temp,int blankX,int blankY){
   temp[blankX][blankY] = temp[blankX][blankY-1];
            temp[blankX][blankY-1] = 0;
            return temp;
}
std::list<STATE> getStates(STATE current);

bool goalTest(STATE state){
    STATE goal =  {
                {1,2,3},
                {4,5,6},
                {7,8,0} };
        for(int x =0;x<3;++x){
           if(state[x]!=goal[x]){
            return false;
           }
        }
     
   return true;
}
int getHeuristics(STATE state){
    STATE goal = {{1,2,3},
                {4,5,6},
                {7,8,9}};
    int heuristic = 0;
    for(int x=0;x<3;++x){
        for(int y=0;y<3;++y){
            if(state[x][y]!=goal[x][y])
             heuristic++;
        }}    
        //std::cout<<heuristic<<"\n";
    return heuristic;
  
}
struct Explored{
    std::pair<Node,int> node;
    bool operator==(const Explored &other)const{
        if(node.first==other.node.first)
        return true;
        return false;
    }
};
std::vector<STATE> aStar(STATE &start){
    std::priority_queue<Node,std::vector<Node>,std::greater<Node>> frontier;
    std::vector<Node> explored;
    std::unordered_map<STATE,int,StateHash> exp_fvalue;
    std::unordered_map<STATE,STATE,StateHash> parent; 
    Node snode = {start,0,getHeuristics(start)};
    snode.f_value = snode.cost+snode.heurestics;
    exp_fvalue[start] = snode.heurestics;
    parent[start] =  start;
    int count =0;
    std::vector<STATE> path;
    if(goalTest(start)){
        path.push_back(start);
        return path;
    }
        frontier.push(snode);
    while(!(frontier.empty())){
      
        Node current = frontier.top();
        frontier.pop();
        exp_fvalue[current.state] = current.f_value;
        
        if(goalTest(current.state)){
            STATE i = current.state;
            path.push_back(i);
            while(!(i==start)){
                i = parent[i];
                path.push_back(i);
            }
            return path;
        }
        for(auto child:getStates(current.state)){
            Node nchild = {child,current.cost+1,getHeuristics(child)};
            nchild.f_value = nchild.cost+nchild.heurestics;

            if(exp_fvalue.find(child)==exp_fvalue.end()||nchild.f_value<exp_fvalue[nchild.state]){
                frontier.push(nchild);
                parent[child] = current.state;
                if(nchild.f_value<exp_fvalue[nchild.state])
               exp_fvalue[child] = nchild.f_value;
                //std::cout<<"here";
            }
            
            
        }
      count++;

    }
    
    return path;

}
int main(){
STATE init= {
                {7,8,2},
                {1,3,6},
                {5,4,0} };

std::vector<STATE> nresult = aStar(init);
int stepsTaken = 0;
std::reverse(nresult.begin(),nresult.end());
for(auto result:nresult){
    stepsTaken++;
    for(auto i:result){
        for(auto j:i){
            std::cout<<j<<" ";
    }
    std::cout<<"\n";
}
std::cout<<"\n";
}
std::cout<<"Steps taken: "<<stepsTaken-1;
}

std::list<STATE> getStates(STATE current){
    int blankX;
    int blankY;
    std::list<STATE> states;
    for(int x=0;x<3;++x){
        for(int y = 0;y<3;++y){
            if(current[x][y]==0){
                blankX = x;
                blankY = y;
                break;
            }}}
    
    STATE temp = current;
    if(blankY>0){
        temp = moveLeft(current,blankX,blankY);
            states.push_back(temp);
    }
    if(blankY<2){
        temp = current;
         temp = moveRight(current,blankX,blankY);
            states.push_back(temp);
    }
    if(blankX>0){
        temp = current;
    temp = moveUp(current,blankX,blankY);
            states.push_back(temp);
    }
    if(blankX<2){
        temp = current;
        temp = moveDown(current,blankX,blankY);
            states.push_back(temp);
    }

    
    return states;
}
