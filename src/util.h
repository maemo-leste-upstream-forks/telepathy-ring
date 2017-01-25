/*
 * util.h - Headers for Ring utility functions
 * Copyright (C) 2006 Collabora Ltd.
 * Copyright (C) 2006 Nokia Corporation
 *   @author Robert McQueen <robert.mcqueen@collabora.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __GABBLE_UTIL_H__
#define __GABBLE_UTIL_H__

#include <config.h>

#include <glib.h>
#include <telepathy-glib/telepathy-glib.h>

#include "ring-connection.h"

/* Guarantees that the resulting hash is in lower-case */
gchar *sha1_hex (const gchar *bytes, guint len);

/* A SHA1 digest is 20 bytes long */
#define SHA1_HASH_SIZE 20
void sha1_bin (const gchar *bytes, guint len, guchar out[SHA1_HASH_SIZE]);

gchar *ring_generate_id (void);

G_GNUC_WARN_UNUSED_RESULT
gchar *ring_encode_jid (const gchar *node, const gchar *domain,
    const gchar *resource);

gchar *ring_remove_resource (const gchar *jid);
gchar *ring_normalize_room (TpHandleRepoIface *repo, const gchar *jid,
    gpointer context, GError **error);
TpHandle ring_get_room_handle_from_jid (TpHandleRepoIface *room_repo,
    const gchar *jid);

void ring_signal_connect_weak (gpointer instance, const gchar *detailed_signal,
    GCallback c_handler, GObject *user_data);
guint ring_idle_add_weak (GSourceFunc function, GObject *object);

GPtrArray *ring_g_ptr_array_copy (GPtrArray *source);

gchar * ring_peer_to_jid (RingConnection *conn,
    TpHandle peer,
    const gchar *resource);

gboolean
ring_flag_from_nick (GType flag_type, const gchar *nick,
    guint *value);

void ring_simple_async_succeed_or_fail_in_idle (gpointer self,
    GAsyncReadyCallback callback,
    gpointer user_data,
    gpointer source_tag,
    const GError *error);

GSimpleAsyncResult *ring_simple_async_countdown_new (gpointer self,
    GAsyncReadyCallback callback,
    gpointer user_data,
    gpointer source_tag,
    gssize todo);
void ring_simple_async_countdown_inc (GSimpleAsyncResult *simple);
void ring_simple_async_countdown_dec (GSimpleAsyncResult *simple);

/* Boilerplate for telling servers which implement XEP-0079 not to store these
 * messages for delivery later. Include it in your call to wocky_stanza_build()
 * like so:
 *
 *    wocky_stanza_build (WOCKY_STANZA_TYPE_MESSAGE, WOCKY_STANZA_SUB_TYPE_NONE,
 *       NULL, jid,
 *       '(', "close",
 *         ':', NS_TUBES,
 *         '@', "tube", id_str,
 *       ')',
 *       RING_AMP_DO_NOT_STORE_SPEC,
 *       NULL);
 *
 * Every 1000th user will win a Marshall amplifier!
 */
#define RING_AMP_DO_NOT_STORE_SPEC \
          '(', "amp", \
            ':', NS_AMP, \
            '(', "rule", \
              '@', "condition", "deliver-at", \
              '@', "value", "stored", \
              '@', "action", "error", \
            ')', \
            '(', "rule", \
              '@', "condition", "match-resource", \
              '@', "value", "exact", \
              '@', "action", "error", \
            ')', \
          ')'


#endif /* __GABBLE_UTIL_H__ */
