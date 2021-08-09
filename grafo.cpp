#include <iostream>
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
bool visitato[MAXN];
int Dist[MAXN];

// per approx
float _clos[MAXN];
float _harm[MAXN];
float _betw[MAXN];
int _sigma[MAXN];
float _delta[MAXN];
list<int> Pred[MAXN];
float _lin[MAXN];
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
		current=next;
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
	for(int i=0; i<N; ++i)
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

	// valori normalizzati per confrontarli con le approssimazioni
	cout << (S-1)*1./clos << " " << harm/(S-1) << endl;
	// Lin
	// cout << (float)(S^2)*clos << " ";
}

void geom_sample(vector<int> sample, vector<int> comp)
{
	for(auto s:sample) {
		cout << "sample: " << s << endl;
		for(auto i:comp) {
			visitato[i]=false;
		}
		queue<int> Q;
		Q.push(s);
		visitato[s]=true;
		Dist[s]=0;
		stack<int> _S;
		_sigma[s]=1;

		while(!Q.empty()) {
			auto u=Q.front();
			Q.pop();
			_S.push(u);
			for(auto v:adj[u]) {
				if(!visitato[v]) {
					Q.push(v);
					visitato[v]=true;
					Dist[v]=Dist[u]+1;
					_clos[v]+=Dist[v];
					_harm[v]+=1./Dist[v];
				}
				// Abbiamo già incontrato u, e fornisce un SP per v
				if(Dist[v] == Dist[u]+1) {
					_sigma[v]+=_sigma[u];
					Pred[v].push_back(u);
				}
			}
		}
		while(!_S.empty()) {
			auto t=_S.top();
			_S.pop();
			for(auto w:Pred[t]) {
					_delta[w]+=(float) ((float)_sigma[w]/(double)_sigma[t])*(1.+_delta[t]);
			}
			if(t!=s) {
					_betw[t]+=_delta[t];
			}
		}
	}
}

void printTop(vector<int> Cc)
{
	priority_queue<pair<float,int>> P,Q,R;
	int m=10;
	for (auto i:Cc) {
		// O(S*log(m)) tramite heap min
		if(P.size()<(unsigned)m) {
				P.push(pair<float,int>(_clos[i],i));
		} else if(P.top().first<_clos[i]) {
				P.pop();
				P.push(pair<float,int>(_clos[i], i));
		}
		if(Q.size()<(unsigned)m) {
				Q.push(pair<float,int>(_harm[i],i));
		} else if(Q.top().first <_harm[i]) {
				Q.pop();
				Q.push(pair<float,int>(_harm[i], i));
		}
		if(R.size()<(unsigned)m) {
				R.push(pair<float,int>(_betw[i],i));
		} else if(R.top().first<_betw[i]) {
				R.pop();
				R.push(pair<float,int>(_betw[i], i));
		}
	}
	cout << "i\t| Closeness\t\t\t| Harmonic\t\t\t| Closeness" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	for (int i=0; i<m; ++i) {
		int _i=P.top().second;
		int __i=Q.top().second;
		int ___i=R.top().second;
		Actor p = *(find_if(A.begin(), A.end(), [_i](const Actor &a) { return a.id == _i; }));
		Actor q = *(find_if(A.begin(), A.end(), [__i](const Actor &a) { return a.id == __i; }));
		Actor r = *(find_if(A.begin(), A.end(), [___i](const Actor &a) { return a.id == ___i; }));
		if(i == 0)
			cout << "1 🏅" << "\t| " << p.name << "\t\t\t| " << q.name << endl;
		else
			cout << i+1 << "\t| " << p.name << "\t\t\t| " << q.name << "\t\t\t| " << r.name << endl;
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

	readNames();

	buildG();

	// freopen("txt/grafo.txt","w",stdout);
	// printG();
	// fclose(stdout);

	// reset
	freopen("txt/centrality.txt", "w", stdout);
	fclose(stdout);

	for(int i=0; i<N; ++i)
		visitato[i]=false;
	for(int i=1; i<N; ++i) {
		if(!adj[i].empty()&&!visitato[i]) {
			vector<int> Cc=componente(i);
			int S=Cc.size();
			if(S>1000) {
				// eps=1, delta=0.1(probablity >=99%)(k=35)
				int k=ceil(2*(log(2)+log(S)+log(10)));
				vector<int> sample;
				for (int i=0; i<k; ++i) {
					sample.push_back(Cc[(rand() % S)]);
				}
				for(auto i:Cc) {
					_harm[i]=0;
					_clos[i]=0;
					_betw[i]=0;
					_sigma[i]=0;
					_delta[i]=0;
					_lin[i]=0;
				}

				struct timeval beg, end;
				gettimeofday(&beg, NULL);
				cout << "INIZIO GEOM_SAMPLE" << endl;
				geom_sample(sample,Cc);
				cout << "FINE GEOM_SAMPLE" << endl;
				gettimeofday(&end, NULL);
				cout << "Tempo di esecuzione: " << "\n\t" << ((end.tv_sec - beg.tv_sec)*1000000 + end.tv_usec - beg.tv_usec)/1000 << " ms" << endl;
				freopen("txt/centrality.txt", "a", stdout);
				for(auto j:Cc) {
					// _lin[j]=(float)(S^2)*_clos[j];
					// sintassi: nodo: grado closeness harmonic betweenness (TODO: /N(N-1) o Cosa?)
					cout << j << ": " << adj[j].size() << " " << (((float)(S-1)*(float)k)/((float)S*_clos[j])) << " " << ((float)S*_harm[j])/((float)(S-1)*(float)k) << " " << (2.*_betw[j])/((float)k*(float)S*(float)(S-1))  << endl;
				}
				cout << endl;
				fclose(stdout);

				// TODO: implement exact betweenness to check values...
				// geom_exact(306,S);
				// geom_exact(616,S);

				freopen("TOP.txt","w",stdout);
				printTop(Cc);
				fclose(stdout);

				sample.clear();
			}
		}
	}
	return 0;
}
