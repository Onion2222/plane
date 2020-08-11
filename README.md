# PLANE
## Description

Il s'agit d'un mini-jeu à exécuter dans le terminal.
Pour l'instant, le programme n'a été testé que sur Bash, veuillez toutefois faire attention aux dimensions du terminal qui peuvent empêcher le programme de s'afficher correctement (modifier hauteur_win  et largeur_win pour adapter la fenetre du jeu à votre terminal).

## Jeu

Vous êtes au contrôle d'un avion embarquant 500 obus. Le but du jeu est de détruire tous les véhicules ennemis au sol en les bombardant.

Contrôle:
- **Touches Directionnelles** pour contrôler l'avion
- **Espace** pour lâcher un obus
- **R** pour regénerer le terrain
- **F1** pour afficher le debug

## Installation
Vous avez besoin de la bibliothèque ncurse pour compiler le programme.
### Ubuntu (apt-get):
Installer ncurses:
```
$ sudo apt-get install libncurses5-dev libncursesw5-dev
```
Compiler:
```
$ make
```
Et voilà.
