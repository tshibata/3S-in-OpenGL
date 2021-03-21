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

static void get_pointer_position(GtkGLArea * area, gint * x, gint * y)
{
	GdkDisplay * display = gtk_widget_get_display((GtkWidget *) area);
	GdkSeat * seat = gdk_display_get_default_seat(display);
	GdkDevice * device = gdk_seat_get_pointer(seat);
	gint x0, y0, x1, y1;
	gdk_window_get_origin(gtk_widget_get_window(GTK_WIDGET(area)), & x0, & y0);
	gdk_device_get_position(device, NULL, & x1, & y1);
	* x = x1 - x0;
	* y = y1 - y0;
}

static gboolean render(GtkGLArea * area, GdkGLContext * context)
{
	if (gtk_gl_area_get_error(area) != NULL)
	{
		return FALSE;
	}

	gint x, y;
	get_pointer_position(area, & x, & y);

	if (! update(x, y))
	{
		return FALSE;
	}

	gtk_widget_queue_draw(GTK_WIDGET(area));

	return TRUE;
}

static void press(GtkGLArea * area, GdkEventButton * event, void * data)
{
	gint x, y;
	get_pointer_position(area, & x, & y);
	switch (event->button)
	{
	case 1:
		buttonPressed(- 1, 0, 0);
		break;
	case 2:
		buttonPressed(0, 0, 0);
		break;
	case 3:
		buttonPressed(1, 0, 0);
		break;
	}
}

static void release(GtkGLArea * area, GdkEventButton * event, void * data)
{
	gint x, y;
	get_pointer_position(area, & x, & y);
	switch (event->button)
	{
	case 1:
		buttonReleased(- 1, 0, 0);
		break;
	case 2:
		buttonReleased(0, 0, 0);
		break;
	case 3:
		buttonPressed(1, 0, 0);
		break;
	}
}

static void scroll(GtkGLArea * area, GdkEventScroll * event, void * data)
{
	if (event->delta_y != 0)
	{
		gint x, y;
		get_pointer_position(area, & x, & y);
		wheelMoved(- event->delta_y, 0, 0);
	}
}

int main(int argc, char * * argv)
{
	gtk_init(& argc, & argv);

	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Shadow, Solid & Screen");
	gtk_window_set_default_size(GTK_WINDOW(window), screenWidth, screenHeight);

	GtkWidget * area = gtk_gl_area_new();
	gtk_gl_area_set_required_version(GTK_GL_AREA(area), 3, 3);
	gtk_gl_area_set_auto_render(GTK_GL_AREA(area), TRUE);
	gtk_gl_area_set_has_alpha(GTK_GL_AREA(area), FALSE);
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(area), TRUE);
	gtk_gl_area_set_has_stencil_buffer(GTK_GL_AREA(area), TRUE);
	gtk_gl_area_set_use_es(GTK_GL_AREA(area), FALSE);

	gtk_container_add(GTK_CONTAINER(window), area);

	g_signal_connect(area, "realize", G_CALLBACK(realize), NULL);
	g_signal_connect(area, "unrealize", G_CALLBACK(unrealize), NULL);
	g_signal_connect(area, "render", G_CALLBACK(render), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_set_events(area, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK/* | GDK_SCROLL_MASK*/);
	g_signal_connect(area, "button_press_event", G_CALLBACK(press), NULL);
	g_signal_connect(area, "button_release_event", G_CALLBACK(release), NULL);
	// GtkArea does not give scroll events...
	gtk_widget_set_events(window, GDK_SCROLL_MASK);
	g_signal_connect(window, "scroll_event", G_CALLBACK(scroll), NULL);

	gtk_widget_show_all(GTK_WIDGET(window));

	gtk_main();

	return 0;
}
