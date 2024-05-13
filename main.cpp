#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cuda_runtime.h>

// Sporcu sınıfı
class Sporcu {
private:
    float konum;
    float hiz;
    bool bitisCizgisineUlasti;
    bool parkurGecisiYapildi;
    int parkurGecisiZamani;

public:
    Sporcu() : konum(0), hiz(0), bitisCizgisineUlasti(false), parkurGecisiYapildi(false), parkurGecisiZamani(0) {}

    void setHiz(float h) { hiz = h; }
    float getHiz() const { return hiz; }

    void setKonum(float k) { konum = k; }
    float getKonum() const { return konum; }

    bool bitisCizgisineUlastiMi(float bitisKonumu) {
        if (!bitisCizgisineUlasti && konum >= bitisKonumu) {
            bitisCizgisineUlasti = true;
            return true;
        }
        return false;
    }

    void parkurGecisi() {
        if (!parkurGecisiYapildi) {
            parkurGecisiYapildi = true;
            parkurGecisiZamani = 10; // Parkur geçişinde geçirilecek süre (10 saniye)
        }
    }

    void parkurGecisiZamaniniAzalt() {
        if (parkurGecisiZamani > 0) {
            --parkurGecisiZamani;
        }
    }

    bool parkurGecisiTamamlandiMi() const {
        return parkurGecisiZamani == 0;
    }
};

// Takim sınıfı
class Takim {
private:
    std::vector<Sporcu> sporcular;
public:
    void sporcuEkle(const Sporcu& sporcu) {
        sporcular.push_back(sporcu);
    }

    const std::vector<Sporcu>& getSpurcular() const {
        return sporcular;
    }
};

// Yaris sınıfı
class Yaris {
private:
    std::vector<Takim> takimlar;
    int toplamTakimSayisi;
    int sporcuSayisi;

public:
    Yaris(int takimSayisi, int sporcuSayisi) : toplamTakimSayisi(takimSayisi), sporcuSayisi(sporcuSayisi) {
        srand(time(NULL));
        for (int i = 0; i < toplamTakimSayisi; ++i) {
            Takim takim;
            for (int j = 0; j < sporcuSayisi; ++j) {
                float hiz = 0, konum = 0;
                Sporcu sporcu;
                sporcu.setHiz(hiz);
                sporcu.setKonum(konum);
                takim.sporcuEkle(sporcu);
            }
            takimlar.push_back(takim);
        }
    }

    void yarisBaslat(float bitisKonumu) {
        std::cout << "Yarış başladı!" << std::endl;
        int parkur = 1; // Başlangıçta birinci parkur
        while (true) {
            for (auto& takim : takimlar) {
                for (auto& sporcu : takim.getSpurcular()) {
                    if (!sporcu.bitisCizgisineUlastiMi(bitisKonumu)) {
                        if (!sporcu.parkurGecisiTamamlandiMi()) {
                            sporcu.parkurGecisiZamaniniAzalt();
                        } else {
                            sporcu.parkurGecisi();
                        }
                    }
                }
            }

            // Her bir sporcu için CUDA işlemlerinin çağrılması

            // Her bir sporcu için güncellenmiş konum ve hızların ekrana yazdırılması

            // Bitiş koşulu kontrolü
            bool yarisBitti = true;
            for (auto& takim : takimlar) {
                for (auto& sporcu : takim.getSpurcular()) {
                    if (!sporcu.bitisCizgisineUlastiMi(bitisKonumu)) {
                        yarisBitti = false;
                        break;
                    }
                }
            }
            if (yarisBitti) break;

            parkur++;
        }
        std::cout << "Yarış bitti!" << std::endl;
    }
};

int main() {
    int takimSayisi, sporcuSayisi;
    std::cout << "Takım sayısını giriniz: ";
    std::cin >> takimSayisi;
    std::cout << "Her takımdaki sporcu sayısını giriniz: ";
    std::cin >> sporcuSayisi;

    Yaris yaris(takimSayisi, sporcuSayisi);
    float bitisKonumu = 5; // Bitiş çizgisi konumu (örneğin yüzme parkurunun uzunluğu)
    yaris.yarisBaslat(bitisKonumu);

    return 0;
}

