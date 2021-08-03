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

// Centrality
int degree(int x)
{
	return adj[x].size();
}
float lin(int x)
{
	int sum=0;
	for(auto u=1;(u<N&&u!=x);++u) {
		if(!adj[u].empty()) {
			// sum+=SP(u,x);
		}
	}
	return (((adj[x].size())^2)/sum);
}
int harmonic(int x)
{
	int sum=0;
	for(auto u=1;(u<N&&u!=x);++u) {
		if(!adj[u].empty()) {
			// sum+=1/SP(u,x);
		}
	}
	return sum;
}

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

float closeness(int s)
{
	for(auto i=0;i<N;++i)
		visitato[i]=false;

	list<int> Q;
	Q.push_back(s); // enqueue
	// invariante: Q contiene sono solo nodi visitati di cui conosciamo la distanza da s
	visitato[s]=true;
	Dist[s]=0;

	float sum=0;

	while(!Q.empty()){
		auto u=Q.front(); // non viene estratto
		Q.pop_front();
		for(auto v:adj[u]){
			if(!visitato[v]){
				Q.push_back(v);
				visitato[v]=true;
				Dist[v]=Dist[u]+1;
				sum+=Dist[v];
			}
		}
	}

	return (1/sum);
}

void Cludo( int s ){
  float harmonic = 0;
  float Clos = 0;

  for (auto i=0; i<N; i++)
    visitato[i] = false;

  queue<int> coda;
  coda.push(s);
  visitato[s] = true;
  Dist[s] = 0;

  cout << "BFS\n\nsequenza di archi che portano a nodi non ancora visitati: " << endl;

  while( !coda.empty() ){
    auto u = coda.front();
    coda.pop();
    for ( auto v: adj[u] ) {
      if (!visitato[v]){
        cout << '(' << u << ", " << v << ')';
        coda.push(v);
        visitato[v] = true;
        Dist[v] = Dist[u] + 1;
				harmonic += 1.0 / (float)Dist[v];
				Clos += Dist[v];
				cout << '\t' << "++" << 1.0 / (float)Dist[v] << '\t' << "= " << harmonic << endl;
      }
    }
  }


  cout << endl << "L'indice armonico del nodo è " << harmonic << endl;
  cout << endl << "L'indice di closeness del nodo è " << 1.0 / (float)Clos << endl;
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
	// TODO: vedi implementazione iterativa visita da Ludo
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

		struct timeval beg, end;

		gettimeofday(&beg, NULL);
		freopen("txt/debug.txt", "w", stdout);
		cout << closeness(93) << endl;
		// Cludo(93);
		fclose(stdout);
		gettimeofday(&end, NULL);
		cout << "Tempo: " << "\n\t" << ((end.tv_sec - beg.tv_sec)*1000000 + end.tv_usec - beg.tv_usec)/1000 << " ms" << endl;

	// DFS();
	// DFSrec(93);
	return 0;
}
