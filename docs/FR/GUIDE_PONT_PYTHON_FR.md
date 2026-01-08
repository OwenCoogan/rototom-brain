# Guide : Utiliser le Pont Python MIDI (Français)

Ce guide explique comment utiliser le script Python pour faire fonctionner votre ESP32-S3 avec GarageBand via un pont MIDI.

## Prérequis

### 1. Installer Python

Vérifiez que Python 3 est installé sur votre Mac :

```bash
python3 --version
```

Si Python n'est pas installé, téléchargez-le depuis [python.org](https://www.python.org/downloads/)

### 2. Installer les bibliothèques nécessaires

Ouvrez le **Terminal** et tapez :

```bash
pip3 install pyserial python-rtmidi
```

Si vous obtenez une erreur de permissions, utilisez :

```bash
pip3 install --user pyserial python-rtmidi
```

## Étape 1 : Modifier le code pour utiliser Serial MIDI

**Important** : Le pont Python fonctionne avec Serial MIDI, pas avec USB MIDI natif. Si votre code utilise actuellement USB MIDI natif, vous devez le modifier.

### Comment modifier le code

1. **Sauvegardez votre code actuel** (au cas où vous voudriez revenir en arrière)

2. **Remplacez le contenu de `src/midi.cpp`** par la version Serial MIDI

   Vous pouvez :
   - Copier le contenu de `src/midi_serial.cpp.example` dans `src/midi.cpp`
   - Ou modifier manuellement `src/midi.cpp` pour utiliser Serial au lieu de USB MIDI

3. **Recompilez et uploadez** le code :

   ```bash
   pio run --target upload
   ```

### Vérification

Après avoir uploadé le code, ouvrez le moniteur série. Vous devriez voir :
```
=== Serial MIDI Initialized ===
Use Python bridge script to connect to GarageBand
```

Si vous voyez "Native USB MIDI Initialized", le code n'a pas été modifié correctement.

## Étape 2 : Activer le Pilote IAC (si nécessaire)

1. Ouvrez **Configuration Audio MIDI** (Applications > Utilitaires)
2. Cliquez sur **Fenêtre** > **Afficher le Studio MIDI**
3. Double-cliquez sur **Pilote IAC** (ou **IAC Driver**)
4. Cochez **Périphérique en ligne**
5. Cliquez sur **Appliquer**

> **Note** : Si vous ne voyez pas le Pilote IAC, consultez `IAC_DRIVER_FR.md` pour les solutions.

## Étape 3 : Trouver le port série de votre ESP32

1. **Branchez votre ESP32-S3** à votre Mac via USB
2. Ouvrez le **Terminal**
3. Tapez cette commande pour lister les ports série :

```bash
ls /dev/cu.usbmodem* /dev/cu.SLAB_USBtoUART* 2>/dev/null
```

Vous devriez voir quelque chose comme :
```
/dev/cu.usbmodem14103
```

ou

```
/dev/cu.SLAB_USBtoUART
```

**Notez le nom exact du port** - vous en aurez besoin pour l'étape suivante.

## Étape 4 : Lancer le pont Python

1. Ouvrez le **Terminal**
2. Naviguez vers le dossier de votre projet :

```bash
cd /Users/owencoogan/Desktop/rototom-brain
```

3. Lancez le script Python avec votre port série :

```bash
python3 serial_midi_bridge.py /dev/cu.usbmodem14103 115200
```

**Remplacez `/dev/cu.usbmodem14103`** par le port que vous avez trouvé à l'étape 3.

### Exemple complet :

```bash
cd /Users/owencoogan/Desktop/rototom-brain
python3 serial_midi_bridge.py /dev/cu.usbmodem14103 115200
```

### Si le script ne trouve pas automatiquement le port :

Vous pouvez aussi lancer le script sans arguments, et il essaiera de trouver automatiquement le port :

```bash
python3 serial_midi_bridge.py
```

## Étape 5 : Vérifier que le pont fonctionne

Quand le script démarre, vous devriez voir :

```
Connected to /dev/cu.usbmodem14103 at 115200 baud
MIDI output connected to: IAC Driver Bus 1

Bridge is running! Press Ctrl+C to stop.

MIDI messages from ESP32-S3 will be forwarded to: IAC Driver Bus 1
Make sure to select this MIDI port in GarageBand.
```

**Laissez ce terminal ouvert** - le pont doit rester actif pour fonctionner.

## Étape 6 : Configurer GarageBand

1. **Ouvrez GarageBand**
2. **Créez une nouvelle piste** d'instrument logiciel
3. **Sélectionnez la piste** et cliquez sur l'icône d'engrenage (⚙️)
4. Sous **Entrée MIDI**, sélectionnez **Pilote IAC Bus 1** (ou **IAC Driver Bus 1**)
5. **Chargez un instrument** sur la piste (piano, synthé, etc.)

## Étape 7 : Tester

1. **Appuyez sur le bouton MIDI** de votre ESP32-S3
2. Vous devriez voir dans le terminal :
   ```
   MIDI: Note On - Note=60, Velocity=...
   ```
3. **Vous devriez entendre un son** dans GarageBand !

## Dépannage

### Le script ne trouve pas le port série

- Vérifiez que l'ESP32 est bien branché
- Essayez de débrancher et rebrancher le câble USB
- Vérifiez que le port n'est pas utilisé par un autre programme (comme le moniteur série de PlatformIO)
- Fermez PlatformIO Serial Monitor si il est ouvert

### Erreur "No module named 'pyserial'"

Installez les dépendances :
```bash
pip3 install pyserial python-rtmidi
```

### Erreur "No MIDI output ports available"

1. Vérifiez que le Pilote IAC est activé (Étape 2)
2. Redémarrez Configuration Audio MIDI
3. Redémarrez le script Python

### Pas de son dans GarageBand

- Vérifiez que vous avez sélectionné **Pilote IAC Bus 1** comme entrée MIDI
- Vérifiez qu'un instrument est chargé sur la piste
- Vérifiez le volume de la piste dans GarageBand
- Vérifiez que le script Python est toujours en cours d'exécution

### Le script se ferme immédiatement

- Vérifiez que le port série est correct
- Vérifiez que le baud rate correspond (115200 par défaut)
- Vérifiez les messages d'erreur dans le terminal

## Arrêter le pont

Pour arrêter le pont Python, appuyez sur **Ctrl+C** dans le terminal.

## Automatiser le démarrage (Optionnel)

Si vous voulez que le pont démarre automatiquement, vous pouvez créer un script shell :

1. Créez un fichier `start_midi_bridge.sh` :

```bash
#!/bin/bash
cd /Users/owencoogan/Desktop/rototom-brain
python3 serial_midi_bridge.py /dev/cu.usbmodem14103 115200
```

2. Rendez-le exécutable :

```bash
chmod +x start_midi_bridge.sh
```

3. Lancez-le :

```bash
./start_midi_bridge.sh
```

## Comparaison : Pont Python vs MIDI USB Natif

| Fonctionnalité | Pont Python | MIDI USB Natif |
|---------------|-------------|----------------|
| Configuration | Nécessite Python + IAC | Aucune configuration |
| Complexité | Plus complexe | Plus simple |
| Fiabilité | Dépend du script | Plus fiable |
| Latence | Légèrement plus élevée | Plus faible |
| Débogage | Messages dans terminal | Messages série |

**Recommandation** : Utilisez le MIDI USB natif si possible (plus simple). Utilisez le pont Python seulement si le MIDI USB natif ne fonctionne pas.

## Besoin d'aide ?

Si vous rencontrez des problèmes :
1. Vérifiez les messages dans le terminal du script Python
2. Vérifiez le moniteur série de PlatformIO pour voir si l'ESP32 envoie des messages
3. Consultez `IAC_DRIVER_FR.md` pour les problèmes avec le Pilote IAC
4. Consultez `VERIFICATION_MIDI_FR.md` pour vérifier le MIDI USB natif
