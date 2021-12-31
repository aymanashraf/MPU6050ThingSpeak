/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== UART.c ========
 */

#include <stdint.h>
#include <ti/drivers/UART.h>

/* Externs */
extern const UART_Config UART_config[];

/* Used to check status and initialization */
static int UART_count = -1;

/* Default UART parameters structure */
const UART_Params UART_defaultParams = {
    UART_MODE_BLOCKING,   /* readMode */
    UART_MODE_BLOCKING,   /* writeMode */
    UART_WAIT_FOREVER,    /* readTimeout */
    UART_WAIT_FOREVER,    /* writeTimeout */
    NULL,                 /* readCallback */
    NULL,                 /* writeCallback */
    UART_RETURN_NEWLINE,  /* readReturnMode */
    UART_DATA_TEXT,       /* readDataMode */
    UART_DATA_TEXT,       /* writeDataMode */
    UART_ECHO_ON,         /* readEcho */
#if defined(MSP430WARE)
    9600,                 /* baudRate */
#else
    115200,               /* baudRate */
#endif
    UART_LEN_8,           /* dataLength */
    UART_STOP_ONE,        /* stopBits */
    UART_PAR_NONE,        /* parityType */
    (uintptr_t) NULL      /* custom */
};

/*
 *  ======== UART_close ========
 */
void UART_close(UART_Handle handle)
{
    handle->fxnTablePtr->closeFxn(handle);
}

/*
 *  ======== UART_control ========
 */
int UART_control(UART_Handle handle, unsigned int cmd, void *arg)
{
    return (handle->fxnTablePtr->controlFxn(handle, cmd, arg));
}

/*
 *  ======== UART_init ========
 */
void UART_init(void)
{
    if (UART_count == -1) {
        /* Call each driver's init function */
        for (UART_count = 0; UART_config[UART_count].fxnTablePtr != NULL; UART_count++) {
            UART_config[UART_count].fxnTablePtr->initFxn((UART_Handle)&(UART_config[UART_count]));
        }
    }
}

/*
 *  ======== UART_open ========
 */
UART_Handle UART_open(unsigned int index, UART_Params *params)
{
    UART_Handle         handle;

    if (index >= UART_count) {
        return (NULL);
    }

    /* If params are NULL use defaults */
    if (params == NULL) {
        params = (UART_Params *) &UART_defaultParams;
    }

    /* Get handle for this driver instance */
    handle = (UART_Handle)&(UART_config[index]);

    return (handle->fxnTablePtr->openFxn(handle, params));

}

/*
 *  ======== UART_Params_init ========
 */
void UART_Params_init(UART_Params *params)
{
    *params = UART_defaultParams;
}

/*
 *  ======== UART_read ========
 */
int UART_read(UART_Handle handle, void *buffer, size_t size)
{
    return (handle->fxnTablePtr->readFxn(handle, buffer, size));
}

/*
 *  ======== UART_readPolling ========
 */
int UART_readPolling(UART_Handle handle, void *buffer, size_t size)
{
    return (handle->fxnTablePtr->readPollingFxn(handle, buffer, size));
}

/*
 *  ======== UART_readCancel ========
 */
void UART_readCancel(UART_Handle handle)
{
    handle->fxnTablePtr->readCancelFxn(handle);
}

/*
 *  ======== UART_write ========
 */
int UART_write(UART_Handle handle, const void *buffer, size_t size)
{
    return (handle->fxnTablePtr->writeFxn(handle, buffer, size));
}

/*
 *  ======== UART_writePolling ========
 */
int UART_writePolling(UART_Handle handle, const void *buffer, size_t size)
{
    return (handle->fxnTablePtr->writePollingFxn(handle, buffer, size));
}

/*
 *  ======== UART_writeCancel ========
 */
void UART_writeCancel(UART_Handle handle)
{
    handle->fxnTablePtr->writeCancelFxn(handle);
}
