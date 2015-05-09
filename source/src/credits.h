#ifndef _CREDITS_H_
#define _CREDITS_H_

typedef struct {
    /**
     * @brief Callback that is called when the credits are done.
	 * @remarks It's also called if the user cancelles the credits sequence.
     * @remarks void ended();
     */
	void *ended;
} CreditsConfig;

CreditsConfig creditsConfig;

/**
 * @brief Starts the credits sequence
 */
void credits_start();

#include "credits.c"

#endif // #ifndef _CREDITS_H_