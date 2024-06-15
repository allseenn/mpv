#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *user_entry;
GtkWidget *host_entry;
GtkWidget *port_entry;
GtkWidget *pass_entry;
GtkWidget *path_entry;
GtkWidget *file_entry;
GtkWidget *mon_entry;

void apply_css(GtkWidget *widget, GtkStyleProvider *provider) {
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget), provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    if (GTK_IS_CONTAINER(widget)) {
        gtk_container_forall(GTK_CONTAINER(widget), (GtkCallback)apply_css, provider);
    }
}

void execute_command(const char *command) {
    const char *user = gtk_entry_get_text(GTK_ENTRY(user_entry));
    const char *host = gtk_entry_get_text(GTK_ENTRY(host_entry));
    const char *port = gtk_entry_get_text(GTK_ENTRY(port_entry));
    const char *pass = gtk_entry_get_text(GTK_ENTRY(pass_entry));

    char ssh_command[1024];
    snprintf(ssh_command, sizeof(ssh_command), 
            "sshpass -p '%s' ssh -p %s %s@%s 'cat << EOF | socat - /tmp/mpvsocket\n"
             "{command: %s }\n"
             "EOF'",
            pass, port, user, host, command);
    
    system(ssh_command);
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *command = (const char *)data;
    execute_command(command);
}

void open_file_command() {
    const char *user = gtk_entry_get_text(GTK_ENTRY(user_entry));
    const char *host = gtk_entry_get_text(GTK_ENTRY(host_entry));
    const char *port = gtk_entry_get_text(GTK_ENTRY(port_entry));
    const char *pass = gtk_entry_get_text(GTK_ENTRY(pass_entry));

    const char *path = gtk_entry_get_text(GTK_ENTRY(path_entry));
    const char *file = gtk_entry_get_text(GTK_ENTRY(file_entry));
    const char *mon = gtk_entry_get_text(GTK_ENTRY(mon_entry));

    char open_file[256];
    snprintf(open_file, sizeof(open_file), 
    "sshpass -p '%s' ssh -p %s %s@%s \"screen -dmS mpv_session bash -c 'DISPLAY=:0 mpv --fullscreen --screen=%s --input-ipc-server=/tmp/mpvsocket %s/%s'\"",
    pass, port, user, host, mon, path, file);

    system(open_file);
}

void close_file_command() {
    const char *user = gtk_entry_get_text(GTK_ENTRY(user_entry));
    const char *host = gtk_entry_get_text(GTK_ENTRY(host_entry));
    const char *port = gtk_entry_get_text(GTK_ENTRY(port_entry));
    const char *pass = gtk_entry_get_text(GTK_ENTRY(pass_entry));

    char close_command[256];
    snprintf(close_command, sizeof(close_command), 
        "sshpass -p '%s' ssh -p %s %s@%s \"screen -S mpv_session -X quit && rm /tmp/mpvsocket\"",
        pass, port, user, host);

    system(close_command);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    printf("Compiled with GTK version: %d.%d.%d\n", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
    printf("Running with GTK version: %d.%d.%d\n", gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());

    // Create and configure the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MPV SSH Control");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Create a grid and add it to the main window
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create entry widgets
    user_entry = gtk_entry_new();
    host_entry = gtk_entry_new();
    port_entry = gtk_entry_new();
    pass_entry = gtk_entry_new();
    path_entry = gtk_entry_new();
    file_entry = gtk_entry_new();
    mon_entry = gtk_entry_new();

    // Set default text for entry widgets
    gtk_entry_set_text(GTK_ENTRY(user_entry), getenv("USER"));
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE); // Скрыть вводимые символы
    gtk_entry_set_text(GTK_ENTRY(host_entry), "localhost");
    gtk_entry_set_text(GTK_ENTRY(port_entry), "22");
    gtk_entry_set_text(GTK_ENTRY(path_entry), "~/");
    gtk_entry_set_text(GTK_ENTRY(file_entry), "");
    gtk_entry_set_text(GTK_ENTRY(mon_entry), "0");

    // Add labels and entry widgets to the grid
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("username"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), user_entry, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("password"), 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), pass_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("hostname"), 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), host_entry, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("port"), 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), port_entry, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("pathname"), 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), path_entry, 4, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("filename"), 5, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), file_entry, 5, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("mon"), 6, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mon_entry, 6, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("open/close"), 7, 0, 1, 1);

    // Create and add the open button to the grid 
    GtkWidget *open_button = gtk_button_new_with_label("⏏️");
    g_signal_connect(open_button, "clicked", G_CALLBACK(open_file_command), NULL);
    gtk_grid_attach(GTK_GRID(grid), open_button, 7, 1, 1, 1);

    // Create and add the close button to the grid "⏹️"
    GtkWidget *close_button = gtk_button_new_with_label("⏹️");
    g_signal_connect(close_button, "clicked", G_CALLBACK(close_file_command), NULL);
    gtk_grid_attach(GTK_GRID(grid), close_button, 7, 2, 1, 1);

    // Create an array of labels and commands for other buttons
    const char *labels[7] = {"⏪", "▶️/⏸️", "⏩", "⏬", "⏫", "🔉", "🔊"};
    const char *commands[7] = {
        "[\"seek\", -5]",
        "[\"cycle\", \"pause\"]",
        "[\"seek\", 5]",
        "[\"add\", \"speed\", -0.2]",
        "[\"add\", \"speed\", 0.2]",
        "[\"add\", \"volume\", -5]",
        "[\"add\", \"volume\", 5]",
    };

    // Create and add other buttons to the grid
    for (int i = 0; i < 7; i++) {
        GtkWidget *button = gtk_button_new_with_label(labels[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), (gpointer)commands[i]);
        gtk_grid_attach(GTK_GRID(grid), button, i, 2, 1, 1);
    }

    // Connect the destroy signal for the main window
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Apply CSS styling
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    apply_css(window, GTK_STYLE_PROVIDER(provider));

    // Show all widgets and start the main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
