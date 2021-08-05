#include <iostream>
#include <vector>
#include <string>
#include <fstream> // getline
#include <algorithm> // find
#include <list>	// coda

#include <queue>
#include <sys/time.h> // per gettimeofday

using namespace std;

/*
 * definizioni per le liste di adiacenza
*/
constexpr auto MAXN = 14000000; // massimo numero di nodi
vector<int> adj[MAXN];
bool visitato[MAXN];
int Dist[MAXN];
int N;

// struct Attore{
	// int id;
	// string name;
// };

// struct Film{
	// int id;
	// string name;
// };

void buildG()
{
	freopen("txt/info.txt", "r", stdin);

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
					if( find(adj[u].begin(), adj[u].end(), v) == end(adj[u]) ) { // find è O(n) con n=lunghezza lista di adiacenza
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
	for(auto u=1;u<N;++u) {
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

int degree(int x)
{
	return adj[x].size();
}

void geometric(int s)
{
	for(auto i=0;i<N;++i)
		visitato[i]=false;

	list<int> Q;
	Q.push_back(s);
	visitato[s]=true;
	Dist[s]=0;

	float clos=0;
	float harm=0;

	while(!Q.empty()){
		auto u=Q.front();
		Q.pop_front();
		for(auto v:adj[u]){
			if(!visitato[v]){
				Q.push_back(v);
				visitato[v]=true;
				Dist[v]=Dist[u]+1;
				clos+=Dist[v];
				harm+=1/Dist[v];
			}
		}
	}

	// Closeness
  cout << 1/clos << " ";
	// Harmonic
  cout << harm << " ";
	// Lin
  // cout << (Cc(x)^2)*clos << " ";
	cout << endl;
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

	// freopen("txt/grafo.txt","w",stdout);
	// printG();

	// struct timeval beg, end;
	// gettimeofday(&beg, NULL);
	// STUFF
	// gettimeofday(&end, NULL);
	// cout << "Tempo: " << "\n\t" << ((end.tv_sec - beg.tv_sec)*1000000 + end.tv_usec - beg.tv_usec)/1000 << " ms" << endl;

		freopen("txt/debug.txt", "w", stdout);
		geometric(306);
		fclose(stdout);

	// DFS();
	// DFSrec(93);
	return 0;
}
