#include <stdio.h>
#include "myids.h"

int main()
{
    int uid, gid;

    get_ids(&uid, &gid);
    printf("User ID is %d. Group ID is %d.\n", uid, gid);
    return 0;
}