#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
 * definizioni per le liste di adiacenza
*/
constexpr auto MAXN = 4077922; // massimo numero di nodi
vector<int> adj[MAXN];
bool visitato[MAXN];
int M,N;

struct attore{
	int id;
	string name;
};

struct film{
	int id;
	string name;
};

void leggiGrafo()
{
	freopen("txt/info.txt", "r", stdin);

	cin >> M;
	cin >> N;

	freopen("txt/Relazioni.txt", "r", stdin);
	string s;

	for(auto i=0;i<M;i++){
			int u,v;
			string del = “ ”;
			cin >> s;
			int film=s.substr(0, s.find(del));
			int next=film;
			int j=0;
			if(next == film) {
				int u=s.substr(1, s.find(del));
				int v=s.substr(1, s.find(del));
			 // controllo di non star duplicando un arco
				if( find(adj[u].begin(), adj[u].end(), v) == end(adj[u]) ) { // find è O(n) con n=length(lista di adiacenza)
					adj[u].push_back(v);
					adj[v].push_back(u);
				}
				j++;
				cin >> s;
				int film=s.substr(0, s.find(del));
				int next=film;
			}
	} // deg(u)=adj[u].size()
}

void stampaGrafo()
{
	cout << "liste di adiacenza" << endl;
	for(auto u=0;u<N;u++){
		cout << u << ": ";
		for(auto v:adj[u]) // NOICE
			cout << v << " ";
		cout << endl;
	}
}

/* Visita DFS in profondità */
void DFSrec(int u)
{
	visitato[u]=true;
	for(auto v:adj[u])
		if(!visitato[v]){
			cout << "{"<< u << ", " << v << "}" << endl;
			DFSrec(v);
		}
}

void DFS()
{
	int cc=0;
	for(auto i=0;i<N;i++) // inizializzazione: nessun nodo visitato
		visitato[i]=false;

	for(auto i=0;i<N;i++)
		if(!visitato[i]){
			cc++;
			cout << "- " << cc << "a componente connessa: "  <<  i  << endl;
			DFSrec(i);
		}
}

int main()
{
	leggiGrafo();

	stampaGrafo();
	cout << endl;

	DFS();
	return 0;
}
