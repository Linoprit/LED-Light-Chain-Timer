import serial
import time
from datetime import datetime

# Konfiguration der seriellen Schnittstelle
SERIAL_PORT = '/dev/ttyACM0'  # unter Windows z. B. 'COM3'
BAUDRATE = 115200               # Anpassen je nach Konfiguration
#BAUDRATE = 38400

def main():
    start_time = None
    buffer = ""

    try:
        with serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1) as ser:
            print(f"Höre auf {SERIAL_PORT} mit {BAUDRATE} Baudrate...")

            while True:
                if ser.in_waiting > 0:
                    char = ser.read(1).decode(errors='ignore')
                    buffer += char

                    if char == '\n':
                        line = buffer.strip()
                        buffer = ""

                        if line == 'A':
                            start_time = datetime.now()
                            print(f"[A] Startzeit gespeichert: {start_time.strftime('%H:%M:%S.%f')[:-3]}")

                        elif line == 'B':
                            if start_time:
                                end_time = datetime.now()
                                delta = end_time - start_time
                                print(f"[B] Zeitdifferenz: {delta.total_seconds():.3f} Sekunden")
                            else:
                                print("[B] Fehler: Startzeit wurde noch nicht empfangen.")

                        else:
                            print(f"Unbekannte Nachricht empfangen: {line}")

                time.sleep(50 / 1000)
    except serial.SerialException as e:
        print(f"Fehler beim Zugriff auf die serielle Schnittstelle: {e}")
    except KeyboardInterrupt:
        print("\nBeendet durch Benutzer.")

if __name__ == "__main__":
    main()
