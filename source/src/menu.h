#ifndef _MENU_H_
#define _MENU_H_

typedef struct {
    /**
     * @brief Callback that starts the game.
     * @remarks void startGame();
     */
    void *startGame;

    /**
     * @brief Callback that loads a given game.
     * @remarks void loadGame(int slot);
     */
    void *loadGame;

	/**
	 * @brief Call that starts the credits.
	 * @remarks void startCredits();
	 */
	void *startCredits;

    /**
     * @brief Callback that quits the game.
     * @remarks void quitGame();
     */
    void *quitGame;

} MenuConfig;

MenuConfig menuConfig;

/**
 * @brief Opens the menu
 *
 * Loads the menu level and activates menu logic.
 * Starts all internal menu loops attached to level entities
 * so loading another level will destroy all menu logic.
 *
 * @remarks Overrides the current level as it calls level_load internally.
 */
void menu_open();

#include "menu.c"

#endif // _MENU_H_
