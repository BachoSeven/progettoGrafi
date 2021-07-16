#!/bin/sh

## Estraggo i film non adulti; salvo identificativi e nomi
zcat title.basics.tsv.gz | tail -n +2 | awk -F'\t' '$5 != 1' | cut -f1,3 > txt/FilmNonAdulti.txt

## Estraggo gli attori/attrici; salvo identificativi e nomi
zcat name.basics.tsv.gz | tail -n +2 | grep -E "actor|actress" | cut -f1,2 > txt/Attorə.txt

## Estraggo gli attori/attrici; salvo identificativi film-attore
zcat title.principals.tsv.gz | tail -n +2 | grep -E "actor|actress" | cut -f1,3 > txt/Relazioni.txt
