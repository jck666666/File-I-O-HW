#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

bool g_find = false;

void GetSpecifiedFile(const char *path, char *name)
 {

     DIR *pDir;
     struct dirent* pEntry = NULL;
     char nextPath[512];

     if (!(pDir = opendir(path)))
         return;  

     while ((pEntry = readdir(pDir)) != NULL)
     {
         if ((strcmp(pEntry->d_name, ".") == 0) || (strcmp(pEntry->d_name, "..") == 0))
		continue;

         if (pEntry->d_type == DT_REG)
         {
             if (strncmp(pEntry->d_name, name, sizeof(name)) == 0)
             {
                 g_find = true;
                 printf("%s/%s\n", path, pEntry->d_name);
             }
         }
         else if (pEntry->d_type == DT_DIR)
         {
	     if ((strlen(path)<2) && (strncmp(path, "/", 1)==0))
	     {
		 sprintf(nextPath, "/%s", pEntry->d_name);
	     }
	     else
		 sprintf(nextPath, "%s/%s", path, pEntry->d_name);

             GetSpecifiedFile(nextPath, name);
         }
     }

     closedir(pDir);
 }


int main(int argc, char *argv[])
 {
    char *topdir, *file, pwd[2]= "."; 
    if (argc != 3) 
    {
        topdir=pwd;
        file = argv[1];
    }    
    else 
    {
        topdir=argv[1];
        file = argv[2]; 
    }
       

    printf( "from path: %s\n",topdir); 
    printf( "scan file: %s\n",file); 

    GetSpecifiedFile(topdir, file);

    if (!g_find)
    {
        printf("cannot find the corresponding file");
    }

    return 0;
 }
