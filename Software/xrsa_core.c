// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xrsa_core.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XRsa_core_CfgInitialize(XRsa_core *InstancePtr, XRsa_core_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Ctrl_BaseAddress = ConfigPtr->Ctrl_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XRsa_core_Start(XRsa_core *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_AP_CTRL) & 0x80;
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XRsa_core_IsDone(XRsa_core *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XRsa_core_IsIdle(XRsa_core *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XRsa_core_IsReady(XRsa_core *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XRsa_core_EnableAutoRestart(XRsa_core *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_AP_CTRL, 0x80);
}

void XRsa_core_DisableAutoRestart(XRsa_core *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_AP_CTRL, 0);
}

void XRsa_core_Set_base_r(XRsa_core *InstancePtr, XRsa_core_Base_r Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 0, Data.word_0);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 4, Data.word_1);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 8, Data.word_2);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 12, Data.word_3);
}

XRsa_core_Base_r XRsa_core_Get_base_r(XRsa_core *InstancePtr) {
    XRsa_core_Base_r Data;

    Data.word_0 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 0);
    Data.word_1 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 4);
    Data.word_2 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 8);
    Data.word_3 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_BASE_R_DATA + 12);
    return Data;
}

void XRsa_core_Set_exp(XRsa_core *InstancePtr, XRsa_core_Exp Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 0, Data.word_0);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 4, Data.word_1);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 8, Data.word_2);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 12, Data.word_3);
}

XRsa_core_Exp XRsa_core_Get_exp(XRsa_core *InstancePtr) {
    XRsa_core_Exp Data;

    Data.word_0 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 0);
    Data.word_1 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 4);
    Data.word_2 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 8);
    Data.word_3 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_EXP_DATA + 12);
    return Data;
}

void XRsa_core_Set_modulus(XRsa_core *InstancePtr, XRsa_core_Modulus Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 0, Data.word_0);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 4, Data.word_1);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 8, Data.word_2);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 12, Data.word_3);
}

XRsa_core_Modulus XRsa_core_Get_modulus(XRsa_core *InstancePtr) {
    XRsa_core_Modulus Data;

    Data.word_0 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 0);
    Data.word_1 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 4);
    Data.word_2 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 8);
    Data.word_3 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_MODULUS_DATA + 12);
    return Data;
}

XRsa_core_Result XRsa_core_Get_result(XRsa_core *InstancePtr) {
    XRsa_core_Result Data;

    Data.word_0 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_RESULT_DATA + 0);
    Data.word_1 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_RESULT_DATA + 4);
    Data.word_2 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_RESULT_DATA + 8);
    Data.word_3 = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_RESULT_DATA + 12);
    return Data;
}

u32 XRsa_core_Get_result_vld(XRsa_core *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_RESULT_CTRL);
    return Data & 0x1;
}

void XRsa_core_InterruptGlobalEnable(XRsa_core *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_GIE, 1);
}

void XRsa_core_InterruptGlobalDisable(XRsa_core *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_GIE, 0);
}

void XRsa_core_InterruptEnable(XRsa_core *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_IER);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_IER, Register | Mask);
}

void XRsa_core_InterruptDisable(XRsa_core *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_IER);
    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_IER, Register & (~Mask));
}

void XRsa_core_InterruptClear(XRsa_core *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XRsa_core_WriteReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_ISR, Mask);
}

u32 XRsa_core_InterruptGetEnabled(XRsa_core *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_IER);
}

u32 XRsa_core_InterruptGetStatus(XRsa_core *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XRsa_core_ReadReg(InstancePtr->Ctrl_BaseAddress, XRSA_CORE_CTRL_ADDR_ISR);
}

