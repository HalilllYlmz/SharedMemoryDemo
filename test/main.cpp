#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

struct Car {
    char brand[50];
    char model[50];
    unsigned int year;
};

void readFromSharedMemory() {
    // Paylaşılan bellek alanına erişim için aç
    int shm_fd = shm_open("/my_shared_memory", O_RDONLY, 0666);
    if (shm_fd == -1) {
        std::cerr << "Shared memory açılamadı!" << std::endl;
        return;
    }

    // Hafızayı map et
    void* ptr = mmap(0, sizeof(Car), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Memory map başarısız!" << std::endl;
        close(shm_fd);
        return;
    }

    // Struct verisini oku
    Car car;
    memcpy(&car, ptr, sizeof(Car));

    // Veriyi yazdır
    std::cout << "Brand: " << car.brand << std::endl;
    std::cout << "Model: " << car.model << std::endl;
    std::cout << "Year: " << car.year << std::endl;

    // Belleği temizle
    munmap(ptr, sizeof(Car));
    close(shm_fd);
}

int main() {
    readFromSharedMemory();
    return 0;
}
