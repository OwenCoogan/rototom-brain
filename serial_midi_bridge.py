#!/usr/bin/env python3
"""
Serial to MIDI Bridge for macOS
Converts Serial MIDI messages from ESP32-S3 to standard MIDI output

Requirements:
    pip install pyserial python-rtmidi

Usage:
    python3 serial_midi_bridge.py [serial_port] [baud_rate]
    
    Example:
    python3 serial_midi_bridge.py /dev/cu.usbmodem* 115200
"""

import sys
import serial
import rtmidi
import time
import glob

def find_serial_port():
    """Find available serial ports"""
    if sys.platform.startswith('darwin'):  # macOS
        ports = glob.glob('/dev/cu.usbmodem*') + glob.glob('/dev/cu.SLAB_USBtoUART*')
    elif sys.platform.startswith('win'):  # Windows
        ports = ['COM%s' % (i + 1) for i in range(256)]
    else:  # Linux
        ports = glob.glob('/dev/ttyUSB*') + glob.glob('/dev/ttyACM*')
    
    return ports

def main():
    # Find serial port
    if len(sys.argv) > 1:
        serial_port = sys.argv[1]
    else:
        ports = find_serial_port()
        if not ports:
            print("No serial ports found. Please specify a port:")
            print("  python3 serial_midi_bridge.py /dev/cu.usbmodem* 115200")
            sys.exit(1)
        serial_port = ports[0]
        print(f"Using serial port: {serial_port}")
    
    # Get baud rate
    baud_rate = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    
    # Open serial port
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=1)
        print(f"Connected to {serial_port} at {baud_rate} baud")
        print("\n⚠️  IMPORTANT: Make sure PlatformIO Serial Monitor is CLOSED!")
        print("   The serial port can only be used by one program at a time.\n")
    except serial.SerialException as e:
        if "could not open port" in str(e).lower() or "access denied" in str(e).lower():
            print(f"Error: Cannot open serial port {serial_port}")
            print("The port is likely in use by another program (PlatformIO Serial Monitor?).")
            print("Please close PlatformIO Serial Monitor and try again.")
        else:
            print(f"Error opening serial port: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error opening serial port: {e}")
        sys.exit(1)
    
    # Initialize MIDI output
    midiout = rtmidi.MidiOut()
    available_ports = midiout.get_ports()
    
    if not available_ports:
        print("=" * 60)
        print("ERREUR : Aucun port MIDI disponible !")
        print("=" * 60)
        print("\nLe pilote IAC n'est pas activé ou n'est pas visible.")
        print("\nÉtapes pour activer le pilote IAC :")
        print("1. Ouvrez Configuration Audio MIDI (Applications > Utilitaires)")
        print("2. Cliquez sur Fenêtre > Afficher le Studio MIDI")
        print("3. Double-cliquez sur 'Pilote IAC' ou 'IAC Driver'")
        print("4. Cochez 'Périphérique en ligne' ou 'Device is online'")
        print("5. Cliquez sur 'Appliquer' ou 'Apply'")
        print("6. Fermez Configuration Audio MIDI")
        print("7. Relancez ce script")
        print("\nSi le pilote IAC n'apparaît pas, consultez :")
        print("  - docs/FR/IAC_DRIVER_FR.md")
        print("  - docs/FR/ACTIVER_IAC_FR.md (nouveau guide de dépannage)")
        print("\n" + "=" * 60)
        sys.exit(1)
    
    # Debug: show all available ports
    print(f"\nAvailable MIDI ports ({len(available_ports)}):")
    for i, port in enumerate(available_ports):
        print(f"  {i}: {port}")
    
    # Use first available MIDI port (or IAC Driver if available)
    midi_port_name = None
    for port in available_ports:
        if 'IAC' in port or 'Virtual' in port:
            midi_port_name = port
            break
    
    if not midi_port_name:
        midi_port_name = available_ports[0]
    
    try:
        midiout.open_port(available_ports.index(midi_port_name))
        print(f"MIDI output connected to: {midi_port_name}")
    except Exception as e:
        print(f"Error opening MIDI port: {e}")
        sys.exit(1)
    
    print("\nBridge is running! Press Ctrl+C to stop.\n")
    print("MIDI messages from ESP32-S3 will be forwarded to:", midi_port_name)
    print("Make sure to select this MIDI port in GarageBand.\n")
    
    # Buffer for MIDI messages
    buffer = bytearray()
    
    try:
        while True:
            # Read available bytes
            try:
                if ser.in_waiting > 0:
                    data = ser.read(ser.in_waiting)
                    buffer.extend(data)
            except serial.SerialException as e:
                print(f"\nSerial error: {e}")
                print("Device may be disconnected or port is in use by another program.")
                print("Make sure PlatformIO Serial Monitor is closed.")
                break
                
                # Process complete MIDI messages (3 bytes: status, note, velocity)
                while len(buffer) >= 3:
                    # Check if we have a valid MIDI status byte (0x80-0x9F for note on/off)
                    if buffer[0] >= 0x80 and buffer[0] <= 0x9F:
                        status = buffer[0]
                        note = buffer[1]
                        velocity = buffer[2]
                        
                        # Validate MIDI values
                        if note <= 127 and velocity <= 127:
                            midi_message = [status, note, velocity]
                            midiout.send_message(midi_message)
                            
                            # Debug output
                            msg_type = "Note On" if (status & 0xF0) == 0x90 else "Note Off"
                            print(f"MIDI: {msg_type} - Note={note}, Velocity={velocity}")
                        
                        # Remove processed message
                        buffer = buffer[3:]
                    else:
                        # Skip invalid byte and try again
                        buffer = buffer[1:]
            
            time.sleep(0.001)  # Small delay to prevent CPU spinning
            
    except KeyboardInterrupt:
        print("\n\nStopping bridge...")
    finally:
        try:
            ser.close()
        except:
            pass
        try:
            # rtmidi MidiOut doesn't have close(), but we can delete it
            del midiout
        except:
            pass
        print("Bridge stopped.")

if __name__ == "__main__":
    main()
