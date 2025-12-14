// common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>

// 確保接收指定長度的所有數據
int recv_all(int socket, void *buffer, size_t length) {
    char *ptr = (char*) buffer;
    size_t remaining = length;
    while (remaining > 0) {
        ssize_t received = recv(socket, ptr, remaining, 0);
        if (received < 0) {
            if (errno == EINTR) continue; 
            return -1; 
        }
        if (received == 0) return 0; // 對方關閉連線
        ptr += received;
        remaining -= received;
    }
    return 1; // 成功
}

// 確保發送指定長度的所有數據
int send_all(int socket, const void *buffer, size_t length) {
    const char *ptr = (const char*) buffer;
    size_t remaining = length;
    while (remaining > 0) {
        ssize_t sent = send(socket, ptr, remaining, 0);
        if (sent < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        ptr += sent;
        remaining -= sent;
    }
    return 1;
}

#endif