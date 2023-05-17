#include "IMT2020129_data_structures.h"

#ifndef _SERVER_H
#define _SERVER_H

bool single_usr_auth(normalUser currUser);
bool joint_usr_auth(jointUser currUser);
bool admin_auth(admin currUser);
void server_task(int nsd);

#endif // !SERVER_H_
