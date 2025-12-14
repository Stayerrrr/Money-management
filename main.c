#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ================ Variabel global ================

// Variabel login
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
FILE *salDeposit;
const char *namaFile_deposit = "deposit.txt";

// Buffer untuk membaca baris dari file
#define MAX_LINE_LENGTH 200
char line[MAX_LINE_LENGTH];

// Array menulis di file saldo
double saldo_tabungan;
// array deposit
double depoSaldo;

// Array menulis di file history transfer
double kirimTf;
int noRekeningTf;
char tanggalTf[10];

// Deklarasi fungsi
void menu();
void lihatTabungan();
void transfer();
void peminjaman();
void bayar_tagihan();
// done
// ================= LIHAT TABUNGAN DAN DEPOSIT =================
void deposit() {
    // Fixed: membaca file saldo - open and read correctly
    saldo = fopen(namaFile_saldo, "r");
    if (saldo == NULL) {
        printf("Gagal membuka file saldo!\n");
        return;
    }

    if (fgets(line, sizeof(line), saldo) != NULL) {
        sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    }
    fclose(saldo);

    // Fixed: membaca file deposit - open and read correctly
    salDeposit = fopen(namaFile_deposit, "r");
    if (salDeposit == NULL) {
        // If deposit file doesn't exist, prompt to enter initial deposit
        printf("\nMasukkan uang deposit awal: ");
        scanf("%lf", &depoSaldo);
        // Create and write to file
        salDeposit = fopen(namaFile_deposit, "w");
        if (salDeposit != NULL) {
            fprintf(salDeposit, "Saldo Deposit: Rp. %.2lf\n", depoSaldo);
            fclose(salDeposit);
        }
    } else {
        if (fgets(line, sizeof(line), salDeposit) != NULL) {
            sscanf(line, "Saldo Deposit: Rp. %lf", &depoSaldo);
        }
        fclose(salDeposit);
    }

    printf("\n=== DEPOSIT ===\n");
    printf("Saldo anda saat ini: Rp. %.2lf\n", saldo_tabungan);
    printf("Saldo Deposit: Rp. %.2lf\n", depoSaldo);

    printf("\nIngin menambah uang deposit? (y/n): ");
    char kembali;
    scanf(" %c", &kembali);
    if (kembali == 'n' || kembali == 'N') {
        // Fixed: Removed recursive call to deposit() to prevent stack overflow. Now returns to menu.
        return;
    } else {
        double tambahDeposit;
        printf("----------------------------------------\n");
        while (1)
        {
            printf("Masukkan jumlah uang yang ingin ditambahkan ke deposit: ");
            scanf("%lf", &tambahDeposit);

            if (tambahDeposit <= 0) {
                printf("Jumlah harus lebih dari 0!\n");
                continue;
            }
            if (saldo_tabungan < tambahDeposit) {
                printf("Saldo tabungan tidak cukup untuk menambah deposit!\n");
                continue;
            }
            break;
        }
        
        depoSaldo += tambahDeposit;
        // Update file deposit
        FILE *temp = fopen("temp_deposit.txt", "w");
        if (temp == NULL) {
            printf("Gagal membuka file!\n");
            return;
        }
        fprintf(temp, "Saldo Deposit: Rp. %.2lf\n", depoSaldo);
        fclose(temp);
        remove(namaFile_deposit);
        rename("temp_deposit.txt", namaFile_deposit);

        // Update saldo file
        FILE *tempp = fopen("temp_saldo.txt", "w");
        if (tempp == NULL) {
            printf("Gagal membuka file!\n");
            return;
        }
        saldo_tabungan -= tambahDeposit;
        fprintf(tempp, "Saldo: Rp. %.2lf\n", saldo_tabungan);
        fclose(tempp);
        remove(namaFile_saldo);
        rename("temp_saldo.txt", namaFile_saldo);

        printf("----------------------------------------\n");
        printf("Deposit berhasil ditambahkan sebesar Rp. %.2lf\n", tambahDeposit);
        printf("Saldo Deposit sekarang: Rp. %.2lf\n", depoSaldo);
        printf("----------------------------------------\n");
        printf("Tekan enter untuk lanjut...");
        getchar();
        getchar();
    }
}
void tabungan() {
    saldo = fopen(namaFile_saldo, "r");
    if (saldo == NULL) {
        printf("Gagal membuka file tabungan.\n");
        return;
    }
    while (fgets(line, sizeof(line), saldo)) break;
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);

    printf("\n================================\n");
    printf("Saldo: Rp. %.2lf", saldo_tabungan);
    printf("\n================================");

    fclose(saldo);

    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
    getchar();

    // Fixed: Removed recursive call to lihatTabungan() to prevent infinite recursion. Now returns to the loop in lihatTabungan().
    return;
}
void lihatTabungan() {
    while (1) {
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
            tabungan();
        } else if (pilihan == 2) {
            deposit();
        } else if (pilihan == 3) {
            break;
        } else {
            printf("\nPilihan hanya (1/2/3)\n");
            printf("Tekan enter untuk lanjut ");
            getchar();
            getchar();
        }
    }
    return;
}
// done
// ========================= TRANSFER =========================
void transfer() {
    char system_operasi[10] = "nt";  // contoh nilai

    if (strcmp(system_operasi, "nt") == 0) {
        system("cls");   // perintah clear di Windows
    } else {
        system("clear"); // perintah clear di Linux/Unix
    }

    history_transfer = fopen(namaFile_history_transfer, "a+");

    printf("=== Transfer ===\n");
    while (1)
    {
        printf("Masukkan jumlah transfer minimal 20.000 : ");
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
        scanf("%10s", tanggalTf);
        getchar();
        break;
    }

    // Fixed: membaca file saldo - now opens the file before reading to avoid reading from unopened file pointer
    saldo = fopen(namaFile_saldo, "r");
    if (saldo == NULL) {
        printf("Gagal membuka file saldo.\n");
        fclose(history_transfer);
        return;
    }
    while (fgets(line, sizeof(line), saldo)) break;
    sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
    fclose(saldo);

    while (1)
    {
        printf("Masukkan nomor rekening (max 6 digit)   : ");
        scanf("%d", &noRekeningTf);

        if (noRekeningTf < MIN_NOREKENING || noRekeningTf > MAX_NOREKENING) {
            printf("Nomor rekening harus 1-6 digit!\n");
        } else {
            if (saldo_tabungan < kirimTf) {
                printf("\nSaldo tidak cukup! Mau tf lagi (y/n)? ");
                char lagi;
                scanf(" %c", &lagi);
                if (lagi == 'y' || lagi == 'Y') {
                    fclose(history_transfer);
                    return; // Return to menu to allow another transfer
                } else {
                    fclose(history_transfer);
                    return; // Return to menu
                }
            } else {
                fprintf(history_transfer ,"%s | %.2lf | %d\n", tanggalTf, kirimTf, noRekeningTf);
                printf("\nTransfer sebesar %.2lf tanggal %s ke rekening %d berhasil!\n",
                    kirimTf, tanggalTf, noRekeningTf);
                    break;
            }
        }
    }
    fclose(history_transfer);

        // untuk menghitung saldo sesudah mentransfer
        saldo = fopen(namaFile_saldo , "r");
        FILE *temp = fopen("temp.txt", "w");
        if (saldo == NULL || temp == NULL) { // mengecek apakah file berhasil dibuat
            printf("Gagal membuka file!\n");
            return;
        }

        if (saldo_tabungan < kirimTf) { // kondisi jika saldo kurang dari transfer
            printf("Saldo tidak mencukupi!\n");
            fclose(saldo);
            fclose(temp);
            remove("temp.txt");
            return;
        }

        saldo_tabungan -= kirimTf;
        fprintf(temp, "Saldo: Rp. %.2lf\n",saldo_tabungan);

        fclose(saldo);
        fclose(temp);
        remove(namaFile_saldo);
        rename("temp.txt", namaFile_saldo);

        printf("Apakah Anda ingin melakukan transfer lagi? (y/n): ");
        char lagi;
        scanf(" %c", &lagi);
        if (lagi == 'y' || lagi == 'Y') {
            return; // Return to menu to allow another transfer
        } else {
            history_transfer = fopen(namaFile_history_transfer, "r");
            if (history_transfer == NULL) {
                printf("History transfer belum tersedia.\n");
                return;
            }

            printf("\n=== History Transfer ===\n");

            while (fgets(line, sizeof(line), history_transfer)) {
                printf("%s", line);
            }

            fclose(history_transfer);
            printf("------------------------------------");
            printf("\nTekan Enter untuk melanjutkan...");
            char a[4];
            scanf("%c", &a); // wait for Enter key
            getchar();
        }
    return; // Return to menu
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
            // Fixed: Replaced recursive call to peminjaman() with return to prevent nested menus.
            return;
        }

    } else if (pilihan == 2) {
        printf("Masukkan jumlah pinjaman Multiguna: ");
        scanf("%lf", &jumlah);

        if (jumlah < 5000000 || jumlah > 500000000) {
            printf("Multiguna minimum 5jt dan maksimum 500jt!\n");
        } else {
            printf("Pengajuan Multiguna sebesar %.0lf berhasil!\n", jumlah);
            return;
        }

    } else if (pilihan == 3) {
        return;
    }else {
        printf("Pilihan tidak valid!\n");
        return;
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
    }
}

// ========================= MENU UTAMA =========================
void menu() {
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
        default: printf("Pilihan salah, coba lagi!\n"); break;
        }
    } while (pilihanm != 5);
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

    if (fgets(line, sizeof(line), saldo) == NULL) {
        printf("Masukkan saldo awal tabungan Anda: ");
        scanf("%lf", &saldo_tabungan);
        fprintf(saldo, "Saldo: Rp. %.2lf\n", saldo_tabungan);
        fclose(saldo);
        printf("Saldo anda sekarang adalah: Rp. %.2lf", saldo_tabungan);
        printf("\n--------------------------------------");
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
        getchar();
    } else {
        sscanf(line, "Saldo: Rp. %lf", &saldo_tabungan);
        fclose(saldo);
        printf("Saldo anda sekarang adalah: Rp. %.2lf", saldo_tabungan);
        printf("\n--------------------------------------");
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
        getchar();
    }



    menu();
    return 0;
}