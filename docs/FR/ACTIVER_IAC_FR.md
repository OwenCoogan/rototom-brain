# Activer le Pilote IAC - Guide de Dépannage (Français)

## Problème : "No MIDI output ports available!"

Si vous voyez cette erreur, cela signifie que le pilote IAC n'est pas activé ou n'est pas visible.

## Solution Étape par Étape

### Étape 1 : Ouvrir Configuration Audio MIDI

1. Ouvrez le **Finder**
2. Allez dans **Applications** > **Utilitaires**
3. Double-cliquez sur **Configuration Audio MIDI**
   - (En anglais : **Audio MIDI Setup**)

### Étape 2 : Afficher le Studio MIDI

1. Dans la barre de menus, cliquez sur **Fenêtre**
2. Sélectionnez **Afficher le Studio MIDI**
   - (En anglais : **Window** > **Show MIDI Studio**)

### Étape 3 : Chercher le Pilote IAC

Dans la fenêtre du Studio MIDI, cherchez :
- **Pilote IAC** (français)
- **IAC Driver** (anglais)
- **Gestionnaire IAC** (parfois en français)

### Étape 4 : Si le Pilote IAC est visible

1. **Double-cliquez** sur l'icône du Pilote IAC
2. Dans la fenêtre qui s'ouvre, **cochez la case** :
   - **"Périphérique en ligne"** (français)
   - **"Device is online"** (anglais)
3. Cliquez sur **Appliquer** (ou **Apply**)
4. **Fermez** Configuration Audio MIDI
5. **Relancez** le script Python

### Étape 5 : Si le Pilote IAC n'est PAS visible

#### Solution A : Créer le périphérique manuellement

1. Dans le Studio MIDI, cliquez sur le bouton **"+"** (plus) en bas
2. Sélectionnez **"Créer un périphérique externe"** ou **"Create External Device"**
3. Cherchez **"IAC Driver"** dans la liste
4. Si vous ne le trouvez pas, essayez de cliquer sur **"Ajouter un périphérique"**

#### Solution B : Réinitialiser les préférences MIDI

1. **Fermez** Configuration Audio MIDI
2. Ouvrez le **Terminal**
3. Tapez cette commande :

```bash
rm ~/Library/Preferences/com.apple.audio.audiomidisetup.plist
```

4. **Rouvrez** Configuration Audio MIDI
5. Le pilote IAC devrait maintenant apparaître

#### Solution C : Vérifier les permissions

1. Ouvrez **Réglages Système** (ou **Préférences Système** sur les anciennes versions)
2. Allez dans **Confidentialité et sécurité** > **Confidentialité**
3. Vérifiez que **Configuration Audio MIDI** a les permissions nécessaires

### Étape 6 : Vérifier que ça fonctionne

Après avoir activé le pilote IAC, testez avec cette commande dans le Terminal :

```bash
python3 -c "import rtmidi; m = rtmidi.MidiOut(); ports = m.get_ports(); print('Ports MIDI:', ports)"
```

Vous devriez voir quelque chose comme :
```
Ports MIDI: ['IAC Driver Bus 1']
```

Si vous voyez des ports MIDI, le pilote IAC est activé ! Relancez le script Python.

## Vérification Visuelle

Après activation, dans Configuration Audio MIDI > Studio MIDI, vous devriez voir :
- L'icône **Pilote IAC** avec un indicateur qu'il est actif
- **Pilote IAC Bus 1** dans la liste des périphériques

## Alternative : Utiliser un autre port MIDI virtuel

Si le pilote IAC ne fonctionne toujours pas, vous pouvez installer un autre port MIDI virtuel :

### Option 1 : LoopBe1 (Windows/Mac)
- Téléchargez depuis [nerds.de](https://www.nerds.de/)
- Crée un port MIDI virtuel

### Option 2 : MIDI Patchbay (Mac)
- Application tierce qui crée des ports MIDI virtuels

**Mais d'abord**, essayez de faire fonctionner le pilote IAC car c'est la solution la plus simple et intégrée à macOS.

## Si Rien Ne Fonctionne

Si après toutes ces étapes le pilote IAC ne fonctionne toujours pas :

1. **Redémarrez votre Mac** - parfois macOS a besoin d'un redémarrage pour reconnaître les changements MIDI
2. **Vérifiez votre version de macOS** - certaines versions ont des bugs avec le pilote IAC
3. **Utilisez le MIDI USB natif** à la place - c'est plus simple et ne nécessite pas le pilote IAC

## Commandes Utiles pour Vérifier

```bash
# Vérifier les ports MIDI disponibles
python3 -c "import rtmidi; m = rtmidi.MidiOut(); print(m.get_ports())"

# Vérifier si python-rtmidi fonctionne
python3 -c "import rtmidi; print('OK')"
```

