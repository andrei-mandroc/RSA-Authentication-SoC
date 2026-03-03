#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xrsa_core.h"    // Driver Hardware
#include "xgpio.h"        // Driver Butoane
#include "sleep.h"
#include "xil_io.h"       // Timer Manual

//configurare
#define RSA_DEVICE_ID   0 
#ifndef XPAR_AXI_GPIO_BTN_DEVICE_ID
 #define GPIO_BTN_ID  0
#else
 #define GPIO_BTN_ID  XPAR_AXI_GPIO_BTN_DEVICE_ID
#endif

#define CHANNEL_BTN     1 

//timer Zynq
#define GLOBAL_TMR_BASEADDR     0xF8F00200
#define GLOBAL_TMR_CTRL_ADDR    0xF8F00208
#define CPU_CLOCK_HZ            666666687
#define TIMER_FREQ_HZ           (CPU_CLOCK_HZ / 2)

XRsa_core RsaInstance;
XGpio GpioBtn;

//variabile globale
long long p, q, n, phi, e, d;
#define MSG_LENGTH 60
char text_original[MSG_LENGTH] = "astazi";

//stocare rezultate criptare si decriptare
long long criptat_sw[MSG_LENGTH];
long long criptat_hw[MSG_LENGTH];
char decriptat_sw[MSG_LENGTH];
char decriptat_hw[MSG_LENGTH];

//flaguri de stare
int chei_generate = 0;
int mesaj_criptat_ready = 0;


//FUNCTII TIMER
void timer_start() { Xil_Out32(GLOBAL_TMR_CTRL_ADDR, 0x01); }
u32 timer_get_value() { return Xil_In32(GLOBAL_TMR_BASEADDR); }
double ticks_to_us(u32 ticks) { return ((double)ticks * 1000000.0) / (double)TIMER_FREQ_HZ; }


//functii criptare software

bool isPrime(long long num) {
    if (num < 2) return false;
    for (long long i = 2; i * i <= num; i++) if (num % i == 0) return false;
    return true;
}
long long generateNumber(long long min, long long max) {
    long long num; do { num = rand() % (max - min + 1) + min; } while(!isPrime(num)); return num;
}
long long gcd(long long a, long long b) { while (b != 0) { long long t = b; b = a % b; a = t; } return a; }
long long coprime(long long phi) { long long e=phi/4096; while(e<phi) { if(gcd(e,phi)==1) break; e++; } return e; }
long long modInverse(long long e, long long phi) { for(long long d=1; d<phi; d++) if(((long long)e*d)%phi==1) return d; return -1; }


long long modExp_Software(long long base, long long exp, long long mod) {

long long result = 1;

base = base % mod;

for (long long i = 0; i < exp; i++) {

result = (result * base) % mod;

}

return result;

}



//functie hardware

long long modExp_Hardware(long long base, long long exp, long long mod) {
    XRsa_core_Base_r b_st = {0}; b_st.word_0 = (u32)base;
    XRsa_core_Exp    e_st = {0}; e_st.word_0 = (u32)exp;
    XRsa_core_Modulus m_st = {0}; m_st.word_0 = (u32)mod;

    XRsa_core_Set_base_r(&RsaInstance, b_st);
    XRsa_core_Set_exp(&RsaInstance, e_st);
    XRsa_core_Set_modulus(&RsaInstance, m_st);

    XRsa_core_Start(&RsaInstance);
    while (!XRsa_core_IsDone(&RsaInstance));

    XRsa_core_Result res_st = XRsa_core_Get_result(&RsaInstance);
    long long result = (long long)res_st.word_0;
    if (result >= mod) result -= mod;
    return result;
}


//ACTIUNI BUTOANE

// BTN 0: generare chei
void actiune_generare() {
    xil_printf("\n\r-------------------------------------------\n\r");
    xil_printf("[BTN 0] Generare Chei (SW)...\n\r");
    
    u32 t1 = timer_get_value();
    p = generateNumber(1000, 3000); 
    q = generateNumber(3001, 5000); 
    n = p * q;
    phi = (p - 1) * (q - 1);
    e = coprime(phi);
    d = modInverse(e, phi);
    u32 t2 = timer_get_value();

    xil_printf("  -> Timp Generare: %.2f us\n\r", ticks_to_us(t2 - t1));
    xil_printf("  p=%d, q=%d, n=%d\n\r", (int)p, (int)q, (int)n);
    xil_printf("  Public Key (e,n): (%d, %d)\n\r", (int)e, (int)n);
    xil_printf("  Private Key (d,n): (%d, %d)\n\r", (int)d, (int)n);
    
    chei_generate = 1;
    mesaj_criptat_ready = 0; // resetam starea urmatoare, ca sa fortam criptarea din nou
}

// BTN 1: criptare
void actiune_criptare() {
    if (!chei_generate) { xil_printf("[!] EROARE: Genereaza cheile intai (BTN 0)!\n\r"); return; }
    
    xil_printf("\n\r-------------------------------------------\n\r");
    xil_printf("[BTN 1] Criptare Mesaj: '%s'\n\r", text_original);
    
    int len = strlen(text_original);
    u32 t1, t2;

    //software
    t1 = timer_get_value();
    for(int i=0; i<len; i++) criptat_sw[i] = modExp_Software((long long)text_original[i], e, n);
    t2 = timer_get_value();
    double time_sw = ticks_to_us(t2 - t1);

    //hardware
    t1 = timer_get_value();
    for(int i=0; i<len; i++) criptat_hw[i] = modExp_Hardware((long long)text_original[i], e, n);
    t2 = timer_get_value();
    double time_hw = ticks_to_us(t2 - t1);

    // afisare rezultate
    xil_printf("  Criptat SW (vector): "); for(int i=0; i<len; i++) xil_printf("%d ", (int)criptat_sw[i]); xil_printf("\n\r");
    xil_printf("  Criptat HW (vector): "); for(int i=0; i<len; i++) xil_printf("%d ", (int)criptat_hw[i]); xil_printf("\n\r");
    
    printf("  Timp SW: %.2f us | Timp HW: %.2f us\n\r", time_sw, time_hw);
    
    // validare
    int err = 0;
    for(int i=0; i<len; i++) if(criptat_sw[i] != criptat_hw[i]) err=1;
    
    if(!err) {
        xil_printf("[OK] Rezultate Identice.\n\r");
        mesaj_criptat_ready = 1;
          if (time_hw < time_sw)
            printf("  => Hardware-ul este de %.2fx mai rapid.\n\r", time_sw/time_hw);
          else
            printf("  => Hardware-ul este mai lent (overhead AXI la date mici).\n\r");
    

    } else {
        xil_printf("[ERR] Rezultate Diferite!\n\r");
    }
}

// BTN 2: decriptare
void actiune_decriptare() {
    if (!mesaj_criptat_ready) { xil_printf("[!] EROARE: Cripteaza intai (BTN 1)!\n\r"); return; }
    
    xil_printf("\n\r-------------------------------------------\n\r");
    xil_printf("[BTN 2] Decriptare...\n\r");
    
    int len = strlen(text_original);
    u32 t1, t2;

    //software
    t1 = timer_get_value();
    for(int i=0; i<len; i++) decriptat_sw[i] = (char)modExp_Software(criptat_hw[i], d, n);
    t2 = timer_get_value();
    decriptat_sw[len] = '\0';
    double time_sw = ticks_to_us(t2 - t1);

    //hardware
    t1 = timer_get_value();
    for(int i=0; i<len; i++) decriptat_hw[i] = (char)modExp_Hardware(criptat_hw[i], d, n);
    t2 = timer_get_value();
    decriptat_hw[len] = '\0';
    double time_hw = ticks_to_us(t2 - t1);

    // afisare rezultate
    xil_printf("  Decriptat SW (text): %s\n\r", decriptat_sw);
    xil_printf("  Decriptat HW (text): %s\n\r", decriptat_hw);
    printf("  Timp SW: %.2f us | Timp HW: %.2f us\n\r", time_sw, time_hw);

    if (strcmp(text_original, decriptat_hw) == 0) {
        printf("[SUCCES] Mesaj Corect. Speedup: %.2fx\n\r", time_sw/time_hw);
    } else {
        xil_printf("[FAIL] Mesaj Corupt.\n\r");
    }
}


int main() {
    init_platform();
    
    // Init Hardware RSA
    XRsa_core_Config *Cfg = XRsa_core_LookupConfig(RSA_DEVICE_ID);
    if (XRsa_core_CfgInitialize(&RsaInstance, Cfg) != XST_SUCCESS) {
        xil_printf("ERR: RSA Init Failed\n\r"); return -1;
    }

    // Init GPIO
    if (XGpio_Initialize(&GpioBtn, GPIO_BTN_ID) != XST_SUCCESS) {
        XGpio_Initialize(&GpioBtn, 0); // Fallback
    }
    XGpio_SetDataDirection(&GpioBtn, CHANNEL_BTN, 0xF); // Input

    // Init Timer
    timer_start();
    srand(1234);

    xil_printf("\n\r=== SISTEM RSA ZYNQ ===\n\r");
    xil_printf("Instructiuni:\n\r");
    xil_printf(" BTN 0: Generare Chei\n\r");
    xil_printf(" BTN 1: Criptare\n\r");
    xil_printf(" BTN 2: Decriptare\n\r");

    u32 btn_val = 0, btn_last = 0;

    while(1) {
        btn_val = XGpio_DiscreteRead(&GpioBtn, CHANNEL_BTN);

        if (btn_val != 0 && btn_val != btn_last) {
            usleep(200000); // Debounce
            
            if (btn_val & 0x1) actiune_generare();      // Bit 0
            else if (btn_val & 0x2) actiune_criptare(); // Bit 1
            else if (btn_val & 0x4) actiune_decriptare();// Bit 2
        }
        btn_last = btn_val;
        usleep(10000);
    }

    cleanup_platform();
    return 0;
}