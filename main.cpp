#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

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

            // İkinci parkurdayken hızları 3 katına çıkar
            if (parkur == 2) {
                hiz *= 3;
            }
            // Üçüncü parkurdayken hızları ilk parkurdaki hızlarının 1/3'üne indir
            else if (parkur == 3) {
                hiz /= 3;
            }
        }
    }
};

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

class Yaris {
private:
    std::vector<Takim> takimlar;
    int toplamTakimSayisi;
    int sporcuSayisi;
    int parkur;

public:
    Yaris(int takimSayisi, int sporcuSayisi) : toplamTakimSayisi(takimSayisi), sporcuSayisi(sporcuSayisi), parkur(1) {
        srand(time(NULL));
        for (int i = 0; i < toplamTakimSayisi; ++i) {
            Takim takim;
            for (int j = 0; j < sporcuSayisi; ++j) {
                float hiz = rand() % 5 + 1; // Rastgele hız (1-5 arası)
                float konum = 0;
                Sporcu sporcu;
                sporcu.setHiz(hiz);
                sporcu.setKonum(konum);
                takim.sporcuEkle(sporcu);
            }
            takimlar.push_back(takim);
        }
    }

    void setSporcuBilgisi(int takimIndex, int sporcuIndex, float hiz, float konum) {
        takimlar[takimIndex].getSpurcular()[sporcuIndex].setHiz(hiz);
        takimlar[takimIndex].getSpurcular()[sporcuIndex].setKonum(konum);
    }

    void yarisBaslat(float bitisKonumu) {
        std::cout << "Yarış başladı!" << std::endl;
        while (true) {
            for (auto& takim : takimlar) {
                for (auto& sporcu : takim.getSpurcular()) {
                    if (!sporcu.bitisCizgisineUlastiMi(bitisKonumu)) {
                        if (!sporcu.parkurGecisiTamamlandiMi()) {
                            sporcu.parkurGecisiZamaniniAzalt();
                        } else {
                            sporcu.parkurGecisi();
                        }
                        float yeniKonum = sporcu.getKonum() + sporcu.getHiz();
                        sporcu.setKonum(yeniKonum);
                    }
                }
            }
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
        takimSirala();
        derecelendirmeYap();
    }

    void takimSirala() {
        // Takımların toplam hızlarını hesapla ve sırala
        std::vector<std::pair<int, float>> takimHizlari;
        for (int i = 0; i < toplamTakimSayisi; ++i) {
            float toplamHiz = 0;
            for (int j = 0; j < sporcuSayisi; ++j) {
                toplamHiz += takimlar[i].getSpurcular()[j].getHiz();
            }
            takimHizlari.push_back(std::make_pair(i, toplamHiz));
        }
        std::sort(takimHizlari.begin(), takimHizlari.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        // Takım sıralamasını ekrana yazdır
        std::cout << "Takım Sıralaması:\n";
        for (int i = 0; i < toplamTakimSayisi; ++i) {
            std::cout << "Takım " << takimHizlari[i].first + 1 << ": " << takimHizlari[i].second << " birim/s\n";
        }
    }

    void derecelendirmeYap() {
        // Tüm sporcuların derecelerini hesapla ve sırala
        std::vector<std::pair<int, float>> sporcuDereceleri;
        for (int i = 0; i < toplamTakimSayisi; ++i) {
            for (int j = 0; j < sporcuSayisi; ++j) {
                sporcuDereceleri.push_back(std::make_pair((i * sporcuSayisi) + j, takimlar[i].getSpurcular()[j].getKonum()));
            }
        }
        std::sort(sporcuDereceleri.begin(), sporcuDereceleri.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        // Sporcu derecelerini ekrana yazdır
        std::cout << "\nBireysel Dereceler:\n";
        for (int i = 0; i < toplamTakimSayisi * sporcuSayisi; ++i) {
            std::cout << "Sporcu " << sporcuDereceleri[i].first + 1 << ": " << sporcuDereceleri[i].second << " birim\n";
        }
    }
};

int main() {
    int takimSayisi, sporcuSayisi;
    std::cout << "Takım sayısını giriniz: ";
    std::cin >> takimSayisi;
    std::cout << "Her takımdaki sporcu sayısını giriniz: ";
    std::cin >> sporcuSayisi;

    Yaris yaris(takimSayisi, sporcuSayisi);

    // Sporcuların hız ve konum bilgilerini kullanıcıdan al
    for (int i = 0; i < takimSayisi; ++i) {
        for (int j = 0; j < sporcuSayisi; ++j) {
            float hiz, konum;
            std::cout << "Takım " << i + 1 << ", Sporcu " << j + 1 << " için hız ve konumu giriniz: ";
            std::cin >> hiz >> konum;
            yaris.setSporcuBilgisi(i, j, hiz, konum);
        }
    }

    // Yarışı başlat ve sonuçları ekrana yazdır
    float bitisKonumu = 5; // Bitiş çizgisi konumu (örneğin yüzme parkurunun uzunluğu)
    yaris.yarisBaslat(bitisKonumu);

    return 0;
}
