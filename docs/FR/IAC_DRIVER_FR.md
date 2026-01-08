# Configuration du Pilote IAC sur macOS (Français)

## ⚠️ IMPORTANT : Vérifiez d'abord votre configuration !

**Si votre ESP32-S3 utilise le MIDI USB natif** (implémenté avec Adafruit TinyUSB), vous **N'AVEZ PAS BESOIN** du pilote IAC ! 

Votre ESP32 devrait apparaître directement comme un périphérique MIDI dans GarageBand. Vérifiez d'abord dans GarageBand si votre ESP32 apparaît dans la liste des périphériques MIDI avant de configurer le pilote IAC.

Le pilote IAC n'est nécessaire **QUE** si vous utilisez le pont Python (Serial MIDI). Si vous utilisez le MIDI USB natif, passez directement à la section "Utilisation avec GarageBand" ci-dessous.

---

## Qu'est-ce que le Pilote IAC ?

Le **Pilote IAC** (Inter-Application Communication Driver) est un pilote MIDI virtuel intégré à macOS qui permet aux applications de communiquer entre elles via MIDI. C'est essentiel pour faire fonctionner le pont MIDI Python avec GarageBand.

## Configuration étape par étape

### 1. Ouvrir Configuration Audio MIDI

- Ouvrez le **Finder**
- Allez dans **Applications** > **Utilitaires**
- Double-cliquez sur **Configuration Audio MIDI**
  - (En anglais : **Audio MIDI Setup**)

### 2. Afficher le Studio MIDI

- Dans la barre de menus en haut, cliquez sur **Fenêtre**
- Sélectionnez **Afficher le Studio MIDI**
  - (En anglais : **Window** > **Show MIDI Studio**)

### 3. Activer le Pilote IAC

- Dans la fenêtre du Studio MIDI, cherchez une icône **Pilote IAC**, **Gestionnaire IAC**, ou **IAC Driver**
- Si vous ne le voyez pas, il peut être caché ou désactivé (voir section "Si le Pilote IAC n'apparaît pas" ci-dessous)
- **Double-cliquez** sur cette icône pour ouvrir ses paramètres

### 4. Activer le périphérique

- Dans la fenêtre qui s'ouvre, cochez la case **Périphérique en ligne**
  - (En anglais : **Device is online**)
- Cliquez sur **Appliquer** (ou **Apply**)

### 5. Vérification

- Vous devriez maintenant voir **Pilote IAC Bus 1** (ou **IAC Driver Bus 1**) dans la liste des périphériques MIDI
- Ce port virtuel sera disponible dans GarageBand et autres applications MIDI

## Utilisation avec GarageBand

### Si vous utilisez MIDI USB natif (Recommandé - Pas besoin de pilote IAC)

1. **Ouvrez GarageBand**
2. **Créez une nouvelle piste** d'instrument logiciel
3. **Sélectionnez la piste** et cliquez sur l'icône d'engrenage (⚙️) pour les paramètres
4. Sous **Entrée MIDI**, cherchez **"ESP32-S3 MIDI"** ou un nom similaire
5. Sélectionnez votre ESP32-S3 directement
6. Votre appareil devrait maintenant envoyer des notes MIDI à GarageBand !

**Si votre ESP32 n'apparaît pas :**
- Débranchez et rebranchez le câble USB
- Attendez 2-3 secondes
- Redémarrez GarageBand
- Vérifiez dans **Configuration Audio MIDI** > **Studio MIDI** si l'ESP32 apparaît

### Si vous utilisez le pont Python (Nécessite le pilote IAC)

1. **Ouvrez GarageBand**
2. **Créez une nouvelle piste** d'instrument logiciel
3. **Sélectionnez la piste** et cliquez sur l'icône d'engrenage (⚙️) pour les paramètres
4. Sous **Entrée MIDI**, sélectionnez **Pilote IAC Bus 1** (ou **IAC Driver Bus 1**)
5. Assurez-vous que le script Python est en cours d'exécution
6. Votre ESP32-S3 devrait maintenant envoyer des notes MIDI à GarageBand via le pont Python !

## Dépannage

### Le Pilote IAC n'apparaît pas

Si vous ne voyez pas le Pilote IAC dans le Studio MIDI, essayez ces solutions :

#### Solution 1 : Créer le pilote IAC manuellement

1. Dans **Configuration Audio MIDI**, allez dans **Fenêtre** > **Afficher le Studio MIDI**
2. Cliquez sur le bouton **"+"** (plus) en bas de la fenêtre
3. Sélectionnez **"Créer un périphérique externe"** ou **"Create External Device"**
4. Dans la liste des périphériques, cherchez **"IAC Driver"** ou **"Pilote IAC"**
5. Si vous ne le trouvez pas, essayez de cliquer sur **"Ajouter un périphérique"** ou **"Add Device"**

#### Solution 2 : Vérifier les préférences système

1. Ouvrez **Préférences Système** (ou **Réglages Système** sur macOS Ventura+)
2. Allez dans **Sécurité et confidentialité** > **Confidentialité**
3. Vérifiez que **Configuration Audio MIDI** a les permissions nécessaires

#### Solution 3 : Réinitialiser le Studio MIDI

1. Fermez **Configuration Audio MIDI**
2. Ouvrez le **Terminal**
3. Tapez cette commande pour supprimer les préférences MIDI :
   ```bash
   rm ~/Library/Preferences/com.apple.audio.audiomidisetup.plist
   ```
4. Rouvrez **Configuration Audio MIDI** - le pilote IAC devrait réapparaître

#### Solution 4 : Utiliser une alternative (Recommandé)

Si le pilote IAC ne fonctionne toujours pas, vous avez deux options :

**Option A : Utiliser MIDI USB natif (Meilleure solution)**
- Si votre ESP32-S3 utilise le MIDI USB natif (implémenté dans le code), vous n'avez **PAS BESOIN** du pilote IAC
- L'ESP32 apparaîtra directement comme un périphérique MIDI dans GarageBand
- Pas besoin de pont Python ni de pilote IAC !

**Option B : Utiliser un autre port MIDI virtuel**
- Le script Python peut utiliser d'autres ports MIDI virtuels
- Vérifiez dans GarageBand quels ports MIDI sont disponibles

### GarageBand ne voit pas le port
- Vérifiez que le pilote IAC est bien activé dans Configuration Audio MIDI
- Redémarrez GarageBand après avoir activé le pilote
- Assurez-vous que le script Python est en cours d'exécution

### Pas de son dans GarageBand
- Vérifiez que vous avez sélectionné le bon port MIDI dans GarageBand
- Assurez-vous qu'un instrument est chargé sur la piste
- Vérifiez le volume de la piste dans GarageBand

## Pourquoi utiliser le Pilote IAC ?

Le Pilote IAC permet au script Python de créer un port MIDI virtuel que macOS et GarageBand reconnaissent comme un périphérique MIDI standard. Sans cela, le script ne pourrait pas envoyer de MIDI à GarageBand car il n'y aurait pas de port MIDI disponible.

## Alternative : MIDI natif USB

Si vous implémentez le MIDI USB natif sur votre ESP32-S3 (sans pont Python), vous n'avez **pas besoin** du Pilote IAC car l'ESP32 apparaîtra directement comme un périphérique MIDI dans GarageBand.
