#include <iostream>  
#include <fstream>   
#include <string>
#include <iomanip> 
using namespace std;

struct Buku {
    string kode, judul, pengarang;
    int tahun;
    Buku* next; 
};

Buku* head = nullptr; 
string namaFileGlobal; 

bool login() {
    string username, password;
    cout << "===== LOGIN =====\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "123") {
        cout << "Login berhasil!\n";
        return true;
    } else {
        cout << "Login gagal. Username/password salah.\n";
        return false;
    }
}

void tentukanNamaFile() {
    cout << "Masukkan nama file: ";
    cin >> namaFileGlobal; 
}

void tambahBuku(int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        cout << "\nData buku ke-" << (i+1) << ":\n";

        Buku* baru = new Buku; 
        cout << "Masukkan Kode Buku     : ";
        cin >> baru->kode;
        cin.ignore(); 
        cout << "Masukkan Judul Buku    : ";
        getline(cin, baru->judul);
        cout << "Masukkan Pengarang     : ";
        getline(cin, baru->pengarang);
        cout << "Masukkan Tahun Terbit  : ";
        cin >> baru->tahun;

        baru->next = nullptr; 

        if (head == nullptr) {
            head = baru; 
        } else {
            Buku* temp = head; 
            while (temp->next != nullptr)
                temp = temp->next; 
            temp->next = baru; 
        }
    }

    ofstream file(namaFileGlobal, ios::app); 
    if (!file) return;

    Buku* temp = head; 
    while (temp != nullptr) {
        file << temp->kode << "|" << temp->judul << "|" << temp->pengarang << "|" << temp->tahun << endl;
        temp = temp->next; 
    }
    file.close();

    while (head != nullptr) {
        Buku* hapus = head; 
        head = head->next;  
        delete hapus; 
    }
}

void muatDariFile() {
    head = nullptr; 
    ifstream file(namaFileGlobal); 
    if (!file) return;

    string line, kode, judul, pengarang;
    int tahun;

    while (getline(file, line)) {
        size_t pos1 = line.find("|");
        size_t pos2 = line.find("|", pos1 + 1);
        size_t pos3 = line.find("|", pos2 + 1);

        kode = line.substr(0, pos1);
        judul = line.substr(pos1 + 1, pos2 - pos1 - 1);
        pengarang = line.substr(pos2 + 1, pos3 - pos2 - 1);
        tahun = stoi(line.substr(pos3 + 1)); 

        Buku* baru = new Buku{kode, judul, pengarang, tahun, nullptr}; 

        if (head == nullptr) {
            head = baru; 
        } else {
            Buku* temp = head; 
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = baru; 
        }
    }
    file.close();
}

void tampilkanBuku() {
    if (head == nullptr) return; 

    cout << "\nDaftar Buku:\n";
    cout << left
         << setw(10) << "Kode"
         << setw(30) << "Judul"
         << setw(25) << "Pengarang"
         << setw(6) << "Tahun" << endl;
    cout << string(71, '=') << endl;
    Buku* temp = head; 
    while (temp != nullptr) {
        cout << left
             << setw(10) << temp->kode
             << setw(30) << temp->judul
             << setw(25) << temp->pengarang
             << setw(6) << temp->tahun << endl;
        temp = temp->next; 
    }
}

void cariBuku() {
    if (head == nullptr) { 
        cout << "Data buku kosong.\n";
        return;
    }

    string kodeCari;
    cout << "Masukkan kode buku yang dicari: ";
    cin >> kodeCari; 

    Buku* temp = head; 
    bool ditemukan = false; 

    while (temp != nullptr) {
        if (temp->kode == kodeCari) {
            cout << "\nBuku ditemukan:\n";
            cout << "Kode     : " << temp->kode << endl;
            cout << "Judul    : " << temp->judul << endl;
            cout << "Pengarang: " << temp->pengarang << endl;
            cout << "Tahun    : " << temp->tahun << endl;
            ditemukan = true;
            break; 
        }
        temp = temp->next; 
    }

    if (!ditemukan) 
        cout << "Buku dengan kode tersebut tidak ditemukan.\n";
}

void urutBuku() {
    if (head == nullptr || head->next == nullptr) return;

    bool swapped;
    do {
        swapped = false; 
        Buku* curr = head; 
        Buku* prev = nullptr; 

        while (curr->next != nullptr) {
            if (curr->judul > curr->next->judul) {
                Buku* nextNode = curr->next; 
                curr->next = nextNode->next; 
                nextNode->next = curr; 

                if (prev == nullptr) {
                    head = nextNode; 
                } else {
                    prev->next = nextNode; 
                }
                swapped = true; 
                prev = nextNode; 
            } else {
                prev = curr; 
                curr = curr->next; 
            }
        }
    } while (swapped);
    cout << "Data buku berhasil diurutkan berdasarkan judul.\n";
}

void hapusBuku() {
    if (head == nullptr) {
        cout << "Data buku kosong.\n";
        return;
    }

    string kodeHapus;
    cout << "Masukkan kode buku yang ingin dihapus: ";
    cin >> kodeHapus;

    Buku* temp = head; 
    Buku* prev = nullptr; 
    while (temp != nullptr && temp->kode != kodeHapus) {
        prev = temp; 
        temp = temp->next; 
    }

    if (temp == nullptr) {
        cout << "Buku dengan kode tersebut tidak ditemukan.\n";
        return;
    }

    if (prev == nullptr) {
        head = head->next; 
    } else {
        prev->next = temp->next;
    }

    delete temp; 
    cout << "Buku berhasil dihapus.\n";

    ofstream file(namaFileGlobal); 
    Buku* curr = head; 
    while (curr != nullptr) {
        file << curr->kode << "|" << curr->judul << "|" << curr->pengarang << "|" << curr->tahun << endl;
        curr = curr->next; 
    }
    file.close(); 
}

int main() {
    while (!login()) {
        cout << "Silakan coba lagi.\n\n";
    }

    int pilihan;
    do {
        cout << "\n=== MENU PERPUSTAKAAN MINI ===\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Buku\n";
        cout << "3. Cari Buku\n";     
        cout << "4. Urutkan Buku\n";
        cout << "5. Hapus Buku\n";  
        cout << "6. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        string ulang;
        switch (pilihan) {
            case 1:
                tentukanNamaFile(); 
                do {
                    int jumlah;
                    cout << "Berapa banyak buku yang ingin ditambahkan? ";
                    cin >> jumlah;
                    tambahBuku(jumlah); 
                    cout << "Ingin menambahkan lagi? (y/n): ";
                    cin >> ulang;
                } while (ulang == "y");
                break;

            case 2:
                tentukanNamaFile(); 
                muatDariFile();     
                tampilkanBuku(); 
                break;

            case 3:
                tentukanNamaFile(); 
                muatDariFile(); 
                cariBuku(); 
                break;
                
            case 4:
                tentukanNamaFile(); 
                muatDariFile(); 
                urutBuku(); 
                tampilkanBuku(); 
                break;

            case 5:
                tentukanNamaFile(); 
                muatDariFile(); 
                hapusBuku(); 
                tampilkanBuku(); 
                break;

            case 6:
                cout << "Terima kasih!\n"; 
                break;

            default:
                break;
        }
    } while (pilihan != 6);

    return 0;
}
