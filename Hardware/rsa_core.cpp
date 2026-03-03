#include <ap_int.h>

// Definim tipuri de date pe 128 biti (pentru testare rapida)
// Poti schimba la 1024 ulterior, dar dureaza mai mult sinteza.
typedef ap_uint<128> rsa_data_t;

void rsa_core(rsa_data_t base, rsa_data_t exp, rsa_data_t modulus, rsa_data_t &result) {
    // Interfete AXI4-Lite pentru procesorul Zynq
    #pragma HLS INTERFACE s_axilite port=base bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=exp bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=modulus bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=result bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    // Algoritmul: Exponentiere Modulara (Binary Method)
    rsa_data_t res = 1;
    rsa_data_t b = base;
    rsa_data_t e = exp;

    WHILE_LOOP: while (e > 0) {
        #pragma HLS PIPELINE off
        if (e[0] == 1) { 
            res = (res * b) % modulus;
        }
        b = (b * b) % modulus;
        e = e >> 1;
    }
    result = res;
}