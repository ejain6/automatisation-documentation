#!/bin/bash

#Creation du volume sae103
docker volume create sae103

#On run le conteneur Clock en tant que sae103-forever et on lui monte le volume sae103
docker container run --name sae103-forever -d -v sae103:/work bigpapoo/clock

#Tranfère des fichiers C et du doc.md et les css vers le volume
liste_fichiers_c=$(ls *.c)
for fichier_c in $liste_fichiers_c
do
    docker container cp $fichier_c sae103-forever:/work
done
docker container cp doc.md sae103-forever:/work

#Transfer des fichiers gendoc-tech.php, gendoc-user.php, les css et config
docker container cp ressources/gendoc-tech.php sae103-forever:/work
docker container cp ressources/gendoc-user.php sae103-forever:/work
docker container cp ressources/config sae103-forever:/work
docker container cp ressources/style.css sae103-forever:/work
docker container cp ressources/style_doc_user.css sae103-forever:/work

docker exec sae103-forever sh /work chmod +x gendoc-user.php
docker exec sae103-forever sh /work chmod +x gendoc-tech.php


#Création du conteneur sae103-php
docker image pull bigpapoo/sae103-php
docker container run --rm --name sae103-php -v sae103:/work bigpapoo/sae103-php:latest php gendoc-tech.php --build
docker container run --rm --name sae103-php -v sae103:/work bigpapoo/sae103-php:latest php gendoc-user.php doc.md

#Création du conteneur sae103html2pdp
docker image pull bigpapoo/sae103-html2pdf

docker container cp sae103-forever:/work/config .
version1=$(cut -d = -f 2 config | tail -n 1 | tr . -)

docker container run --rm --name sae103-html2pdf -v sae103:/work bigpapoo/sae103-html2pdf:latest weasyprint /work/doc-tech-${version1}.html /work/DOC-TECHNIQUE.pdf
docker container run --rm --name sae103-html2pdf -v sae103:/work bigpapoo/sae103-html2pdf:latest weasyprint /work/doc-user-${version1}.html /work/DOC-UTILISATEUR.pdf

#Copie les fichiers pour faire l'archive
rm -r doss_archive
mkdir doss_archive

docker container cp sae103-forever:/work/doc-tech-${version1}.html doss_archive
docker container cp sae103-forever:/work/doc-user-${version1}.html doss_archive
docker container cp sae103-forever:/work/src1.c doss_archive
docker container cp sae103-forever:/work/src2.c doss_archive
docker container cp sae103-forever:/work/src3.c doss_archive
docker container cp sae103-forever:/work/DOC-TECHNIQUE.pdf doss_archive
docker container cp sae103-forever:/work/DOC-UTILISATEUR.pdf doss_archive
docker container cp sae103-forever:/work/style_doc_user.css doss_archive
docker container cp sae103-forever:/work/style.css doss_archive

nom=$(cut -d = -f 2 config | head -n 1 | tr " " _ | tr "ABCDEFGHIJKLMNOPQRSTUVW" "abcdefghijklmnopqrstuvwxyz")
version2=$(cut -d = -f 2 config | tail -n 1)

tar czvf ${nom}-${version2}.tar.gz ./doss_archive

docker container stop sae103-forever
docker container rm sae103-forever
docker volume rm sae103
