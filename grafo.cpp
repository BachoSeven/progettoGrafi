#include <iostream>
#include <vector>
#include <string>
#include <fstream> // getline
#include <algorithm> // find
#include <queue>	// coda
#include <math.h> // ceil
#include <sys/time.h> // per gettimeofday

using namespace std;

constexpr int MAXN = 14000000; // massimo numero di nodi
vector<int> adj[MAXN];
bool visitato[MAXN];
int Dist[MAXN];
int N;
// per approx
float _clos[MAXN];
float _harm[MAXN];
float _lin[MAXN];
int S; // grandezza componenti

struct Attore{
	int id;
	string name;
};

struct Film{
	int id;
	string name;
};

void createstructs()
{
	ifstream attori("txt/Attorə.txt");
	ifstream film("txt/FilmFiltrati.txt");

	string s,t;
	string del=" ";

	vector<Attore> A;
	vector<Film> F;
	while(getline(attori,s)) {
		Attore tmp;
		tmp.id=stoi(s.substr(0, s.find(del)));
		tmp.name=s.substr(s.find(del)+1);
		A.push_back(tmp);
	}
	while(getline(film,t)) {
		Film tmp;
		tmp.id=stoi(t.substr(0, t.find(del)));
		tmp.name=t.substr(t.find(del)+1);
		F.push_back(tmp);
	}
}

void buildG()
{
	freopen("txt/info.txt", "r", stdin);

	cin >> N; // valore massimo di un identificativo di un attore
	N++;

	ifstream relazioni("txt/Relazioni.txt");
	string s,t,r;

	int u,v;
	string del=" ";
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
	for(int u=1;u<N;++u) {
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

vector<int> componente(int x)
{
	vector<int> Cc;
	queue<int> Q;
	Q.push(x);
	visitato[x]=true;

	while(!Q.empty()) {
		auto u=Q.front();
		Q.pop();
		for(auto v:adj[u]) {
			if(!visitato[v]) {
				Q.push(v);
				visitato[v]=true;
				Cc.push_back(v);
			}
		}
	}
	return Cc;
}

void geom_exact(int s,int S)
{
	for(int i=0;i<N;++i)
		visitato[i]=false;

	queue<int> Q;
	Q.push(s);
	visitato[s]=true;
	Dist[s]=0;

	float clos=0;
	float harm=0;

	while(!Q.empty()) {
		auto u=Q.front();
		Q.pop();
		for(auto v:adj[u]) {
			if(!visitato[v]) {
				Q.push(v);
				visitato[v]=true;
				Dist[v]=Dist[u]+1;
				clos+=Dist[v];
				harm+=1./Dist[v];
			}
		}
	}

	cout << (S-1)*1./clos << " " << harm/(S-1) << endl;
	// Lin
	// cout << (float)(S^2)*clos << " ";
	// cout << endl;
}

// TODO: implementa betwenness assieme a queste
void geom_sample(vector<int> sample, vector<int> comp)
{
	for(auto s:sample) {
		for(auto i:comp) {
			visitato[i]=false;
		}
		queue<int> Q;
		Q.push(s);
		visitato[s]=true;
		Dist[s]=0;

		while(!Q.empty()) {
			auto u=Q.front();
			Q.pop();
			for(auto v:adj[u]) {
				if(!visitato[v]) {
					Q.push(v);
					visitato[v]=true;
					Dist[v]=Dist[u]+1;
					_clos[v]+=Dist[v];
					_harm[v]+=1./Dist[v];
				}
			}
		}
	}
}

int main()
{
	srand(time(NULL));

	buildG();

	for(int i=0;i<N;++i)
		visitato[i]=false;
	for(int i=1;i<N;++i) {
		if(!adj[i].empty()&&!visitato[i]) {
			vector<int> Cc=componente(i);
			int S=Cc.size();
			if(S>100) {
				// // delta=0.1; eps=0.5 (k=139)
				// int k=ceil(2*(4)*(log(2)+log(S)+log(10)));
				// delta=0.05; eps=0.4 (k=225)
				int k=ceil(2*(6.25)*(log(2)+log(S)+log(20)));
				vector<int> sample;
				for (int i=0;i<k;++i) {
					sample.push_back(Cc[(rand() % S)]);
				}
				for(auto i:Cc) {
					_harm[i]=0;
					_clos[i]=0;
				}

				struct timeval beg, end;
				gettimeofday(&beg, NULL);
				geom_sample(sample,Cc);
				gettimeofday(&end, NULL);
				cout << "Tempo di esecuzione: " << "\n\t" << ((end.tv_sec - beg.tv_sec)*1000000 + end.tv_usec - beg.tv_usec)/1000 << " ms" << endl;

				freopen("txt/centrality.txt", "a", stdout);
				for(auto j:Cc) {
					// _lin[j]=(float)(S^2)*_clos[j];
					// sintassi: nodo: grado closeness harmonic
					cout << j << ": " << adj[j].size() << " " << (((float)(S-1)*(float)k)/((float)S*_clos[j])) << " " << ((float)S*_harm[j])/((float)(S-1)*(float)k) << endl;
				}
				cout << endl;
				fclose(stdout);


				sample.clear();
			}
		}
	}

	vector<int> Cc=componente(306);
	int S=Cc.size();
	cout << "Esatti: " << endl << "616: " << geom_exact(616,S) << endl << "306: " geom_exact(306,S) << endl;

	// freopen("txt/grafo.txt","w",stdout);
	// printG();
	// fclose(stdout);

	// DFS();
	// DFSrec(93);
	return 0;
}
