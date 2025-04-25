# Cliente-Servidor TCP/UDP

Programa para el envío de mensajes de texto a través de la red empleando TCP o UDP como capa de transporte.

La versión 1 del programa envía mensajes predefinidos. Es ideal para probar el proceso de forma rápida.
La versión 2 permite que los usuarios decidan que mensaje enviar y qué puerto utilizar para el servidor, personalizando el programa.

## Instrucciones

### Compilar:

```
g++ conn_tcp_upd_v2.cpp -o conn_tcp_upd
```

### Ejecutar el servidor (TCP o UDP):

```
./conn_tcp_upd server tcp 8080  # Modo servidor TCP
```

```
./conn_tcp_upd server udp 8080 # Modo servidor UDP
```

### Ejecutar el cliente (TCP o UDP):
```
./conn_tcp_upd client tcp 8080 127.0.0.1  # Cliente TCP
```

```
./conn_tcp_upd client udp 8080 127.0.0.1  # Cliente UDP
```

## Ejemplo de ejecución

### Terminal 1 (Servidor TCP):
```
$ ./conn_tcp_upd server tcp 8080
Servidor TCP esperando conexiones en el puerto 8080...
Cliente conectado desde 127.0.0.1
Mensaje recibido: ¡Hola desde el cliente TCP!
Respuesta enviada al cliente.
```

### Terminal 2 (Cliente TCP):
```
$ ./conn_tcp_upd client tcp 8080 127.0.0.1
Ingrese el mensaje a enviar (TCP): ¡Hola desde el cliente TCP!
Mensaje enviado al servidor TCP.
Respuesta del servidor: Mensaje recibido por el servidor TCP
```
