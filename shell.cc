#include "shell.hh"

int main()
{ std::string input;
    std::string prompt = ""; // De string die wordt uitgeprint.
    int fd = syscall(SYS_open, "prompt.txt", O_RDONLY, 0755); // open; bestandsnaam; read only; permisions.

    // maakt een variable byte datatype char van de grootte 1 byte. 
    char byte[1]; 

    // aanroep syscal read; leest vanuit fd (is al open); wat je leest schrijf je naar byte; 1 bepaalt hoeveel in een keer wordt gelezen.
    while(syscall(SYS_read, fd, byte, 1)){
        prompt.append(byte);
    } 

    while(true){ 
        std::cout << prompt;                   // Print het prompt
        std::getline(std::cin, input);         // Lees een regel
        if (input == "new_file") new_file();   // Kies de functie
        else if (input == "ls") list();        //   op basis van
        else if (input == "src") src();        //   de invoer
        else if (input == "find") find();
        else if (input == "seek") seek();
        else if (input == "exit") return 0;
        else if (input == "quit") return 0;
        else if (input == "error") return 1;

        if (std::cin.eof()) return 0; 
    }
}      // EOF is een exit

void new_file(){ 
    std::string bestandsnaam = "";
    std::string tekst = "";
    std::string regel = "";

    std::cout << "Bestandsnaam: ";
    std::getline(std::cin, bestandsnaam);

    // const voor geen aangebrachte veranderingen. * = pointer: c_str zet om.
    const char* bestand[] = {bestandsnaam.c_str()};

    // Maakt een bestand aan met naam bestandsnaam
    int fd = syscall(SYS_creat, bestand[0], 0755);

    std::cout << "Tekst: (EOF om af te sluiten)";

    while(std::getline(std::cin, tekst)){
        if(tekst == "<EOF>"){
            break;
        }
        else{
            regel += tekst + "\n";
        }
    }
    const char* nieuwe_regel[] = {regel.c_str()};
    syscall(SYS_write, fd, nieuwe_regel[0], regel.size());
}

void list(){
    // Maakt een kopie van het proeces
    if(syscall(SYS_fork) == 0){ 
        const char* command [] = {"/bin/ls", "-l", "-a", NULL};
        syscall(SYS_execve, command[0], command, NULL); // voert ls -la uit in de kopie.
    }
    else{
        syscall(SYS_wait4, 0, NULL, NULL, NULL); // Wacht tot de fork overleidt
    }
}

void find(){
    
    // input vanuit de commandline
    std::cout << "Input: ";
    std::string find_char = "";
    std::getline(std::cin, find_char);
    
    // Pipe setup
    int fd[2]; // Hoeveel pipes
    syscall(SYS_pipe, &fd);
    
    if(syscall(SYS_fork) == 0){
        
        // Koppeling uitvoer huidig proces aan invoer pipe
        syscall(SYS_close, fd[0]);
        syscall(SYS_dup2, fd[1], 1);
        
        const char* command1[] = {"/usr/bin/find", ".", NULL};
        syscall(SYS_execve, command1[0], command1, NULL);
    }
    else{
        syscall(SYS_wait4, 0, NULL, NULL, NULL);
    }

    if(syscall(SYS_fork) == 0){
        
        // Koppeling uitvoer pipe aan invoer huidige proces
        syscall(SYS_close, fd[1]);
        syscall(SYS_dup2, fd[0], 0);
    
        const char* command2[] = {"/bin/grep", find_char.c_str(), NULL};
        syscall(SYS_execve, command2[0], command2, NULL);
    }
    else{
        syscall(SYS_close, fd[0]);
        syscall(SYS_close, fd[1]);
        syscall(SYS_wait4, 0, NULL, NULL, NULL);
    }
}

void seek(){ // ToDo: Implementeer volgens specificatie.
    int seek = syscall(SYS_creat, "seek", 0755);
    int loop = syscall(SYS_creat, "loop", 0755);

    char teken1[1] = {'x'};
    char teken2[1] = {'\0'};

    syscall(SYS_write, seek, teken1, 1);
    syscall(SYS_lseek, seek, 5000000, 1);
    syscall(SYS_write, seek, teken1, 1);

    syscall(SYS_write, loop, teken1, 1);

    for(unsigned int i=0; i < 5000000; i++){
        syscall(SYS_write, loop, teken2, 1);
    }

    syscall(SYS_write, loop, teken1, 1);
}

void src(){ // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
    int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
    char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
    while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
        std::cout << byte; 
}                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
