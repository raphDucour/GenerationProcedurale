# GenerationProcedural

Developed with Unreal Engine 5

## 🏰 Procedural Dungeon Generator (C++)
Système de génération de donjons 3D par assemblage de modules (Blueprints) avec gestion intelligente des collisions et des probabilités.

### ⚙️ Logique de Génération (`AGenerator.cpp`)
* **Assemblage par Connecteurs :** Utilise les `FTransform` de points d'ancrage (`UArrowComponent`) pour aligner parfaitement l'entrée d'une nouvelle salle sur la sortie d'une précédente.
* **Système de Probabilités :** Algorithme de sélection pondérée (`PickRoomIndexByProbability`) permettant de contrôler la rareté de chaque type de salle.
* **Contraintes de Spawn :** * `bUnique` : Supprime la salle de la liste après spawn (ex: salle de Boss).
    * `bMustSpawn` : Garantit l'apparition des salles critiques en fin de génération.
* **Gestion des "Open Exits" :** Liste dynamique stockant les sorties disponibles pour garantir une expansion organique sans impasses prématurées.

### 🛡️ Validation & Physique
* **Détection d'Overlap :** Avant validation, chaque salle utilise un `UBoxComponent` pour vérifier les collisions avec les salles existantes via `GetOverlappingActors`.
* **Auto-Correction :** En cas de collision détectée, l'acteur est immédiatement détruit et le générateur tente un nouvel embranchement pour garantir un donjon sans bugs visuels.

### 🏗️ Architecture des Salles (`ARoom.cpp`)
* **EntryPoint & Exits :** Hiérarchie de composants `SceneComponent` pour définir les flux de navigation.
* **Base Blueprint :** Classe parente C++ flexible permettant de créer une infinité de variantes visuelles tout en conservant la logique de connexion.
