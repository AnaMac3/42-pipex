# **Pipex**
Proyecto pipex del cursus 42.

### Introducción

blablabla

El programa debe ejecutarse de la siguiente manera: *./pipex infile cmd1 cmd2 outfile*
Y tiene que emular el comportamiento del comando *< infile cmd1 | cmd2 > outfile*, donde:
  - infile y outfile son nombres de archivos.
  - cmd1 y cmd2 son comandos de shell.

¿Qué hace este comando? 

Toma el contenido de infile y lo envía como entrada estándar (stdin) al cmd1. El pipe (|) conecta la salida estándar (stdout) del cmd1 con la entrada (stdin) del cmd2. El resultado de ejecutar el cmd1 se pasa directamente al cmd2 sin guardarlo en un archivo temporal. Así, se ejecutan dos comandos en cadena. La salida estándar de cmd2 se guarda en outfile. Si el archivo existe, se sobreescribe; si no, se crea uno nuevo. En lugar de mostrar los resultados por pantalla, se rederigen a outfile.


### Explicación de las funciones autorizadas

***open()***

Abre un archivo y obtiene su descriptor de archivo (file descriptor - fd) (número entero que representa el archivo abierto en el sistema operativo). 

Prototipo: int open(const char *pathname, int flags, mode_t mode)

  - pahtname: ruta del archivo.
  - flags: entero que especifica el modo en el que se abre el archivo. Algunos modos comunes:
      - O_RDONLY: solo lectura.
      - O_WRONLY: solo escritura.
      - O_RDWR: lectura y escritura.
      - O_CREAT: crea el archivo si no existe.
      - O_TRUNC: si el archivo existe, trunca su tamaño a 0, es decir, elimina su contenido.
      - O_APPEND: escribe al final del archivo sin truncarlo.
  - mode: solo se utiliza con O_CREAT. Se establecen los permisos del archivo, expresados en octal. Por ejemplo, 0644 indica: permisos de lectura y escritura para el propietario, solo lectura para el grupo y solo lectura para otros. 
    
Devuelve un file descriptor (número entero) en caso de éxito y -1 en caso de error.

***close()***

Cierra un archivo que ha sido abierto mediante open(). Al cerrarlo, el sistema libera el file descriptor y cualquier recurso asociado, permitiendo que el sistema operativo reutilice el descriptor para otras operaciones. 

Prototipo: int close(int fd)

Devuelve 0 si el archivo se cierra correctamente y -1 y errno en caso de error.

***read()***

Lee datos desde un archivo (a partir de su fd) o entrada (como stdin).

Prototipo: ssize_t read (int fd, void *buffer, size_t count)

  - fd: descriptor de archivo desde el que se quiere leer. Se trata del entero obtenido mediante open() o dup() o un descritor estándar como 0 (stdin).
  - buffer: puntero a un buffer (o espacio de memoria) donde se almacenarán los datos leídos.
  - count: número máximo de bytes que se intentarán leer.
  Devuelve el número de bytes leídos, que pueden ser menos que count si se llega al final del archivo o si hay menos datos disponibles en la entrada. Si se alcanza el final del archivo (EOF), devuelve 0. Devuelve -1 y establece errno en caso de error.

***perror()***

Imprime un mensaje de error en la salida de error estándar (stderr). Muestra un mensaje personalizado junto con la descripción del error correspondiente a errno (variable global que almacena códigos de error específicos de la última operación fallida).

Prototipo: void perror(const char *s)

  - s: mensaje personalizado que se imprimirá antes de la descripción del error del sistema. Si s es NULL o una cadena vacía, solo se muestra la descripción del error.

***strerror()***

Convierte el valor de errno en un mensaje de error legible para los humanos. Devuelve una cadena de texto que describe el error. Útil para mostrar mensajes personalizados en combinación con otras salidas, o cuando prefieres usar tu propio método de impresión en lugar de perror().

Prototipo: char *strerror(int errnum)

  - errnum: código de error que se desea convertir en un mensaje de error. Típicamente es el valor de errno después de una operación fallida.

Devuelve un puntero a una cadena de caracteres con el mensaje de error correspondiente al valor de errnum.

  **NOTA PERSONAL**

  Con tal de que mis mensajes error sean lo más parecidos posibles a los mensajes error del comando *< file1 cmd1 | cmd2 > file2* , he decidido imprimir mis mensajes personalizados con ft_putstr_fd en vez de utilizar perror() y strerror().

***access()***

Comprueba la accesibilidad de un archivo o directorio. 

Prototipo: int access(const char *pathname, int mode)

  - pahtname: cadena que representa la ruta.
  - mode: entero que especifica el tipo de acceso que queremos verificar.
      - F_OK: verifica la existencia del archivo. 
      - R_OK: accesible para lectura.
      - W_OK: accesible para escritura.
      - X_OK: accesible para ejecución.

Devuelve 0 en si el acceso especificado está permitido. o -1 y errno en caso contrario.

***dup()***

Duplica descriptores de archivo. Útil para redirigir entradas y salidas, especialmente en los procesos que necesitan manipular la entrada estándar (stdin), la salida estándar (stdout) o la salida de error estándar (stderr).

Prototipo: int dup(int oldfd)

  - oldfd: file descriptor que se desea duplicar.

Devuelve el nuevo descritor de archivo duplicado o 1 en caso de error, estableciendo errno. El nuevo descritor de archivo apunta al mismo archivo que oldfd, compartiendo el mismo puntero de archivo y heredando los mismos permisos de acceso.

***dup2()***

Permite duplicar un file descriptor y también especificar en qué número de file descriptor queremos que se duplique.

Prototipo: int dup(int oldfd, int newfd)

  - oldfd: file descriptor que se desea duplicar.
  - newfd: nuevo file descriptor. Si newfd está en uso, dup2 lo cierra antes de duplicar oldfd en él.

Devuelve el nuevos file descriptor, es decir, newfd, o -1 en caso de error.

***execve()***

Reemplaza el proceso actual con un nuevo programa. Se utiliza para ejecutar un programa especificado en un archivo ejecutable, con el entorno y los argumentos que se le proporcionan.  

Prototipo: int execve(const char *filename, char *const argv[], char *const envp[])

  - filename: ruta del archivo ejecutable que queremos ejecutar.
  - argv: arreglo de punteros a cadenas de caracteres que representan los argumentos del programa. Generalmente, argv[0] almacena el nombre del programa y el último elemeneto debe ser NULL.
  - envp: arreglo de punteros a cadenas de caracteres que contiene las variables de entorno.

En caso de éxito, no devuelve nada; el proeso actual se reemplaza por el nuevo programa y la ejecución continúa desde el punto de entrada del nuevo programa. En caso de error devuelve -1 y establece errno.

***exit()***

Termina un programa de manera controlada. Permite salir de un programa con un código de estado que indica si la salida fue exitosa o si ocurrió un error.

Prototipo: void exit(int status)

  - status: entero que se devuelve al sistema operativo cuando el programa termina. Este valor puede indicar éxito (0) o error (valor distinto de 0).

***fork()***

Genera un nuevo proceso hijo a partir de un proceso padre. El proceso hijo es prácticamente una copia del proceso padre, pero tiene un identificador de proceso (PID) único y un identificador de proceso padre (PPID) único. El proceso padre y el proceso hijo se ejecutan simultáneamente, aunque pueden comportarse de manera diferente.

Prototipo: fd_t fork(void)

En el proceso padre, fork() devuelve el PID del proceso hijo. El el proceso hijo, fork() devuelve 0. Si fork() falla, devuelve -1 en el proceso padre y no crea ningún proceso hijo.

    Ejemplo:
    pid_t pid;
    
    pid = fork();
    if (pid < 0)
    {
      perror("Fork: ");
      exit(EXIT_FAILURE);
    }
    else if (pid == 0)
      //Estamos en el proceso hijo
    else //otro número positivo
      //Estamos en el proceso padre

***pipe()***

Crea un canal de comunicación unidireccional entre procesos. Permite que un proceso (el escritor) envíe datos a otro proceso (el lector) a través de un buffer de memoria. La función pipe() crea un array de dos file descriptors que representan los extremos del pipe.
  - pipefd[0]: extremo de lectura.
  - pipefd[1]: extremo de escritura.
  pipefd[1] escribirá en su fd y pipefd[0] leerá el pipefd[1] y escribirá en su propio fd.

Prototipo: int pipe(int pipefd[2])

Devuelve 0 en caso de éxito y en caso de error devuelve -1 y establece errno.

***unlink()***

Elimina un nombre de archivos del sistema de archivos, lo que significa que el archivo ya no es accesible mediante ese nombre. Pero el archivo no se elimina físicamente hasta que no haya más referencias a él. 

Concepto de contabilidad de enlaces: cada arhcivo en Unix/Linux tiene un contador de enlaces. Si el contador llega a 0 significa que no hay más nombres de archivo apuntando a los datos del archivo y el contenido se libera. Si aún hay otros nombres de archivo apuntando al mismo contenido, el archivo permanece en el disco. 

Prototipo: int unlink(const char *pathname)
  - pathname: ruta del archivo a eliminar.

Devuelve 0 en caso de éxito y -1 y errno en caso de error.

Para eliminar un archivo, el proceso debe tener los permisos adecuados. Si no se tienen o el archivo está en uso, unlink() fallará. Si el archivo está abierto al llamar a unlink(), su contenido no se eliminará hasta que todos los fd de archivo que apuntan a él sean cerrados. 

***wait()***

Permite que un proceso padre suspenda su ejecución hasta que uno de sus procesos hijo termine. Cuando un proceso hijo finaliza, wait() recupera su estado de salida, lo que permite al proceso padre realizar tareas específicas dependiendo del resultado de la ejecución del hijo.

Prototipo: pid_t wait(int *status)

  - status: puntero a un entero donde wait() almacena el estado de salida del hijo. Este valor sirve para obtener información sobre cómo terminó el proceso hijo, normalmente mediante macros como WIFEXITED o WEXITSTATUS...

Devuelve el PID del hijo que terminó en caso de éxito. Si no hay hijos a los que esperar, devuelve -1 y establece errno.

***waitpid()***

Permite que un proceso padre espere a que un proceso hijo termine su ejecución. Es más flexible que wait porque permite especificar a qué proceso hijo se debe esperar. Útil cuando hay múltiples procesos hijo.

Prototipo: pid_t waitpid(pid_t pid, int *status, int options)

  - pid: puede ser el PID del proceso hijo al que se quiere esperar (un número positivo). Si es -1 indica que se espera a cualquier proceso hijo (comportamiento similar a wait()). Si es 0 espera a cualquier proceso hijo del mismo grupo de procesos. Si es otro valor negativo, espera a cualquier hijo cuyo grupo de procesos sea el mismo que el del proceso padre.
  - status: puntero a un entero donde wait() almacena el estado de salida del hijo. Este valor sirve para obtener información sobre cómo terminó el proceso hijo, normalmente mediante macros como WIFEXITED o WEXITSTATUS...
  - options: 0 indica el comportamiento estándar, espera a que el hijo termine. WNOHANG no bloquea, si no hay hijos que no hayan terminado, retorna inmediatamente. WUNTRACED también retorna si un hijo está detenido.

Devuelve el PID del hijo que terminó en caso de éxito. En caso de error, devuelve -1 y establece errno.

Usar waitpid() es esencial para evitar que los procesos hijos se conviertan en zombies. Cada vez que un hijo termina, si el padre no lo recoge, este proceso hijo permanece en la tabla de procesos como un proceso zombie hasta que el padre llame a una de estas funciones. Un proceso zombie es un proceso que ha temrinado su ejecución pero todavía tiene una entrada en la tabla de procesos del sistema.
Comprobar procesos zomies: despues de ejecutar tu programa, usar comando *ps aux | grep pipex* y ver si hay alguna 'Z' en la columna de estado (la octava columna). La columna de estado mostrará una Z indicando que el proceso está en estado zombie.

**Parte bonus:**

Gestionar múltiples pipes.

El programa debe ejecutarse así: *./pipex infile cmd1 cmd2 cmd3 ... cmdn outfile*

Y tiene que emular el comportamiento del comando: *< infile cmd1 | cmd2 | cmd3 ... | cmdn > outfile*

¿Qué hace este comando?

Comandos adicionales en la misma cadena de tuberías. Cada comando tomará la salida del comando anterior como su entrada. La salida final del último comando se guarda en outfile. 

Cada proceso intermedio debe tomar la entrada desde el pipe de lectura del proceso anterior(prev_pipefd[0]) y enviar su salida al pipe de escritura del proceso actual (pipefd[1]). 

También tiene que aceptar << y >> cuando el primer parámetro es "here_doc":

*./pipex here\_doc LIMITADOR cmd cmd1 file*

Debe comportarse como: *cmd << LIMITADOR | cmd1 >> file*

*cmd << LIMITADOR*: este comando se conoce como here-document o heredoc. Permite introducir múltiples líneas de texto como entrada para el comando. El texto se introduce hasta que se encuentra una línea que contiene solo "LIMITADOR".

*|*: toma la salida del comando anterior y la envía como entrada al siguiente comando.

*cmd1 >> file*: el comando recibe la entrada del pipe, >> es el operador de redirección para añadir (append) la salida al final del archivo. Si el archivo no existe, se crea.

### Recursos
Teoría y guías &rarr; [AQUÍ](https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901), [AQUÍ](https://reactive.so/post/42-a-comprehensive-guide-to-pipex/), [AQUÍ](https://medium.com/@omimouni33/pipex-the-42-project-understanding-pipelines-in-c-71984b3f2103)

Tester &rarr; [AQUÍ](https://github.com/vfurmane/pipex-tester)
