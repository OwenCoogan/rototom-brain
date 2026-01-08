# MIDI Troubleshooting Guide

## Device Not Showing Up in GarageBand

### If Using Native USB MIDI (Current Implementation)

1. **Upload the code** to your ESP32-S3
2. **Disconnect and reconnect** the USB cable
3. **Wait 2-3 seconds** for macOS to recognize the device
4. **Check Audio MIDI Setup:**
   - Open **Audio MIDI Setup** (Applications > Utilities)
   - Click **Window** > **Show MIDI Studio**
   - Look for **"ESP32-S3 MIDI"** or similar device name
   - If it appears, GarageBand should see it too

5. **In GarageBand:**
   - Create a new Software Instrument track
   - Click the track settings (gear icon)
   - Under **MIDI Input**, select your ESP32-S3 device
   - If it doesn't appear, try restarting GarageBand

6. **If still not showing:**
   - Try a different USB cable (must support data, not just charging)
   - Try a different USB port
   - Check System Information > USB to see if device is recognized
   - Restart your Mac

### If Using Serial MIDI Bridge (Python Script)

1. **Make sure the bridge is running:**
   ```bash
   python3 serial_midi_bridge.py /dev/cu.usbmodem* 115200
   ```

2. **Enable IAC Driver:**
   - Audio MIDI Setup > Window > Show MIDI Studio
   - Double-click **IAC Driver**
   - Check **Device is online**
   - Click **Apply**

3. **In GarageBand:**
   - Select **"IAC Driver Bus 1"** as MIDI input (not the ESP32 directly)

## Testing MIDI Output

1. **Check Serial Monitor:**
   - Open PlatformIO Serial Monitor
   - Press the MIDI button on your device
   - You should see: `MIDI: Note On - note=60, velocity=...`

2. **If no Serial output:**
   - Check that `midi_init()` is called in `setup()`
   - Verify the MIDI button is connected and working

3. **If Serial shows messages but GarageBand doesn't receive:**
   - Verify MIDI device selection in GarageBand
   - Check that you're using a **Software Instrument** track (not Audio)
   - Try a different MIDI channel if your device supports it

## Common Issues

### "Device not recognized" on macOS
- USB cable might be charge-only
- Try different USB port
- Check USB port isn't damaged

### MIDI messages sent but no sound
- Check track volume in GarageBand
- Verify instrument is loaded on the track
- Check track isn't muted
- Ensure MIDI input is selected correctly

### Serial Monitor not working after USB MIDI
- USB MIDI and Serial CDC might conflict
- Try disconnecting/reconnecting
- Use a different USB port for programming if available

## Switching Between Serial MIDI and USB MIDI

If you need to switch back to Serial MIDI (for debugging), you can:

1. Comment out the USB MIDI includes in `midi.cpp`
2. Uncomment the Serial MIDI code
3. Rebuild and upload

Or keep both implementations and use a compile-time flag to switch.

