#!/bin/sh

links="https://datasets.imdbws.com/name.basics.tsv.gz https://datasets.imdbws.com/title.principals.tsv.gz https://datasets.imdbws.com/title.basics.tsv.gz"

for l in $links; do
	# -O	: scrivi su file con nome dato dal server
	# -C -: continua da dove eri rimasto
	# &		: esegui i download in background --> paralleli
	curl -O $l -C - &
done; wait

## Estraggo i film non adulti
## Salvo identificativi e nomi dei film
zcat title.basics.tsv.gz | tail -n +2 | awk -F'\t' '$5 != 1' | cut --output-delimiter=' ' -f1,3 > txt/FilmNonAdulti.txt

## Estraggo solo gli attori/attrici
## Salvo identificativi ed i nomi
zcat name.basics.tsv.gz | grep -E "actor|actress" | cut --output-delimiter=' ' -f1,2 > txt/Attorə.txt

## Estraggo gli attori/attrici da title.principals
## Salvo identificativi film-attore
## Infine, rimuovo i film adulti pure da qui, utilizzando FilmNonAdulti.txt
zcat title.principals.tsv.gz | grep -E "actor|actress" | cut --output-delimiter=' ' -f1,3 | awk 'FNR==NR { A[$1]; next } $1 in A' txt/FilmNonAdulti.txt - > txt/Relazioni.txt

nodi="$(wc -l txt/Attorə.txt)"
