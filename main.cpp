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

    bool bitisCizgisineUlastiMi(float bitisKonumu) 
    {
        if (!bitisCizgisineUlasti && konum >= bitisKonumu) 
        {
            bitisCizgisineUlasti = true;
            return true;
        }
        return false;
    }

    void parkurGecisi() 
    {
        if (!parkurGecisiYapildi) 
        {
            parkurGecisiYapildi = true;
            parkurGecisiZamani = 10; // Parkur geçişinde geçirilecek süre (10 saniye)

            // İkinci parkurda hızlar 3 katına çıkar
            if (parkur == 2) 
            {
                hiz *= 3;
            }
            // Üçüncü parkurda hızlar ilk parkurdaki hızlarının 1/3'üne iner
            else if (parkur == 3) 
            {
                hiz /= 3;
            }
        }
    }
};

class Takim 
{
private:
    std::vector<Sporcu> sporcular;
public:
    void sporcuEkle(const Sporcu& sporcu) 
    {
        sporcular.push_back(sporcu);
    }

    const std::vector<Sporcu>& getSpurcular() const 
    {
        return sporcular;
    }
};

class Yaris 
{
private:
    std::vector<Takim> takimlar;
    int toplamTakimSayisi;
    int sporcuSayisi;
    int parkur;

public:
    Yaris(int takimSayisi, int sporcuSayisi) : toplamTakimSayisi(takimSayisi), sporcuSayisi(sporcuSayisi), parkur(1) 
    {
        srand(time(NULL));
        for (int i = 0; i < toplamTakimSayisi; ++i) 
        {
            Takim takim;
            for (int j = 0; j < sporcuSayisi; ++j) 
            {
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

    void setSporcuBilgisi(int takimIndex, int sporcuIndex, float hiz, float konum) 
    {
        takimlar[takimIndex].getSpurcular()[sporcuIndex].setHiz(hiz);
        takimlar[takimIndex].getSpurcular()[sporcuIndex].setKonum(konum);
    }

    void Yaris::yarisBaslat(float yuzmeMesafesi, float bisikletMesafesi, float kosuMesafesi) 
    {
        float bitisKonumu = 0;
        std::cout << "Yüzme parkuru başladı!" << std::endl;
        while (bitisKonumu < yuzmeMesafesi) 
        {
            // Yüzme parkuru için sporcuların hareketi
            for (auto& takim : takimlar) 
            {
                for (auto& sporcu : takim.getSpurcular()) 
                {
                    if (!sporcu.bitisCizgisineUlastiMi(yuzmeMesafesi)) 
                    {
                        sporcu.parkurGecisi();
                        float yeniKonum = sporcu.getKonum() + sporcu.getHiz();
                        sporcu.setKonum(yeniKonum);
                    }
                }
        
            }
            bitisKonumu += yuzmeMesafesi / sporcuSayisi;
        }

        bitisKonumu = 0;
        std::cout << "Bisiklet parkuru başladı!" << std::endl;
        while (bitisKonumu < bisikletMesafesi) 
        {
            // Bisiklet parkuru için sporcuların hareketi
            for (auto& takim : takimlar) 
            {
                for (auto& sporcu : takim.getSpurcular()) 
                {
                    if (!sporcu.bitisCizgisineUlastiMi(bisikletMesafesi)) 
                    {
                        sporcu.parkurGecisi();
                        float yeniKonum = sporcu.getKonum() + sporcu.getHiz();
                        sporcu.setKonum(yeniKonum);
                    }
                }
            }
            bitisKonumu += bisikletMesafesi / sporcuSayisi;
        }

        bitisKonumu = 0;
        std::cout << "Koşu parkuru başladı!" << std::endl;
        while (bitisKonumu < kosuMesafesi) 
        {
            // Koşu parkuru için sporcuların hareketi
            for (auto& takim : takimlar) {
                for (auto& sporcu : takim.getSpurcular()) 
                {
                    if (!sporcu.bitisCizgisineUlastiMi(kosuMesafesi)) 
                    {
                        sporcu.parkurGecisi();
                        float yeniKonum = sporcu.getKonum() + sporcu.getHiz();
                        sporcu.setKonum(yeniKonum);
                    }
                }
            }
            bitisKonumu += kosuMesafesi / sporcuSayisi;
        }
    
        std::cout << "Yarış bitti!" << std::endl;
            takimSirala();
            derecelendirmeYap();
    }

    void takimSirala() 
    {
        // Takımların toplam hızlarını hesapla ve sırala
        std::vector<std::pair<int, float>> takimHizlari;
        for (int i = 0; i < toplamTakimSayisi; ++i) 
        {
            float toplamHiz = 0;
            for (int j = 0; j < sporcuSayisi; ++j) 
            {
                toplamHiz += takimlar[i].getSpurcular()[j].getHiz();
            }
            takimHizlari.push_back(std::make_pair(i, toplamHiz));
        }
        std::sort(takimHizlari.begin(), takimHizlari.end(), [](const auto& a, const auto& b) 
        {
            return a.second > b.second;
        });

        // Takım sıralamasını ekrana yazdır
        std::cout << "Takım Sıralaması:\n";
        for (int i = 0; i < toplamTakimSayisi; ++i) 
        {
            std::cout << "Takım " << takimHizlari[i].first + 1 << ": " << takimHizlari[i].second << " birim/s\n";
        }
    }

    void derecelendirmeYap() 
    {
        // Tüm sporcuların derecelerini hesapla ve sırala
        std::vector<std::pair<int, float>> sporcuDereceleri;
        for (int i = 0; i < toplamTakimSayisi; ++i) 
        {
            for (int j = 0; j < sporcuSayisi; ++j) 
            {
                sporcuDereceleri.push_back(std::make_pair((i * sporcuSayisi) + j, takimlar[i].getSpurcular()[j].getKonum()));
            }
        }
        std::sort(sporcuDereceleri.begin(), sporcuDereceleri.end(), [](const auto& a, const auto& b) 
        {
            return a.second > b.second;
        });

        // Sporcu derecelerini ekrana yazdır
        std::cout << "\nBireysel Dereceler:\n";
        for (int i = 0; i < toplamTakimSayisi * sporcuSayisi; ++i) 
        {
            std::cout << "Sporcu " << sporcuDereceleri[i].first + 1 << ": " << sporcuDereceleri[i].second << " birim\n";
        }
    }
};

int main() {
    int takimSayisi = 300; // Toplam takım sayısı
    int sporcuSayisi = 3; // Her takımdaki sporcu sayısı

    Yaris yaris(takimSayisi, sporcuSayisi);

    float bitisKonumu = 5; // Bitiş çizgisi konumu 
    yaris.yarisBaslat(bitisKonumu);

    int sporcuNumarasi;
    std::cout << "Lutfen bilgi almak istediginiz sporcu numarasini girin: ";
    std::cin >> sporcuNumarasi;

    yaris.sporcuBilgisiYazdir(sporcuNumarasi);

    return 0;
}
