# Mastermind

## Développeuse

- [Sofia CETIN](https://code.up8.edu/scetin)



## Présentation du Mastermind

Le jeu du Mastermind est un jeu ayant pour but de trouver une combinaison de quatre couleurs. Le jeu se joue habituellement à deux joueurs avec l'un d'entre eux établissant la combinaison, et l'autre devant la deviner. 

Cependant, dans le cadre de ce projet, le jeu se joue à un joueur et la combinaison est générée aléatoirement par le programme. L'interface fonctionne via un style drag-and-drop: le joueur doit glisser une couleur dans une des positions de l'essai. Attention toutefois, un essai ne pourra être validé s'il comporte plusieurs fois la même couleur ou qu'une position n'est pas remplie.

Tout au long des manches, le joueur aura la possibilité de voir combien de couleurs sont correctes dans ses essais(à gauche), et combien de couleurs sont correctes et bien placées(à droite).

<img src="assets/readme_image1.png" width="300" alt="Une image exemple du jeu">

_Un aperçu du jeu_

## Prérequis

### Simple Direct Media Layer 3

Ce programme nécessite les packages [SDL3](https://wiki.libsdl.org/SDL3/FrontPage) et [SDL3_ttf](https://wiki.libsdl.org/SDL3_ttf/FrontPage) afin de fonctionner.

Avec Homebrew sur MacOS, vous pouvez lancer les commandes:

```bash
brew install sdl3
brew install sdl3_ttf
```

Pour les autres plateformes, vous pouvez consulter la [documentation officielle de SDL](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md).

## Compilation et exécution

### Compiler avec GCC

Le makefile inclut dans le dépôt permet de directement compiler le programme, à condition d'avoir GCC et tous les prérequis nécessaires installés sur votre machine. 

Positionnez-vous dans le répertoire **mastermind/** et lancez la commande:

```bash
make
```

Vous pouvez également supprimer l'exécutable généré avec:

```bash
make clean
```

### Lancer le programme

Vous pouvez donc lancer le programme avec l'exécutable.
a
 Vous avez également l'option, si vous le souhaitez, d'indiquer en argument de l'exécutable d'afficher la combinaison correcte dans le terminal afin de faciliter le débogage.

```bash
./mastermind.out test
```