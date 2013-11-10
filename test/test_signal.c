/*
    MPRIS plugin for Deadbeef player
    Copyright (C) 2011 HuangCongyu <huangcongyu2006@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <stdlib.h>
#include <stdio.h>
#include <gio/gio.h>
#include <glib.h>
#include <glib/gprintf.h>

static GMainLoop *loop = NULL;

static void on_properties_changed (GDBusProxy          *proxy,
                                GVariant            *changed_properties,
                                const gchar* const  *invalidated_properties,
                                gpointer             user_data)
{
    /* 
     * Note that we are guaranteed that changed_properties and
     * invalidated_properties are never NULL
     */

    if (g_variant_n_children (changed_properties) > 0){
        GVariantIter *iter;
        const gchar *key;
        GVariant *value;

        g_printf ("*** Properties Changed:\n");
        g_variant_get(changed_properties, "a{sv}", &iter);
        while (g_variant_iter_loop(iter, "{&sv}", &key, &value)){
            gchar *value_str;
            value_str = g_variant_print (value, TRUE);
            g_printf("      %s -> %s\n", key, value_str);
            g_free(value_str);
        }
        g_variant_iter_free(iter);
    }

    if (g_strv_length((GStrv) invalidated_properties) > 0){
        guint n;
        g_printf ("*** Properties Invalidated:\n");
        for (n = 0; invalidated_properties[n] != NULL; n++){
            const gchar *key = invalidated_properties[n];
            g_printf("      %s\n", key);
        }
    }
}

static void on_signal (GDBusProxy *proxy,
                    gchar      *sender_name,
                    gchar      *signal_name,
                    GVariant   *parameters,
                    gpointer    user_data)
{
    gchar *parameters_str;

    parameters_str = g_variant_print (parameters, TRUE);
    g_printf ("*** Received Signal: %s: %s\n",
                            signal_name, parameters_str);
    g_free (parameters_str);
}

int main(int argc, char **argv)
{
    if(argc < 4){
        g_printf("Usage: test_signal name object interface \n");
        return 0;
    }
    GError *error;
    GDBusProxy *proxy;
#if (GLIB_MAJOR_VERSION <= 2 && GLIB_MINOR_VERSION < 36)
    g_type_init ();
#endif
    loop = NULL;
    proxy = NULL;
    loop = g_main_loop_new (NULL, FALSE);

    error = NULL;
    proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
                                         G_DBUS_PROXY_FLAGS_NONE,
                                         NULL, /* GDBusInterfaceInfo */
                                         argv[1],
                                         argv[2],
                                         argv[3],
                                         NULL, /* GCancellable */
                                         &error);
    if(proxy == NULL){
        g_printf("Error creating proxy: %s\n", error->message);
        g_error_free (error);
        goto out;
    }

    g_signal_connect (proxy,
                    "g-properties-changed",
                    G_CALLBACK(on_properties_changed),
                    NULL);
    g_signal_connect (proxy,
                    "g-signal",
                    G_CALLBACK(on_signal),
                    NULL);

    g_main_loop_run (loop);
 out:
    if (proxy != NULL)
        g_object_unref (proxy);
    if (loop != NULL)
        g_main_loop_unref (loop);
    return 0; 
}
