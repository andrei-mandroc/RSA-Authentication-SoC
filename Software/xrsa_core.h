// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XRSA_CORE_H
#define XRSA_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xrsa_core_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Ctrl_BaseAddress;
} XRsa_core_Config;
#endif

typedef struct {
    u64 Ctrl_BaseAddress;
    u32 IsReady;
} XRsa_core;

typedef u32 word_type;

typedef struct {
    u32 word_0;
    u32 word_1;
    u32 word_2;
    u32 word_3;
} XRsa_core_Base_r;

typedef struct {
    u32 word_0;
    u32 word_1;
    u32 word_2;
    u32 word_3;
} XRsa_core_Exp;

typedef struct {
    u32 word_0;
    u32 word_1;
    u32 word_2;
    u32 word_3;
} XRsa_core_Modulus;

typedef struct {
    u32 word_0;
    u32 word_1;
    u32 word_2;
    u32 word_3;
} XRsa_core_Result;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XRsa_core_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XRsa_core_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XRsa_core_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XRsa_core_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XRsa_core_Initialize(XRsa_core *InstancePtr, UINTPTR BaseAddress);
XRsa_core_Config* XRsa_core_LookupConfig(UINTPTR BaseAddress);
#else
int XRsa_core_Initialize(XRsa_core *InstancePtr, u16 DeviceId);
XRsa_core_Config* XRsa_core_LookupConfig(u16 DeviceId);
#endif
int XRsa_core_CfgInitialize(XRsa_core *InstancePtr, XRsa_core_Config *ConfigPtr);
#else
int XRsa_core_Initialize(XRsa_core *InstancePtr, const char* InstanceName);
int XRsa_core_Release(XRsa_core *InstancePtr);
#endif

void XRsa_core_Start(XRsa_core *InstancePtr);
u32 XRsa_core_IsDone(XRsa_core *InstancePtr);
u32 XRsa_core_IsIdle(XRsa_core *InstancePtr);
u32 XRsa_core_IsReady(XRsa_core *InstancePtr);
void XRsa_core_EnableAutoRestart(XRsa_core *InstancePtr);
void XRsa_core_DisableAutoRestart(XRsa_core *InstancePtr);

void XRsa_core_Set_base_r(XRsa_core *InstancePtr, XRsa_core_Base_r Data);
XRsa_core_Base_r XRsa_core_Get_base_r(XRsa_core *InstancePtr);
void XRsa_core_Set_exp(XRsa_core *InstancePtr, XRsa_core_Exp Data);
XRsa_core_Exp XRsa_core_Get_exp(XRsa_core *InstancePtr);
void XRsa_core_Set_modulus(XRsa_core *InstancePtr, XRsa_core_Modulus Data);
XRsa_core_Modulus XRsa_core_Get_modulus(XRsa_core *InstancePtr);
XRsa_core_Result XRsa_core_Get_result(XRsa_core *InstancePtr);
u32 XRsa_core_Get_result_vld(XRsa_core *InstancePtr);

void XRsa_core_InterruptGlobalEnable(XRsa_core *InstancePtr);
void XRsa_core_InterruptGlobalDisable(XRsa_core *InstancePtr);
void XRsa_core_InterruptEnable(XRsa_core *InstancePtr, u32 Mask);
void XRsa_core_InterruptDisable(XRsa_core *InstancePtr, u32 Mask);
void XRsa_core_InterruptClear(XRsa_core *InstancePtr, u32 Mask);
u32 XRsa_core_InterruptGetEnabled(XRsa_core *InstancePtr);
u32 XRsa_core_InterruptGetStatus(XRsa_core *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
