/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Frank Quinn (http://fquinner.github.io)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*=========================================================================
  =                             Includes                                  =
  =========================================================================*/

#include <stdint.h>
#include <mama/mama.h>
#include <timers.h>
#include "zmqbridgefunctions.h"
#include <mama/integration/mama.h>
#include <mama/integration/bridge.h>
#include <wombat/strutils.h>


/*=========================================================================
  =                Typedefs, structs, enums and globals                   =
  =========================================================================*/

/* Default payload names and IDs to be loaded when this bridge is loaded */
static char*        PAYLOAD_NAMES[]         =   { "qpidmsg", NULL };
static char         PAYLOAD_IDS[]           =   { MAMA_PAYLOAD_QPID, '\0' };

/*=========================================================================
  =                              Macros                                   =
  =========================================================================*/

/* Version identifiers */
#define             BRIDGE_NAME            "zmq"
#define             BRIDGE_VERSION         "1.0"


/*=========================================================================
  =               Public interface implementation functions               =
  =========================================================================*/

mama_status
zmqBridge_init (mamaBridge bridgeImpl)
{
    mama_status status         = MAMA_STATUS_OK;
    const char* runtimeVersion = NULL;

    /* Reusable buffer to populate with property values */
    char propString[MAX_INTERNAL_PROP_LEN];
    versionInfo rtVer;

    mama_log (MAMA_LOG_LEVEL_SEVERE, "noopBridge_init(): IN INIT");

    /* Will set the bridge's compile time MAMA version */
    MAMA_SET_BRIDGE_COMPILE_TIME_VERSION(BRIDGE_NAME);

    /* Enable extending of the base bridge implementation */
    status = mamaBridgeImpl_setReadOnlyProperty (bridgeImpl,
                                                 MAMA_PROP_EXTENDS_BASE_BRIDGE,
                                                 "true");

    /* Get the runtime version of MAMA and parse into version struct */
    runtimeVersion = mamaInternal_getMetaProperty (MAMA_PROP_MAMA_RUNTIME_VER);
    strToVersionInfo (runtimeVersion, &rtVer);

    return MAMA_STATUS_OK;
}

const char*
zmqBridge_getVersion (void)
{
    return BRIDGE_VERSION;
}

const char*
zmqBridge_getName (void)
{
    return BRIDGE_NAME;
}

mama_status
zmqBridge_getDefaultPayloadId (char ***name, char **id)
{
    if (NULL == name || NULL == id)
    {
        return MAMA_STATUS_NULL_ARG;
    }
    /*
     * Populate name with the value of all supported payload names, the first
     * being the default
     */
    *name   = PAYLOAD_NAMES;

    /*
     * Populate id with the char keys for all supported payload names, the first
     * being the default
     */
    *id     = PAYLOAD_IDS;

     return MAMA_STATUS_OK;
}
