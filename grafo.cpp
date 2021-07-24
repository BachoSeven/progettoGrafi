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
int M,N,O;

struct Attore{
	int id;
	string name;
};

struct filmcrew{
	int id;
	string name;
};

void leggiGrafo()
{
	freopen("txt/info.txt", "r", stdin);

	cin >> M; // #"relazioni"
	cin >> N; // #nodi
	cin >> O; // max num attori nello stesso film

	freopen("txt/Relazioni.txt", "r", stdin);
	string s, t;

	int u,v;
	string del = " ";

	cin >> s;
	int current=stoi(s.substr(0, s.find(del)));

	int j=1;
	while(j<M) {
		cin >> t;
		int next=stoi(t.substr(0, t.find(del)));
		vector<string> filmcrew[O];
		if(next == current) {
			int u=stoi(s.substr(1, s.find(del)));
			filmcrew.push_back(u);
		}
		while(next == current) {
			int v=stoi(t.substr(1, t.find(del)));
			filmcrew.push_back(v);
			j++;
			// current=next; // Non serve :)
			cin >> t;
			next=stoi(t.substr(0, t.find(del)));
		}

		s=t;
		current=next;
		j++;

		for(int i=0; i<filmcrew.size(); i++) {
			for(int j=i; i<filmcrew.size(); i++) {
				u=filmcrew[i];
				v=filmcrew[j];
			 // controllo di non star duplicando un arco
				if( find(adj[u].begin(), adj[u].end(), v) == end(adj[u]) ) { // find è O(n) con n=length(lista di adiacenza)
					adj[u].push_back(v);
					adj[v].push_back(u);
				}
			}
		}
		delete[] filmcrew;
	}
} // deg(u)=adj[u].size()

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
