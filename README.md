# Outil de documentation automatisé pour programmes C

## Réalisation du projet
Ce projet, réalisé lors de ma 1ère année de BUT informatique, avait pour objectif de créer un outil de documentation automatisé pour des programmes en langage C, par équipe de 3 personnes

### Nous avons donc réalisé les taches suivantes : 

- Recherche de la syntaxe des outils de documentation afin d'élaborer la notre (style Doxygen)
- Recherche et documentation de fichiers sources complets sur lesquels travailler (dont un [snake](/src1.c) et un [puissance 4](/src2.c))
- Maquettage des pages de documentation créées à partir des fichiers sources et de la doc utilisateur
- Création d'une [documentation utilisareur](/doc.md) (MAJ au fur et a mesure des changements)
- Création d'un script de [génération d'une doc utilisateur html](/ressources/gendoc-user.php) en fonction de la [documentation utilisareur](/doc.md) (format markdown)
- Création d'un script de [génération d'une doc technique html](/ressources/gendoc-tech.php) en fonction des fichiers sources
- Créarion d'un script bash qui va :
  - Créer un volume et y attacher un conteneur qui boucle dans le vide
  - Copier les fichiers C sources du répertoire courant dans le volume ainsi que les fichisers du dossier [ressources](/ressources/)
  - Executer les programmes de génération de documentation html évoqués plus tôt
  - Créer le dossier [doss_archive](/doss_archive/) et y mettre les fichiers nécessaires
  - Créer une archive, contenant les paramètres donnés dans le fichier [config](/ressources/config)
  - Fermer et supprimer les conteneurs et volumes utilisés proprement

