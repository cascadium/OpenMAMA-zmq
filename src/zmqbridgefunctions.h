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

#ifndef ZMQ_BRIDGE_FUNCTIONS__
#define ZMQ_BRIDGE_FUNCTIONS__

#include <mama/mama.h>
#include <mama/io.h>
#include <mama/integration/queue.h>
#include <mama/integration/transport.h>
#include <mama/integration/subscription.h>
#include <mama/integration/publisher.h>
#include <mama/integration/inbox.h>
#include <mama/integration/msg.h>

#if defined(__cplusplus)
extern "C" {
#endif

MAMAExpBridgeDLL
extern mama_status
zmqBridge_init (mamaBridge bridgeImpl);

MAMAExpBridgeDLL
extern const char*
zmqBridge_getVersion (void);

MAMAExpBridgeDLL
extern const char*
zmqBridge_getName (void);

MAMAExpBridgeDLL
extern mama_status
zmqBridge_getDefaultPayloadId (char*** name, char** id);

MAMAExpBridgeDLL
extern int
zmqBridgeMamaTransport_isValid (transportBridge transport);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaTransport_destroy (transportBridge transport);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaTransport_create (transportBridge* result,
                               const char*      name,
                               mamaTransport    parent);

MAMAExpBridgeDLL
extern mama_status zmqBridgeMamaSubscription_create
                              (subscriptionBridge* subscriber,
                               const char*         source,
                               const char*         symbol,
                               mamaTransport       transport,
                               mamaQueue           queue,
                               mamaMsgCallbacks    callback,
                               mamaSubscription    subscription,
                               void*               closure );


MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaSubscription_mute (subscriptionBridge subscriber);

MAMAExpBridgeDLL
extern  mama_status
zmqBridgeMamaSubscription_destroy (subscriptionBridge subscriber);

MAMAExpBridgeDLL
extern int
zmqBridgeMamaSubscription_isValid (subscriptionBridge bridge);

MAMAExpBridgeDLL
extern int
zmqBridgeMamaSubscription_isTportDisconnected (subscriptionBridge subsc);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaSubscription_muteCurrentTopic (subscriptionBridge subsc);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_createByIndex (
                              publisherBridge*  result,
                              mamaTransport     tport,
                              int               tportIndex,
                              const char*       topic,
                              const char*       source,
                              const char*       root,
                              mamaPublisher     parent);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_destroy (publisherBridge publisher);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_send (publisherBridge publisher, mamaMsg msg);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_sendReplyToInbox (publisherBridge publisher,
                                         void*           request,
                                         mamaMsg         reply);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_sendReplyToInboxHandle (publisherBridge publisher,
                                               void*           wmwReply,
                                               mamaMsg         reply);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_sendFromInbox (publisherBridge  publisher,
                                      mamaInbox        inbox,
                                      mamaMsg          msg);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_sendFromInboxByIndex (publisherBridge   publisher,
                                             int               tportIndex,
                                             mamaInbox         inbox,
                                             mamaMsg           msg);

MAMAExpBridgeDLL
extern mama_status
zmqBridgeMamaPublisher_setUserCallbacks (publisherBridge         publisher,
                                         mamaQueue               queue,
                                         mamaPublisherCallbacks* cb,
                                         void*                   closure);

#if defined(__cplusplus)
}
#endif

#endif /*ZMQ_BRIDGE_FUNCTIONS__*/
