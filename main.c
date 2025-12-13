#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Variabel global
char username[25];
double inputPassword;

// Konstanta untuk batasan nomor rekening dan transfer
#define MAX_NOREKENING 999999
#define MIN_NOREKENING 100000
#define MIN_TRANSFER 20000

// File untuk menyimpan data
FILE *saldo;
const char *namaFile_saldo = "saldo.txt";
FILE *history_transfer;
const char *namaFile_history_transfer = "history_transfer.txt";

// Buffer untuk membaca baris dari file
#define MAX_LINE_LENGTH 200
char line[MAX_LINE_LENGTH];

// Array menulis di file tabungan
double saldo_tabungan;

// Array menulis di file history transfer
double kirimTf;
long noRekeningTf;
char tanggalTf[10];

// Deklarasi fungsi
int menu();
void lihatTabungan();
void transfer();
void peminjaman();
void bayar_tagihan();

// ========================= LIHAT TABUNGAN =========================
void lihatTabungan() {
    char system_operasi[10] = "nt";  // contoh nilai

    if (strcmp(system_operasi, "nt") == 0) {
        system("cls");   // perintah clear di Windows
    } else {
        system("clear"); // perintah clear di Linux/Unix
    }

    int pilihan;
    printf("Masukkan pilihan anda\n");
    printf("1. Tabungan Anda\n2. Tabungan Deposit\n3. Kembali ke menu\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        saldo = fopen(namaFile_saldo, "r");
        if (saldo == NULL) {
            printf("Gagal membuka file tabungan.\n");
        }
        while (fgets(line, sizeof(line), saldo)) break;
        sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);

        printf("================================\n");
        printf("Saldo: Rp. %.2lf", saldo_tabungan);
        printf("\n================================");
        
        fclose(saldo);
        printf("\nTekan Enter untuk melanjutkan...");
        char a[4];
        scanf("%c", &a); // wait for Enter key
        getchar();
    } else if (pilihan == 2) {
        printf("\nIni tabungan deposit\n");
        printf("Ingin kembali ke menu? (y/n): ");
        char kembali;
        scanf(" %c", &kembali);
        if (kembali == 'y' || kembali == 'Y') {
            lihatTabungan();
        }
    } else if (pilihan == 3) {
        menu();
        return;
    } else {
        printf("\nPilihan salah\n");
        lihatTabungan();
    }
}

// ========================= TRANSFER =========================
void transfer() {
    char system_operasi[10] = "nt";  // contoh nilai

    if (strcmp(system_operasi, "nt") == 0) {
        system("cls");   // perintah clear di Windows
    } else {
        system("clear"); // perintah clear di Linux/Unix
    }

    history_transfer = fopen(namaFile_history_transfer, "a+");

    printf("=== Transfer ===");
    while (1)
    {
        printf("\nMasukkan jumlah transfer minimal 20.000 : ");
        scanf("%lf", &kirimTf);
        
        if (kirimTf < MIN_TRANSFER) {
            printf("\nMinimal transfer adalah 20.000\n");
        } else {
            break;
        }
        
    }

    while (1)
    {
        printf("Masukkan tanggal transfer (DD/MM/YYYY)  : ");
        scanf("%9s", tanggalTf);
        break;
    }
    
    while (1)
    {
        printf("Masukkan nomor rekening (max 6 digit)   : ");
        scanf("%ld", &noRekeningTf);
    
        if (noRekeningTf < MIN_NOREKENING || noRekeningTf > MAX_NOREKENING) {
            printf("Nomor rekening harus 1-6 digit!\n");
        } else {
            printf("\nTransfer sebesar %.2lf tanggal %s ke rekening %ld berhasil!\n",
                    kirimTf, tanggalTf, noRekeningTf);
            break;
        }
    }

    fclose(history_transfer);
    printf("Apakah Anda ingin melakukan transfer lagi? (y/n): ");
    char lagi;
    scanf(" %c", &lagi);
    if (lagi == 'y' || lagi == 'Y') {
        transfer();
        return;
    }
    menu();
}

// ========================= PEMINJAMAN =========================
void peminjaman() {
    int pilihan;
    double jumlah;

    printf("\n=== Menu Peminjaman ===\n");
    printf("1. KPR (3jt - 128jt)\n");
    printf("2. Multiguna (5jt - 500jt)\n");
    printf("3. Kembali ke menu utama\n");
    printf("Pilih: ");
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        printf("Masukkan jumlah pinjaman KPR: ");
        scanf("%lf", &jumlah);

        if (jumlah < 3000000 || jumlah > 128000000) {
            printf("KPR minimum 3jt dan maksimum 128jt!\n");
        } else {
            printf("Pengajuan KPR sebesar %.0lf berhasil!\n", jumlah);
            peminjaman();
        }

    } else if (pilihan == 2) {
        printf("Masukkan jumlah pinjaman Multiguna: ");
        scanf("%lf", &jumlah);

        if (jumlah < 5000000 || jumlah > 500000000) {
            printf("Multiguna minimum 5jt dan maksimum 500jt!\n");
        } else {
            printf("Pengajuan Multiguna sebesar %.0lf berhasil!\n", jumlah);
            peminjaman();
        }

    } else if (pilihan == 3) {
        menu();
    }else {
        printf("Pilihan tidak valid!\n");
        peminjaman();
    }
}

// ========================= BAYAR TAGIHAN =========================
void bayar_tagihan() {
    int pilihan;
    double nominal;

    while (1) {
        printf("=== Menu Tagihan ===\n");
        printf("1. PLN\n");
        printf("2. Internet / Paket Kuota\n");
        printf("3. Air\n");
        printf("4. E-Wallet\n");
        printf("5. Asuransi\n");
        printf("6. Pajak Daerah\n");
        printf("7. Pendidikan\n");
        printf("8. Kembali\n");
        printf("Masukkan pilihan: ");
        scanf("%d", &pilihan);

        if (pilihan == 8) {
            break;
        }

        if (pilihan < 1 || pilihan > 7) {
            printf("Pilihan tidak valid!\n");
            continue;
        }

        printf("Masukkan nominal pembayaran: ");
        scanf("%lf", &nominal);

        switch (pilihan) {
            case 1: printf("Pembayaran PLN sebesar %.2lf berhasil!\n", nominal); break;
            case 2: printf("Pembayaran Internet sebesar %.2lf berhasil!\n", nominal); break;
            case 3: printf("Pembayaran Air sebesar %.2lf berhasil!\n", nominal); break;
            case 4: printf("Pembayaran E-Wallet sebesar %.2lf berhasil!\n", nominal); break;
            case 5: printf("Pembayaran Asuransi sebesar %.2lf berhasil!\n", nominal); break;
            case 6: printf("Pembayaran Pajak Daerah sebesar %.2lf berhasil!\n", nominal); break;
            case 7: printf("Pembayaran Pendidikan sebesar %.2lf berhasil!\n", nominal); break;
        }
        menu();
    }
}

// ========================= MENU UTAMA =========================
int menu() {
    int pilihanm;

    do {
        char system_operasi[10] = "nt";  // contoh nilai

        if (strcmp(system_operasi, "nt") == 0) {
            system("cls");   // perintah clear di Windows
        } else {
            system("clear"); // perintah clear di Linux/Unix
        }

        printf("=== Menu Pilihan ===\n");
        printf("1. Lihat Tabungan\n");
        printf("2. Transfer\n");
        printf("3. Peminjaman\n");
        printf("4. Bayar Tagihan\n");
        printf("5. Keluar\n");
        printf("Masukkan pilihan: ");
        scanf("%d", &pilihanm);

        switch (pilihanm) {
        case 1: lihatTabungan(); break;
        case 2: transfer(); break;
        case 3: peminjaman(); break;
        case 4: bayar_tagihan(); break;
        case 5: printf("\nTerima kasih telah menggunakan aplikasi ini!\n"); break;
        default:
        printf("Pilihan salah, coba lagi!\n");
        }
    } while (pilihanm != 5);
    return pilihanm;
}

// ========================= MAIN =========================
int main() {
    char correctUsername[] = "Tioganteng123";
    double correctPassword = 606060;

    char system_operasi[10] = "nt";  // contoh nilai

    if (strcmp(system_operasi, "nt") == 0) {
        system("cls");   // perintah clear di Windows
    } else {
        system("clear"); // perintah clear di Linux/Unix
    }

    printf("\nSelamat Datang di Aplikasi Akubisa-mobile\n");

    while (1) {
        printf("Masukkan username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        if (strcmp(username, correctUsername) == 0) {
            printf("Masukkan password: ");
            scanf("%lf", &inputPassword);

            if (inputPassword == correctPassword) {
                printf("\nLogin berhasil!\n");
                break;
            } else {
                printf("Password salah!\n");
            }
        } else {
            printf("Username salah!\n");
        }
        getchar(); // Clear buffer
    }

    saldo = fopen(namaFile_saldo, "a+"); // append dan read
    if (saldo == NULL) {
        printf("Gagal membuka file tabungan.\n");
        return 1;
    }

    saldo = fopen(namaFile_saldo, "r+");
    if (saldo == NULL) {
        saldo = fopen(namaFile_saldo, "w+");
    }

    if (fgets(line, sizeof(line), saldo) == NULL) {
        printf("Masukkan saldo awal tabungan Anda: ");
        scanf("%lf", &saldo_tabungan);
        fprintf(saldo, "Saldo: Rp. %.2lf\n", saldo_tabungan);
        fclose(saldo);
        printf("Saldo anda sekarang adalah: Rp. %.2lf", saldo_tabungan);
        printf("\n--------------------------------------");
        printf("\nTekan Enter untuk melanjutkan...");
        char a[4];
        scanf("%c", &a); // wait for Enter key
        getchar();
    } else {
        sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
        fclose(saldo);
        printf("Saldo anda sekarang adalah: Rp. %.2lf", saldo_tabungan);
        printf("\n--------------------------------------");
        printf("\nTekan Enter untuk melanjutkan...");
        char a[4];
        scanf("%c", &a); // wait for Enter key
        getchar();
    }



    menu();
    return 0;
}