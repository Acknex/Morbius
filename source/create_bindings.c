TEXT* BINDINGS__listTxt = { strings = 10000; }
var BINDINGS__textLen = 0;

void BINDINGS_start(STRING* file)
{
	var vHandle = file_open_write(file);
	if (vHandle != NULL)
	{
		file_str_write(vHandle, "#ifndef BINDINGS_H\r\n");
		file_str_write(vHandle, "#define BINDINGS_H\r\n");
		file_str_write(vHandle, "\r\n/* This file is auto generated. DO NOT MODIFY MANUALLY! */\r\n");
		file_close(vHandle);
	}
}

void BINDINGS_stop(STRING* file)
{
	var vHandle = file_open_append(file);
	if (vHandle != NULL)
	{
		file_str_write(vHandle, "\r\n#endif");
		file_close(vHandle);
	}
}

void BINDINGS_writeFiles(STRING* file, STRING* directory)
{
	var i = 0;
	STRING* str = "";
	
	if (BINDINGS__textLen == 0)
	{
		//error("No files found. Skipping write process...");
		return;
	}
	
	var vHandle = file_open_append(file);
	if (vHandle != NULL)
	{
		file_str_write(vHandle, "\r\n/*-----------------------------*/\r\n");
		file_str_write(vHandle, "/* ");
		file_str_write(vHandle, directory);
		file_str_write(vHandle, " */\r\n");
		file_str_write(vHandle, "/*-----------------------------*/\r\n");
		for (i = 0; i < BINDINGS__textLen; i++)
		{
			str_cpy(str, "#define PRAGMA_BIND \"");
			str_cat(str, (BINDINGS__listTxt->pstring)[i]);
			str_cat(str, "\"\r\n");
			file_str_write(vHandle, str);
		}

		file_close(vHandle);
	}
}

void BINDINGS_readFiles(STRING* directory, STRING* extension)
{
	STRING* searchStr = "";
	str_cpy(searchStr, directory);
	str_cat(searchStr, "\\*.");
	str_cat(searchStr, extension);

	BINDINGS__textLen = txt_for_dir(BINDINGS__listTxt, searchStr);
}

TEXT* dirlist = 
{ 
	string("sounds\\dialog01_jcl","sounds\\dialog05_cbabe", "sounds\\dialog02_fritz", "sounds\\dialog04_griechin", "sounds\\dialog03_galep", "sounds\\items", "sounds", "graphics\\items");
}

void main()
{
	STRING* file = "bindings.h";
	STRING* ext1 = "ogg";
	STRING* ext2 = "wav";
	STRING* ext3 = "tga";

	BINDINGS_start(file);
	var i;
	for (i = 0; i < dirlist->strings; i++)
	{
		BINDINGS_readFiles((dirlist->pstring)[i], ext1);
		BINDINGS_writeFiles(file, (dirlist->pstring)[i]);
		BINDINGS_readFiles((dirlist->pstring)[i], ext2);
		BINDINGS_writeFiles(file, (dirlist->pstring)[i]);
		BINDINGS_readFiles((dirlist->pstring)[i], ext3);
		BINDINGS_writeFiles(file, (dirlist->pstring)[i]);
	}
	BINDINGS_stop(file);	
	

	STRING* str = "";
	str_cpy(str, file);
	str_cat(str, " created.");
	error(str);
	wait(1);
	sys_exit("");
}