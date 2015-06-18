PATH ".";
PATH "sounds";
PATH "src";
PATH "models";
PATH "models\\items";
PATH "level";
PATH "graphics";
PATH "graphics\\cursors";
PATH "graphics\\items";
PATH "graphics\\textures";
PATH "sounds\\dialog01_jcl";
PATH "sounds\\dialog05_cbabe";
PATH "sounds\\dialog02_fritz";
PATH "sounds\\dialog04_greek";
PATH "sounds\\dialog03_galep";
PATH "sounds\\monolog";
PATH "sounds\\items";

RESOURCE "samples.wrs";

WINDOW WINSTART
{
  TITLE "Utopia - The Morbius Conspiracy";
  SIZE 359,640;
  MODE IMAGE;	
  BG_COLOR RGB(0, 0, 0);
  BG_PATTERN <morbius.pcx>,OPAQUE;
  PROGRESS RGB(255, 255, 255), 14, 0, 610, 395, 30;
}