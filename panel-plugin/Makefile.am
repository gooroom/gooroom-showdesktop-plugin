AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE \
	$(PLATFORM_CPPFLAGS)

plugindir = $(libdir)/xfce4/panel/plugins

plugin_LTLIBRARIES = \
	libshowdesktop-plugin.la

libshowdesktop_plugin_la_SOURCES = \
	showdesktop-plugin.h \
	showdesktop-plugin.c \
	$(NULL)

libshowdesktop_plugin_la_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GTK_CFLAGS) \
	$(LIBWNCK_CFLAGS) \
	$(LIBXFCE4UTIL_CFLAGS) \
	$(LIBXFCE4PANEL_CFLAGS) \
	$(PLATFORM_CFLAGS)

libshowdesktop_plugin_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined \
	-export-symbols-regex '^xfce_panel_module_(preinit|init|construct)' \
	$(PLATFORM_LDFLAGS)

libshowdesktop_plugin_la_LIBADD = \
	$(GLIB_LIBS) \
	$(GTK_LIBS) \
	$(LIBWNCK_LIBS)	\
	$(LIBXFCE4UTIL_LIBS) \
	$(LIBXFCE4PANEL_LIBS)

#
# Desktop file
#
desktopdir = \
	$(datadir)/xfce4/panel/plugins

desktop_in_files = \
	showdesktop-plugin.desktop.in.in

desktop_DATA = $(desktop_in_files:.desktop.in.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST = $(desktop_in_files)

DISTCLEANFILES = $(desktop_DATA)
