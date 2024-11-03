import serial
import time

def calculate_crc8(data):
    
    CRC8_DPOLY = 0x31  # Polynôme utilisé pour le calcul du CRC-8 (polynôme standard)
    crc = 0  # Initialisation du CRC à 0
    
    # Pour chaque byte dans les données
    for byte in data:
        extract = byte  # Extraire chaque byte
        # Traiter chaque byte bit par bit
        for _ in range(8):
            sum = (crc ^ extract) & 0x01  # Comparer le bit de poids faible
            crc >>= 1  # Décaler à droite le CRC
            if sum:  # Appliquer le polynôme si nécessaire
                crc ^= CRC8_DPOLY
            extract >>= 1  # Passer au bit suivant de l'octet
    return crc

# Configurer le port COM (remplacer 'COM7' par le port série utilisé)
ser = serial.Serial('COM8', baudrate=9600, timeout=1)

# Ouvrir le fichier contenant les messages hexadécimaux
with open('C:/Users/paulm/OneDrive/Bureau/Software/VirtualCom/log-MSE.txt', 'r') as file:
    for line in file:
        # Nettoyer la ligne en retirant les espaces et les sauts de ligne
        hex_data = line.strip().replace(' ', '')

        # Convertir les données hexadécimales en bytes
        byte_data = bytes.fromhex(hex_data)

        # Vérifier que la trame fait bien 27 octets
        if len(byte_data) == 27:
            # Calculer le CRC sur les données
            crc = calculate_crc8(byte_data)

            # Construire la trame complète : entête, CRC, données, et fin de trame
            frame = bytearray()
            frame.append(0xEE)  # Entête de début (Start of Header)
            frame.append(crc)   # CRC
            frame.extend(byte_data)  # Ajouter les données (27 octets)
            frame.append(0x0A)  # Fin de trame (End of Transmission '\n')

            # Envoyer la trame complète sur le port série
            ser.write(frame)

            # Afficher la trame envoyée pour vérification
            print(f"Envoyé : {frame.hex()}")

            # Attendre un court instant entre les envois
            time.sleep(0.05)
        else:
            print(f"Trame ignorée : taille incorrecte ({len(byte_data)} octets)")

# Fermer le port série
ser.close()
