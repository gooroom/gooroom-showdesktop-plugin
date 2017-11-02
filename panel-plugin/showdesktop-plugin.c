/*
 * Copyright (c) 2005-2007 Jasper Huijsmans <jasper@xfce.org>
 * Copyright (C) 2007-2010 Nick Schermer <nick@xfce.org>
 * Copyright (C) 2017 Gooroom Project Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_MATH_H
#include <math.h>
#endif

#include <stdio.h>

#include "showdesktop-plugin.h"

#include <gtk/gtk.h>
#include <glib-object.h>

#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>

#include <libwnck/libwnck.h>



struct _ShowDesktopPluginClass
{
  XfcePanelPluginClass __parent__;
};

/* plugin structure */
struct _ShowDesktopPlugin
{
	XfcePanelPlugin      __parent__;

	GtkWidget       *button;

	/* the wnck screen */
	WnckScreen *wnck_screen;
};

/* dinstallefine the plugin */
XFCE_PANEL_DEFINE_PLUGIN (ShowDesktopPlugin, showdesktop_plugin)

static gboolean
show_desktop_plugin_button_release_event (GtkToggleButton   *button,
                                          GdkEventButton    *event,
                                          ShowDesktopPlugin *plugin)
{
	WnckWorkspace *active_ws;
	GList         *windows, *li;
	WnckWindow    *window;

	g_return_val_if_fail (IS_SHOWDESKTOP_PLUGIN (plugin), FALSE);
	g_return_val_if_fail (WNCK_IS_SCREEN (plugin->wnck_screen), FALSE);

	if (event->button == 2) {
		active_ws = wnck_screen_get_active_workspace (plugin->wnck_screen);
		windows = wnck_screen_get_windows (plugin->wnck_screen);

		for (li = windows; li != NULL; li = li->next) {
			window = WNCK_WINDOW (li->data);

			if (wnck_window_get_workspace (window) != active_ws)
				continue;

			/* toggle the shade state */
			if (wnck_window_is_shaded (window))
				wnck_window_unshade (window);
			else
				wnck_window_shade (window);
		}
	}

	return FALSE;
}

static gboolean
showdesktop_plugin_size_changed (XfcePanelPlugin *panel_plugin, gint size)
{
	ShowDesktopPlugin *plugin = SHOWDESKTOP_PLUGIN (panel_plugin);

	if (xfce_panel_plugin_get_mode (panel_plugin) == XFCE_PANEL_PLUGIN_MODE_HORIZONTAL) {
		gtk_widget_set_size_request (GTK_WIDGET (panel_plugin), -1, size);
	} else {
		gtk_widget_set_size_request (GTK_WIDGET (panel_plugin), size, -1);
	}

	return TRUE;
}

static void
show_desktop_plugin_toggled (GtkToggleButton   *button,
                             ShowDesktopPlugin *plugin)
{
	gboolean active;

	g_return_if_fail (IS_SHOWDESKTOP_PLUGIN (plugin));
	g_return_if_fail (GTK_IS_TOGGLE_BUTTON (button));
	g_return_if_fail (WNCK_IS_SCREEN (plugin->wnck_screen));

	/* toggle the desktop */
	active = gtk_toggle_button_get_active (button);
	if (active != wnck_screen_get_showing_desktop (plugin->wnck_screen)) {
		wnck_screen_toggle_showing_desktop (plugin->wnck_screen, active);
	}
}

static void
show_desktop_plugin_showing_desktop_changed (WnckScreen        *wnck_screen,
                                             ShowDesktopPlugin *plugin)
{
	g_return_if_fail (IS_SHOWDESKTOP_PLUGIN (plugin));
	g_return_if_fail (WNCK_IS_SCREEN (wnck_screen));
	g_return_if_fail (plugin->wnck_screen == wnck_screen);

	/* update button to user action */
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (plugin->button),
			wnck_screen_get_showing_desktop (wnck_screen));
}

static void
show_desktop_plugin_screen_changed (GtkWidget *widget,
                                    GdkScreen *previous_screen)
{
	ShowDesktopPlugin *plugin = SHOWDESKTOP_PLUGIN (widget);
	WnckScreen        *wnck_screen;
	GdkScreen         *screen;

	g_return_if_fail (IS_SHOWDESKTOP_PLUGIN (widget));

	/* get the new wnck screen */
	screen = gtk_widget_get_screen (widget);
	wnck_screen = wnck_screen_get (gdk_x11_screen_get_screen_number (screen));
	g_return_if_fail (WNCK_IS_SCREEN (wnck_screen));

	/* leave when the wnck screen did not change */
	if (plugin->wnck_screen == wnck_screen)
		return;

	/* disconnect signals from an existing wnck screen */
	if (plugin->wnck_screen != NULL)
		g_signal_handlers_disconnect_by_func (G_OBJECT (plugin->wnck_screen),
				show_desktop_plugin_showing_desktop_changed, plugin);

	/* set the new wnck screen */
	plugin->wnck_screen = wnck_screen;
	g_signal_connect (G_OBJECT (wnck_screen), "showing-desktop-changed",
			G_CALLBACK (show_desktop_plugin_showing_desktop_changed), plugin);

	/* toggle the button to the current state or update the tooltip */
	if (G_UNLIKELY (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (plugin->button)) !=
				wnck_screen_get_showing_desktop (wnck_screen)))
		show_desktop_plugin_showing_desktop_changed (wnck_screen, plugin);
	else
		show_desktop_plugin_toggled (GTK_TOGGLE_BUTTON (plugin->button), plugin);
}


static void
showdesktop_plugin_free_data (XfcePanelPlugin *panel_plugin)
{
	ShowDesktopPlugin *plugin = SHOWDESKTOP_PLUGIN (panel_plugin);

	/* disconnect screen changed signal */
	g_signal_handlers_disconnect_by_func (G_OBJECT (plugin),
			show_desktop_plugin_screen_changed, NULL);

	/* disconnect handle */
	if (plugin->wnck_screen != NULL)
		g_signal_handlers_disconnect_by_func (G_OBJECT (plugin->wnck_screen),
				show_desktop_plugin_showing_desktop_changed, plugin);
}

static void
showdesktop_plugin_init (ShowDesktopPlugin *plugin)
{
	plugin->button      = NULL;
	plugin->wnck_screen = NULL;

/* monitor screen changes */
	g_signal_connect (G_OBJECT (plugin), "screen-changed",
			G_CALLBACK (show_desktop_plugin_screen_changed), NULL);
}

static void
showdesktop_plugin_construct (XfcePanelPlugin *panel_plugin)
{
	ShowDesktopPlugin *plugin = SHOWDESKTOP_PLUGIN (panel_plugin);

	xfce_textdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

	plugin->button = xfce_panel_create_toggle_button ();
	gtk_button_set_relief (GTK_BUTTON (plugin->button), GTK_RELIEF_NONE);
	gtk_container_add (GTK_CONTAINER (plugin), plugin->button);
	gtk_widget_set_name (plugin->button, "show-desktop-button");

	xfce_panel_plugin_add_action_widget (XFCE_PANEL_PLUGIN (plugin), plugin->button);
	gtk_widget_show (plugin->button);

	GtkWidget *image = gtk_image_new ();
	gtk_container_add (GTK_CONTAINER (plugin->button), image);
	gtk_image_set_from_icon_name (GTK_IMAGE (image), "showdesktop-plugin-symbolic", GTK_ICON_SIZE_BUTTON);
	gtk_image_set_pixel_size (GTK_IMAGE (image), 22);

	gtk_widget_show (image);

	g_signal_connect (G_OBJECT (plugin->button), "toggled",
		G_CALLBACK (show_desktop_plugin_toggled), plugin);

	g_signal_connect (G_OBJECT (plugin->button), "button-release-event",
		G_CALLBACK (show_desktop_plugin_button_release_event), plugin);
}

static void
showdesktop_plugin_mode_changed (XfcePanelPlugin *plugin, XfcePanelPluginMode mode)
{
	showdesktop_plugin_size_changed (plugin, xfce_panel_plugin_get_size (plugin));
}

static void
showdesktop_plugin_class_init (ShowDesktopPluginClass *klass)
{
	XfcePanelPluginClass *plugin_class;

	plugin_class = XFCE_PANEL_PLUGIN_CLASS (klass);
	plugin_class->construct = showdesktop_plugin_construct;
	plugin_class->free_data = showdesktop_plugin_free_data;
	plugin_class->size_changed = showdesktop_plugin_size_changed;
	plugin_class->mode_changed = showdesktop_plugin_mode_changed;
}
