project.exe : client.o customer_s.o employee_s.o
    gcc client.o customer_s.o employee_s.o -o project.exe
 
client.o : client.c project_h.h
    gcc -c -g client.c
 
customer_s.o : customer_s.c project_h.h
    gcc -c -g customer_s.c
 
employee_s.o : employee_s.c project_h.h
    gcc -c -g employee_s.c

