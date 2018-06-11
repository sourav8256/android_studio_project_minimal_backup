#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

char* TARGET_ROOT_DIR = "/media/sourav/01D3CFEEE05FFEF0/WORK/Android/AndroidStudioProjects/alltest";
char BACKUP_ROOT_DIR[] = "/home/sourav/backup";
char* CURRENT_PROJECT_DIR = "";


int copyFile(char input[],char output[]){
	FILE *fp1, *fp2;
   char *ch;
 
   // fp1 = fopen("Sample.txt", "r");
   // fp2 = fopen("Output.txt", "w");
 
   fp1 = fopen(input, "rb");
   fp2 = fopen(output, "wb");

   while (1) {
      fread(ch,sizeof(char),1,fp1);
 
      if (feof(fp1))
         break;
      else
         fwrite(ch,sizeof(char),1,fp2);
   }
 
   fclose(fp1);
   fclose(fp2);

   return 0;
}

static void r_mkdir(const char *dir) {
        char tmp[256];
        char *p = NULL;
        size_t len;
        //S_IRWXU

        snprintf(tmp, sizeof(tmp),"%s",dir);
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        mkdir(tmp, 0777);
                        *p = '/';
                }
        mkdir(tmp, 0777);
}

char* concat_str(char *str1,char *str2,char *str3){
	char *s;
	strcpy(s,str1);
	strcat(s,str2);
	strcat(s,str3);

	return s;
}

char *replace_str(char *str, char *orig, char *rep)
{
  static char buffer[4096];
  char *p;

  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}

int check(char str[],char sub[])
{
  printf("%s\n",str);	
  int i, j=0, k;
  for(i=0; str[i]; i++)
  {
    if(str[i] == sub[j])
    {
      for(k=i, j=0; str[k] && sub[j]; j++, k++)
        if(str[k]!=sub[j])
            break;
       if(!sub[j]){
        	//printf("Substring");
        	return 1;
    	}
    }
  }
  //printf("Not a substring");
 return 0;
}

int compareEnd(char str1[],char str2[]){
	if(
		str1[strlen(str1)-1] == str2[strlen(str1)-1]
	&&
		str1[strlen(str1)-2] == str2[strlen(str1)-2]
	&&
		str1[strlen(str1)-3] == str2[strlen(str1)-3]
	)
	{
		return 1;
	} else {
		return 0;
	}
}

int copyDir(char dir[],int level){
	level++;
	struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir);


    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory %s \n",dir );
        return 0;
    }
 
    while ((de = readdir(dr)) != NULL){
    		int j;
    		
    		/*prints spaces before directory to show level*/
    		for(j=0;j<level;j++){
    			//printf("%d   ",j);
    		}

            //printf("%s\n", de->d_name);

    		
            if(strncmp(de->d_name,".",1)!=0 && de->d_type == 4){

            	printf("inside folder %s\n",de->d_name);

            	char s[500];
            	strcpy(s,dir);
            	strcat(s,"/");
            	strcat(s,de->d_name);

            	char backup_dir[500];
        		strcpy(backup_dir,replace_str(s,TARGET_ROOT_DIR,BACKUP_ROOT_DIR));
        		strcpy(backup_dir,replace_str(backup_dir,"app/src/",""));

        		r_mkdir(backup_dir);

            	//printf("main directory is %s\n",backup_dir);

            	//int result = mkdir(replace_str(s,TARGET_ROOT_DIR,BACKUP_ROOT_DIR),0777);
            	//printf("result of mkdir is %d\n",result);
            	copyDir(s,level);
            } else if (de->d_type == 8)
            {

            	printf("inside file %s\n",de->d_name);

            	char s[500];
            	strcpy(s,dir);
            	strcat(s,"/");
            	strcat(s,de->d_name);

            	/* code */
            	char file_loc[500];
            	strcpy(file_loc,dir);
            	strcat(file_loc,"/");
            	strcat(file_loc,de->d_name);

            	char target_loc[500];
        		strcpy(target_loc,replace_str(s,TARGET_ROOT_DIR,BACKUP_ROOT_DIR));
        		strcpy(target_loc,replace_str(target_loc,"app/src/",""));

        		int res = copyFile(file_loc,target_loc);

        		//printf("main directory is %s\n",target_loc);
        		//printf("rename file result %d\n",res);

            }
        }
 
    closedir(dr);    
    return 0;
}

int showDir(char dir[],int level){
	struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir);
    level++;
 
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory %s \n",dir );
        return 0;
    }
 
    while ((de = readdir(dr)) != NULL){
    		int j;
    		
    		/*prints spaces before directory to show level*/
    		for(j=0;j<level;j++){
    			//printf("   ");
    		}

    		

             //printf("%s\n", de->d_name);

            if(strncmp(de->d_name,".",1)!=0 && de->d_type == 4){
	
            	char s[500];
            	strcpy(s,dir);
            	strcat(s,"/");
            	strcat(s,de->d_name);

            	if(level == 1){
    		        //int result = mkdir(replace_str(s,TARGET_ROOT_DIR,BACKUP_ROOT_DIR),0777);
    		        CURRENT_PROJECT_DIR = de->d_name;
    			}

            	if(strcmp(de->d_name,"main") == 0 && strstr(dir,"src")!=NULL){

            		char backup_dir[500];
	        		strcpy(backup_dir,replace_str(s,TARGET_ROOT_DIR,BACKUP_ROOT_DIR));
    	    		strcpy(backup_dir,replace_str(backup_dir,"app/src/",""));
    	    		strcat(backup_dir,"/");
    	    		strcat(backup_dir,de->d_name);

	        		r_mkdir(backup_dir);

   					
            		//int result = mkdir(replace_str(s,TARGET_ROOT_DIR,str),0777);
            		copyDir(s,level);
            	} else {
            		showDir(s,level);
            	}
            }
        }
 
    closedir(dr);    
    return 0;
}



int main(void)
{ 

	

	//copyFile("rename.txt","/home/sourav/backup/main/output2.txt");

    showDir(TARGET_ROOT_DIR,0);
    //printf("%s\n",replace_str("hello world","rld","ppp"));
}
