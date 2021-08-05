#!/bin/sh

updatelinks() {
	links="https://datasets.imdbws.com/name.basics.tsv.gz https://datasets.imdbws.com/title.principals.tsv.gz https://datasets.imdbws.com/title.basics.tsv.gz"

	for l in $links; do
		# download paralleli eseguendo il processo in background
		wget --continue $l &
	done; wait
}


main() {
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

	# valore massimo di un identificativo di un attore dentro Relazioni.txt
	echo "$(cut -d' ' -f2 txt/Relazioni.txt | sort --numeric-sort | tail -1)" > txt/info.txt

	# numero di relazioni totali
	# echo "$(wc -l txt/Relazioni.txt | cut -f1 -d' ')"

	# numero di nodi (solo gli attori che compaiono in film in FilmFiltrati.txt) [sono circa la metà degli attori!]
	# echo "$(cut -d' ' -f2 txt/Relazioni.txt | sort | uniq | wc -l | cut -f1 -d' ')" # Alternativa a sort|uniq: awk '!seen[$0]++'

	## massimo numero di attori in un unico film (è 10)
	# cut -f1 -d' ' txt/Relazioni.txt | uniq -c | awk '{if ($1>x) x=$1} END {print x}'

	## Per splittare ogni film (ma anche no)
	# awk -F' ' '{print>$1}' Relazioni.txt
}

# updatelinks
main
