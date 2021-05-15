# Progetto di ASD, anno accademico 2020/21
---
Questo progetto sostituisce l'esame scritto del corso. Svolto in C/C++ (o altro linguaggio da concordare con il docente), il progetto richiede di applicare le strutture di dati e gli algoritmi sui grafi all'analisi delle reti sociali ([Social Network Analysis](http://en.wikipedia.org/wiki/Social_network)) utilizzando un particolare caso di studio: i dati disponibili sui film presenti nell'[Internet Movie DataBase](http://www.imdb.com/) (IMDB).

È possibile in tal modo definire implicitamente un grafo non diretto ![formula](https://render.githubusercontent.com/render/math?math=G=(V,E)) dove:
  * i vertici in ![formula](https://render.githubusercontent.com/render/math?math=V) sono gli attori e le attrici;
  * gli archi non orientati in ![formula](https://render.githubusercontent.com/render/math?math=E) collegano attori e attrici tra di loro: ![formula](https://render.githubusercontent.com/render/math?math=uv) appartiene a ![formula](https://render.githubusercontent.com/render/math?math=E) se e soltanto se ![formula](https://render.githubusercontent.com/render/math?math=u) e ![formula](https://render.githubusercontent.com/render/math?math=v) hanno recitato insieme in un film.

Il progetto si compone di varie fasi:
  - Scaricare e studiare il formato dei dati utilizzati da IMDB nel [sito ufficiale](http://www.imdb.com/interfaces). In particolare, ai fini del progetto sono necessari soltanto i seguenti file testuali, recuperabili al [link](https://datasets.imdbws.com/):
    - [name.basics.tsv.gz](https://datasets.imdbws.com/name.basics.tsv.gz):       lista di attori, attrici, ecc.
    - [title.basics.tsv.gz](https://datasets.imdbws.com/title.basics.tsv.gz):      lista di film
    - [title.principals.tsv.gz](https://datasets.imdbws.com/title.principals.tsv.gz):  chi ha lavorato in quali film
  - Costruire una rete sociale rappresentata tramite il suddetto grafo ![formula](https://render.githubusercontent.com/render/math?math=G=(V,E)) utilizzando tali file. Utilizzare le liste di adiacenza per memorizzare il grafo (ed evitare problemi di eccessiva occupazione di memoria).
  - Studiare la nozione di centralità dei nodi nei grafi, da applicare alle reti sociali, seguendo il materiale di [questo tutorial online](https://bachoseven.github.io/progettoGrafi/) e il contenuto di [questo paper](AxiomsForCentrality.pdf).
  - Progettare e implementare degli algoritmi per la centralità, selezionandoli da quanto studiato sopra. Evitare gli algoritmi puramente numerici, essendo quest'ultimi studiati in insegnamenti dedicati, preferendo gli algoritmi che usano la struttura combinatoria del grafo descritto sopra.


_Nota sul formato dei file._ I file che terminano in *.tsv (tab separated values), sono utilizzati come formato semplice di scambio: ogni riga testuale rappresenta un "record" i cui campi sono separati da una spaziatura tabulata (`\t` in C/C++). . Occorre quindi leggere una linea alla volta di ciascun file e separare i campi. Per esempio, possiamo reindirizzare l'input da tastiera in modo che il comando di lettura `cin` legga da file invece che da tastiera, usando il comando:

``` cpp
freopen("file.tsv", "r", stdin);
```

A questo punto possiamo leggere ogni riga di `file.tsv` come stringa C++ ed estrarne i campi separati dal delimitatore `\t`. Iterando più volte sulla riga, possiamo ottenere tutti i campi.

``` cpp
using namespace std;
string s;
cin >> s;
string delimitatore = “\t”;
string campo = s.substr(0, s.find(delimitatore));
```

Per decomprimere i file *.tsv.gz lanciare il comando `gunzip *.tsv.gz` Per scorrere un file *.tsv usare il comando "less" ("more" oppure "cat") seguito dal nome del file *.tsv, poiché alcuni editor potrebbero non essere in grado di aprire tali file.

Per vedere le prime N linee di un file *.tsv (o le ultime N righe) lanciare il comando "head -N" (oppure "tail -N") seguito dal nome del file *.tsv, dove N è appunto il numero di righe che si vuole esaminare. Per vedere i singoli caratteri, i tab, ecc., lanciare il comando "od -c" seguito dal nome del file *.tsv.
