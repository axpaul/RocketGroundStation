import serial
import time
import random

def calculate_crc8(data):
    CRC8_DPOLY = 0x31  # Polynôme utilisé pour le calcul du CRC-8
    crc = 0  # Initialisation du CRC à 0
    
    for byte in data:
        extract = byte
        for _ in range(8):
            sum = (crc ^ extract) & 0x01
            crc >>= 1
            if sum:
                crc ^= CRC8_DPOLY
            extract >>= 1
    return crc

# Configurer le port COM (remplacer 'COM8' par le port série utilisé)
ser = serial.Serial('COM8', baudrate=9600, timeout=1)

# Nombre de trames valides avant d'envoyer une trame aléatoire incorrecte
valid_frames_count = 2
frame_counter = 0

# Ouvrir le fichier contenant les messages hexadécimaux
with open('C:/Users/paulm/OneDrive/Bureau/Software/RocketGroundStation/source Qt/VirtualSerialCom/log-MSE.txt', 'r') as file:
    for line in file:
        hex_data = line.strip().replace(' ', '')
        byte_data = bytes.fromhex(hex_data)

        if len(byte_data) == 27:
            frame_counter += 1

            if frame_counter <= valid_frames_count:
                # Envoi d'une trame valide
                crc = calculate_crc8(byte_data)

                frame = bytearray()
                frame.append(0xEE)  # Entête de début
                frame.append(crc)   # CRC valide
                frame.extend(byte_data)  # Ajouter les données (27 octets)
                frame.append(0x0A)  # Fin de trame

                # Envoyer la trame valide sur le port série
                ser.write(frame)
                print(f"Envoyé (valide) : {frame.hex()}")

            else:
                # Envoi d'une trame aléatoire avec un CRC incorrect
                random_data = bytearray(random.getrandbits(8) for _ in range(27))  # Trame aléatoire
                crc_invalid = (calculate_crc8(random_data) + 1) & 0xFF  # CRC incorrect

                frame = bytearray()
                frame.append(0xEE)  # Entête de début
                frame.append(crc_invalid)   # CRC incorrect
                frame.extend(random_data)  # Ajouter les données aléatoires (27 octets)
                frame.append(0x0A)  # Fin de trame

                # Envoyer la trame incorrecte sur le port série
                ser.write(frame)
                print(f"Envoyé (aléatoire incorrect) : {frame.hex()}")

                # Réinitialiser le compteur de trames valides
                frame_counter = 0

            # Attendre un court instant entre les envois
            time.sleep(1)
        else:
            print(f"Trame ignorée : taille incorrecte ({len(byte_data)} octets)")

# Fermer le port série
ser.close()
