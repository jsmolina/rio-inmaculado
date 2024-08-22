#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h> 

// Code based on mills32 Little-Game-Engine-for-VGA-EGA

struct dirent *direntry;
struct stat s;
unsigned char nfiles = 0;
DIR *folder;

int check_ext(unsigned char *name){
	int type = 0;
	int len = strlen((const char*)name);
	unsigned long *ext = (unsigned long*) &name[len-4];
	switch (ext[0]){
		//file extensions
		case 0x504D422E:
		case 0x706D622E: printf("A BMP\n"); type = 1; break;
		case 0x584D542E:
		case 0x786D742E: printf("A TMX\n"); type = 2; break;
		case 0x4D47562E:
		case 0x6D67762E: printf("A VGM\n"); type = 3; break;
	}
	return type;
}


int main(int argc, char **argv) {
	unsigned short filesize = 0x0000;
	unsigned long dat_offset = 32 + 2048;
	unsigned long file_pos = 0;
    unsigned char i, arg;
	unsigned char ssize = 0;
	unsigned char *extract_file;
	unsigned char number = argc-1;
    FILE *dat;
	FILE *file;
	unsigned char *buffer = (unsigned char*) calloc(1,65535);
	unsigned char is_folder = 0;
	
    printf("- Static files compacter (based on Mills32 code) starting...");
	if (!argv[1]) {printf("\nusage\n- Create dat: makedat file1 file2 ...\n- List dat: makedat -l FILE.DAT\n"); return 0;}
	if (argc > 64) {printf("\nOnly 64 files are supported\n"); return 0;}

    extract_file = (unsigned char*)argv[1];
	if ((extract_file[0] == '-')&&(extract_file[1] == 'l')) {
		printf("\nFiles in dat:\n"); 
		unsigned char *name = (unsigned char*)argv[2];
		if (!argv[2]) {printf("\nSpecify a DAT file: -e FILE.DAT\n"); return 1;}
		dat = fopen((const char*)name,"rb");
        if (!dat) {printf("Can't open the file %s!\n", name); return 1;}
		fseek(dat, 22, SEEK_SET); 
		fread(&number,1,1,dat);
		for (i=1; i<number+1; i++) {
			int type = 0;
			fseek(dat, 32*i, SEEK_SET);
			memset(buffer,0,16);
			//Read file name
			fread(buffer,1,16,dat);
			printf("- %s\n",buffer);
        }
        fclose(dat);
        return 0;
	}

	printf("\n");
    //Is it a folder?
    stat(argv[1],&s);
    if (s.st_mode & S_IFDIR) is_folder = 1;
    
    //CREATE FILE
    dat = fopen("DATA.DAT","wb");
    
    fprintf(dat,"LDAT  LT-ENGINE FILES:          ");
    fseek(dat, 22, SEEK_SET); fwrite(&number,1,1,dat);

    if (!is_folder){
        for (arg=1; arg<argc; arg++) {
            int i = 0;
            int namepos = 0;
            int type = 0;
            int index = 0;
            char line[128];
            unsigned char start_map_data = 0;
            unsigned char *name = (unsigned char*)argv[arg];
            //if dropped file (windows) remove path
            for (i = 0; i < strlen((const char*)name);i++){if (name[i] == 0x5C) namepos = i+1;}
            memmove(&name[0],&name[namepos],16);
            //Print file name
            fseek(dat, arg*32, SEEK_SET);
            fprintf(dat,"%s",name);
            if ((strlen((const char*)name))>12){printf("\nCan't use names bigger than 8 characters %s\n",name); return 0;}
            //Check file type
            type = check_ext(name);
            //if (!type){printf("file format not recognized %s\n",name);return 0;}
            //Read file to insert
            file = fopen((const char*)name,"rb");
            if (!file) {printf("\nCan't find %s\n",name); return 0;}
            fseek(file, 0, SEEK_END);
            filesize = ftell(file);
            rewind(file);
            fread(buffer,filesize,1,file);
            //Write file offset in data, and file size
            file_pos = dat_offset;
            fseek(dat, (arg*32)+16, SEEK_SET);
            fwrite(&file_pos,4,1,dat);
            fwrite(&filesize,2,1,dat);
            
            //Write actual file
            fseek(dat,file_pos, SEEK_SET);
            fwrite(buffer,filesize,1,dat);
            
            dat_offset += filesize;
            
            fclose(file);
        }
    } else {
        //READ FOLDER
        chdir(argv[1]);
        folder = opendir(".");
        nfiles = 1;
        while((direntry = readdir(folder)) != NULL){
            stat(direntry->d_name,&s);
            if (!(s.st_mode & S_IFDIR)) {
                int i = 0;
                int namepos = 0;
                int type = 0;
                int index = 0;
                char line[128];
                unsigned char start_map_data = 0;
                unsigned char *name = (unsigned char*)direntry->d_name;
                //if dropped file (windows) remove path
                for (i = 0; i < strlen((const char*)name);i++){
                    if (name[i] == 0x5C) namepos = i+1;
                }
                memmove(&name[0],&name[namepos],16);
                //Print file name
                fseek(dat, nfiles*32, SEEK_SET);
                fprintf(dat,"%s",name);
                if ((strlen((const char*)name))>12){printf("\nCan't use names bigger than 8 characters %s\n",name); return 0;}
                //Check file type
                type = check_ext(name);
                //if (!type){printf("file format not recognized %s\n",name);return 0;}
                //Read file to insert
                file = fopen((const char*)name,"rb");
                if (!file) {printf("\nCan't find %s\n",name); return 0;}
                switch (type){
                    case 0:
                    case 1:
                    case 3://BMP VGM or unknown
                        fseek(file, 0, SEEK_END);
                        filesize = ftell(file);
                        rewind(file);
                        fread(buffer,filesize,1,file);
                    break;
                    case 2://store tmx maps more efficiently
                        while(!start_map_data){	//read lines 
                            memset(line, 0, 64);
                            fgets(line, 64, file);
                            if((line[2] == '<')&&(line[3] == 'd')) start_map_data = 1;//<data encoding="csv">
                        }
                        start_map_data = 0;
                        filesize = ftell(file);
                        //Read header
                        fseek(file, 0, SEEK_SET);
                        fread(buffer,filesize,1,file);
                        //read map in bytes
                        for (index = 0; index < 256*19; index++){
                            unsigned char tile = 0;
                            fscanf(file, "%c,",&tile);if (!tile) tile++;
                            buffer[filesize++] = tile; 
                        }
                        //advance to bkg map
                        while(!start_map_data){	//read lines 
                            memset(line, 0, 64);
                            fgets(line, 64, file);
                            if((line[2] == '<')&&(line[3] == 'd')) start_map_data = 1;//<data encoding="csv">
                        }
                        //paste lines
                        sprintf((char*)&buffer[filesize],"\n</data>\n </layer>\n <layer id=\"2\" name=\"col\" width=\"256\" height=\"19\">\n  <data encoding=\"csv\">\n");
                        filesize+= 94;
                        //read bkg map in bytes
                        for (index = 0; index < 256*19; index++){
                            unsigned char tile = 0;
                            fscanf(file, "%c,",&tile);if (!tile) tile++;
                            buffer[filesize++] = tile; 
                        }
                        //paste end
                        sprintf((char*)&buffer[filesize],"\n</data>\n </layer>\n</map>\n");
                        filesize+= 26;
                    break;
                }
                
                file_pos = dat_offset;
                //Write file offset in data, and file size
                fseek(dat, (nfiles*32)+16, SEEK_SET);
                fwrite(&file_pos,sizeof(dat_offset),1,dat);
                fwrite(&filesize,sizeof(filesize),1,dat);
                
                //Write actual file
                fseek(dat,file_pos, SEEK_SET);
                fwrite(buffer,filesize,1,dat);
                
                dat_offset += filesize;
                nfiles++;
            }
        }
    }
    fclose(dat);

	printf("- DATA.DAT CREATED\n");
	free(buffer);
	buffer = NULL;
    return 0;
}