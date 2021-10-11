#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <stack>
#include <fstream> // getline
#include <algorithm> // find
#include <math.h> // ceil
#include <sys/time.h> // per gettimeofday

using namespace std;

struct Film {
	int id;
	string name;
};

struct Actor {
	int id;
	string name;
};

// valore massimo di un identificativo di un attore
int N;
constexpr int MAXN=13000000;
vector<int> adj[MAXN];
bool visited[MAXN];
int Dist[MAXN];

// per approx
float Clos[MAXN];
float Harm[MAXN];
float Betw[MAXN];
bool direction[MAXN];
int Sigma[MAXN];
float Delta[MAXN];
list<int> Pred[MAXN];
int S; // grandezza componenti

vector<Actor> A;
vector<Film> F;

void readNames()
{
	ifstream attori("txt/Attorə.txt");
	ifstream film("txt/FilmFiltrati.txt");

	string s,t;
	string del=" ";

	while(getline(attori,s)) {
		Actor tmp;
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
	ifstream relazioni("txt/Relazioni.txt");
	string s,t,r;

	int u,v;
	string del=" ";
	vector<int> filmcrew;

	getline(relazioni,s);
	int currentFilm=stoi(s.substr(0, s.find(del)));

	while(getline(relazioni,t)) {
		int nextFilm=stoi(t.substr(0, t.find(del)));

		if(nextFilm == currentFilm) {
			u=stoi(s.substr(s.find(del)+1));
			filmcrew.push_back(u);
			while((nextFilm == currentFilm)&&(getline(relazioni,r))) {
				v=stoi(t.substr(t.find(del)+1));
				filmcrew.push_back(v);
				t=r;
				nextFilm=stoi(t.substr(0, t.find(del)));
			}

			for(auto primo=filmcrew.begin(); primo!=filmcrew.end(); ++primo) {
				for(auto secondo=primo+1; secondo!=filmcrew.end(); ++secondo) {
					u=*primo;
					v=*secondo;
					// controllo di non star duplicando un arco
					if(find(adj[u].begin(), adj[u].end(), v) == end(adj[u])) { // find è O(n) con n=lunghezza lista di adiacenza
						adj[u].push_back(v);
						adj[v].push_back(u);
					}
				}
			}
		}
		currentFilm=nextFilm;
		s=t;
		filmcrew.clear();
	}
}

void printG()
{
	for(int u=1; u<N; ++u) {
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
	vector<int> comp;
	queue<int> Q;
	Q.push(x);
	visited[x]=true;

	while(!Q.empty()) {
		auto u=Q.front();
		Q.pop();
		for(auto v:adj[u]) {
			if(!visited[v]) {
				Q.push(v);
				visited[v]=true;
				comp.push_back(v);
			}
		}
	}
	return comp;
}

void geom_sample(vector<int> sample, vector<int> comp)
{
	for(auto s:sample) {
		for(auto i:comp) {
			visited[i]=false;
		}
		queue<int> Q;
		Q.push(s);
		visited[s]=true;
		Dist[s]=0;
		stack<int> _S;
		Sigma[s]=1;

		while(!Q.empty()) {
			auto u=Q.front();
			Q.pop();
			_S.push(u);
			for(auto v:adj[u]) {
				if(!visited[v]) {
					Q.push(v);
					visited[v]=true;
					Dist[v]=Dist[u]+1;
					Clos[v]+=Dist[v];
					Harm[v]+=1./Dist[v];
				}
				// Abbiamo già incontrato u, e fornisce un SP per v
				if(Dist[v] == Dist[u]+1) {
					Sigma[v]+=Sigma[u];
					Pred[v].push_back(u);
				}
			}
		}
		while(!_S.empty()) {
			auto t=_S.top();
			_S.pop();
			for(auto w:Pred[t]) {
				if(!direction[w]) {
					Delta[w]+=((float)Sigma[w]/(float)Sigma[t])*((1./(float)Dist[w])+Delta[t]);
				} else {
					Delta[w]+=((float)Sigma[w]/(float)Sigma[t])*((1./(float)Dist[t]-1./(float)Dist[w])+Delta[t]);
				}
			}
			if(t!=s) {
					Betw[t]+=(float)Dist[t]*Delta[t];
			}
			// cleanup
			Pred[t].clear();
			Delta[t]=0;
			Sigma[t]=0;
		}
	}
}

void geom_exact(int s,int S)
{
	for(int i=0; i<N; ++i)
		visited[i]=false;

	queue<int> Q;
	Q.push(s);
	visited[s]=true;
	Dist[s]=0;

	float clos=0;
	float harm=0;

	while(!Q.empty()) {
		auto u=Q.front();
		Q.pop();
		for(auto v:adj[u]) {
			if(!visited[v]) {
				Q.push(v);
				visited[v]=true;
				Dist[v]=Dist[u]+1;
				clos+=Dist[v];
				harm+=1./Dist[v];
			}
		}
	}

	// valori normalizzati per confrontarli con le approssimazioni
	cout << (S-1)*1./clos << " " << harm/(S-1) << endl;
}

void printTop(vector<int> Cc)
{
	priority_queue<pair<float,int>> P,Q,R;
	int m=100;
	for (auto i:Cc) {
		// O(S*log(m)) tramite heap min
		if(P.size()<(unsigned)m) {
				P.push(pair<float,int>(Clos[i],i));
		} else if(P.top().first<Clos[i]) {
				P.pop();
				P.push(pair<float,int>(Clos[i], i));
		}
		if(Q.size()<(unsigned)m) {
				Q.push(pair<float,int>(Harm[i],i));
		} else if(Q.top().first <Harm[i]) {
				Q.pop();
				Q.push(pair<float,int>(Harm[i], i));
		}
		if(R.size()<(unsigned)m) {
				R.push(pair<float,int>(Betw[i],i));
		} else if(R.top().first<Betw[i]) {
				R.pop();
				R.push(pair<float,int>(Betw[i], i));
		}
	}
 cout << left << setw(4) << "i" << left << setw(30) << "Closeness" << left << setw(30) << "Harmonic" << left << setw(30) << "Betweenness" << "\n\n";
	for (int i=0; i<m; ++i) {
		int j=P.top().second;
		Actor p = *(find_if(A.begin(), A.end(), [j](const Actor &a) { return a.id == j; }));
		int j=Q.top().second;
		Actor q = *(find_if(A.begin(), A.end(), [j](const Actor &a) { return a.id == j; }));
		int j=R.top().second;
		Actor r = *(find_if(A.begin(), A.end(), [j](const Actor &a) { return a.id == j; }));
		if(i == 0)
			cout << left << setw(4) << "1🏅" << left << setw(30) << p.name << left << setw(30) << q.name << left << setw(30) << r.name << endl;
		else
			cout << left << setw(4) << i+1 << left << setw(30) << p.name << left << setw(30) << q.name << left << setw(30) << r.name << endl;
		P.pop();
		Q.pop();
		R.pop();
	}
}

int main()
{
	srand(time(NULL));

	freopen("txt/info.txt", "r", stdin);
	cin >> N;
	N++;

	readNames();

	buildG();

	// freopen("txt/grafo.txt","w",stdout);
	// printG();

	for(int i=0; i<N; ++i) {
		visited[i]=false;
	}
	Actor brad = *(find_if(A.begin(), A.end(), [](const Actor &a) { return a.name == "Brad Pitt"; }));
	vector<int> Cc=componente(brad.id);
	int S=Cc.size();
	// eps=0.1, delta=0.01(probablity >=99% => k=436
	// int k=ceil(2*(1./0.09)*(log(2)+log(S)+log(100)));
	// con k~1k si ottengono risultati coerenti per la betweenness
	int k=1000;
	vector<int> sample;
	for (int j=0; j<k; ++j) {
		int r=rand()%S;
		sample.push_back(Cc[r]);
		// 0=forward, 1=backward
		direction[Cc[r]]=rand()%2 == 0;
	}
	for(auto i:Cc) {
		Harm[i]=0;
		Clos[i]=0;
		Betw[i]=0;
		Sigma[i]=0;
		Delta[i]=0;
	}

	struct timeval beg, end;
	gettimeofday(&beg, NULL);
	geom_sample(sample,Cc);
	gettimeofday(&end, NULL);
	cout << "Tempo impiegato per calcolare la centralità: " << "\n\t" << ((float)((end.tv_sec - beg.tv_sec)*1000000 + end.tv_usec - beg.tv_usec))/1000000. << " s" << endl;

	freopen("txt/centrality.txt", "w", stdout);
	for(auto j:Cc) {
		// nodo: grado closeness harmonic betweenness(*2 stimatore corretto, ma /2 perché undirected(brandes2008))
		cout << j << ": " << adj[j].size() << " " << (((float)(S-1)*(float)k)/((float)S*Clos[j])) << " " << ((float)S*Harm[j])/((float)(S-1)*(float)k) << " " << Betw[j]/((float)k*(float)(S-1))  << endl;
	}

	// geom_exact(306,S);
	// geom_exact(616,S);

	freopen("txt/top.txt","w",stdout);
	printTop(Cc);

	fclose(stdout);
	return 0;
}
