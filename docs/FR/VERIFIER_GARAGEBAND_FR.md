# Comment Vérifier le MIDI dans GarageBand (Français/English)

## Méthode 1 : Si vous utilisez MIDI USB Natif

### Step 1: Open GarageBand
1. **Ouvrez GarageBand** (ou **Open GarageBand**)
2. Créez un nouveau projet ou ouvrez un projet existant

### Step 2: Create a Software Instrument Track
1. Cliquez sur **"+"** (plus) en haut à gauche pour ajouter une piste
2. Sélectionnez **"Instrument Logiciel"** ou **"Software Instrument"**
3. Choisissez un instrument (piano, synthé, etc.)

### Step 3: Select MIDI Input
1. **Sélectionnez la piste** que vous venez de créer
2. Cliquez sur l'icône **⚙️** (engrenage/settings) sur la piste
3. Dans les paramètres, cherchez **"Entrée MIDI"** ou **"MIDI Input"**
4. **Sélectionnez votre ESP32-S3** dans la liste
   - Il devrait apparaître comme **"ESP32-S3 MIDI"** ou un nom similaire
   - Si vous ne le voyez pas, voir la section "Dépannage" ci-dessous

### Step 4: Test MIDI
1. **Appuyez sur le bouton MIDI** de votre ESP32-S3
2. **Vous devriez entendre un son** dans GarageBand !
3. Vous devriez aussi voir l'indicateur de niveau audio bouger

---

## Méthode 2 : Si vous utilisez le Pont Python (IAC Driver)

### Step 1: Make Sure Python Bridge is Running
1. **Ouvrez le Terminal**
2. Vérifiez que le script Python est en cours d'exécution :
   ```bash
   python3 serial_midi_bridge.py /dev/cu.usbmodem1301 115200
   ```
3. Vous devriez voir : `Bridge is running! Press Ctrl+C to stop.`

### Step 2: Open GarageBand
1. **Ouvrez GarageBand**
2. Créez un nouveau projet ou ouvrez un projet existant

### Step 3: Create a Software Instrument Track
1. Cliquez sur **"+"** (plus) en haut à gauche
2. Sélectionnez **"Instrument Logiciel"** ou **"Software Instrument"**
3. Choisissez un instrument

### Step 4: Select IAC Driver as MIDI Input
1. **Sélectionnez la piste**
2. Cliquez sur l'icône **⚙️** (engrenage/settings)
3. Sous **"Entrée MIDI"** ou **"MIDI Input"**, sélectionnez :
   - **"Pilote IAC Bus 1"** (français)
   - **"IAC Driver Bus 1"** (anglais)

### Step 5: Test MIDI
1. **Appuyez sur le bouton MIDI** de votre ESP32-S3
2. **Vérifiez le Terminal** - vous devriez voir :
   ```
   MIDI: Note On - Note=60, Velocity=...
   ```
3. **Vous devriez entendre un son** dans GarageBand !

---

## Vérification Visuelle dans GarageBand

### Indicateurs à Vérifier :

1. **Indicateur de niveau audio** (VU meter)
   - Devrait bouger quand vous envoyez des notes MIDI
   - Se trouve à côté du nom de la piste

2. **Indicateur MIDI** (si disponible)
   - Certaines versions de GarageBand montrent un indicateur quand des messages MIDI sont reçus

3. **Piano Roll**
   - Ouvrez le Piano Roll (double-cliquez sur la région de la piste)
   - Les notes MIDI devraient apparaître si vous enregistrez

---

## Dépannage : Le MIDI ne Fonctionne Pas

### Problème : L'ESP32 n'apparaît pas dans la liste MIDI

**Pour MIDI USB Natif :**
1. Débranchez et rebranchez le câble USB
2. Attendez 2-3 secondes
3. Redémarrez GarageBand
4. Vérifiez dans **Configuration Audio MIDI** > **Studio MIDI** si l'ESP32 apparaît
5. Vérifiez que le code est bien uploadé sur l'ESP32

**Pour le Pont Python :**
1. Vérifiez que le script Python est en cours d'exécution
2. Vérifiez que le Pilote IAC est activé (voir `ACTIVER_IAC_FR.md`)
3. Redémarrez GarageBand

### Problème : Pas de son mais les messages MIDI sont envoyés

1. **Vérifiez le volume** :
   - Volume de la piste dans GarageBand
   - Volume du Mac
   - Volume de l'instrument

2. **Vérifiez qu'un instrument est chargé** :
   - La piste doit avoir un instrument sélectionné
   - Essayez de changer d'instrument

3. **Vérifiez que la piste n'est pas en sourdine** :
   - Le bouton "M" (Mute) ne doit pas être activé

4. **Vérifiez les paramètres MIDI** :
   - L'entrée MIDI est bien sélectionnée
   - Le canal MIDI correspond (généralement canal 1)

### Problème : Les messages MIDI n'arrivent pas

1. **Vérifiez le moniteur série** (PlatformIO Serial Monitor) :
   - Vous devriez voir : `MIDI: Note On - note=60, velocity=...`
   - Si vous ne voyez rien, le problème vient de l'ESP32

2. **Vérifiez le Terminal** (si vous utilisez le pont Python) :
   - Vous devriez voir les messages MIDI affichés
   - Si vous ne voyez rien, le problème vient de la connexion série

3. **Vérifiez la connexion USB** :
   - Le câble supporte le transfert de données
   - Essayez un autre port USB

---

## Test Rapide

### Test Simple :
1. Ouvrez GarageBand
2. Créez une piste d'instrument logiciel
3. Sélectionnez l'entrée MIDI (ESP32 ou IAC Driver)
4. Chargez un instrument (piano)
5. Appuyez sur le bouton MIDI de l'ESP32
6. **Vous devriez entendre un son !**

### Test avec Moniteur Série :
1. Ouvrez PlatformIO Serial Monitor
2. Appuyez sur le bouton MIDI
3. Vous devriez voir : `MIDI: Note On - note=60, velocity=...`
4. Si vous voyez ce message, l'ESP32 fonctionne correctement

---

## Vérification Avancée

### Vérifier dans Configuration Audio MIDI :

1. Ouvrez **Configuration Audio MIDI** (Applications > Utilitaires)
2. Cliquez sur **Fenêtre** > **Afficher le Studio MIDI**
3. **Pour MIDI USB Natif** : Votre ESP32 devrait apparaître dans la liste
4. **Pour le Pont Python** : Le Pilote IAC devrait être visible et actif

### Vérifier les Ports MIDI Disponibles :

Dans le Terminal :
```bash
# Vérifier les ports MIDI (pour le pont Python)
python3 -c "import rtmidi; m = rtmidi.MidiOut(); print('Ports:', m.get_ports())"
```

Vous devriez voir `['IAC Driver Bus 1']` ou similaire.

---

## Checklist de Vérification

Avant de tester dans GarageBand, vérifiez :

- [ ] Le code est uploadé sur l'ESP32
- [ ] Le moniteur série montre des messages MIDI
- [ ] (Pour pont Python) Le script Python est en cours d'exécution
- [ ] (Pour pont Python) Le Pilote IAC est activé
- [ ] GarageBand est ouvert
- [ ] Une piste d'instrument logiciel est créée
- [ ] Un instrument est chargé sur la piste
- [ ] L'entrée MIDI est correctement sélectionnée
- [ ] Le volume n'est pas à zéro
- [ ] La piste n'est pas en sourdine

Si tous ces points sont vérifiés et que ça ne fonctionne toujours pas, consultez les guides de dépannage :
- `../EN/MIDI_TROUBLESHOOTING.md`
- `ACTIVER_IAC_FR.md`
- `VERIFICATION_MIDI_FR.md`
