#define _POSIX_C_SOURCE 200809L // Allow use of POSIX functions like realpath and strdup

#include <X11/Xlib.h> // Include the Xlib header for X11 functions
#include <X11/Xutil.h> // Include the Xutil header for X11 utility functions
#include <errno.h> // Include errno for error handling
#include <limits.h> // Include limits.h for PATH_MAX
#include <signal.h> // Include signal.h for signal handling
#include <stdio.h> // Include stdio.h for input/output functions
#include <stdlib.h> // Include stdlib.h for general utilities like exit
#include <string.h> // Include string.h for string manipulation functions
#include <sys/types.h> // Include sys/types.h for data types like pid_t
#include <unistd.h> // Include unistd.h for POSIX API functions like fork and access

#ifndef PATH_MAX // If PATH_MAX is not defined, define it as 4096
#define PATH_MAX 4096
#endif

typedef enum { // Define an enumeration for different terminal kinds
    TERMINAL_XTERM,
    TERMINAL_GNOME,
    TERMINAL_XFCE,
    TERMINAL_KONSOLE
} TerminalKind;

typedef struct { // Define a structure to represent a game
    const char *title;
    const char *description;
    const char *binary;
} Game;

// Define an array of games with their titles, descriptions, and binary names
static const Game games[] = {
    {"Batalha Naval", "Posicione navios e explore habilidades especiais.", "batalha_naval"},
    {"Super Trunfo", "Compare cidades e atributos em uma disputa de cartas.", "super_trunfo"},
    {"Xadrez", "Simule os movimentos das pecas no tabuleiro.", "xadrez"}
};

// Define constants for the number of games, window dimensions, card dimensions, and gaps
enum {
    GAME_COUNT = sizeof(games) / sizeof(games[0]),
    WINDOW_WIDTH = 820,
    WINDOW_HEIGHT = 560,
    CARD_X = 44,
    CARD_Y = 176,
    CARD_WIDTH = 732,
    CARD_HEIGHT = 82,
    CARD_GAP = 16
};

// Define a structure to represent the application state, including display, window, graphics context, font, colors, dimensions, hovered game index, and status message
typedef struct {
    Display *display;
    Window window;
    GC gc;
    XFontStruct *font;
    unsigned long background;
    unsigned long panel;
    unsigned long hover;
    unsigned long foreground;
    unsigned long muted;
    unsigned long accent;
    unsigned long border;
    int width;
    int height;
    int hovered_game;
    char status[160];
} App;

// Function to allocate a color pixel based on the color name
static unsigned long color_pixel(Display *display, const char *name)
{
    XColor screen_color;
    XColor exact_color;
    Colormap colormap = DefaultColormap(display, DefaultScreen(display));

    if (XAllocNamedColor(display, colormap, name, &screen_color, &exact_color)) {
        return screen_color.pixel;
    }
    return BlackPixel(display, DefaultScreen(display));
}

// Function to draw text on the window at a specified position with a specified color
static void draw_text(App *app, int x, int baseline, const char *text,
                      unsigned long color)
{
    XSetForeground(app->display, app->gc, color);
    XDrawString(app->display, app->window, app->gc, x, baseline, text,
                (int)strlen(text));
}

// Function to draw the entire interface, including background, title, game cards, and status message
static void draw_interface(App *app)
{
    XSetForeground(app->display, app->gc, app->background);
    XFillRectangle(app->display, app->window, app->gc, 0, 0,
                   (unsigned int)app->width, (unsigned int)app->height);

    draw_text(app, 46, 58, "C GAMES", app->accent);
    draw_text(app, 46, 91, "Escolha um jogo", app->foreground);
    draw_text(app, 46, 118, "Tres projetos academicos em C, reunidos em um so lugar.",
              app->muted);

    for (int index = 0; index < GAME_COUNT; index++) {
        int y = CARD_Y + index * (CARD_HEIGHT + CARD_GAP);
        unsigned long fill = index == app->hovered_game ? app->hover : app->panel;

        XSetForeground(app->display, app->gc, fill);
        XFillRectangle(app->display, app->window, app->gc, CARD_X, y,
                       CARD_WIDTH, CARD_HEIGHT);
        XSetForeground(app->display, app->gc, app->border);
        XDrawRectangle(app->display, app->window, app->gc, CARD_X, y,
                       CARD_WIDTH, CARD_HEIGHT);

        draw_text(app, CARD_X + 22, y + 34, games[index].title, app->foreground);
        draw_text(app, CARD_X + 22, y + 59, games[index].description, app->muted);
        draw_text(app, CARD_X + CARD_WIDTH - 34, y + 48, ">", app->accent);
    }

    XSetForeground(app->display, app->gc, app->border);
    XDrawLine(app->display, app->window, app->gc, 46, app->height - 65,
              app->width - 46, app->height - 65);
    draw_text(app, 46, app->height - 38,
              app->status[0] == '\0' ? "Clique em um jogo para iniciar" : app->status,
              app->muted);
    draw_text(app, app->width - 118, app->height - 38, "ESC  sair", app->muted);
    XFlush(app->display);
}

static int game_at_position(int x, int y)
{
    if (x < CARD_X || x >= CARD_X + CARD_WIDTH) {
        return -1;
    }

    for (int index = 0; index < GAME_COUNT; index++) {
        int card_y = CARD_Y + index * (CARD_HEIGHT + CARD_GAP);
        if (y >= card_y && y < card_y + CARD_HEIGHT) {
            return index;
        }
    }
    return -1;
}

static int find_executable(const char *name, char *path, size_t path_size)
{
    const char *search_path = getenv("PATH");
    char *path_copy;
    char *entry;
    char *save_pointer = NULL;

    if (search_path == NULL || path_size == 0) {
        return 0;
    }

    path_copy = strdup(search_path);
    if (path_copy == NULL) {
        return 0;
    }

    for (entry = strtok_r(path_copy, ":", &save_pointer); entry != NULL;
         entry = strtok_r(NULL, ":", &save_pointer)) {
        int length = snprintf(path, path_size, "%s/%s",
                              entry[0] == '\0' ? "." : entry, name);
        if (length > 0 && (size_t)length < path_size && access(path, X_OK) == 0) {
            free(path_copy);
            return 1;
        }
    }

    free(path_copy);
    return 0;
}

static int find_terminal(char *path, size_t path_size, TerminalKind *kind)
{
    static const struct {
        const char *name;
        TerminalKind kind;
    } choices[] = {
        {"x-terminal-emulator", TERMINAL_XTERM},
        {"xterm", TERMINAL_XTERM},
        {"gnome-terminal", TERMINAL_GNOME},
        {"xfce4-terminal", TERMINAL_XFCE},
        {"konsole", TERMINAL_KONSOLE}
    };

    for (size_t index = 0; index < sizeof(choices) / sizeof(choices[0]); index++) {
        if (find_executable(choices[index].name, path, path_size)) {
            *kind = choices[index].kind;
            return 1;
        }
    }
    return 0;
}

static void launch_game(App *app, int game_index, const char *project_root)
{
    char game_path[PATH_MAX];
    char terminal_path[PATH_MAX];
    TerminalKind terminal_kind;
    int length = snprintf(game_path, sizeof(game_path), "%s/build/%s",
                          project_root, games[game_index].binary);

    if (length < 0 || (size_t)length >= sizeof(game_path) ||
        access(game_path, X_OK) != 0) {
        snprintf(app->status, sizeof(app->status),
                 "Execute make para compilar os jogos antes de inicia-los.");
        draw_interface(app);
        return;
    }

    if (!find_terminal(terminal_path, sizeof(terminal_path), &terminal_kind)) {
        snprintf(app->status, sizeof(app->status),
                 "Instale xterm ou um terminal grafico compativel.");
        draw_interface(app);
        return;
    }

    pid_t child = fork();
    if (child == 0) {
        switch (terminal_kind) {
        case TERMINAL_GNOME:
            execl(terminal_path, terminal_path, "--wait", "--", game_path,
                  (char *)NULL);
            break;
        case TERMINAL_XFCE:
            execl(terminal_path, terminal_path, "--hold", "-e", game_path,
                  (char *)NULL);
            break;
        case TERMINAL_KONSOLE:
            execl(terminal_path, terminal_path, "-e", game_path, (char *)NULL);
            break;
        case TERMINAL_XTERM:
            execl(terminal_path, terminal_path, "-e", game_path, (char *)NULL);
            break;
        }
        _exit(127);
    }

    if (child < 0) {
        snprintf(app->status, sizeof(app->status), "Falha ao iniciar o jogo: %s",
                 strerror(errno));
    } else {
        snprintf(app->status, sizeof(app->status), "Jogo aberto em uma nova janela de terminal.");
    }
    draw_interface(app);
}

// Function to determine the project root directory based on the program's path
static int project_root_from_path(const char *program_path, char *project_root,
                                  size_t root_size)
{
    char resolved_path[PATH_MAX];
    char build_directory[PATH_MAX];
    char *directory;

    if (realpath(program_path, resolved_path) == NULL) {
        return 0;
    }

    if (snprintf(build_directory, sizeof(build_directory), "%s", resolved_path) < 0) {
        return 0;
    }
    directory = strrchr(build_directory, '/');
    if (directory == NULL) {
        return 0;
    }
    *directory = '\0';

    directory = strrchr(build_directory, '/');
    if (directory == NULL) {
        return 0;
    }
    *directory = '\0';

    return snprintf(project_root, root_size, "%s", build_directory) > 0;
}

static void initialize_colors(App *app)
{
    app->background = color_pixel(app->display, "#10191d");
    app->panel = color_pixel(app->display, "#1a292d");
    app->hover = color_pixel(app->display, "#263b3a");
    app->foreground = color_pixel(app->display, "#f1f2e9");
    app->muted = color_pixel(app->display, "#a8b7b1");
    app->accent = color_pixel(app->display, "#d8ef78");
    app->border = color_pixel(app->display, "#34484a");
}

int main(int argc, char **argv)
{
    App app = {0};
    XSizeHints size_hints = {0};
    XEvent event;
    Atom close_atom;
    char project_root[PATH_MAX];
    int screen;

    if (argc < 1 || !project_root_from_path(argv[0], project_root,
                                             sizeof(project_root))) {
        fprintf(stderr, "Nao foi possivel localizar a pasta do projeto.\n");
        return EXIT_FAILURE;
    }

    app.display = XOpenDisplay(NULL);
    if (app.display == NULL) {
        fprintf(stderr, "Nao foi possivel abrir uma janela X11. Verifique o DISPLAY.\n");
        return EXIT_FAILURE;
    }

    screen = DefaultScreen(app.display);
    app.width = WINDOW_WIDTH;
    app.height = WINDOW_HEIGHT;
    app.hovered_game = -1;
    initialize_colors(&app);

    app.window = XCreateSimpleWindow(app.display, RootWindow(app.display, screen),
                                     0, 0, (unsigned int)app.width,
                                     (unsigned int)app.height, 0,
                                     app.background, app.background);
    XStoreName(app.display, app.window, "C Games | Menu de Jogos");
    XSelectInput(app.display, app.window,
                 ExposureMask | ButtonPressMask | PointerMotionMask | KeyPressMask |
                 StructureNotifyMask);
    app.gc = XCreateGC(app.display, app.window, 0, NULL);
    app.font = XLoadQueryFont(app.display, "9x15");
    if (app.font == NULL) {
        app.font = XLoadQueryFont(app.display, "fixed");
    }
    if (app.font != NULL) {
        XSetFont(app.display, app.gc, app.font->fid);
    }

    size_hints.flags = PMinSize;
    size_hints.min_width = WINDOW_WIDTH;
    size_hints.min_height = WINDOW_HEIGHT;
    XSetWMNormalHints(app.display, app.window, &size_hints);
    close_atom = XInternAtom(app.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(app.display, app.window, &close_atom, 1);
    signal(SIGCHLD, SIG_IGN);
    XMapWindow(app.display, app.window);

    while (1) {
        XNextEvent(app.display, &event);
        switch (event.type) {
        case Expose:
            if (event.xexpose.count == 0) {
                draw_interface(&app);
            }
            break;
        case ConfigureNotify:
            app.width = event.xconfigure.width;
            app.height = event.xconfigure.height;
            draw_interface(&app);
            break;
        case MotionNotify: {
            int hovered_game = game_at_position(event.xmotion.x, event.xmotion.y);
            if (hovered_game != app.hovered_game) {
                app.hovered_game = hovered_game;
                draw_interface(&app);
            }
            break;
        }
        case ButtonPress: {
            int selected_game = game_at_position(event.xbutton.x, event.xbutton.y);
            if (selected_game >= 0) {
                launch_game(&app, selected_game, project_root);
            }
            break;
        }
        case KeyPress: {
            char key_buffer[8];
            KeySym key_symbol;
            int key_length = XLookupString(&event.xkey, key_buffer,
                                           sizeof(key_buffer), &key_symbol, NULL);
            if (key_symbol == XK_Escape || key_symbol == XK_q || key_symbol == XK_Q) {
                XCloseDisplay(app.display);
                return EXIT_SUCCESS;
            }
            if (key_length > 0 && key_buffer[0] >= '1' &&
                key_buffer[0] <= '0' + GAME_COUNT) {
                launch_game(&app, key_buffer[0] - '1', project_root);
            }
            break;
        }
        case ClientMessage:
            if ((Atom)event.xclient.data.l[0] == close_atom) {
                XCloseDisplay(app.display);
                return EXIT_SUCCESS;
            }
            break;
        }
    }
}