#ifndef _CLIENT_H
#define _CLIENT_H

void single_usr_login(int sd);
void joint_usr_login(int sd);
void admin_login(int sd);
void menu_ui(int sd);
void deposit(int sd);
void withdraw(int sd);
void print_balance(int sd);
void pass_change(int sd);
void print_acc_detail(int sd);
void add_acc(int sd);
void del_acc(int sd);
void mod_acc(int sd);
void find_acc(int sd);

#endif // !_CLIENT_H_
