#!/bin/sh

links="https://datasets.imdbws.com/name.basics.tsv.gz https://datasets.imdbws.com/title.principals.tsv.gz https://datasets.imdbws.com/title.basics.tsv.gz"

for l in $links; do
	# download paralleli eseguendo il processo in background
	curl -O -C - $l &
done; wait

zcat title.basics.tsv.gz |\
	tail -n +2 |\
	# Estraggo i film non adulti
	awk -F'\t' '$5 != 1' |\
	# Salvo identificativi e nomi dei film
	cut --output-delimiter=' ' -f1,3 |\
	# Utilizzo `sed` per rendere l'output più leggibile
	sed 's/^tt0*//g' > txt/FilmFiltrati.txt

zcat name.basics.tsv.gz |\
	# Estraggo solo gli attori/attrici
	awk -F'\t' '$5 ~ "actor" || $5 ~ "actress"' |\
	# Salvo identificativi ed i nomi
	cut --output-delimiter=' ' -f1,2 |\
	# Utilizzo `sed` per rendere l'output più leggibile
	sed 's/^nm0*//g' > txt/Attorə.txt

zcat title.principals.tsv.gz |\
	# Estraggo gli attori/attrici da title.principals
	awk -F'\t' '$4 == "actor" || $4 == "actress"' |\
	# Salvo identificativi film-attore
	cut --output-delimiter=' ' -f1,3 |\
	# Utilizzo `sed` per rendere l'output più leggibile (e coerente con FilmFiltrati.txt)
	sed 's/^tt0*//g;s/nm0*//g' |\
	# Rimuovo i film adulti pure da qui, utilizzando FilmFiltrati.txt
	awk 'FNR==NR { A[$1]; next } $1 in A' txt/FilmFiltrati.txt - > txt/Relazioni.txt

# numero di archi (con possibili archi duplicati) [NON HA SENSO]
echo "$(cat txt/Relazioni.txt | cut -d' ' -f2 | sort | uniq | wc -l | cut -f1 -d' ')" > txt/info.txt
# numero di nodi
echo "$(wc -l txt/Attorə.txt | cut -f1 -d' ')" >> txt/info.txt
