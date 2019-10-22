/* 
 *  Copyright (C) 2015-2019 Gooroom <gooroom@gooroom.kr>
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

#ifndef __SHOWDESKTOP_PLUGIN_H__
#define __SHOWDESKTOP_PLUGIN_H__

#include <glib.h>
#include <libxfce4panel/libxfce4panel.h>

G_BEGIN_DECLS
typedef struct _ShowDesktopPluginClass ShowDesktopPluginClass;
typedef struct _ShowDesktopPlugin      ShowDesktopPlugin;

#define TYPE_SHOWDESKTOP_PLUGIN            (showdesktop_plugin_get_type ())
#define SHOWDESKTOP_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_SHOWDESKTOP_PLUGIN, ShowDesktopPlugin))
#define SHOWDESKTOP_PLUGIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  TYPE_SHOWDESKTOP_PLUGIN, ShowDesktopPluginClass))
#define IS_SHOWDESKTOP_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_SHOWDESKTOP_PLUGIN))
#define IS_SHOWDESKTOP_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  TYPE_SHOWDESKTOP_PLUGIN))
#define SHOWDESKTOP_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  TYPE_SHOWDESKTOP_PLUGIN, ShowDesktopPluginClass))

GType showdesktop_plugin_get_type      (void) G_GNUC_CONST;

void  showdesktop_plugin_register_type (XfcePanelTypeModule *type_module);

G_END_DECLS

#endif /* !__SHOWDESKTOP_PLUGIN_H__ */
