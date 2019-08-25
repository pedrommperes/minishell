#include "header.h"

int count = 0;
int SIZE = 5;
int N = 10;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t *tid = NULL;

char buf[5][100];
int prodptr = 0, consptr = 0;
pthread_mutex_t trinco_p = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trinco_c = PTHREAD_MUTEX_INITIALIZER;
sem_t pode_prod;
sem_t pode_cons;
int num_paths = 0;

//char** buf;

int main (int argc, const char * argv[])
{
    char *line;

    while(1)
    {
        struct sigaction sa;
        sa.sa_handler = SIG_IGN;    // Ignorar sinal

        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGTSTP, &sa, NULL);

        rl_attempted_completion_function = character_name_completion;

        printf ("\n");
        line = readline("msh$ ");

        if(line == NULL)
        {
            perror("msh$ malloc error!\n");
            exit(1);
        }

        if (!strcmp(line, "exit")) exit(EXIT_SUCCESS);

        if (!strcmp(line, "")) continue;

        add_history(line);

        CMD* root = parse_line(line);

        count = 0;
        //print_command_list(root);
        //free_command_list(root);
        //free(line);


        if (strcmp(root->name, "cd") == 0)
        {
            msh_cd(root->argv[1]);
        }

        if (strcmp(root->name, "pwd") == 0)
        {
            msh_pwd();
        }

        if (strcmp(root->name, "myfind") == 0)
        {

            if (root->argv[1] != NULL && root->argv[2] != NULL)
            {
                chdir(root->argv[1]);
                char* path = get_current_dir_name();

                int i = 0;

                pthread_t tidf;
                pthread_t tids[N];

                sem_init(&pode_prod, 0, N-1); // espaços livres no buffer
                sem_init(&pode_cons, 0, 1); // items no buffer, ja la vai ter 1 item

                strcpy(buf[prodptr], path);
                num_paths++;
                prodptr = (prodptr+1) % N;

                pthread_create(&tidf, NULL, &produtor, (void*) path);

                for (i = 0; i < N; i++)
                {
                    pthread_create(&tids[i], NULL, &consumidor, (void*) root->argv[2]);
                }

                pthread_join(tidf, NULL);

                for (i = 0; i < N; i++)
                {
                    pthread_join(tids[i], NULL);
                }

                pthread_mutex_destroy(&trinco_p);
                pthread_mutex_destroy(&trinco_c);
                sem_destroy(&pode_prod);
                sem_destroy(&pode_cons);

            }
        }

        if (strcmp(root->name, "myls") == 0)
        {
            if (root->argv[1] == NULL)
            {
                DIR *d = NULL;
                struct dirent *dir = NULL;
                struct stat mystat;
                char path[1025];

                d = opendir(".");

                if (d)
                {
                    while((dir = readdir(d)) != NULL)
                    {
                        strcpy(path, dir->d_name);
                        lstat(path, &mystat);
                        print_files_color2(dir->d_name, mystat.st_mode, 1);
                    }

                    printf ("\n");

                    closedir(d);
                }
            }

            else
            {
                char argum1[MAXARGS] = "";
                char argum2[MAXARGS] = "-a";
                char argum3[MAXARGS] = "-l";
                char argum4[MAXARGS] = "-al";
                char argum5[MAXARGS] = "-R";

                strcat(argum1, root->argv[1]);

                if (strcmp(argum1, argum2) == 0)
                {
                    if (root->argv[2] != NULL)
                    {
                        DIR *d = NULL;
                        struct dirent *dir = NULL;
                        struct stat mystat;
                        char path[1025];

                        d = opendir(root->argv[2]);

                        if (d)
                        {
                            while((dir = readdir(d)) != NULL)
                            {
                                strcpy(path, dir->d_name);
                                lstat(path, &mystat);
                                print_files_color2(dir->d_name, mystat.st_mode, 2);
                            }

                            printf ("\n");

                            closedir(d);
                        }
                    }

                    else
                    {
                        DIR *d = NULL;
                        struct dirent *dir = NULL;
                        struct stat mystat;
                        char path[1025];

                        d = opendir(".");

                        if (d)
                        {
                            while((dir = readdir(d)) != NULL)
                            {
                                strcpy(path, dir->d_name);
                                lstat(path, &mystat);
                                print_files_color2(dir->d_name, mystat.st_mode, 2);
                            }

                            printf ("\n");

                            closedir(d);
                        }
                    }
                }

                else if (strcmp(argum1, argum3) == 0)
                {
                    if (root->argv[2] != NULL)
                    {
                        DIR *d = NULL;
                        struct dirent *dir = NULL;
                        struct stat mystat;
                        char path[1025];

                        d = opendir(root->argv[2]);

                        if (d)
                        {
                            while((dir = readdir(d)) != NULL)
                            {
                                strcpy(path, dir->d_name);
                                lstat(path, &mystat);
                                char the_dot = '.';

                                if (dir->d_name[0] == the_dot || dir->d_name[1] == the_dot)
                                {

                                }

                                else
                                {
                                    print_filetype(mystat.st_mode);
                                    print_permissions(mystat.st_mode);
                                    printf(" %d\t", mystat.st_nlink);
                                    printf("%s ", getpwuid(mystat.st_uid)->pw_name);
                                    printf("%s ", getgrgid(mystat.st_gid)->gr_name);
                                    printf("%d\t", mystat.st_size);
                                    printf(" ");
                                    print_time(mystat.st_mtime);
                                    printf(" ");
                                    print_files_color2(dir->d_name, mystat.st_mode, 3);
                                    printf("\n");
                                }
                            }

                            printf ("\n");

                            closedir(d);
                        }
                    }

                    else
                    {
                        DIR *d = NULL;
                        struct dirent *dir = NULL;
                        struct stat mystat;
                        char path[1025];

                        d = opendir(".");

                        if (d)
                        {
                            while((dir = readdir(d)) != NULL)
                            {
                                strcpy(path, dir->d_name);
                                lstat(path, &mystat);
                                char the_dot = '.';

                                if (dir->d_name[0] == the_dot || dir->d_name[1] == the_dot)
                                {

                                }

                                else
                                {
                                    print_filetype(mystat.st_mode);
                                    print_permissions(mystat.st_mode);
                                    printf(" %d\t", mystat.st_nlink);
                                    printf("%s ", getpwuid(mystat.st_uid)->pw_name);
                                    printf("%s ", getgrgid(mystat.st_gid)->gr_name);
                                    printf("%d\t", mystat.st_size);
                                    printf(" ");
                                    print_time(mystat.st_mtime);
                                    printf(" ");
                                    print_files_color2(dir->d_name, mystat.st_mode, 3);
                                    printf("\n");
                                }
                            }

                            printf ("\n");

                            closedir(d);
                        }
                    }
                }

                else if (strcmp(argum1, argum4) == 0)
                {
                    if (root->argv[2] != NULL)
                    {
                        DIR *d = NULL;
                        struct dirent *dir = NULL;
                        struct stat mystat;
                        char path[1025];

                        d = opendir(root->argv[2]);

                        if (d)
                        {
                            while((dir = readdir(d)) != NULL)
                            {
                                strcpy(path, dir->d_name);
                                lstat(path, &mystat);
                                print_filetype(mystat.st_mode);
                                print_permissions(mystat.st_mode);
                                printf(" %d\t", mystat.st_nlink);
                                printf("%s ", getpwuid(mystat.st_uid)->pw_name);
                                printf("%s ", getgrgid(mystat.st_gid)->gr_name);
                                printf("%d\t", mystat.st_size);
                                printf(" ");
                                print_time(mystat.st_mtime);
                                printf(" ");
                                print_files_color2(dir->d_name, mystat.st_mode, 3);
                                printf("\n");
                            }

                            printf ("\n");

                            closedir(d);
                        }
                    }
                    else
                    {
                        DIR *d = NULL;
                        struct dirent *dir = NULL;
                        struct stat mystat;
                        char path[1025];

                        d = opendir(".");

                        if (d)
                        {
                            while((dir = readdir(d)) != NULL)
                            {
                                strcpy(path, dir->d_name);
                                lstat(path, &mystat);
                                print_filetype(mystat.st_mode);
                                print_permissions(mystat.st_mode);
                                printf(" %d\t", mystat.st_nlink);
                                printf("%s ", getpwuid(mystat.st_uid)->pw_name);
                                printf("%s ", getgrgid(mystat.st_gid)->gr_name);
                                printf("%d\t", mystat.st_size);
                                printf(" ");
                                print_time(mystat.st_mtime);
                                printf(" ");
                                print_files_color2(dir->d_name, mystat.st_mode, 3);
                                printf("\n");
                            }

                            printf ("\n");

                            closedir(d);
                        }
                    }
                }

                //-R
                else if (strcmp(argum1, argum5) == 0)
                {

                    if (root->argv[2] != NULL)
                    {
                        char* dirname = root->argv[2];
                        int i = 0;

                        tid = malloc(5000 * sizeof(*tid));
                        tid = (pthread_t *) malloc(1 * sizeof(pthread_t));
                        pthread_create (&tid[0], NULL, &RecDir, (void*)dirname);

                        int k = count;

                        for (i = 0; i < k; i++)
                        {
                            pthread_join (tid[i], NULL);
                            free(&tid[i]);
                        }
                        free(tid);
                        printf ("\n");
                    }

                    else
                    {
                        char* dirname = ".";
                        int i = 0;

                        tid = malloc(300 * sizeof(*tid));
                        tid = (pthread_t *) malloc (1 * sizeof(pthread_t));
                        pthread_create (&tid[0], NULL, &RecDir, (void*)dirname);

                        int k = count;

                        for (i = 0; i < k; i++)
                        {
                            pthread_join (tid[i], NULL);
                            free(&tid[i]);
                        }

                        free(tid);
                        printf ("\n");
                    }
                }

                else if (strcmp(argum1, "") == 0)
                {
                    DIR *d = NULL;
                    struct dirent *dir = NULL;
                    struct stat mystat;
                    char path[1025];

                    d = opendir(".");

                    if (d)
                    {
                        while((dir = readdir(d)) != NULL)
                        {
                            strcpy(path, dir->d_name);
                            lstat(path, &mystat);
                            print_files_color2(dir->d_name, mystat.st_mode, 1);
                        }

                        printf ("\n");

                        closedir(d);
                    }
                }

                else
                {
                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));

                    DIR *d = NULL;
                    struct dirent *dir = NULL;
                    struct stat mystat;
                    char path[1025];
                    char the_dot = '.';

                    d = opendir(argum1);

                    if (d)
                    {
                        while((dir = readdir(d)) != NULL)
                        {
                            strcpy(path, dir->d_name);
                            lstat(path, &mystat);
                            print_files_color2(dir->d_name, mystat.st_mode, 1);
                        }

                        printf ("\n");

                        closedir(d);
                    }
                }
            }
        }

        else
        {
            if (root->next == NULL)
            {
                pid_t pid;
                int result;

                pid = fork();

                //Processo filho
                if (pid == 0)
                {
                    if (root->infile != NULL)   // <
                    {
                        int fd = open(root->infile, O_RDONLY, 0644);    // Abre para ler
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                    }


                    if (root->outfile != NULL)   // >
                    {
                        int fd = open(root->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);   // Abre para escrever
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }

                    if (root->errfile != NULL)   // 2>
                    {
                        int fd = open(root->errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);   // Abre para escrever
                        dup2(fd, STDERR_FILENO);
                        close(fd);
                    }

                    execute (root);
                    perror("$msh Erro no execute.");
                    exit(EXIT_FAILURE);
                }

                else if (pid < 0)
                {
                    //Erro no fork
                    perror("msh$ Erro no fork.");
                    exit(EXIT_FAILURE);
                }

                else
                {
                    //Processo pai
                    wait(NULL);
                }
            }

            else
            {
                int number_of_pipes = 0, i = 0, result = 0, j = 0;
                CMD* temp;
                CMD* temp2;
                temp = root;
                temp2 = root;
                pid_t pid;

                while (temp2->next != NULL)
                {
                    number_of_pipes++;
                    temp2 = temp2->next;
                }

                int number_of_commands = number_of_pipes + 1;

                int fds[2];
                int past; // Contem a saida do pipe anterior

                for (i = 0; i < number_of_commands - 1; i++)
                {
                    if(pipe(fds) < 0)       // Cria um pipe para cada comando (excepto o ultimo)
                    {
                        perror("$msh Erro no pipe.");
                        exit(EXIT_FAILURE);
                    }

                    pid = fork();  // Cria um processo para cada comando (excepto o ultimo)

                    if (pid == 0) // Para o novo processo, o filho herda os file descriptors do pai
                    {
                        if (i != 0) // Se não for o primeiro comando
                        {
                            dup2(past, STDIN_FILENO); // Recebe como entrada a saida do pipe passado
                        }

                        dup2(fds[1], STDOUT_FILENO); // A saida vai para o novo pipe

                        execute(temp);
                        perror("$msh Erro no execute.");
                        exit(EXIT_FAILURE);
                    }

                    else if (pid < 0)
                    {
                        //Erro no fork
                        perror("msh$ Erro no fork.");
                        exit(EXIT_FAILURE);
                    }

                    close(fds[1]); // Fecha o pipe para o pai

                    if (i != 0) close(past); // Fecha o pipe passado para o pai

                    past = fds[0]; // O pipe atual agora é o passado

                    temp = temp->next;  // Comando seguinte na lista
                }

                pid = fork();

                if (pid == 0) // Executado para o ultimo processo
                {
                    if (i != 0) // Se ele também não for o primeiro
                    {
                        dup2(past, STDIN_FILENO); // Coloca a entrada para a saida do ultimo pipe
                    }

                    if (temp->outfile != NULL)   // >
                    {
                        int fd = open(temp->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);   // Abre para escrever
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }

                    if (temp->errfile != NULL)   // 2>
                    {
                        int fd = open(temp->errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);   // Abre para escrever
                        dup2(fd, STDERR_FILENO);
                        close(fd);
                    }

                    execute(temp);
                    perror("$msh Erro no execute.");
                    exit(STDIN_FILENO);
                }

                else if (pid < 0)
                {
                    //Erro no fork
                    perror("msh$ Erro no fork.");
                    exit(STDIN_FILENO);
                }

                //Espera pelos filhos
                for(i = 0; i < number_of_commands; i++)
                {
                    wait(NULL);
                }
            }
        }

        //free_command_list(root);
        //free(line);
    }
}

// Função que executa um comando
void execute (CMD* root)
{
    if (execvp(root->name, root->argv) == -1) // Name ou argv[0]
    {
        if (strcmp(root->name, "cd") != 0 && strcmp(root->name, "myfind") != 0) perror("msh$ Erro.");
    }

    exit(STDIN_FILENO);
}

// Função que vai tratar do change diretory
void msh_cd (char *args)
{
    if (args == NULL)
    {
        printf("msh$ cd não tem argumentos. \n");
    }

    else
    {
        if (chdir(args) != 0) // Mudar do diretorio atual para o que esta indicado nos argumentos (args)
        {
            perror("msh$");
        }

        else
        {
            printf ("Mudança de diretorio para: ");
            msh_pwd(); // Print do diretory atual
        }
    }

}

// Função que trata de imprimir o diretorio atual
void msh_pwd()
{
    char dir[MAX_SIZE];

    getcwd (dir, MAX_SIZE);

    if (dir == NULL)
    {
        perror("Erro.");
    }

    printf ("%s\n", dir);
}

char **character_name_completion(const char *text, int start, int end)
{
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, character_name_generator);
}

char *character_name_generator(const char *text, int state)
{
    static int list_index, len;
    char *name;
    size_t i, n = 0;

    if (!state)
    {
        list_index = 0;
        len = strlen(text);
    }

    char **array = get_path();
    char **list = scan_directory(array, &n);

    while (name = list[list_index++])
    {
        if (strncmp(name, text, len) == 0)
        {
            return strdup(name);
        }

    }

    free(list);

    return NULL;
}

// Faz o scan dos diretorios no PATH e guarda todos os ficheiros
char **scan_directory(char** dirnames, size_t *elems)
{
    DIR *d = NULL;
    struct dirent *dir = NULL;
    char **list = NULL;
    char **array = dirnames;
    int j = 0;

    // Vai percorrer o array de strings que contem os caminhos na variavel PATH
    for (j = 0; j < sizeof(array); j++)
    {
        char* dirname = NULL;
        dirname = array[j];

        d = opendir(dirname);   // abre o diretorio

        if (d)
        {
            while ((dir = readdir(d)) != NULL)  // vai percorrer cada diretorio do diretorio da variavel PATH
            {
                list = realloc(list, sizeof(*list) * (*elems + 1));
                list[(*elems)++] = strdup(dir->d_name);
            }
            closedir(d);
        }
    }

    return list;
}

// Retorna um array de strings com todos os caminhos do PATH
char **get_path()
{
    char **array;
    char *orig_path_var = getenv("PATH");
    char *path_var = strdup(orig_path_var ? orig_path_var : ""); // so para o caso do PATH ser nulo
    char *the_dot = ".";
    int j;
    int len = strlen(path_var);
    int nb_colons = 0;
    char pathsep = ':';
    int current_colon = 0;

    // primeiro contamos quantos caminhos temos, e dividimos quase como um strtok faria
    for (j = 0; j < len; j++)
    {
        if (path_var[j] == pathsep)
        {
            nb_colons++;
            path_var[j] = '\0';
        }
    }

    // alocar memoria para o array de strings
    array = malloc((nb_colons + 1) * sizeof(*array));

    array[0] = path_var;  // primeiro caminho

    // caminhos restantes
    for (j = 0; j < len; j++)
    {
        if (path_var[j] == '\0')
        {
            current_colon++;
            array[current_colon] = path_var + j + 1;
            if (array[current_colon][0] == '\0')
            {
                // caso especial: adicionar um "." se o caminho estiver vazio
                array[current_colon] = the_dot;
            }

        }
    }

    return array;
}

void print_files_color2 (char* dirname, mode_t mystat, int option)
{
    char the_dot = '.';
    char path[1025];
    strcpy(path, dirname);

    if (option == 1)
    {
        if (mystat & S_IXUSR && !S_ISDIR(mystat) && !S_ISLNK(mystat))
        {
            printf (KGRN "%s  " RESET, dirname);
        }

        else if (S_ISLNK(mystat))
        {
            printf (KBLU "%s  " RESET, dirname);
        }

        else if (dirname[0] == the_dot || dirname[1] == the_dot)
        {

        }

        else printf ("%s  ", dirname);
    }

    else if (option == 2)
    {
        if (mystat & S_IXUSR && !S_ISDIR(mystat) && !S_ISLNK(mystat))
        {
            printf (KGRN "%s  " RESET, dirname);
        }

        else if (S_ISLNK(mystat))
        {
            printf (KBLU "%s  " RESET, dirname);
        }

        else if (dirname[0] == the_dot && strcmp(dirname, ".") != 0 && strcmp(dirname, "..") != 0)
        {
            printf (KRED "%s  " RESET, dirname);
        }

        else printf ("%s  ", dirname);
    }

    else if (option == 3)
    {
        if (mystat & S_IXUSR && !S_ISDIR(mystat) && !S_ISLNK(mystat))
        {
            printf (KGRN "%s  " RESET, dirname);
        }

        else if (S_ISLNK(mystat))
        {
            char link_buf[512];
            int count = readlink(dirname, link_buf, sizeof(link_buf));

            if (count >= 0)
            {
                link_buf[count] = '\0';

                printf(KBLU "%s" RESET " -> %s", dirname, link_buf);
            }
        }

        else if (dirname[0] == the_dot && strcmp(dirname, ".") != 0 && strcmp(dirname, "..") != 0)
        {
            printf (KRED "%s  " RESET, dirname);
        }

        else printf ("%s  ", dirname);
    }
}

void print_files_color (char* dirname, struct stat mystat, int option)
{
    char the_dot = '.';
    char path[1025];
    strcpy(path, dirname);

    if (option == 1)
    {
        if (lstat(path, &mystat) != 0)
        {
            perror("stat: ");
        }

        else if (mystat.st_mode & S_IXUSR && !S_ISDIR(mystat.st_mode) && !S_ISLNK(mystat.st_mode))
        {
            printf (KGRN "%s  " RESET, dirname);
        }

        else if (S_ISLNK(mystat.st_mode))
        {
            printf (KBLU "%s  " RESET, dirname);
        }

        else if (dirname[0] == the_dot || dirname[1] == the_dot)
        {

        }

        else printf ("%s  ", dirname);
    }

    else if (option == 2)
    {
        if (lstat(path, &mystat) != 0)
        {
            perror("stat: ");
        }

        else if (mystat.st_mode & S_IXUSR && !S_ISDIR(mystat.st_mode) && !S_ISLNK(mystat.st_mode))
        {
            printf (KGRN "%s  " RESET, dirname);
        }

        else if (S_ISLNK(mystat.st_mode))
        {
            printf (KBLU "%s  " RESET, dirname);
        }

        else if (dirname[0] == the_dot && strcmp(dirname, ".") != 0 && strcmp(dirname, "..") != 0)
        {
            printf (KRED "%s  " RESET, dirname);
        }

        else printf ("%s  ", dirname);
    }

    else if (option == 3)
    {
        if (lstat(path, &mystat) != 0)
        {
            perror("stat: ");
        }

        else if (mystat.st_mode & S_IXUSR && !S_ISDIR(mystat.st_mode) && !S_ISLNK(mystat.st_mode))
        {
            printf (KGRN "%s  " RESET, dirname);
        }

        else if (S_ISLNK(mystat.st_mode))
        {
            char link_buf[512];
            int count = readlink(dirname, link_buf, sizeof(link_buf));

            if (count >= 0)
            {
                link_buf[count] = '\0';

                printf(KBLU "%s" RESET " -> %s", dirname, link_buf);
            }
        }

        else if (dirname[0] == the_dot && strcmp(dirname, ".") != 0 && strcmp(dirname, "..") != 0)
        {
            printf (KRED "%s  " RESET, dirname);
        }

        else printf ("%s  ", dirname);
    }
}

void print_permissions(mode_t mode)
{
    // Se true então "r" senão "-"
    putchar((mode & S_IRUSR) ? 'r' : '-');
    putchar((mode & S_IWUSR) ? 'w' : '-');
    putchar((mode & S_IXUSR) ? 'x' : '-');
    putchar((mode & S_IRGRP) ? 'r' : '-');
    putchar((mode & S_IWGRP) ? 'w' : '-');
    putchar((mode & S_IXGRP) ? 'x' : '-');
    putchar((mode & S_IROTH) ? 'r' : '-');
    putchar((mode & S_IWOTH) ? 'w' : '-');
    putchar((mode & S_IXOTH) ? 'x' : '-');
}

void print_filetype(mode_t mode)
{
    switch (mode & S_IFMT) //considerar apenas os bits envolvidos na determinaçao do tipo de ficheiro
    {
    case S_IFREG:
        putchar('-');
        break;
    case S_IFDIR:
        putchar('d');
        break;
    case S_IFLNK:
        putchar('l');
        break;
    case S_IFCHR:
        putchar('c');
        break;
    case S_IFBLK:
        putchar('b');
        break;
    case S_IFSOCK:
        putchar('s');
        break;
    case S_IFIFO:
        putchar('f');
        break;
    }
}

void print_time(time_t mod_time)
{
    // data atual com ano
    time_t curr_time;
    time(&curr_time);
    struct tm* t = localtime(&curr_time);
    const int curr_mon = t->tm_mon;
    const int curr_yr = 1970 + t->tm_year;

    t = localtime(&mod_time);
    const int mod_mon = t->tm_mon;
    const int mod_yr = 1970 + t->tm_year;

    // determinar formato baseado no ano
    const char* format = ((mod_yr == curr_yr) && (mod_mon >= (curr_mon - 6))) ? "%b %e %H:%M" : "%b %e  %Y";

    char time_buf[128];
    strftime(time_buf, sizeof(time_buf), format, t);
    printf("%s", time_buf);
}

void* RecDir(void* param)
{
    pthread_mutex_lock(&mutex);
    int j = count++;


    char* path;
    path = (char*) param;
    DIR *d = opendir(path);
    struct stat mystat;

    if(!d)
    {
        perror(path);
        return;
    }

    struct dirent *ep;
    char newdir[512];

    printf("\n\n%s:\n", path);

    while((ep = readdir(d)))
    {
        if(strncmp(ep->d_name, ".", 1) != 0)
        {
            lstat(ep->d_name, &mystat);
            print_files_color2(ep->d_name, mystat.st_mode, 1);
            //printf("%s ", ep->d_name);
        }
    }

    closedir(d);
    d = opendir(path);

    while((ep = readdir(d)))
        if(strncmp(ep->d_name, ".", 1) != 0)
        {
            //strcpy(path, ep->d_name);
            //lstat(ep->d_name, &mystat);

            //if(mystat.st_mode & S_IFMT & S_IFDIR)
            if(ep->d_type == 4)
            {
                sprintf(newdir, "%s/%s", path, ep->d_name);
                char* new_next;
                new_next = strdup(newdir);
                tid = (pthread_t *) malloc ((j + 1) * sizeof(pthread_t));
                pthread_create (&tid[j], NULL, &RecDir, (void*)new_next);
                //RecDir(new_next);

            }
        }
    closedir(d);
    pthread_mutex_unlock(&mutex);
}

void* produtor(void* param)
{
    char* fn;
    fn = (char*) param;

    findRec(fn);
}


void findRec(char *fn)
{
    DIR *dir;
    struct dirent *entry;
    int count;
    char path[1025];

    if ((dir = opendir(fn)) == NULL)
        perror("opendir() error");
    else
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                if (entry->d_type == 4)
                {
                    sem_wait(&pode_prod);
                    pthread_mutex_lock(&trinco_p);

                    strcpy(path, fn);
                    strcat(path, "/");
                    strcat(path, entry->d_name);

                    strcpy(buf[prodptr], path);
                    num_paths++;
                    prodptr = (prodptr+1) % N;

                    pthread_mutex_unlock(&trinco_p);
                    sem_post(&pode_cons);

                    findRec(path);
                }
            }
        }
        closedir(dir);
    }
}


void* consumidor(void* param)
{
    char* fn;
    fn = (char*) param;

    while (1)
    {
        if (num_paths == 0)
        {

        }
        //printf ("%s\n", buf[consptr]);
        sem_wait(&pode_cons);
        pthread_mutex_lock(&trinco_c);
        //printf ("%s--\n", buf[consptr]);
        //printf ("%d-- ", num_paths);

        char* path = buf[consptr];
        consptr = (consptr+1) % N;
        num_paths--;

        pthread_mutex_unlock(&trinco_c);
        sem_post(&pode_prod);
        //consome(item);

        DIR *d;
        struct dirent *dir;
        d = opendir(path);
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (strcmp(fn, dir->d_name) == 0)
                {
                    printf("%s\n", path);
                    printf("%s\n", dir->d_name);
                }
            }
            closedir(d);
        }
    }
}


