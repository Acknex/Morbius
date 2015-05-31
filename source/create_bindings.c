TEXT* BINDINGS__listTxt = { strings = 10000; }
STRING* BINDINGS__curDir = "";
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

void BINDINGS_writeFiles(STRING* file)
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
		file_str_write(vHandle, BINDINGS__curDir);
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
	str_cpy(BINDINGS__curDir, directory);
	str_cpy(searchStr, directory);
	str_cat(searchStr, "\\*.");
	str_cat(searchStr, extension);

	BINDINGS__textLen = txt_for_dir(BINDINGS__listTxt, searchStr);
}

void BINDINGS_main(STRING* file, TEXT* dirs, TEXT* exts)
{
	BINDINGS_start(file);

	var i, j;
	for (i = 0; i < dirs->strings; i++)
	{
		for (j = 0; j < exts->strings; j++)
		{
			BINDINGS_readFiles((dirs->pstring)[i], (exts->pstring)[j]);
			BINDINGS_writeFiles(file);
		}
	}

	BINDINGS_stop(file);	
}


//ADJUST TO NEEDS FROM HERE ON
TEXT* dirlist = 
{ 
	string(
	"sounds\\dialog01_jcl", 
	"sounds\\dialog02_fritz", 
	"sounds\\dialog03_galep", 
	"sounds\\dialog04_greek", 
	"sounds\\dialog05_cbabe", 
	"sounds\\items", 
	"sounds\\monolog", 
	"graphics\\items",
	"models\\items",
	"shaders"
	);
}

TEXT* extlist =
{
	string(
	"ogg",
	"wav",
	"png",
	"mdl",
	"fxo"
	);
}

void main()
{
	STRING* file = "bindings.h";

	BINDINGS_main(file, dirlist, extlist);

	STRING* str = "";
	str_cpy(str, file);
	str_cat(str, " created.");
	error(str);
	wait(1);
	sys_exit("");
}