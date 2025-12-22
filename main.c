#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ================ Variabel global ================
char username[25];
double inputPassword;

// konstanta 
#define MAX_NOREKENING 999999
#define MIN_NOREKENING 100000
#define MIN_TRANSFER 20000

// variabel file
FILE *saldo;
const char *namaFile_saldo = "saldo.txt";
FILE *history_transfer;
const char *namaFile_history_transfer = "history_transfer.txt";
FILE *salDeposit;
const char *namaFile_deposit = "deposit.txt";
FILE *history_pinjaman;
const char *namaFile_pinjam = "history_pinjaman.txt";
FILE *tagihan;
const char *namaFile_tagihan = "tagihan.txt";

// variabel line
#define MAX_LINE_LENGTH 200
char line[MAX_LINE_LENGTH];

// variabel untuk file saldo
double saldo_tabungan;
double depoSaldo;

// variabel untuk file transfer
double kirimTf;
int noRekeningTf;
char tanggalTf[11],historyTf[MAX_LINE_LENGTH];

// variabel untuk file pinjaman
char tanggalPinjam[11];

// ================= FUNGSI BANTU =================
void updateSaldo(double saldoBaru) {
    FILE *temp = fopen("temp_saldo.txt", "w");
    if (temp == NULL) {
        printf("Gagal update saldo!\n");
        return;
    }
    fprintf(temp, "Saldo: Rp. %.2lf\n", saldoBaru);
    fclose(temp);
    remove(namaFile_saldo);
    rename("temp_saldo.txt", namaFile_saldo);
}
void lihatSaldo() {
    saldo = fopen(namaFile_saldo, "r");
    if (!saldo) return;
    fgets(line, sizeof(line), saldo);
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);
}

// ================= DEKLARASI =================
void menu();
void lihatTabungan();
void transfer();
void peminjaman();
void bayar_tagihan();
void hapusTransfer();

// done
// ================= LIHAT TABUNGAN & DEPOSIT =================
void tabungan() {
    lihatSaldo();
    printf("\nSaldo Anda: Rp. %.2lf\n", saldo_tabungan);
    printf("Tekan Enter...");
    getchar(); getchar();
}

void deposit() {
    lihatSaldo();

    salDeposit = fopen(namaFile_deposit, "r");
    if (salDeposit) {
        fgets(line, sizeof(line), salDeposit);
        sscanf(line, "Saldo Deposit: Rp. %lf", &depoSaldo);
        fclose(salDeposit);
    } else depoSaldo = 0;

    printf("\nSaldo Tabungan: Rp. %.2lf\n", saldo_tabungan);
    printf("Saldo Deposit : Rp. %.2lf\n", depoSaldo);

    printf("Tambah deposit? (y/n): ");
    char c; scanf(" %c", &c);
    if (c == 'n' || c == 'N') return;

    double tambah;
    printf("Jumlah deposit: ");
    scanf("%lf", &tambah);

    if (tambah <= 0 || tambah > saldo_tabungan) {
        printf("Jumlah tidak valid!\n");
        return;
    }

    saldo_tabungan -= tambah;
    depoSaldo += tambah;
    updateSaldo(saldo_tabungan);

    FILE *t = fopen(namaFile_deposit, "w");
    fprintf(t, "Saldo Deposit: Rp. %.2lf\n", depoSaldo);
    fclose(t);

    printf("\nDeposit berhasil!\n");
    printf("Tekan enter...");
    getchar(); getchar();
}

void lihatTabungan() {
    int p;
    do {
        char system_operasi[10] = "nt";  // contoh nilai
        if (strcmp(system_operasi, "nt") == 0) {
            system("cls");   // perintah clear di Windows
        } else {
            system("clear"); // perintah clear di Linux/Unix
        }
        printf("===== MENU TABUNGANS =====");
        printf("\n1. Tabungan\n2. Deposit\n3. Kembali\nPilih: ");
        scanf("%d", &p);
        if (p == 1) tabungan();
        else if (p == 2) deposit();
    } while (p != 3);
}
// done
// ================= TRANSFER =================
void transfer() {
    char input[20];

    lihatSaldo();

    char system_operasi[10] = "nt";  // contoh nilai
        if (strcmp(system_operasi, "nt") == 0) {
            system("cls");   // perintah clear di Windows
        } else {
            system("clear"); // perintah clear di Linux/Unix
        }

    printf("=== TRANSFER ===\n");
    printf("Saldo anda: %.2lf\n", saldo_tabungan);
    while (1)
    {
        printf("Masukkan jumlah transfer (Minimal 20000) Atau tekan y untuk kembali ke menu\n");
        printf("> ");

        scanf("%s", input);

        // Back to menu
        if (input[0] == 'y' || input[0] == 'Y') {
            return; // keluar dari fungsi transfer
        } 
        int temp = atoi(input);
        
        if (temp < 20000) {
            printf("Saldo transfer minimal 20k \n");
            continue;
        } 
          kirimTf = temp;

        printf("Masukkan nomer rekening: ");
        scanf("%d", &noRekeningTf);
        if (noRekeningTf < MIN_NOREKENING || noRekeningTf > MAX_NOREKENING) {
            printf("No rekening salah!\n");
            continue;
        }
        printf("Tanggal transfer (DD/MM/YEAR): ");
        scanf("%s", tanggalTf);
        
        break;

    }
    
    
    history_transfer = fopen(namaFile_history_transfer, "a");
    fprintf(history_transfer, "%s | %.2lf | %d\n",
            tanggalTf, kirimTf, noRekeningTf);
    fclose(history_transfer);

    saldo_tabungan -= kirimTf;
    updateSaldo(saldo_tabungan);

    printf("\nTransfer berhasil!\n");
    printf("-----------------------\n");

    lihatSaldo();
    printf("Sisa Saldo anda: %.2lf", saldo_tabungan);

    printf("\nHistory Transfer\n");
    history_pinjaman = fopen(namaFile_history_transfer, "r");
    if (history_pinjaman != NULL) {
        while (fgets(line, sizeof(line), history_pinjaman)) {
            printf("%s", line);
        }
        fclose(history_pinjaman);
    }
    printf("\nTekan enter...");
    getchar(); getchar();
}
// done
// ================= PEMINJAMAN (SALDO BERTAMBAH) =================
void peminjaman() {
    int pilihan;
    double jumlah;
    char p[2][20] = {"KPR", "Multiguna"};

    while (1) {
        char system_operasi[10] = "nt";  // contoh nilai
        if (strcmp(system_operasi, "nt") == 0) {
            system("cls");   // perintah clear di Windows
        } else {
            system("clear"); // perintah clear di Linux/Unix
        }
        lihatSaldo();

        printf("\n=== MENU PINJAMAN ===\n");
        printf("1. Ajukan KPR (3jt - 128jt)\n");
        printf("2. Ajukan Multiguna (5jt - 50jt)\n");
        printf("3. Bayar Pinjaman\n");
        printf("4. Kembali\n");
        printf("Pilih: ");

        if (scanf("%d", &pilihan) != 1) {
            while (getchar() != '\n');
            printf("Input tidak valid!\n");
            getchar();
            continue;
        }
        while (getchar() != '\n');

        // ================= KEMBALI =================
        if (pilihan == 4) return;

        // ================= BAYAR PINJAMAN =================
        if (pilihan == 3) {
            history_pinjaman = fopen(namaFile_pinjam, "r");
            if (!history_pinjaman) {
                printf("Belum ada pinjaman.\n");
                getchar();
                continue;
            }

            int no = 1;
            printf("=== Pinjaman ===\n");
            while (fgets(line, sizeof(line), history_pinjaman)) {
                printf("%d | %s", no++, line);
            }
            fclose(history_pinjaman);

            if (no == 1) {
                printf("Belum ada pinjaman.\n");
                getchar();
                continue;
            }

            int noHapus;
            while (1)
            {
                printf("\nPilih mana yang mau dibayar: ");
                if (scanf("%d", &noHapus) != 1) {
                    while (getchar() != '\n');
                    printf("Input tidak valid!\n");
                    continue;
                }
                while (getchar() != '\n');
                break;
            }

            history_pinjaman = fopen(namaFile_pinjam, "r");
            FILE *temp = fopen("temp.txt", "w");

            if (!history_pinjaman || !temp) {
                printf("Gagal membuka file!\n");
                if (history_pinjaman) fclose(history_pinjaman);
                if (temp) fclose(temp);
                getchar();
                continue;
            }

            int idx = 1;
            char tanggal[11], jenis[20];
            double jumlahPinjaman = 0;
            int ditemukan = 0;

            while (fgets(line, sizeof(line), history_pinjaman)) {
                if (idx == noHapus) {
                    // Simpan data pinjaman yang dibayar
                    sscanf(line, "%10[^|] | %19[^|] | %lf",
                        tanggal, jenis, &jumlahPinjaman);
                    ditemukan = 1;
                } else {
                    // Salin pinjaman lain
                    fputs(line, temp);
                }
                idx++;
            }

            fclose(history_pinjaman);
            fclose(temp);

            if (ditemukan == 0) {
                printf("Nomor pinjaman tidak ditemukan!\n");
                remove("temp.txt");
                getchar();
                continue;
            }

            // Cek saldo
            if (jumlahPinjaman > saldo_tabungan) {
                printf("Saldo tidak cukup untuk membayar pinjaman!\n");
                remove("temp.txt");
                getchar();
                continue;
            }

            // Update saldo
            saldo_tabungan -= jumlahPinjaman;
            updateSaldo(saldo_tabungan);

            // Replace file
            remove(namaFile_pinjam);
            rename("temp.txt", namaFile_pinjam);

            printf("\nPinjaman berhasil dibayar!\n");
            printf("Jumlah dibayar: Rp. %.2lf\n", jumlahPinjaman);
            printf("Saldo sekarang: Rp. %.2lf\n", saldo_tabungan);

            printf("\nTekan Enter...");
            getchar();
            continue;
        }

        // ================= AJUKAN PINJAMAN =================
        if (pilihan != 1 && pilihan != 2) {
            printf("Pilihan tidak valid!\n");
            getchar();
            continue;
        }

        while (1)
        {
            printf("Masukkan jumlah pinjaman: ");
            if (scanf("%lf", &jumlah) != 1) {
                while (getchar() != '\n');
                printf("Input salah!\n");
                getchar();
                continue;
            }
            while (getchar() != '\n');
    
            if (pilihan == 1 && (jumlah < 3000000 || jumlah > 128000000)) {
                printf("Jumlah KPR tidak sesuai!\n");
                getchar();
                continue;
            } else if (pilihan == 2 && (jumlah < 5000000 || jumlah > 500000000)) {
                printf("Jumlah Multiguna tidak sesuai!\n");
                getchar();
                continue;
            } else {
                break;
            }
        }
        

        printf("Tanggal (DD/MM/YYYY): ");
        scanf("%10s", tanggalPinjam);
        while (getchar() != '\n');

        saldo_tabungan += jumlah;
        updateSaldo(saldo_tabungan);

        history_pinjaman = fopen(namaFile_pinjam, "a");
        fprintf(history_pinjaman, "%s | %s | %.2lf\n",
                tanggalPinjam, p[pilihan - 1], jumlah);
        fclose(history_pinjaman);

        printf("\nPinjaman disetujui!\n");
        printf("Saldo sekarang: %.2lf\n", saldo_tabungan);
        printf("Tekan Enter...");
        getchar();
    }
}
// 
// ================= BAYAR TAGIHAN (SALDO BERKURANG) =================
void bayar_tagihan() {
    int p;
    double n;

    lihatSaldo();

    char typeTagihan[7][20] = {"PLN","Internet","Air","E-Wallet","Asuransi","Pajak","Pendidikan"};
    
    while (1) {
        char system_operasi[10] = "nt";  // contoh nilai
        if (strcmp(system_operasi, "nt") == 0) {
            system("cls");   // perintah clear di Windows
        } else {
            system("clear"); // perintah clear di Linux/Unix
        }
        printf("=== Menu Tagihan ===");
        printf("\n1. PLN\n2. Internet\n3. Air\n4. E-Wallet\n");
        printf("5. Asuransi\n6. Pajak\n7. Pendidikan\n8. Kembali\nPilih: ");
        scanf("%d", &p);
        if (p < 1 || p > 8) {
            printf("\nPilihan tidak valid!\n");
            getchar(); getchar();
            continue;
        }

        if (p == 8) break;
        
        printf("Nominal: ");
        scanf("%lf", &n);
        
        if (n <= 0 || n > saldo_tabungan) {
            printf("Saldo tidak cukup!\n");
            continue;
        }

        tagihan = fopen(namaFile_tagihan, "a");
        if (tagihan == NULL) {
            printf("Gagal membuka file tagihan!\n");
            return;
        }

        fprintf(tagihan, "%s | %.2lf\n", typeTagihan[p - 1], n);
        fclose(tagihan);

        printf("\n=== History Pembayaran Tagihan ===\n");

        tagihan = fopen(namaFile_tagihan, "r");
        if (tagihan == NULL) {
            printf("Belum ada history tagihan.\n");
            return;
        }

        while (fgets(line, sizeof(line), tagihan)) {
            printf("%s", line);
        }
        fclose(tagihan);

        saldo_tabungan -= n;
        updateSaldo(saldo_tabungan);

        printf("Pembayaran berhasil!\n");
        printf("Sisa saldo: Rp. %.2lf\n", saldo_tabungan);
        printf("Tekan Enter...");
        getchar(); getchar();
    }
}

// ================= MENU =================
void menu() {
    int p;
    do {
        char system_operasi[10] = "nt";  // contoh nilai
        if (strcmp(system_operasi, "nt") == 0) {
            system("cls");   // perintah clear di Windows
        } else {
            system("clear"); // perintah clear di Linux/Unix
        }

        printf("===== MENU UTANA =====");
        printf("\n1. Lihat Tabungan\n2. Transfer\n3. Peminjaman\n4. Bayar Tagihan\n5. Keluar\nPilih: ");
        scanf("%d", &p);

        if (p == 1) lihatTabungan();
        else if (p == 2) transfer();
        else if (p == 3) peminjaman();
        else if (p == 4) bayar_tagihan();
        else if (p == 5) printf("\nProgram selesai. Terima kasih!\n");
        else printf("\n Pilihan tidak valid!\n");

    } while (p != 5);
}

// ================= MAIN =================
int main() {
    char correctUsername[] = "Tioganteng123";
    double correctPassword = 606060;

    char system_operasi[10] = "nt";  // contoh nilai
        if (strcmp(system_operasi, "nt") == 0) {
            system("cls");   // perintah clear di Windows
        } else {
            system("clear"); // perintah clear di Linux/Unix
        }

    printf("Selamat Datang di Aplikasi Akubisa-mobile\n");

    while (1) {
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Password: ");
        scanf("%lf", &inputPassword);
        getchar();

        if (!strcmp(username, correctUsername) &&
            inputPassword == correctPassword) break;

        printf("Login salah!\n");
    }

    saldo = fopen(namaFile_saldo, "a+");
    if (!fgets(line, sizeof(line), saldo)) {
        printf("Masukkan saldo awal: ");
        scanf("%lf", &saldo_tabungan);
        fprintf(saldo, "Saldo: Rp. %.2lf\n", saldo_tabungan);
    }
    fgets(line, sizeof(line), saldo);
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);

    printf("\nLogin berhasil\n");

    printf("\nSaldo anda: %.2lf", saldo_tabungan);
    printf("\nTekan enter...");
    getchar(); 
    menu();
    return 0;
}