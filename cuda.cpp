#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cuda_runtime.h>

// CUDA çekirdek fonksiyonu
__global__ void hesaplaKonumVeHiz(float *hizlar, float *konumlar, int sporcuSayisi, int parkur) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x; // Thread indeksi hesaplanır

    // Sporcuların hızlarının ve konumlarının güncellenmesi
    if (idx < sporcuSayisi) {
        if (parkur == 1) {
            // İlk parkur
            hizlar[idx] = 1 + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/(5-1)); // Rastgele hız belirleme
            konumlar[idx] += hizlar[idx]; // Konum güncelleme
        } else if (parkur == 2) {
            // İkinci parkur
            hizlar[idx] *= 3; // Hızı 3 katına çıkarma
            konumlar[idx] += hizlar[idx]; // Konum güncelleme
        } else if (parkur == 3) {
            // Üçüncü parkur
            hizlar[idx] /= 3; // Hızı 1/3'üne indirme
            konumlar[idx] += hizlar[idx]; // Konum güncelleme
        }
    }
}

int main() {
    int sporcuSayisi = 300; // Toplam sporcu sayısı
    int parkur = 1; // Başlangıçta birinci parkur

    // CUDA ile kullanılacak değişkenlerin tanımlanması
    float *hizlar, *konumlar;
    cudaMallocManaged(&hizlar, sporcuSayisi * sizeof(float));
    cudaMallocManaged(&konumlar, sporcuSayisi * sizeof(float));

    // Rastgele sayı üretimi için tohum belirlenmesi
    srand(time(NULL));

    // CUDA çekirdeğinin çağrılması
    hesaplaKonumVeHiz<<<(sporcuSayisi + 255) / 256, 256>>>(hizlar, konumlar, sporcuSayisi, parkur);
    
    // CUDA işlemlerinin bitmesini bekleme
    cudaDeviceSynchronize();

    // Hesaplanan hız ve konumları yazdırma
    std::cout << "Sporcularin Hizlari: ";
    for (int i = 0; i < sporcuSayisi; ++i) {
        std::cout << hizlar[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Sporcularin Konumlari: ";
    for (int i = 0; i < sporcuSayisi; ++i) {
        std::cout << konumlar[i] << " ";
    }
    std::cout << std::endl;

    // Bellekten ayırma
    cudaFree(hizlar);
    cudaFree(konumlar);

    return 0;
}
