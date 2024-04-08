#include <gtk/gtk.h>


GtkWidget *entry;
GtkWidget *result_entry; 


static void activate(GtkApplication *app, gpointer user_data);
static void button_clicked(GtkWidget *widget, gpointer data);
static void clear_clicked(GtkWidget *widget, gpointer data);
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
static void number_button_clicked(GtkWidget *widget, gpointer data);
static void use_button_clicked(GtkWidget *widget, gpointer data);
static void delete_button_clicked(GtkWidget *widget, gpointer data);


char *calculate(const char *expression) {
    float num1, num2;
    char operator;
    int result;
    char *error_message = NULL;
    
    result = sscanf(expression, "%f %c %f", &num1, &operator, &num2);
    if (result != 3) {
        error_message = strdup("Invalid expression format.");
        return error_message;
    }

    float result_value;

    switch(operator) {
        case '+':
            result_value = num1 + num2;
            break;
        case '-':
            result_value = num1 - num2;
            break;
        case '*':
            result_value = num1 * num2;
            break;
        case '/':
            if (num2 == 0) {
                error_message = strdup("Error! Division by zero is not allowed.");
                return error_message;
            }
            result_value = num1 / num2;
            break;
        case '^':
            result_value = 1;
            for (int i = 0; i < num2; i++) {
                result_value *= num1;
            }
            break;
        default:
            error_message = strdup("Error! Invalid operator.");
            return error_message;
    }

    
    if (result_value == (int)result_value) {
        return strdup(g_strdup_printf("%.0f", result_value));
    } else {
        return strdup(g_strdup_printf("%.2f", result_value));
    }
}


static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *result_grid;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CCalculator Basic");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);
    g_signal_connect(entry, "key_press_event", G_CALLBACK(on_key_press), NULL);
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_widget_set_vexpand(entry, TRUE);

    button = gtk_button_new_with_label("del");
    g_signal_connect(button, "clicked", G_CALLBACK(delete_button_clicked), entry); 
    gtk_grid_attach(GTK_GRID(grid), button, 4, 0, 1, 1); 
    gtk_widget_set_size_request(button, 50, 34);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    result_grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), result_grid, 0, 1, 4, 1);

    result_entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(result_entry), 1);
    g_object_set(result_entry, "editable", FALSE, NULL); 
    gtk_grid_attach(GTK_GRID(result_grid), result_entry, 0, 0, 4, 1); 
    gtk_widget_set_hexpand(result_entry, TRUE);
    gtk_widget_set_vexpand(result_entry, TRUE);

    
    button = gtk_button_new_with_label("Use");
    g_signal_connect(button, "clicked", G_CALLBACK(use_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 1, 1, 1);
    gtk_widget_set_size_request(button, 50, 34);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    
    GtkWidget *number_grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(number_grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(number_grid), TRUE);
    gtk_grid_attach(GTK_GRID(grid), number_grid, 0, 2, 3, 4);

    for (int i = 1; i <= 9; i++) {
        gchar *button_label = g_strdup_printf("%d", i);
        button = gtk_button_new_with_label(button_label);
        g_signal_connect(button, "clicked", G_CALLBACK(number_button_clicked), entry);
        gtk_grid_attach(GTK_GRID(number_grid), button, (i - 1) % 3, (i - 1) / 3, 1, 1);
        gtk_widget_set_size_request(button, 50, 50);
        gtk_widget_set_hexpand(button, TRUE);
        gtk_widget_set_vexpand(button, TRUE);
        g_free(button_label);
    }

    button = gtk_button_new_with_label("0");
    g_signal_connect(button, "clicked", G_CALLBACK(number_button_clicked), entry);
    gtk_grid_attach(GTK_GRID(number_grid), button, 1, 3, 1, 1);
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    button = gtk_button_new_with_label(".");
    g_signal_connect(button, "clicked", G_CALLBACK(number_button_clicked), entry);
    gtk_grid_attach(GTK_GRID(number_grid), button, 2, 3, 1, 1); 
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);
    
    button = gtk_button_new_with_label("=");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry); 
    gtk_grid_attach(GTK_GRID(grid), button, 4, 4, 1, 2);
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    
    button = gtk_button_new_with_label("+");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 2, 1, 1);
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    button = gtk_button_new_with_label("-");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 2, 1, 1);
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    button = gtk_button_new_with_label("*");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 3, 1, 1);
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    button = gtk_button_new_with_label("/");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 3, 1, 1);
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    button = gtk_button_new_with_label("^");
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 4, 1, 1);
    gtk_widget_set_size_request(button, 50, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);

    
    button = gtk_button_new_with_label("C");
    g_signal_connect(button, "clicked", G_CALLBACK(clear_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 5, 1, 1);
    gtk_widget_set_size_request(button, 60, 50);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);
    
    gtk_widget_show_all(window);
}



static void use_button_clicked(GtkWidget *widget, gpointer data) {
    
    const gchar *result_text = gtk_entry_get_text(GTK_ENTRY(result_entry));

    
    gtk_entry_set_text(GTK_ENTRY(entry), result_text);

    
    gtk_entry_set_text(GTK_ENTRY(result_entry), "");
}

static void delete_button_clicked(GtkWidget *widget, gpointer data) {
    
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(data));
    
    
    gint len = strlen(text);
    
    
    if (len > 0) {
        gchar *new_text = g_strndup(text, len - 1);
        gtk_entry_set_text(GTK_ENTRY(data), new_text);
        g_free(new_text);
    }
}


static void button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *label_text = gtk_button_get_label(GTK_BUTTON(widget));
    gchar *text = g_strdup(gtk_entry_get_text(GTK_ENTRY(data)));
    if (g_strcmp0(label_text, "=") == 0) {
        gchar *result_text = calculate(text);
        gtk_entry_set_text(GTK_ENTRY(result_entry), result_text ? result_text : "Invalid expression");
        if (result_text) g_free(result_text);
    } else {
        gchar *new_text = g_strconcat(text, label_text, NULL);
        gtk_entry_set_text(GTK_ENTRY(data), new_text);
        g_free(new_text);
    }
    g_free(text);
}



static void clear_clicked(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(data), "");
    gtk_entry_set_text(GTK_ENTRY(result_entry), "");  
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return) {
        
        button_clicked(gtk_button_new_with_label("="), entry);
        return TRUE;  
    } else if (event->keyval == GDK_KEY_c || event->keyval == GDK_KEY_C) {
			
		gtk_entry_set_text(GTK_ENTRY(entry), "");
		gtk_entry_set_text(GTK_ENTRY(result_entry), "");
		return TRUE;
		
	} else if (event->keyval == GDK_KEY_u || event->keyval == GDK_KEY_U) {
        
        const gchar *result_text = gtk_entry_get_text(GTK_ENTRY(result_entry));

            
        gtk_entry_set_text(GTK_ENTRY(entry), result_text);

        gtk_editable_set_position(GTK_EDITABLE(entry), -1);

        
        gtk_entry_set_text(GTK_ENTRY(result_entry), "");
        return TRUE;
    }
	
    return FALSE;  //Continue
}

static void number_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    gchar *text = g_strdup(gtk_entry_get_text(GTK_ENTRY(data)));
    gchar *new_text = g_strconcat(text, button_label, NULL);
    gtk_entry_set_text(GTK_ENTRY(data), new_text);
    g_free(text);
    g_free(new_text);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("deus.gtk.calculator", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
