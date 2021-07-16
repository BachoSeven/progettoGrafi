#!/bin/sh

links="https://datasets.imdbws.com/name.basics.tsv.gz https://datasets.imdbws.com/title.principals.tsv.gz https://datasets.imdbws.com/title.basics.tsv.gz"
for l in $links; do
	curl -O $l &
done; wait

## Estraggo i film non adulti; salvo identificativi e nomi
zcat title.basics.tsv.gz | tail -n +2 | awk -F'\t' '$5 != 1' | cut -f1,3 > txt/FilmNonAdulti.txt

## Estraggo gli attori/attrici; salvo identificativi e nomi
zcat name.basics.tsv.gz | grep -E "actor|actress" | cut -f1,2 > txt/Attorə.txt

## Estraggo gli attori/attrici; salvo identificativi film-attore
zcat title.principals.tsv.gz | grep -E "actor|actress" | cut -f1,3 > txt/Relazioni.txt

nodi="$(wc -l txt/Attorə.txt)"
