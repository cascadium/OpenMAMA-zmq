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

#include <stdlib.h>
#include <string.h>
#include <mama/mama.h>
#include <mama/integration/msg.h>
#include "transport.h"
#include "msg.h"

#include <wombat/memnode.h>
#include <mama/integration/bridge/base.h>

#include "zmqbridgefunctions.h"


/*=========================================================================
  =                              Macros                                   =
  =========================================================================*/

#define     ZMQ_MSG_PROPERTY_LEN     1024


/*=========================================================================
  =              Public interface implementation functions                =
  =========================================================================*/

mama_status
zmqBridgeMamaMsgImpl_serialize (msgBridge      msg,
                                mamaMsg        source,
                                void**         target,
                                size_t*        size)
{
    mama_size_t msgSize = 0;
    const void* msgBuffer = NULL;
    char* sendSubject = NULL;
    char* targetSubject = NULL;
    char* replyInboxName = NULL;
    void* serializedBuffer = NULL;
    size_t serializedBufferSize = 0;
    mama_size_t payloadSize = 0;
    size_t msgSubjectByteCount = 0;
    size_t msgInboxByteCount = 0;
    //size_t msgReplyToByteCount = 0;
    size_t msgTargetSubjectByteCount = 0;
    size_t serializedSize = 0;
    baseMsgType msgType;

    // Serialize payload
    mama_status status = mamaMsg_getByteBuffer (source, &msgBuffer, &msgSize);

    // Gather required state from base bridge
    baseBridgeMamaMsgImpl_getSendSubject(msg, &sendSubject);
    baseBridgeMamaMsgImpl_getMsgType(msg, &msgType);
    baseBridgeMamaMsgImpl_getTargetSubject(msg, &targetSubject);
    baseBridgeMamaMsgImpl_getInboxName(msg, &replyInboxName);

    serializedSize = strlen(sendSubject) + 10 + msgSize;
    switch (msgType)
    {
    case BASE_MSG_INBOX_REQUEST:
        serializedSize += strlen (replyInboxName) + 1;
        break;
    case BASE_MSG_INBOX_RESPONSE:
        serializedSize += strlen (targetSubject) + 1;
        break;
    case BASE_MSG_SUB_REQUEST:
    case BASE_MSG_PUB_SUB:
    default:
        break;
    }

    status = baseBridgeMamaMsgImpl_getSerializationBuffer (msg,
                                                           &serializedBuffer,
                                                           serializedSize);
    if (MAMA_STATUS_OK != status)
    {
        mama_log (MAMA_LOG_LEVEL_ERROR, "Could not allocate serialization buffer");
        return status;
    }

    // Ok great - we have a buffer now of appropriate size, let's populate it
    uint8_t* bufferPos = (uint8_t*)serializedBuffer;

    // Copy across the subject
    msgSubjectByteCount = strlen (sendSubject) + 1;
    memcpy (bufferPos, sendSubject, msgSubjectByteCount);
    bufferPos += msgSubjectByteCount;

    // Leave 8 bytes empty - receive side will be thankful for them
    memset ((void*)bufferPos, 0, 8);
    bufferPos += 8;

    // Copy across the message type
    *bufferPos = (uint8_t) msgType;
    bufferPos++;

    switch (msgType)
    {
    case BASE_MSG_INBOX_REQUEST:
        // Copy across inbox name
        msgInboxByteCount = strlen (replyInboxName) + 1;
        memcpy (bufferPos, replyInboxName, msgInboxByteCount);
        bufferPos += msgInboxByteCount;
        break;
    case BASE_MSG_INBOX_RESPONSE:
        msgTargetSubjectByteCount = strlen (targetSubject) + 1;
        memcpy (bufferPos, targetSubject, msgTargetSubjectByteCount);
        bufferPos += msgTargetSubjectByteCount;
        break;
    case BASE_MSG_SUB_REQUEST:
    case BASE_MSG_PUB_SUB:
    default:
        break;
    }

    // Copy across the payload
    memcpy ((void*)bufferPos, msgBuffer, msgSize);

    // Update payload length
    baseBridgeMamaMsgImpl_setPayloadSize(msg, msgSize);

    // Populate return pointers
    *target = serializedBuffer;
    *size = serializedSize;

    return status;
}

mama_status
zmqBridgeMamaMsgImpl_deserialize (msgBridge        msg,
                                  const void*      source,
                                  mama_size_t      size,
                                  mamaMsg          target)
{
    uint8_t* bufferPos = (uint8_t*)source;
    baseMsgType msgType = BASE_MSG_PUB_SUB;
    void* replyHandle = NULL;

    // Skip past the subject - don't care about that here
    bufferPos += strlen((char*)source) + 1;

    // Leave 8 bytes empty - receive side will be thankful for them
    memset ((void*)bufferPos, 0, 8);
    bufferPos += 8;

    // Set the message type
    msgType = (baseMsgType) *bufferPos;
    bufferPos++;

    switch (msgType)
    {
    case BASE_MSG_INBOX_REQUEST:
        baseBridgeMamaMsgImpl_getReplyHandle (msg, &replyHandle);

        baseBridgeMamaMsgReplyHandleImpl_setInboxName (&replyHandle,
                                                       (const char*)bufferPos);
        bufferPos += strlen ((const char*)bufferPos) + 1;
        break;
    case BASE_MSG_INBOX_RESPONSE:
        baseBridgeMamaMsgImpl_setTargetSubject(msg, (const char*)bufferPos);
        bufferPos += strlen ((const char*)bufferPos) + 1;
        break;
    case BASE_MSG_SUB_REQUEST:
    case BASE_MSG_PUB_SUB:
    default:
        break;
    }

    // Parse the payload into a MAMA Message
    size_t payloadSize = size - (bufferPos - (uint8_t*)source);

    mama_log (MAMA_LOG_LEVEL_FINER,
              "zmqBridgeMamaMsgImpl_deserialize(): "
              "Received %lu bytes [payload=%lu; type=%d]",
              size,
              payloadSize,
              msgType);

    mama_status status = mamaMsgImpl_setMsgBuffer (target,
                                                   (void*) bufferPos,
                                                   (uint32_t)payloadSize,
                                                   *bufferPos);

    return status;
}
