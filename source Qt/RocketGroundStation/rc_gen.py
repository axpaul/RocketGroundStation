import http.server
import socketserver
import os

# Configuration du chemin des tuiles et du port
TILE_PATH = os.path.join(os.path.dirname(__file__), "map")
PORT = 8081

class TileRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        # Extrait le chemin de la requête et supprime le premier '/'
        tile_path = self.path[1:]

        # Construit le chemin complet du fichier de tuile
        full_path = os.path.join(TILE_PATH, tile_path)

        # Vérifie si le fichier de tuile existe
        if os.path.isfile(full_path):
            # Sert la tuile
            self.send_response(200)
            self.send_header("Content-Type", "image/png")
            self.end_headers()
            with open(full_path, "rb") as file:
                self.wfile.write(file.read())
            print(f"Servi: {full_path}")
        else:
            # Renvoie une réponse 404 si la tuile n'est pas trouvée
            self.send_response(404)
            self.send_header("Content-Type", "text/plain")
            self.end_headers()
            self.wfile.write(b"Tile not found")
            print(f"Erreur 404: {full_path} non trouvé")

# Lancement du serveur
with socketserver.TCPServer(("", PORT), TileRequestHandler) as httpd:
    print(f"Serveur de tuiles démarré sur le port {PORT}")
    httpd.serve_forever()
