#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <platform.h>
#include "../common.h"

static void realize(GtkWidget * widget)
{
	printf("realize!\n");
	gtk_gl_area_make_current(GTK_GL_AREA(widget));
	if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
	{
		return;
	}

	initiate(); // FIXME it may fail.
}

static void unrealize(GtkWidget * widget)
{
	printf("unrealize!\n");
	gtk_gl_area_make_current(GTK_GL_AREA(widget));
	if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
	{
		return;
	}
	terminate();
}

static gboolean render(GtkGLArea * area, GdkGLContext * context)
{
	if (gtk_gl_area_get_error(area) != NULL)
	{
		return FALSE;
	}

	if (! update())
	{
		return FALSE;
	}

	gtk_widget_queue_draw(GTK_WIDGET(area));

	return TRUE;
}

int main(int argc, char * * argv)
{
	gtk_init(& argc, & argv);

	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Shadow, Solid & Screen");
	gtk_window_set_default_size(GTK_WINDOW(window), 512, 512);

	GtkWidget * area = gtk_gl_area_new();
	gtk_gl_area_set_required_version(GTK_GL_AREA(area), 3, 3);
	gtk_gl_area_set_auto_render(GTK_GL_AREA(area), TRUE);
	gtk_gl_area_set_has_alpha(GTK_GL_AREA(area), FALSE);
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(area), TRUE);
	gtk_gl_area_set_has_stencil_buffer(GTK_GL_AREA(area), FALSE);
	gtk_gl_area_set_use_es(GTK_GL_AREA(area), FALSE);

	gtk_container_add(GTK_CONTAINER(window), area);

	g_signal_connect(area, "realize", G_CALLBACK(realize), NULL);
	g_signal_connect(area, "unrealize", G_CALLBACK(unrealize), NULL);
	g_signal_connect(area, "render", G_CALLBACK(render), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(GTK_WIDGET(window));

	gtk_main();

	return 0;
}
