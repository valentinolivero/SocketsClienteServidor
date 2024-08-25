import socket

def menu():
    print("Menu:")
    print("1. Generar nombre de usuario")
    print("2. Generar contrasena")
    print("Ingrese una opcion: ", end='')

def main():
    server_address = ('127.0.0.1', 8080)
    buffer_size = 50

    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(server_address)

        while True:
            menu()
            try:
                option = int(input())
            except ValueError:
                print("Entrada invalida. Intentelo de nuevo.")
                continue

            if option == 1:
                try:
                    length = int(input("Ingrese la longitud del nombre de usuario (5-15): "))
                except ValueError:
                    print("Entrada invalida. Intentelo de nuevo.")
                    continue
                message = f"U{length}"
            elif option == 2:
                try:
                    length = int(input("Ingrese la longitud de la contrasena (8-50): "))
                except ValueError:
                    print("Entrada invalida. Intentelo de nuevo.")
                    continue
                message = f"P{length}"
            else:
                print("Opcion invalida. Intentelo de nuevo.")
                continue

            sock.sendall(message.encode())

            response = sock.recv(buffer_size).decode()
            print(f"Respuesta del servidor: {response}")

            another = input("¿Desea hacer otra solicitud? (s/n): ").strip().lower()
            if another != 's':
                break

    except socket.error as e:
        print(f"Error de socket: {e}")
    finally:
        sock.close()

if __name__ == "__main__":
    main()
