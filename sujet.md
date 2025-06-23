# 🧠 Atelier VMV – Reverse Engineering d’un jeu C++ (SFML)

Bienvenue dans cet atelier où vous allez **analyser, tester et démonter** un petit programme en C++... **sans avoir accès au code source** au départ. L’objectif : **débusquer les mécanismes cachés** d’un jeu de labyrinthe !

---

# 🎯 Objectif de l’atelier

Vous partez d’un **exécutable** (compilé) d’un jeu simple en apparence… mais qui contient :
* Des zones invisibles secrètes
* Des combinaisons de touches spéciales
* Des variables jamais affichées
* Des comportements inattendus

Votre mission : **comprendre et révéler tous les secrets du jeu**, à la fois **en jouant**, puis **en analysant le binaire avec Ghidra**.

---

## 📁 Matériel fourni

* Le fichier exécutable : `maze-win64.exe` (pour Windows)
* Aucun accès au code source
* Outils recommandés :
  * [Ghidra, Binary Ninja](https://dogbolt.org) (accessibles en ligne)
  * Bloc-notes ou fichier `.txt` pour noter vos observations

---

# 🕹️ Partie 1 – Exécution naïve du jeu

## 🔄 Étape 1 : Lancer et jouer

1. Lancez le jeu.
2. Jouez normalement. Tentez d'atteindre la sortie.
3. Notez :
   * les touches utilisées
   * l’apparence de la carte
   * les comportements visibles
   * les messages de victoire

**→ Objectif** : comprendre le fonctionnement du gameplay **sans rien savoir du code**.

---

## 🧪 Étape 2 : Tester des comportements étranges

Testez les idées suivantes :

* Et si je me cogne contre les murs ?
* Et si je reste appuyé ?
* Puis-je passer dans des cases vides bizarres ?
* Et si je tape des touches aléatoires (A, B, haut, bas, etc.) ?
* Que se passe-t-il **dans les coins de la map** ?
* Y a-t-il un feedback caché dans la console ? (Regarder les sorties texte !)

---

## 🧩 Étape 3 : Déduction

**À ce stade, tentez de deviner** :

* Y a-t-il des zones spéciales ?
* Un comportement caché ?
* Des touches ayant un effet caché ?
* Un cheat code ?
* Une fin alternative ?

---


# 🕵️ Partie 2 – Analyse statique avec Ghidra

## 🛠️ Étape 1 : Importer le binaire

1. Ouvrez **Ghidra**.
2. Créez un projet.
3. Importez le binaire du jeu.
4. Lancez l’analyse automatique (`YES` à toutes les options par défaut).

---

## 🧠 Étape 2 : Observation globale

Repérez :

* Le point d’entrée (`main`)
* Les chaînes de caractères visibles (`Window`, `Maze`, `Félicitations`, etc.)
* Les **fonctions non appelées**
* Les **variables globales** 

---

## 🔍 Étape 3 : Analyse des fonctions

Tentez d’analyser les fonctions qui constituent ce programme

### Questions à résoudre :

* Quelles touches déclenchent un effet ?
* Que se passe-t-il si une variable est à `true` ?
* Quelle est la condition d’affichage de `"secret ending"` ?
* Est-ce qu’une condition jamais atteinte pourrait être forcée (ex: patch mémoire) ?

---

# 🧠 Partie 3 – Exploitation et synthèse

## ✨ Objectif final

**Révélez tous les secrets du jeu** :

* Où sont les zones secrètes ?
* Quelle est la combinaison spéciale ?
* Que contient la variable `hiddenMessage` ?
* Que se passe-t-il si toutes les zones secrètes sont activées ?
* Peut-on déclencher manuellement la fin secrète via un patch ou cheat engine ?

---

## 💡 Défis bonus (facultatif)

* Modifier le binaire avec un éditeur hexadécimal pour afficher `"debug mode activated"`
* Forcer une variable booléenne à `true` avec CheatEngine
* Recompiler un binaire modifié (si vous avez réussi à reconstruire le code avec Ghidra de manière propre)

