message = """                                                
╔══════════════════════════════════════════════╗
║                                              ║
║        ¡GRACIAS POR JUGAR A MISIFU!          ║
║                                              ║
║  Esta es una version de prueba para          ║
║  ZX Spectrum. ¡Compra la version completa    ║
║  para disfrutar de todas las aventuras       ║
║  de Misifu la gatita!                        ║
║                                              ║
║  Envía un giro postal de 500 pesetas a:      ║
║  Jordi, Apartado de correos:                 ║
║        https://jbinary.itch.io/              ║
║  ¡No te pierdas las futuras aventuras        ║
║  de Misifu en otros sistemas!                ║
║                                              ║
╚══════════════════════════════════════════════╝
"""

# Escribe el mensaje en un archivo de texto con codificación cp850
file_name = "shareware.txt"
with open(file_name, "w", encoding="cp850") as file:
    file.write(message)

print(f"Archivo '{file_name}' creado con exito.")
