#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cctype>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>
using namespace std;

// ======================== [MODUL 2] Struct & Nested Struct ========================
struct Pemain {
    string nama;
};

struct Team {
    int id;
    string namaTeam, namaEvent, pemilikNIM;
    Pemain anggota[10];
    int jumlahAnggota;
};

struct User {
    string nama, nim, role;
};

struct Event {
    int id;
    string nama, lokasi, hadiah, tanggal, peraturan;
};

// ======================== [MODUL 2/4] Global Data ========================
User daftarUser[100];
Event daftarEvent[100];
Team daftarTeam[100];

string daftarPesertaEvent[100][100], daftarEventUser[100][100];
int jumlahPesertaEvent[100] = {0}, jumlahEventUser[100] = {0};

int totalUser = 0, totalEvent = 0, totalTeam = 0, UserLogin;
string roleLogin;

// ======================== Forward Declarations ========================
int inputAngka(const string& pesan);
string inputNIM(const string& pesan);
string inputNama(const string& pesan);
string inputNamaEvent(const string& pesan);
string inputTanggal(const string& pesan);
string inputHadiah(const string& pesan);
string inputPeraturan(const string& pesan);
bool cekDuplikasiUser(const string& nama, const string& nim, User dataUser[], int total);
void lihatEvent(Event data[], int n);
void tampilEvent(Event data[], int n, int idx);
int cariTeamByUser(Team data[], int total, string nim);
void menuSorting(Event data[], int n, int jumlahPeserta[]);
int hitungTeamPerEvent(string namaEvent);

// ======================== [MODUL 3] Validasi & Error Handling ========================

int inputAngka(const string& pesan) {
    string line;
    while (true) {
        cout << pesan;
        getline(cin, line);
        string cleaned = "";
        for (char c : line) {
            if (!isspace(static_cast<unsigned char>(c))) {
                cleaned += c;
            }
        }
        if (cleaned.empty()) {
            cout << "Error: Input tidak boleh kosong! Silakan masukkan angka.\n";
            continue;
        }
        bool hanyaAngka = true;
        for (char c : cleaned) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                hanyaAngka = false;
                break;
            }
        }
        if (!hanyaAngka) {
            cout << "Error: Input harus berupa angka saja!\n";
            continue;
        }
        try {
            return stoi(cleaned);
        } catch (...) {
            cout << "Error: Angka tidak valid!\n";
        }
    }
}

string inputNIM(const string& pesan) {
    string nim;
    while (true) {
        cout << pesan;
        getline(cin, nim);
        string cleaned = "";
        for (char c : nim) {
            if (!isspace(static_cast<unsigned char>(c))) {
                cleaned += c;
            }
        }
        if (cleaned.empty()) {
            cout << "Error: NIM tidak boleh kosong!\n";
            continue;
        }
        bool hanyaAngka = true;
        for (char c : cleaned) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                hanyaAngka = false;
                break;
            }
        }
        if (!hanyaAngka) {
            cout << "Error: NIM hanya boleh berisi angka!\n";
            continue;
        }
        return cleaned;
    }
}

string inputNama(const string& pesan) {
    string nama;
    while (true) {
        cout << pesan;
        getline(cin, nama);
        if (nama.empty()) {
            cout << "Error: Nama tidak boleh kosong!\n";
            continue;
        }
        bool hanyaHuruf = true;
        for (char c : nama) {
            if (!isalpha(static_cast<unsigned char>(c))) {
                hanyaHuruf = false;
                break;
            }
        }
        if (!hanyaHuruf) {
            cout << "Error: Nama hanya boleh huruf (a-z, A-Z)!\n";
            continue;
        }
        return nama;
    }
}

string inputNamaEvent(const string& pesan) {
    string nama;
    while (true) {
        cout << pesan;
        getline(cin, nama);
        if (nama.empty()) {
            cout << "Error: Nama event tidak boleh kosong!\n";
            continue;
        }
        bool valid = true;
        for (char c : nama) {
            if (!isalpha(static_cast<unsigned char>(c)) && !isspace(static_cast<unsigned char>(c))) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "Error: Nama event hanya boleh huruf dan spasi!\n";
            continue;
        }
        return nama;
    }
}

string inputTanggal(const string& pesan) {
    string tgl;
    while (true) {
        cout << pesan;
        getline(cin, tgl);
        if (tgl.empty()) {
            cout << "Error: Tanggal tidak boleh kosong!\n";
            continue;
        }
        regex pattern(R"(^\d{2}/\d{2}/\d{4}$)");
        if (!regex_match(tgl, pattern)) {
            cout << "Error: Format tanggal harus DD/MM/YYYY!\n";
            continue;
        }
        int hari = stoi(tgl.substr(0, 2));
        int bulan = stoi(tgl.substr(3, 2));
        int tahun = stoi(tgl.substr(6, 4));
        if (hari < 1 || hari > 31) {
            cout << "Error: Hari harus 01-31!\n";
            continue;
        }
        if (bulan < 1 || bulan > 12) {
            cout << "Error: Bulan harus 01-12!\n";
            continue;
        }
        if (tahun < 2024 || tahun > 2100) {
            cout << "Error: Tahun harus 2024-2100!\n";
            continue;
        }
        return tgl;
    }
}

string formatRupiah(double nilai) {
    stringstream ss;
    ss << fixed << setprecision(2) << nilai;
    string angka = ss.str();
    string hasil = "Rp";
    int titik = angka.find('.');
    string depan = angka.substr(0, titik);
    string belakang = "," + angka.substr(titik + 1);
    int counter = 0;
    for (int i = depan.length() - 1; i >= 0; i--) {
        if (counter > 0 && counter % 3 == 0) {
            hasil = '.' + hasil;
        }
        hasil = depan[i] + hasil;
        counter++;
    }
    return hasil + belakang;
}

string inputHadiah(const string& pesan) {
    string input;
    while (true) {
        cout << pesan;
        getline(cin, input);
        if (input.empty()) {
            cout << "Error: Hadiah tidak boleh kosong!\n";
            continue;
        }
        string cleaned = "";
        for (char c : input) {
            if (isdigit(static_cast<unsigned char>(c)) || c == '.' || c == ',') {
                cleaned += c;
            }
        }
        if (cleaned.empty()) {
            cout << "Error: Hadiah harus berisi angka!\n";
            continue;
        }
        for (char& c : cleaned) {
            if (c == ',') {
                c = '.';
            }
        }
        try {
            double nilai = stod(cleaned);
            if (nilai < 0) {
                cout << "Error: Hadiah tidak boleh negatif!\n";
                continue;
            }
            return formatRupiah(nilai);
        } catch (...) {
            cout << "Error: Format hadiah tidak valid!\n";
            continue;
        }
    }
}

string inputPeraturan(const string& pesan) {
    string peraturan;
    while (true) {
        cout << pesan;
        getline(cin, peraturan);
        if (peraturan.empty()) {
            cout << "Error: Peraturan tidak boleh kosong!\n";
            continue;
        }
        bool valid = true;
        for (char c : peraturan) {
            if (!isalpha(static_cast<unsigned char>(c)) && !isspace(static_cast<unsigned char>(c))) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "Error: Peraturan hanya boleh huruf dan spasi!\n";
            continue;
        }
        return peraturan;
    }
}

bool cekDuplikasiUser(const string& nama, const string& nim, User dataUser[], int total) {
    for (int i = 0; i < total; i++) {
        if (dataUser[i].nama == nama && dataUser[i].nim == nim) {
            return true;
        }
    }
    return false;
}

int hitungTeamPerEvent(string namaEvent) {
    int count = 0;
    for (int i = 0; i < totalTeam; i++) {
        if (daftarTeam[i].namaEvent == namaEvent) {
            count++;
        }
    }
    return count;
}

// ======================== [MODUL 5] Sorting ========================
void bubbleSortNamaAsc(Event data[], int n, int jumlahPeserta[]) {
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j].nama > data[j + 1].nama) {
                Event tempEvent = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tempEvent;
                int tempPeserta = jumlahPeserta[j];
                jumlahPeserta[j] = jumlahPeserta[j + 1];
                jumlahPeserta[j + 1] = tempPeserta;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

int parseHadiahToNumber(string hadiah) {
    string cleaned = "";
    for (char c : hadiah) {
        if (isdigit(c)) {
            cleaned += c;
        }
    }
    return cleaned.empty() ? 0 : stoi(cleaned);
}

void quickSortHadiahDesc(Event data[], int jumlahPeserta[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        int pivotNilai = parseHadiahToNumber(data[mid].hadiah);
        int i = low, j = high;
        while (i <= j) {
            while (parseHadiahToNumber(data[i].hadiah) > pivotNilai) {
                i++;
            }
            while (parseHadiahToNumber(data[j].hadiah) < pivotNilai) {
                j--;
            }
            if (i <= j) {
                Event tempEvent = data[i];
                data[i] = data[j];
                data[j] = tempEvent;
                int tempPeserta = jumlahPeserta[i];
                jumlahPeserta[i] = jumlahPeserta[j];
                jumlahPeserta[j] = tempPeserta;
                i++;
                j--;
            }
        }
        if (low < j) {
            quickSortHadiahDesc(data, jumlahPeserta, low, j);
        }
        if (i < high) {
            quickSortHadiahDesc(data, jumlahPeserta, i, high);
        }
    }
}

void menuSorting(Event data[], int n, int jumlahPeserta[]) {
    int pilihan;
    do {
        cout << "\n=== MENU SORTING EVENT ===\n";
        cout << "1. Sort by Nama (A-Z) - Bubble Sort\n";
        cout << "2. Sort by Hadiah (Terbesar) - Quick Sort\n";
        cout << "3. Kembali ke Menu Admin\n";
        cout << "Pilihan: ";
        pilihan = inputAngka("");
        
        if (pilihan == 1) {
            bubbleSortNamaAsc(data, n, jumlahPeserta);
            cout << "Event diurutkan berdasarkan Nama (A-Z)\n";
            lihatEvent(data, n);
        } else if (pilihan == 2) {
            quickSortHadiahDesc(data, jumlahPeserta, 0, n - 1);
            cout << "Event diurutkan berdasarkan Hadiah (Terbesar)\n";
            lihatEvent(data, n);
        } else if (pilihan != 3) {
            cout << "Pilihan tidak valid. Masukkan angka 1-3.\n";
        }
    } while (pilihan != 3);
}

// ======================== [MODUL 6] Searching ========================
int linearSearchNama(Event data[], int n, string cari) {
    for (int i = 0; i < n; i++) {
        if (data[i].nama == cari) {
            return i;
        }
    }
    return -1;
}

int binarySearchID(Event data[], int n, int cariID) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j].id > data[j + 1].id) {
                swap(data[j], data[j + 1]);
                swap(jumlahPesertaEvent[j], jumlahPesertaEvent[j + 1]);
            }
        }
    }
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (data[mid].id == cariID) {
            return mid;
        } else if (data[mid].id < cariID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int cariTeamByUser(Team data[], int total, string nim) {
    for (int i = 0; i < total; i++) {
        if (data[i].pemilikNIM == nim) {
            return i;
        }
    }
    return -1;
}

void menuSearching(Event data[], int n) {
    int p = inputAngka("\n=== SEARCHING ===\n1. Cari ID\n2. Cari Nama\nPilihan: ");
    if (p == 1) {
        int id = inputAngka("ID: ");
        int idx = binarySearchID(data, n, id);
        if (idx != -1) {
            cout << "Ditemukan: " << data[idx].nama << " - " << data[idx].lokasi << "\n";
        } else {
            cout << "Tidak ditemukan.\n";
        }
    } else if (p == 2) {
        cout << "Nama: ";
        string nm;
        getline(cin, nm);
        int idx = linearSearchNama(data, n, nm);
        if (idx != -1) {
            cout << "Ditemukan: " << data[idx].nama << " - " << data[idx].lokasi << "\n";
        } else {
            cout << "Tidak ditemukan.\n";
        }
    } else {
        cout << "Pilihan tidak valid.\n";
    }
}

// ======================== [TABLE] Display Functions (ASCII ONLY) ========================

void cetakGarisEvent() {
    // ID(4)+Nama(18)+Lokasi(13)+Hadiah(15)+Tanggal(10)+Peraturan(18)+Team(6)
    cout << "+------+--------------------+---------------+-----------------+------------+--------------------+--------+\n";
}

void tampilEvent(Event data[], int n, int idx) {
    if (idx >= n) return;
    
    int jmlTeam = hitungTeamPerEvent(data[idx].nama);
    
    cout << "| " << right << setw(4) << data[idx].id << " | ";
    cout << left << setw(18) << data[idx].nama.substr(0, 18) << " | ";
    cout << left << setw(13) << data[idx].lokasi.substr(0, 13) << " | ";
    cout << right << setw(15) << data[idx].hadiah << " | ";
    cout << setw(10) << data[idx].tanggal << " | ";
    cout << left << setw(18) << data[idx].peraturan.substr(0, 18) << " | ";
    cout << right << setw(6) << jmlTeam << " |\n";
    
    cetakGarisEvent();
    tampilEvent(data, n, idx + 1);
}

void lihatEvent(Event data[], int n) {
    if (n == 0) { 
        cout << "\nBelum ada event yang terdaftar.\n";
        return; 
    }
    
    cout << "\n";
    cetakGarisEvent();
    cout << "| " << right << setw(4) << "ID" << " | ";
    cout << left << setw(18) << "Nama Event" << " | ";
    cout << left << setw(13) << "Lokasi" << " | ";
    cout << right << setw(15) << "Hadiah" << " | ";
    cout << setw(10) << "Tanggal" << " | ";
    cout << left << setw(18) << "Peraturan" << " | ";
    cout << right << setw(6) << "Team" << " |\n";
    cetakGarisEvent();
    
    tampilEvent(data, n, 0);
    
    cout << "   Keterangan: Kolom Team = jumlah team yang mendaftar\n\n";
}

// Lihat Team & Anggota
void lihatTeamDanAnggota() {
    if (totalTeam == 0) {
        cout << "\nBelum ada team terdaftar.\n";
        return;
    }

    cout << "\n+------------------------------+------+------------------------+------------+\n";
    cout << "| " << left << setw(28) << "Nama Team" << " | ";
    cout << right << setw(4) << "ID" << " | ";
    cout << left << setw(22) << "Event Diikuti" << " | ";
    cout << right << setw(10) << "Jml Angg." << " |\n";
    cout << "+------------------------------+------+------------------------+------------+\n";

    for (int i = 0; i < totalTeam; i++) {
        cout << "| " << left << setw(28) << daftarTeam[i].namaTeam.substr(0, 28) << " | ";
        cout << right << setw(4) << daftarTeam[i].id << " | ";
        cout << left << setw(22) << daftarTeam[i].namaEvent.substr(0, 22) << " | ";
        cout << right << setw(10) << daftarTeam[i].jumlahAnggota << " |\n";

        if (daftarTeam[i].jumlahAnggota > 0) {
            cout << "| " << left << setw(28) << "  [Anggota]:" << " | ";
            cout << right << setw(4) << "" << " | ";
            cout << left << setw(22) << "" << " | ";
            cout << right << setw(10) << "" << " |\n";
            
            string semuaAnggota = "";
            for (int j = 0; j < daftarTeam[i].jumlahAnggota; j++) {
                if (j > 0) semuaAnggota += ", ";
                semuaAnggota += daftarTeam[i].anggota[j].nama;
            }
            
            const int MAX_PER_LINE = 22;
            int pos = 0;
            while (pos < semuaAnggota.length()) {
                int end = min(pos + MAX_PER_LINE, (int)semuaAnggota.length());
                if (end < semuaAnggota.length()) {
                    int koma = semuaAnggota.find(',', pos);
                    if (koma != string::npos && koma <= pos + MAX_PER_LINE) {
                        end = koma + 1;
                    }
                }
                string bagian = semuaAnggota.substr(pos, end - pos);
                while (!bagian.empty() && (bagian.front() == ' ' || bagian.front() == ',')) {
                    bagian.erase(0, 1);
                }
                while (!bagian.empty() && (bagian.back() == ' ' || bagian.back() == ',')) {
                    bagian.pop_back();
                }
                
                if (!bagian.empty()) {
                    cout << "| " << left << setw(28) << "    " + bagian << " | ";
                    cout << right << setw(4) << "" << " | ";
                    cout << left << setw(22) << "" << " | ";
                    cout << right << setw(10) << "" << " |\n";
                }
                pos = end;
            }
        }
        
        cout << "+------------------------------+------+------------------------+------------+\n";
    }
    cout << "\n";
}

// ======================== CRUD TEAM (USER) ========================
void buatTeam(Team data[], int &total, User dataUser[], int uLogin, Event dataEvent[], int totalEvent) {
    if (total >= 100) {
        cout << "Kapasitas team penuh!\n";
        return;
    }
    if (cariTeamByUser(data, total, dataUser[uLogin].nim) != -1) {
        cout << "Anda sudah memiliki team!\n";
        return;
    }

    cout << "\n=== BUAT TEAM BARU ===\n";
    if (totalEvent == 0) {
        cout << "Belum ada event tersedia.\n";
        return;
    }
    
    cout << "\nPilih Event:\n";
    for (int i = 0; i < totalEvent; i++) {
        cout << (i + 1) << ". " << dataEvent[i].nama << "\n";
    }
    
    int p = inputAngka("Nomor Event: ");
    if (p < 1 || p > totalEvent) {
        cout << "Event tidak valid.\n";
        return;
    }

    Team t;
    t.id = total + 1;
    t.namaEvent = dataEvent[p - 1].nama;
    t.pemilikNIM = dataUser[uLogin].nim;
    t.jumlahAnggota = 0;
    t.namaTeam = inputNama("Nama Team: ");

    cout << "\nTambah Pemain Awal (maks. 10):\n";
    while (t.jumlahAnggota < 10) {
        cout << "Nama pemain (Enter = selesai): ";
        string pl;
        getline(cin, pl);
        if (pl.empty()) {
            break;
        }
        bool inv = false;
        for (char c : pl) {
            if (!isalpha(static_cast<unsigned char>(c))) {
                inv = true;
                break;
            }
        }
        if (inv) {
            cout << "Nama pemain hanya boleh huruf!\n";
            continue;
        }
        t.anggota[t.jumlahAnggota++].nama = pl;
    }
    if (t.jumlahAnggota == 0) {
        cout << "Team minimal harus punya 1 pemain!\n";
        return;
    }
    data[total++] = t;
    cout << "Team '" << t.namaTeam << "' berhasil dibuat!\n";
}

void tambahPemain(Team data[], int total, string nim) {
    int idx = cariTeamByUser(data, total, nim);
    if (idx == -1) {
        cout << "Anda tidak memiliki team.\n";
        return;
    }
    if (data[idx].jumlahAnggota >= 10) {
        cout << "Team sudah penuh (maksimal 10 pemain)!\n";
        return;
    }

    cout << "Nama pemain baru: ";
    string namaBaru;
    getline(cin, namaBaru);

    if (namaBaru.empty()) {
        cout << "Nama tidak boleh kosong!\n";
        return;
    }
    bool valid = true;
    for (char c : namaBaru) {
        if (!isalpha(static_cast<unsigned char>(c))) {
            valid = false;
            break;
        }
    }
    if (!valid) {
        cout << "Nama pemain hanya boleh huruf (a-z, A-Z)!\n";
        return;
    }

    data[idx].anggota[data[idx].jumlahAnggota++].nama = namaBaru;
    cout << "Pemain '" << namaBaru << "' berhasil ditambahkan!\n";
}

void lihatPeraturanDanTeam(Team dataTeam[], int totalTeam, User dataUser[], int uLogin, Event dataEvent[], int totalEvent) {
    cout << "\n=== PERATURAN & TEAM TERDAFTAR ===\n";
    
    int idx = cariTeamByUser(dataTeam, totalTeam, dataUser[uLogin].nim);
    if (idx != -1) {
        for (int i = 0; i < totalEvent; i++) {
            if (dataEvent[i].nama == dataTeam[idx].namaEvent) {
                cout << "\nPeraturan: " << dataTeam[idx].namaEvent << "\n";
                cout << (dataEvent[i].peraturan.empty() ? "Belum ada peraturan.\n" : dataEvent[i].peraturan + "\n");
                break;
            }
        }
    } else {
        cout << "Anda belum membuat team.\n";
    }

    cout << "\nDaftar Team:\n";
    cout << "----------------------------------------\n";
    for (int i = 0; i < totalTeam; i++) {
        cout << (i + 1) << ". " << dataTeam[i].namaTeam << " - " << dataTeam[i].namaEvent;
        cout << " (" << dataTeam[i].jumlahAnggota << " anggota)\n";
    }
    cout << "----------------------------------------\n";
}

void updatePemain(Team data[], int total, string nim) {
    int idx = cariTeamByUser(data, total, nim);
    if (idx == -1) {
        cout << "Anda tidak memiliki team.\n";
        return;
    }
    if (data[idx].jumlahAnggota == 0) {
        cout << "Team belum punya pemain.\n";
        return;
    }
    
    cout << "\n=== EDIT PEMAIN ===\n";
    for (int i = 0; i < data[idx].jumlahAnggota; i++) {
        cout << (i + 1) << ". " << data[idx].anggota[i].nama << "\n";
    }
    
    int p = inputAngka("Nomor pemain: ");
    if (p < 1 || p > data[idx].jumlahAnggota) {
        cout << "Nomor tidak valid.\n";
        return;
    }
    string nm = inputNama("Nama baru: ");
    data[idx].anggota[p - 1].nama = nm;
    cout << "Nama pemain berhasil diubah.\n";
}

void hapusPemain(Team data[], int total, string nim) {
    int idx = cariTeamByUser(data, total, nim);
    if (idx == -1) {
        cout << "Anda tidak memiliki team.\n";
        return;
    }
    if (data[idx].jumlahAnggota == 0) {
        cout << "Team belum punya pemain.\n";
        return;
    }
    
    cout << "\n=== HAPUS PEMAIN ===\n";
    for (int i = 0; i < data[idx].jumlahAnggota; i++) {
        cout << (i + 1) << ". " << data[idx].anggota[i].nama << "\n";
    }
    
    int p = inputAngka("Nomor pemain: ");
    if (p < 1 || p > data[idx].jumlahAnggota) {
        cout << "Nomor tidak valid.\n";
        return;
    }
    for (int i = p - 1; i < data[idx].jumlahAnggota - 1; i++) {
        data[idx].anggota[i] = data[idx].anggota[i + 1];
    }
    data[idx].jumlahAnggota--;
    cout << "Pemain berhasil dihapus.\n";
}

void menuKelolaTeam(Team dataTeam[], int &totalTeam, User dataUser[], int uLogin, Event dataEvent[], int totalEvent) {
    int p;
    do {
        cout << "\n=== MENU KELOLA TEAM ===\n";
        cout << "1. Buat Team Baru\n";
        cout << "2. Lihat Peraturan & Daftar Team\n";
        cout << "3. Tambah Pemain\n";
        cout << "4. Ganti Nama Pemain\n";
        cout << "5. Hapus Pemain\n";
        cout << "6. Kembali\n";
        cout << "Pilihan: ";
        p = inputAngka("");
        if (p == 1) {
            buatTeam(dataTeam, totalTeam, dataUser, uLogin, dataEvent, totalEvent);
        } else if (p == 2) {
            lihatPeraturanDanTeam(dataTeam, totalTeam, dataUser, uLogin, dataEvent, totalEvent);
        } else if (p == 3) {
            tambahPemain(dataTeam, totalTeam, dataUser[uLogin].nim);
        } else if (p == 4) {
            updatePemain(dataTeam, totalTeam, dataUser[uLogin].nim);
        } else if (p == 5) {
            hapusPemain(dataTeam, totalTeam, dataUser[uLogin].nim);
        } else if (p != 6) {
            cout << "Pilihan tidak valid. Masukkan angka 1-6.\n";
        }
    } while (p != 6);
}

// ======================== CRUD EVENT (ADMIN) ========================
void tambahEvent(Event data[], int &n) {
    cout << "\n=== TAMBAH EVENT ===\n";
    data[n].nama = inputNamaEvent("Nama Event: ");
    cout << "Lokasi: ";
    string lokasi;
    while (true) {
        getline(cin, lokasi);
        if (!lokasi.empty()) {
            break;
        }
        cout << "Error: Lokasi tidak boleh kosong! Silakan isi: ";
    }
    data[n].lokasi = lokasi;
    data[n].hadiah = inputHadiah("Hadiah (angka): ");
    data[n].tanggal = inputTanggal("Tanggal (DD/MM/YYYY): ");
    data[n].peraturan = inputPeraturan("Peraturan (huruf & spasi): ");
    data[n].id = n + 1;
    n++;
    cout << "Event '" << data[n-1].nama << "' berhasil ditambahkan!\n";
}

void ubahEvent(Event data[], int n) {
    if (n == 0) {
        cout << "Belum ada event.\n";
        return;
    }
    int id = inputAngka("\n=== UBAH EVENT ===\nID Event: ");
    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "Event tidak ditemukan.\n";
        return;
    }
    
    cout << "\nData Saat Ini:\n";
    cout << "Nama   : " << data[idx].nama << "\n";
    cout << "Lokasi : " << data[idx].lokasi << "\n";
    cout << "Hadiah : " << data[idx].hadiah << "\n";
    cout << "Tanggal: " << data[idx].tanggal << "\n";
    cout << "Peraturan: " << data[idx].peraturan << "\n";
    
    data[idx].nama = inputNamaEvent("Nama Baru: ");
    cout << "Lokasi Baru: ";
    string lokasi;
    while (true) {
        getline(cin, lokasi);
        if (!lokasi.empty()) {
            break;
        }
        cout << "Error: Lokasi tidak boleh kosong! Silakan isi: ";
    }
    data[idx].lokasi = lokasi;
    data[idx].hadiah = inputHadiah("Hadiah Baru: ");
    data[idx].tanggal = inputTanggal("Tanggal Baru: ");
    data[idx].peraturan = inputPeraturan("Peraturan Baru: ");
    cout << "Event berhasil diubah.\n";
}

void hapusEvent(Event data[], int &n) {
    if (n == 0) {
        cout << "Belum ada event.\n";
        return;
    }
    cout << "\n=== HAPUS EVENT ===\n";
    lihatEvent(data, n);
    int id = inputAngka("ID Event yang akan dihapus: ");
    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "Event tidak ditemukan.\n";
        return;
    }
    
    cout << "Konfirmasi: Hapus event '" << data[idx].nama << "'? (y/n): ";
    string konfirmasi;
    getline(cin, konfirmasi);
    if (konfirmasi != "y" && konfirmasi != "Y") {
        cout << "Pembatalan penghapusan.\n";
        return;
    }
    
    for (int i = idx; i < n - 1; i++) {
        data[i] = data[i + 1];
        jumlahPesertaEvent[i] = jumlahPesertaEvent[i + 1];
    }
    n--;
    cout << "Event berhasil dihapus.\n";
}

// ======================== AUTH & REGISTRATION ========================
bool login(User dataUser[], int n, string &role, int &idx) {
    cout << "\n=== LOGIN ===\n";
    string nm = inputNama("Nama: ");
    string nim = inputNIM("NIM: ");
    for (int i = 0; i < n; i++) {
        if (dataUser[i].nama == nm && dataUser[i].nim == nim) {
            role = dataUser[i].role;
            idx = i;
            cout << "Login berhasil!\n";
            return true;
        }
    }
    cout << "Nama atau NIM salah.\n";
    return false;
}

void registerUser(User dataUser[], int &n) {
    cout << "\n=== REGISTER ===\n";
    string nm = inputNama("Nama: ");
    string nim = inputNIM("NIM: ");
    if (cekDuplikasiUser(nm, nim, dataUser, n)) {
        cout << "Error: Kombinasi Nama & NIM sudah terdaftar!\n";
        return;
    }
    dataUser[n].nama = nm;
    dataUser[n].nim = nim;
    dataUser[n].role = "user";
    n++;
    cout << "Register berhasil!\n";
}

// ======================== MENU ADMIN ========================
void menuAdmin() {
    int p;
    do {
        cout << "\n=== MENU ADMIN ===\n";
        cout << "1. Tambah Event\n";
        cout << "2. Lihat Event\n";
        cout << "3. Ubah Event\n";
        cout << "4. Hapus Event\n";
        cout << "5. Sorting Event\n";
        cout << "6. Searching\n";
        cout << "7. Lihat Team & Anggota\n";
        cout << "8. Logout\n";
        cout << "Pilihan: ";
        p = inputAngka("");
        if (p == 1) {
            tambahEvent(daftarEvent, totalEvent);
        } else if (p == 2) {
            lihatEvent(daftarEvent, totalEvent);
        } else if (p == 3) {
            ubahEvent(daftarEvent, totalEvent);
        } else if (p == 4) {
            hapusEvent(daftarEvent, totalEvent);
        } else if (p == 5) {
            menuSorting(daftarEvent, totalEvent, jumlahPesertaEvent);
        } else if (p == 6) {
            menuSearching(daftarEvent, totalEvent);
        } else if (p == 7) {
            lihatTeamDanAnggota();
        } else if (p != 8) {
            cout << "Pilihan tidak valid. Masukkan angka 1-8.\n";
        }
    } while (p != 8);
}

// ======================== MENU USER ========================
void menuUser() {
    int p;
    do {
        cout << "\n=== MENU USER ===\n";
        cout << "1. Lihat Event\n";
        cout << "2. Kelola Team\n";
        cout << "3. Logout\n";
        cout << "Pilihan: ";
        p = inputAngka("");
        if (p == 1) {
            lihatEvent(daftarEvent, totalEvent);
        } else if (p == 2) {
            menuKelolaTeam(daftarTeam, totalTeam, daftarUser, UserLogin, daftarEvent, totalEvent);
        } else if (p != 3) {
            cout << "Pilihan tidak valid. Masukkan angka 1-3.\n";
        }
    } while (p != 3);
}

// ======================== MAIN ========================
int main() {
    daftarUser[totalUser++] = {"Albert", "95", "admin"};
    daftarUser[totalUser++] = {"Adri", "110", "admin"};
    daftarUser[totalUser++] = {"Rando", "93", "admin"};
    daftarEvent[totalEvent++] = {1, "Lomba Lari", "Lapangan", "Rp1.000.000,00", "10/06/2026", "Dilarang pakai alas kaki"};

    int p;
    do {
        cout << "\n=== PENDAFTARAN EVENT ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        cout << "Pilihan: ";
        p = inputAngka("");
        if (p == 1) {
            registerUser(daftarUser, totalUser);
        } else if (p == 2) {
            bool ok = false;
            for (int i = 1; i <= 3; i++) {
                if (login(daftarUser, totalUser, roleLogin, UserLogin)) {
                    ok = true;
                    break;
                }
                if (i < 3) {
                    cout << "Login gagal (" << i << "/3). Silakan coba lagi.\n";
                }
            }
            if (!ok) {
                cout << "Login gagal 3 kali. Program ditutup.\n";
                return 0;
            }
            if (roleLogin == "admin") {
                menuAdmin();
            } else {
                menuUser();
            }
        } else if (p != 3) {
            cout << "Pilihan tidak valid. Masukkan angka 1-3.\n";
        }
    } while (p != 3);
    cout << "Program selesai.\n";
    return 0;
}