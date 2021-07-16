#include <iostream>
#include <vector>		 // vector per le liste di adiacenza

using namespace std;

/* modo semplice per definire un grafo: liste di adiacenza */

constexpr auto MAXN = 1000; // massimo numero di nodi
vector<int> adj[MAXN];      // array di MAXN vector = rappresentazione mediante liste di adiacenza
bool visitato[MAXN];        // array di booleani per stabilire quali nodi sono stati visitati
int N,M;                    // N <= MAXN numero di nodi, M = numero di archi

/* lettura da input */

void leggiGrafo()
{
	freopen("txt/grafo.txt", "r", stdin); // redireziona la lettura da stdin alla lettura di un file

	cin >> N >> M;

	for(auto i=0;i<M;i++){ // M archi
		int u,v;
		cin >> u >> v; 			// arco uv non orientato
		adj[u].push_back(v);// v è nella lista di u
		adj[v].push_back(u);// u è nella lista di v
	}
	// nota: grado(u) --> adj[u].size()
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
