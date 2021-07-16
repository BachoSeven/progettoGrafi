#!/bin/sh

links="https://datasets.imdbws.com/name.basics.tsv.gz https://datasets.imdbws.com/title.principals.tsv.gz https://datasets.imdbws.com/title.basics.tsv.gz"

for l in $links; do
	curl \
		# scrivi su file con nome dato dal server
		-O   \
		# continua da dove eri rimasto
		-C - \
		# esegui i download in background --> paralleli
		$l &
done; wait

zcat title.basics.tsv.gz |\
	tail -n +2 |\
	# Estraggo i film non adulti
	awk -F'\t' '$5 != 1' |\
	# Salvo identificativi e nomi dei film
	cut --output-delimiter=' ' -f1,3 |\
	# Utilizzo `sed` per rendere l'output più leggibile
	sed 's/^tt//g;s/^0*//g' > txt/FilmNonAdulti.txt

zcat name.basics.tsv.gz |\
	# Estraggo solo gli attori/attrici
	grep -E "actor|actress" |\
	# Salvo identificativi ed i nomi
	cut --output-delimiter=' ' -f1,2 |\
	# Utilizzo `sed` per rendere l'output più leggibile
	sed 's/^nm//g;s/^0*//g' > txt/Attorə.txt

zcat title.principals.tsv.gz |\
	# Estraggo gli attori/attrici da title.principals
	grep -E "actor|actress" |\
	# Salvo identificativi film-attore
	cut --output-delimiter=' ' -f1,3 |\
	# Rimuovo i film adulti pure da qui, utilizzando FilmNonAdulti.txt
	awk 'FNR==NR { A[$1]; next } $1 in A' txt/FilmNonAdulti.txt - |\
	# Utilizzo `sed` per rendere l'output più leggibile
	sed 's/^tt//g;s/^0*//g' > txt/Relazioni.txt

nodi="$(wc -l txt/Attorə.txt)"
