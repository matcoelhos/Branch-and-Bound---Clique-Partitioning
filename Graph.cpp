#include "Graph.h"

using namespace std;

bool existEl(vector<int> V, int el)
{
    for (int i = 0; i < V.size(); i++)
    {
        if (V[i] == el) return true;
    }
    return false;
}

Graph::Graph(int gSize)
{
    V = gSize;
    E = 0;
    adjMat = new bool * [gSize];
    for (int i = 0; i < gSize; i++)
    {
        adjMat[i] = new bool [gSize];
        for (int j = 0; j < gSize; j++)
        {
            adjMat[i][j] = false;
        }

    }
}

Graph::~Graph()
{
    for (int i = 0; i < V; i++)
    {
        delete adjMat[i];
    }
    delete adjMat;
    //cout << endl << "destrutor" << endl;
}

void Graph::disp()
{
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            cout << adjMat[i][j] << " ";
        }
        cout << endl << endl;
    }
}

void Graph::insertEdge(vector<int>par)
{
    if (!adjMat[par[0]][par[1]])
    {
        adjMat[par[0]][par[1]] = true;
        adjMat[par[1]][par[0]] = true;
        E++;
    }
}

void Graph::removeEdge(vector<int>par)
{
    if (adjMat[par[0]][par[1]])
    {
        adjMat[par[0]][par[1]] = false;
        adjMat[par[1]][par[0]] = false;
        E--;
    }
}

bool Graph::existEdge(vector<int> par)
{
    return adjMat[par[0]][par[1]];
}

vector<int> Graph::vertDegree()
{
    vector<int> ans;
    vector<int> ansf;
    for (int i = 0; i < V; i++)
    {
        int deg = 0;
        for (int j = 0; j < V; j++)
        {
            vector<int> p;
            p.push_back(i);
            p.push_back(j);
            if (i != j && existEdge(p)) deg++;
        }
        ans.push_back(deg);
    }

    for (int i = 0; i < V; i++)
    {
        int degf = ans[i];
        for (int j = 0; j < V; j++)
        {
            vector<int> p;
            p.push_back(i);
            p.push_back(j);
            if (i != j && existEdge(p)) degf += ans[j];
        }
        ansf.push_back(degf);
    }

    return ansf;
}

bool Graph::isClique(vector<int> part)
{
    for (int i = 0; i < part.size(); i++)
    {
        for (int j = i+1; j < part.size(); j++)
        {
            vector<int> p;
            p.push_back(part[i]);
            p.push_back(part[j]);
            if (!existEdge(p)) return false;
        }
    }
    return true;
}

vector<vector<int> > Graph::cliquePartGreedy()
{
    vector<vector<int> > ans;
    vector<int> removeList;
    vector<int> deg;
    deg = vertDegree();

    for (int i = 0; i < V; i++) removeList.push_back(i);

    while (removeList.size() > 0)
    {
        vector<int> clique;
        int biggest = -1, posbig = -1;
        //finding node with biggest degree
        for (int i = 0; i < removeList.size(); i++)
        {
            if (biggest < deg[removeList[i]])
            {
                biggest = deg[removeList[i]];
                posbig = removeList[i];
            }
        }

        clique.push_back(posbig);

        //finding biggest clique from biggest degree node
        vector<int> visitlist;
        for (int j = 0; j < V; j++)
        {
            vector<int> p;
            p.push_back(posbig);
            p.push_back(j);
            if(existEdge(p) && existEl(removeList, j))
            {
                visitlist.push_back(j);
            }
        }
        while (visitlist.size() > 0)
        {
            int lbiggest = -1, posbigl = -1, posl;
            //search for neighbor with biggest degree
            for (int j = 0; j < visitlist.size(); j++)
            {
                if(deg[visitlist[j]] > lbiggest)
                {
                    lbiggest = deg[visitlist[j]];
                    posbigl = visitlist[j];
                    posl = j;
                }
            }
            clique.push_back(posbigl);
            if (!isClique(clique))
            {
                clique.pop_back();
            }
            visitlist.erase(visitlist.begin() + posl);
        }
        //cout << "clique size: " << clique.size();

        for (int i = 0; i < clique.size(); i++)
        {
            int rm = clique[i];
            int iter = 0;
            while (removeList[iter] != rm) iter ++;
            removeList.erase(removeList.begin() + iter);
        }
        if (clique.size() > 0) ans.push_back(clique);
        //cout << " partition size:" << ans.size() << " remain: " << removeList.size() << endl;
    }
    return ans;
}

vector<vector<int> > Graph::cliquePartBTH(double timelimit)
{
    vector<vector<int> > ans;

    ans = this->cliquePartGreedy();

    //experimental upper bound
    int alda = ans.size();

    cout << "Experimental upper bound: " << alda << endl;

    double time = 0;
    clock_t start, stop;

    int vin = 0;
    while (time < timelimit && (vin < V))
    {
        start = clock();
        vector<vector<int> > newsol;
        vector<int> removeList;
        vector<int> deg;
        deg = vertDegree();
        int posbig = vin;

        //creating remove list
        for (int i = 0; i < V; i++) removeList.push_back(i);
        //sorting remove list
        for (int i = 0; i < V-1; i++)
        {
            int biggest = removeList[i], posbig = i;
            for (int j = i+1; j < V; j++)
            {
                if (deg[removeList[posbig]] > deg[removeList[j]])
                {
                    biggest = removeList[j];
                    posbig = j;
                }
            }
            removeList.erase(removeList.begin() + posbig);
            removeList.insert(removeList.begin() + i, biggest);
        }
        //remove list loop
        while (removeList.size() > 0 && newsol.size() < alda)
        {
            vector<int> clique;
            //select remove node from remove list
            if (removeList.size() != V)
            {
                int biggest = INT_MAX;
                posbig = -1;
                //finding node with biggest degree
                for (int i = 0; i < removeList.size(); i++)
                {
                    if (biggest > deg[removeList[i]])
                    {
                        biggest = deg[removeList[i]];
                        posbig = removeList[i];
                    }
                }
            }
            //insert candidate node in clique
            clique.push_back(posbig);

            //finding biggest clique from biggest degree node
            vector<int> visitlist;
            for (int j = 0; j < V; j++)
            {
                vector<int> p;
                p.push_back(posbig);
                p.push_back(j);
                if(existEdge(p) && existEl(removeList, j))
                {
                    visitlist.push_back(j);
                }
            }
            while (visitlist.size() > 0)
            {
                int lbiggest = INT_MAX, posbigl = -1, posl;
                //search for neighbor with lowest degree
                for (int j = 0; j < visitlist.size(); j++)
                {
                    if(deg[visitlist[j]] < lbiggest)
                    {
                        lbiggest = deg[visitlist[j]];
                        posbigl = visitlist[j];
                        posl = j;
                    }
                }
                //inserts neighbor node in candidate clique
                clique.push_back(posbigl);
                //pops last node if candidate clique is not valid
                if (!isClique(clique))
                {
                    clique.pop_back();
                }
                visitlist.erase(visitlist.begin() + posl);
            }
            //remove clique's nodes from removelist
            for (int i = 0; i < clique.size(); i++)
            {
                int rm = clique[i];
                int iter = 0;
                while (removeList[iter] != rm) iter ++;
                removeList.erase(removeList.begin() + iter);
            }
            if (clique.size() > 0) newsol.push_back(clique);
        }
        //updates best solution found
        if (newsol.size() < ans.size())
        {
            cout << "new best: " << newsol.size() << endl;
            ans = newsol;
            alda = ans.size();
        }

        vin++;
        stop = clock();
        time += (((double)(stop - start))/((double)CLOCKS_PER_SEC));
    }
    cout << "best found: " << ans.size() << endl;
    cout << "elapsed time (Heuristic): " << time << " s" << endl;
    return ans;
}

vector<vector<int> > Graph::insertInPartition (vector<vector<int> > part, int elem)
{
    bool inserted = false;
    for (int i = 0; i < part.size() && !inserted; i++)
    {
        part[i].push_back(elem);
        if (!isClique(part[i]))
        {
            part[i].pop_back();
        }
        else inserted = true;
    }

    if (!inserted)
    {
        vector<int> clq;
        clq.push_back(elem);
        part.push_back(clq);
    }
    return part;
}

vector<int> translate (vector<int> trList, vector<int> rmList)
{
    vector<int> ans;
    for (int i = 0; i < trList.size(); i++)
    {
        ans.push_back(rmList[trList[i]]);
        rmList.erase(rmList.begin() + trList[i]);
    }
    return ans;
}

void changeLevel(vector<int> * trList, int level)
{
    int maxLevel = trList->size() - level - 1;
    (*trList)[level] ++;
    if ((*trList)[level] > maxLevel)
    {
        (*trList)[level] = 0;
        changeLevel(trList,level-1);
    }
}

vector<vector<int> > Graph::cliquePartBTE(double timelimit)
{
    vector<vector<int> > ans;
    vector<int> deg;
    deg = vertDegree();
    ans = this->cliquePartGreedy();

    //experimental upper bound
    int alda = ans.size();

    cout << "Experimental upper bound: " << alda << endl;

    vector<int> removeList;
    vector<int> translateList;
    //creates remove list and a translated list
    for (int i = 0; i < V; i++)
    {
       removeList.push_back(i);
       translateList.push_back(0);
    }
    //sorts remove list
    for (int i = 0; i < V; i++)
    {
        int biggest = removeList[i], posbig = i;
        for (int j = i+1; j < V; j++)
        {
            if (deg[removeList[posbig]] > deg[removeList[j]])
            {
                biggest = removeList[j];
                posbig = j;
            }
        }
        removeList.erase(removeList.begin() + posbig);
        removeList.insert(removeList.begin() + i, biggest);
    }

    bool stopPoint = false;
    int maxpos = removeList.size();

    double time = 0, interval;
    clock_t start, stop;

    while (!stopPoint && time < timelimit)
    {
        start = clock();
        vector<vector<int> > newans;
        //translates removelist into roundlist
        vector<int> roundList = translate(translateList,removeList);

        //inserts element into clique partition
        int i = 0;
        while (i < roundList.size() && newans.size() < alda)
        {
            newans = insertInPartition(newans,roundList[i]);
            i++;
        }

        //updates best solution found untill this moment
        if (newans.size() < ans.size() && roundList.size() > 0)
        {
            cout << "new best: " << newans.size() << endl;
            ans = newans;
            alda = ans.size();
        }
        //checks if it has reached endpoint
        stopPoint = true;
        for (int j = 0; j < i && stopPoint; j++)
        {
            if (translateList[j] != translateList.size() - 1 - j) stopPoint = false;
        }
        //calculates next step from branch and bound
        if (!stopPoint) changeLevel(&translateList, i);
        i++;
        for (int j = i; j < translateList.size(); j++)
        {
            translateList[j] = 0;
        }
        stop = clock();
        time += (((double)(stop - start))/((double)CLOCKS_PER_SEC));
    }
    cout << "best found: " << ans.size() << endl;
    cout << "elapsed time (Branch and Bound): " << time << " s" << endl;
    return ans;
}
