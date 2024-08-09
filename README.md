# Explanation of Multicast File Transfer Code

This code demonstrates a multicast file transfer system using UDP sockets in C, where one program acts as the server (sender) and the other as the client (receiver).

## Server (Sender) Code

### Overview

The server reads a file and sends its content over a multicast group. It uses a datagram socket (UDP) to send the file in chunks to all clients that have joined the multicast group.

### Key Steps

1. **Socket Initialization:**
   - A UDP socket is created using the `socket()` function. The socket is configured to use IPv4 and datagram communication.

2. **Multicast Group Setup:**
   - The `groupSock` structure is initialized with the multicast IP address (`226.1.1.1`) and port (`8888`). The local interface for sending multicast datagrams is set to `192.168.172.121` using the `setsockopt()` function.

3. **File Transmission:**
   - The server opens the file specified by the user. It then reads the file in chunks of 1024 bytes and sends each chunk to the multicast group using the `sendto()` function.
   - After all the file data is sent, the server sends a specific message (`"end19970709"`) to signal the end of the transmission.

4. **File Size Calculation:**
   - The server calculates the size of the file using the `stat()` function and prints it to the console.

5. **Cleanup:**
   - The server closes the file and the socket after the transmission is complete.

## Client (Receiver) Code

### Overview

The client receives the multicast data sent by the server, writes it to a file, and stores the file on the local system.

### Key Steps

1. **Socket Initialization:**
   - The client creates a UDP socket and configures it to reuse the address. This allows multiple clients to receive multicast messages on the same port.

2. **Binding and Multicast Group Membership:**
   - The client binds the socket to the multicast port (`8888`) and joins the multicast group (`226.1.1.1`) on the specified local interface (`192.168.172.121`).

3. **Receiving Data:**
   - The client receives the file name first, then opens a file for writing. It enters a loop where it receives data from the server. If the data matches the end message (`"end19970709"`), the client stops receiving.
   - The received data is written to the file.

4. **File Size Calculation:**
   - The client calculates the size of the received file using the `stat()` function and prints it to the console.

5. **Cleanup:**
   - The client closes the file and the socket after the reception is complete.
