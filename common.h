// common.h

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Platform Bağımlı Başlıklar ve Tanımlar ---

#ifdef _WIN32 // Windows için Winsock
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") 
#define CLOSE_SOCKET closesocket
#define SLEEP(sec) Sleep(sec * 1000)
#else // macOS/Linux (POSIX/BSD Sockets) için
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define CLOSE_SOCKET close
#define SOCKET int
#define SLEEP(sec) sleep(sec)
#endif

// --- TCP/IP ve Protokol Ayarları ---
#define CLIENT1_PORT 8080 
#define CLIENT2_PORT 8081 
#define SERVER_IP "127.0.0.1"
#define MAX_DATA_SIZE 256
#define MAX_CONTROL_SIZE 64
#define CRC16_POLYNOMIAL 0x8005 // CRC

// Hata Tespit Yöntemleri
typedef enum {
    PARITY = 1,        // 1. Parity Bit
    TWOD_PARITY = 2,   // 2. 2D Parity (Matrix Parity)
    CRC16 = 3,         // 3. CRC (Cyclic Redundancy Check)
    HAMMING = 4,       // 4. Hamming Code
    CHECKSUM = 5       // 5. Internet Checksum
} Method;

// Paket Yapısı
typedef struct {
    char data[MAX_DATA_SIZE];
    Method method;
    char control_info[MAX_CONTROL_SIZE]; // HEX string
} Packet;

// --- Platform Bağımsız Yardımcı Fonksiyonlar ---

int init_winsock() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup başarısız oldu.\n");
        return -1;
    }
#endif
    return 0;
}

void cleanup_winsock() {
#ifdef _WIN32
    WSACleanup();
#endif
}

const char* get_method_name(Method m) {
    switch (m) {
        case PARITY: return "Parity Bit";
        case TWOD_PARITY: return "2D Parity";
        case CRC16: return "CRC16";
        case HAMMING: return "Hamming Code";
        case CHECKSUM: return "Internet Checksum";
        default: return "UNKNOWN";
    }
}

#endif // COMMON_H