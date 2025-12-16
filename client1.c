#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <unistd.h>     

// HESAPLAMA ALGORITMALARI KISMI
int calcParity(char *data) {
    int count = 0;
    int i,j;
    for (i = 0; i < strlen(data); i++) {
        char c = data[i];
        for (j = 0; j < 8; j++) if ((c >> j) & 1) count++;
    }
    return (count % 2 == 0) ? 0 : 1;
}
int calc2DParity(char *data) {
    unsigned char colParity = 0;
    int i;
    for (i = 0; i < strlen(data); i++) colParity ^= data[i];
    return (int)colParity;
}
unsigned short calcCRC16(char *data) {
    unsigned short crc = 0xFFFF;
    int len = strlen(data);
    int i;
    for (i = 0; i < len; i++) {
        unsigned char x = crc >> 8 ^ data[i];
        x ^= x >> 4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
    }
    return crc;
}
int calcHamming(char *data) {
    if(strlen(data) == 0) return 0;
    unsigned char val = data[0];
    int p1 = (val & 1) ^ ((val >> 1) & 1) ^ ((val >> 3) & 1);
    int p2 = (val & 1) ^ ((val >> 2) & 1) ^ ((val >> 3) & 1);
    int p4 = ((val >> 1) & 1) ^ ((val >> 2) & 1) ^ ((val >> 3) & 1);
    return (p4 << 2) | (p2 << 1) | p1;
}
int calcChecksum(char *data) {
    int sum = 0;
    int i;
    for(i=0; i<strlen(data); i++) sum += data[i];
    return sum % 256;
}

int main() {
    
    
    int s; 
    struct sockaddr_in server;
    char inputData[1000], packet[2000], method[20];
    int choice;
    long result;

    printf("--- CLIENT 1 BASLATILIYOR ---\n");
    

    s = socket(AF_INET , SOCK_STREAM , 0 );
    if (s == -1) { 
        perror("HATA: Socket olusturulamadi");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons( 8888 );

    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("HATA: Server'a baglanilamadi"); 
        return 1;
    }
    printf("Server'a baglandi.\n");

    while(1) {
        printf("\nMetin Giriniz: ");
        scanf("%s", inputData);

        printf("Yontem Sec:\n[1] Parity\n[2] 2D Parity\n[3] CRC-16\n[4] Hamming\n[5] Checksum\nSecim: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: result = calcParity(inputData); strcpy(method, "PARITY"); break;
            case 2: result = calc2DParity(inputData); strcpy(method, "2DPARITY"); break;
            case 3: result = calcCRC16(inputData); strcpy(method, "CRC16"); break;
            case 4: result = calcHamming(inputData); strcpy(method, "HAMMING"); break;
            case 5: result = calcChecksum(inputData); strcpy(method, "CHECKSUM"); break;
            default: result = calcParity(inputData); strcpy(method, "PARITY");
        }

        sprintf(packet, "%s|%s|%X", inputData, method, result);
        printf("Gonderiliyor -> %s\n", packet);
        
        if (send(s , packet , strlen(packet) , 0) < 0) {
            perror("HATA: Gonderilemedi");
            break;
        }
    }
    close(s);
    return 0;
}