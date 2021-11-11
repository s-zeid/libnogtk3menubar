/* libnogtk3menubar
 * An LD_PRELOAD hack to hide the menu bar in GTK+ 3 apps.
 * 
 * Copyright (C) 2015, 2017, 2021 S. Zeid.
 * https://code.s.zeid.me/libnogtk3menubar
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
 * 
 * Except as contained in this notice, the name(s) of the above copyright holders
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization.
 * 
 */

#define _GNU_SOURCE


#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <gtk/gtk.h>


GtkWidget* find_child_of_type(GtkWidget* widget, const char *type_name) {
 // Adapted from <https://stackoverflow.com/a/23497087> by Darius Kucinskas
 
 if (g_ascii_strcasecmp(G_OBJECT_TYPE_NAME(widget), (gchar*)type_name) == 0)
  return widget;
 
 if (GTK_IS_BIN(widget))
  return find_child_of_type(gtk_bin_get_child(GTK_BIN(widget)), type_name);
 
 if (GTK_IS_CONTAINER(widget)) {
  GList *children = gtk_container_get_children(GTK_CONTAINER(widget));
  do {
   GtkWidget* child = find_child_of_type(children->data, type_name);
   if (child != NULL)
    return child;
  } while ((children = g_list_next(children)) != NULL);
 }
 
 return NULL;
}


void (*next_gtk_widget_show)(GtkWidget *widget)
 = NULL;

void gtk_widget_show(GtkWidget *widget) {
 if (GTK_IS_MENU_BAR(widget))
  return;
 next_gtk_widget_show(widget);
 GtkWidget *menubar = find_child_of_type(gtk_widget_get_toplevel(widget), "GtkMenuBar");
 if (menubar != NULL)
  gtk_widget_hide(menubar);
}


void (*next_gtk_widget_show_all)(GtkWidget *widget)
 = NULL;

void gtk_widget_show_all(GtkWidget *widget) {
 if (GTK_IS_MENU_BAR(widget))
  return;
 next_gtk_widget_show_all(widget);
 GtkWidget *menubar = find_child_of_type(gtk_widget_get_toplevel(widget), "GtkMenuBar");
 if (menubar != NULL)
  gtk_widget_hide(menubar);
}


void _init(void) {
 next_gtk_widget_show = dlsym(RTLD_NEXT, "gtk_widget_show");
 next_gtk_widget_show_all = dlsym(RTLD_NEXT, "gtk_widget_show_all");
}
