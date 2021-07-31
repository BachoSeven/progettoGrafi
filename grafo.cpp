#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

/*
 * definizioni per le liste di adiacenza
*/
constexpr auto MAXN = 14000000; // massimo numero di nodi
vector<int> adj[MAXN];
bool visitato[MAXN];
int M,N;

// struct Attore{
	// int id;
	// string name;
// };

// struct Film{
	// int id;
	// string name;
// };

// Centrality
float closeness(int x)
{
	float sum=0;
	for(auto u=0;(u<N&&u!=x);++u) {
		if(!adj[u].empty()) {
			sum+=SP(u,x);
		}
	}
	return (1/sum);
}
int degree(int x)
{
	return adj[x].size();
}
float lin(int x)
{
	int sum=0;
	for(auto u=0;(u<N&&u!=x);++u) {
		if(!adj[u].empty()) {
			sum+=SP(u,x);
		}
	}
	return (((adj[x].size())^2)/sum);
}
int harmonic(int x)
{
	int sum=0;
	for(auto u=0;(u<N&&u!=x);++u) {
		if(!adj[u].empty()) {
			sum+=1/SP(u,x);
		}
	}
	return sum;
}

void buildG()
{
	freopen("txt/info.txt", "r", stdin);

	cin >> M; // #"relazioni"
	cin >> N; // #valore massimo di un identificativo di un attore
	N++;

	ifstream relazioni("txt/Relazioni.txt");
	string s,t,r;

	int u,v;
	string del = " ";
	vector<int> filmcrew;

	getline(relazioni,s);
	int current=stoi(s.substr(0, s.find(del)));

	while(getline(relazioni,t)) {
		int next=stoi(t.substr(0, t.find(del)));

		if(next == current) {
			u=stoi(s.substr(s.find(del)+1));
			filmcrew.push_back(u);
			while((next == current)&&(getline(relazioni,r))) {
				v=stoi(t.substr(t.find(del)+1));
				filmcrew.push_back(v);
				t=r;
				next=stoi(t.substr(0, t.find(del)));
			}
			for (auto primo = filmcrew.begin(); primo != filmcrew.end(); ++primo) {
				for (auto secondo = primo + 1; secondo != filmcrew.end(); ++secondo) {
					u=*primo;
					v=*secondo;
					// controllo di non star duplicando un arco
					if( find(adj[u].begin(), adj[u].end(), v) == end(adj[u]) ) { // find è O(n) con n<10=length massima lista di adiacenza
						adj[u].push_back(v);
						adj[v].push_back(u);
					}
				}
			}
		}
		current=next;
		s=t;
		filmcrew.clear();
	}
}

void printG()
{
	for(auto u=0;u<N;++u) {
		// Stampa lista di adiacenza solo se non è vuota.
		if(!adj[u].empty()) {
			cout << u << ": ";
			for(auto v:adj[u]) {
				cout << v << "\t";
			}
			cout << endl;
		}
	}
}

/* Visita DFS in profondità */
void DFSrec(int u)
{
	visitato[u]=true;
	for(auto v:adj[u]) {
		if(!visitato[v]) {
			cout << "{"<< u << ", " << v << "}" << endl;
			DFSrec(v);
		}
	}
}

void DFS()
{
	int cc=0;
	for(auto i=0;i<N;++i) // inizializzazione: nessun nodo visitato
		visitato[i]=false;

	for(auto i=0;i<N;++i) {
		if(!visitato[i]) {
			cc++;
			cout << "- " << cc << "a componente connessa: "  <<  i  << endl;
			DFSrec(i);
		}
	}
}

int main()
{
	buildG();

	freopen("txt/grafo.txt","w",stdout);
	printG();

	// DFS();
	// DFSrec(93);
	return 0;
}
