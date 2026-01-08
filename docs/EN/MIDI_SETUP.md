# MIDI Setup Guide for GarageBand and DAWs

This guide explains how to connect your ESP32-S3 device to GarageBand or other MIDI-compatible software.

## Overview

Your ESP32-S3 sends MIDI messages over USB Serial (CDC). To use it with GarageBand or other DAWs, you need a MIDI bridge application that converts the Serial MIDI data into standard MIDI that your computer can recognize.

## Method 1: Using Python Serial-to-MIDI Bridge ⭐ (Recommended - Works on All macOS Versions)

**Note:** Hairless MIDI-to-Serial Bridge is not compatible with newer macOS versions. Use Method 4 (Python bridge) instead, which works on all systems.

If you have an older macOS version and Hairless works for you:

### Step 1: Install Hairless MIDI-to-Serial Bridge

1. Download Hairless MIDI-to-Serial Bridge from: https://projectgus.github.io/hairless-midiserial/
2. Install and launch the application

### Step 2: Connect Your Device

1. Connect your ESP32-S3 to your computer via USB
2. Upload your code to the device
3. Open Hairless MIDI-to-Serial Bridge

### Step 3: Configure Hairless

1. In Hairless, select your ESP32-S3 serial port from the "Serial port" dropdown
   - On Mac: Usually appears as `/dev/cu.usbmodem*` or `/dev/cu.SLAB_USBtoUART*`
   - On Windows: Usually appears as `COM*`
2. Set the baud rate to **115200** (or match your Serial.begin() setting)
3. Click "Connect" to start the bridge

### Step 4: Use in GarageBand

1. Open GarageBand
2. Create a new Software Instrument track
3. In GarageBand's MIDI settings, select "Hairless MIDI" as the input source
4. Your device should now send MIDI notes to GarageBand!

## Method 2: Using MIDI-OX (Windows Only)

1. Download and install MIDI-OX from: http://www.midiox.com/
2. Connect your ESP32-S3 device
3. In MIDI-OX, go to Options > MIDI Devices
4. Select your serial port and configure it
5. Route the MIDI input to a virtual MIDI port
6. Select that virtual MIDI port in GarageBand

## Method 3: Using Serial-to-MIDI Bridge on Linux

For Linux users, you can use `ttymidi` or `aconnect`:

```bash
# Install ttymidi
sudo apt-get install ttymidi

# Run ttymidi (replace /dev/ttyUSB0 with your device)
ttymidi -s /dev/ttyUSB0 -b 115200
```

## Method 4: Using Python Serial-to-MIDI Bridge (macOS/Linux/Windows)

This is a cross-platform solution that works on all operating systems, including modern macOS versions.

### Step 1: Install Python Dependencies

```bash
pip3 install pyserial python-rtmidi
```

### Step 2: Enable IAC Driver on macOS (Optional but Recommended)

#### English macOS:
1. Open **Audio MIDI Setup** (Applications > Utilities > Audio MIDI Setup)
2. Click **Window** > **Show MIDI Studio**
3. Double-click **IAC Driver**
4. Check **Device is online**
5. Click **Apply**

#### macOS en français :
1. Ouvrir **Configuration Audio MIDI** (Applications > Utilitaires > Configuration Audio MIDI)
2. Cliquer sur **Fenêtre** > **Afficher le Studio MIDI**
3. Double-cliquer sur **Pilote IAC** (ou **IAC Driver**)
4. Cocher **Périphérique en ligne** (ou **Device is online**)
5. Cliquer sur **Appliquer** (ou **Apply**)

This creates a virtual MIDI port that the Python script can use.
Cela crée un port MIDI virtuel que le script Python peut utiliser.

### Step 3: Run the Bridge Script

1. Connect your ESP32-S3 to your computer
2. Find your device's serial port:
   ```bash
   # On macOS/Linux:
   ls /dev/cu.usbmodem* /dev/cu.SLAB_USBtoUART*
   
   # On Windows, check Device Manager for COM ports
   ```
3. Run the bridge script:
   ```bash
   python3 serial_midi_bridge.py /dev/cu.usbmodem* 115200
   ```
   Replace `/dev/cu.usbmodem*` with your actual port name.

### Step 4: Use in GarageBand

#### English:
1. Open GarageBand
2. Create a new Software Instrument track
3. Click the **⚙️** (gear) icon on the track
4. Under **MIDI Input**, select **IAC Driver Bus 1** (or the MIDI port shown by the script)
5. Load an instrument on the track
6. Press the MIDI button on your ESP32 - you should hear sound!

#### Français:
1. Ouvrez GarageBand
2. Créez une nouvelle piste d'instrument logiciel
3. Cliquez sur l'icône **⚙️** (engrenage) sur la piste
4. Sous **Entrée MIDI**, sélectionnez **Pilote IAC Bus 1** (ou le port MIDI affiché par le script)
5. Chargez un instrument sur la piste
6. Appuyez sur le bouton MIDI de votre ESP32 - vous devriez entendre un son !

**See `../FR/VERIFIER_GARAGEBAND_FR.md` for detailed verification steps.**

The script will display MIDI messages as they're received, which helps with debugging.

## Method 5: Native USB MIDI (Best Solution - No Bridge Needed)

For the best experience without any bridge software, you can implement native USB MIDI directly on the ESP32-S3. This makes it appear as a standard USB MIDI device that macOS recognizes automatically.

**Note:** This requires updating the MIDI implementation code. See the implementation section below for details.

## Troubleshooting

### Device Not Appearing
- Make sure USB CDC is enabled in `platformio.ini` (you have `ARDUINO_USB_CDC_ON_BOOT=1`)
- Check that the USB cable supports data transfer (not just charging)
- Try a different USB port

### MIDI Not Working in GarageBand
- Verify the bridge software is running and connected
- Check that you've selected the bridge as MIDI input in GarageBand
- Make sure you're using a Software Instrument track (not Audio track)
- Try restarting GarageBand after connecting the bridge

### No MIDI Messages Received
- Check the Serial Monitor to see if MIDI messages are being sent
- Verify the baud rate matches in both your code and bridge software
- Ensure `midi_init()` is called in your `setup()` function

## Testing

You can test MIDI output by calling:
- `midi_note_on(60, 100)` - Plays note C4 (middle C) at velocity 100
- `midi_note_off(60)` - Stops note C4
- `midi_trigger(75)` - Triggers a quick note with intensity 75

## MIDI Note Reference

- Middle C (C4) = 60
- C5 = 72
- C3 = 48
- Full range: 0-127 (where 0 is C-1 and 127 is G9)

## Quick Start (Recommended for macOS)

Since Hairless doesn't work on newer macOS versions, here's the quickest way to get started:

1. **Install Python dependencies:**
   ```bash
   pip3 install pyserial python-rtmidi
   ```

2. **Enable IAC Driver** (macOS):
   - **English**: Audio MIDI Setup > Window > Show MIDI Studio > IAC Driver > Device is online
   - **Français**: Configuration Audio MIDI > Fenêtre > Afficher le Studio MIDI > Pilote IAC > Périphérique en ligne
   
   See `../FR/IAC_DRIVER_FR.md` for detailed French instructions.

3. **Find your device port:**
   ```bash
   ls /dev/cu.usbmodem*
   ```

4. **Run the bridge:**
   ```bash
   python3 serial_midi_bridge.py /dev/cu.usbmodem* 115200
   ```

5. **In GarageBand:** Select "IAC Driver Bus 1" as MIDI input

That's it! The bridge will forward MIDI messages from your ESP32-S3 to GarageBand.

## Additional Resources

- [MIDI Specification](https://www.midi.org/specifications)
- [Python-rtmidi Documentation](https://github.com/SpotlightKid/python-rtmidi)
- [GarageBand MIDI Guide](https://support.apple.com/garageband)
