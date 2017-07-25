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

#ifndef MAMA_BRIDGE_ZMQ_INBOX_H__
#define MAMA_BRIDGE_ZMQ_INBOX_H__


/*=========================================================================
  =                             Includes                                  =
  =========================================================================*/

#include <bridge.h>


#if defined(__cplusplus)
extern "C" {
#endif

/*=========================================================================
  =                  Public implementation functions                      =
  =========================================================================*/

/**
 * This function will return the topic on which to reply in order to reach the
 * supplied inbox.
 *
 * @param inboxBridge The inbox implementation to extract the reply subject from.
 *
 * @return const char* containing the subject on which to reply.
 */
zmqBridgeMsgReplyHandle*
zmqBridgeMamaInboxImpl_getReplyHandle (inboxBridge inbox);


#if defined(__cplusplus)
}
#endif

#endif /* MAMA_BRIDGE_ZMQ_INBOX_H__ */
