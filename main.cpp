#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include "Graph.h"

using namespace std;

int main(int argc, char * argv[])
{
    stringstream ss;
    ss << "test" << argv[1];

    Graph * G = NULL;
    char c;

    cout << "start up!" << endl;

    ifstream infile(argv[1]);
    if (!infile)
    {
        cout << "ARQUIVO INDISPONIVEL" << endl;
        return 3;
    };

    string line;
    while (getline(infile, line))
    {
        istringstream i(line);

        char control;
        i >> control;
        if (control == 'p')
        {
            string s;
            int verts, a;
            i >> s >> verts >> a;
            G = new Graph(verts);

        }
        else if (control == 'e' && G != NULL)
        {
            vector<int> ins;
            int v1, v2;
            i >> v1 >> v2;
            ins.push_back(v1-1);
            ins.push_back(v2-1);
            G->insertEdge(ins);
        }
    }

    if (G == NULL)
    {
        cout << "ALERTA! GRAFO NAO INICIALIZADO" << endl;
        return 4;
    }

    cout << "Verts: " << G->V << endl;
    cout << "Edges: " << G->E << endl << endl;

    clock_t start, stop;
    double laricielle, interval;
    vector<vector<int> > part;
    laricielle = (double)stoi(argv[2]);

    /*if (G->V <= 10)*/ part = G->cliquePartBTE(laricielle);
    //else part = G->cliquePartBTH(laricielle);

    cout << "writing to output file" << endl;

    ofstream outfile;
    outfile.open("partition.txt");

    outfile << part.size() << endl;
    for (int i = 0; i < part.size(); i++)
    {
        outfile << part[i].size() << endl;
        for (int j = 0; j < part[i].size(); j++)
        {
            outfile << part[i][j] << endl;
        }
    }
    outfile.close();
    cout << "done" << endl;

    delete G;
    return 0;
}
