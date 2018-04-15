
/*	Lusíadas!
 *
 *	1. Generate N files, each corresponding to a stanza.
 *  PARALLEL
 *	2. At each space or \n, generate a random number r between 1 and 100.
 *	3. There is a "p" chance that a string "aaaa" will be added at each space or \n.
 *	4. If r < p, add "aaaa" at space or \n.
 *	5. Do this for all blocks and save 
 *	END PARALLEL
 *	6. Get all files together
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lusiadas.h"

int total_stanzas;
int nsic[NC + 1];

void prepare_text(int prob) {

	/***
	 *
	 * This function will read files containing the "cantos" of the
	 * Lusíadas and will save each stanza in a different file named
	 * "cantoM_stanzaN", where M is the "canto" number and
	 * N is the stanza number.
	 *
	***/

	int lnumber, number, stanza, canto; // line number, lines in stanza, stanza number
	int c;
	char line[60], stanza_filename[70], canto_filename[70];
	char new_stanza_filename[70], new_canto_filename[70];
	FILE *stanza_file, *canto_file, *new_stanza_file;

//	void create_new_stanza(int prob, FILE *sfile, char *sfilename);

	for (canto = 1; canto <= NC; canto++) {
		sprintf(canto_filename,
			"/home/gustavo/lusiadas/cantos/canto%02d.txt", canto);
		canto_file = fopen(canto_filename, "r");

		while ((c = getc(canto_file)) != EOF) {
			fscanf(canto_file, "%d\n", &stanza);
			sprintf(stanza_filename,
				"/home/gustavo/lusiadas/stanzas/old/canto%02d_stanza%04d.txt",
					canto, stanza);
			stanza_file = fopen(stanza_filename, "w");

			// create new files
			sprintf(new_stanza_filename,
				"/home/gustavo/lusiadas/stanzas/new/new_canto%02d_stanza%04d.txt",
					canto, stanza);
			new_stanza_file = fopen(new_stanza_filename, "rw");

			for (lnumber = 0; lnumber < LIS; lnumber ++) {
				fgets(line, 60, canto_file);
				fprintf(stanza_file, "%s", line);
				fprintf(new_stanza_file, "%s", line);
			}

//			create_new_stanza(prob, new_stanza_file, new_stanza_filename);

			fclose(stanza_file);
			fclose(new_stanza_file);
			total_stanzas++;
	    }

		fclose(canto_file);
		nsic[canto] = stanza;
		printf("Canto %2d: %3d stanzas\n", canto, nsic[canto]);
	}

	printf("Number of stanzas: %d\n", total_stanzas);
}

int rand_num() {

	/***
	 *
	 * This function will generate a random number between 1 and 100,
	 * which will determine if we will add a string "aaaa" at the given
	 * space between words or end of line.
	 *
	***/

	int number;

	srand(time(NULL));
	number = rand() % 100 + 1;

	return number;
}

/* USE THIS ONE
void create_new_stanza(int prob, FILE *sfile, char *sfilename) {

	int c, number;

	while ((c = getc(sfile)) != EOF) {
		if (c == ' ' || c == '\n') {
			number = rand_num();
			if (number <= prob) {
				if (c == ' ')
					fprintf(sfile, " aaaa ");
				else if (c == '\n')
					fprintf(sfile, " aaaa\n");
				printf("%s\n", sfilename);
			}
		}
	}
}

*/

/*
void create_new_stanza(int prob) {

	int number, c, stanza, canto;
	char new_stanza_filename[70];
	FILE *new_stanza_file;

	// get old stanza (they've already been copied to new_stanza, so we will work on these)
	for (canto = 1; canto <= NC; canto++) {
		for (stanza = 1; stanza <= nsic[canto]; stanza++) {
			sprintf(new_stanza_filename,
				"/home/gustavo/lusiadas/stanzas/new/new_canto%02d_stanza%04d.txt",
					canto, stanza);
			new_stanza_file = fopen(new_stanza_filename, "w");

			// read old_stanza and see if we will add "aaaa"
			while ((c = getc(new_stanza_file)) != EOF) {
				if (c == " " || c == "\n") {
					number = rand_num();
					if (number < prob) {
						if (c == " ")
							fprintf(new_stanza_file, " aaaa ");
						else if (c == "\n")
							fprintf(new_stanza_file, " aaaa\n");
					}
				}
			}
		}
	}
}
*/

int main()
{

	int prob; // probability of generating a string "aaaa" at spaces and \n. Must be between 1 and 100
	extern int total_stanzas;
	extern int nsic[NC + 1];

	total_stanzas = 0;

	prob = 80;
	if (prob < 1 || prob > 100) {
		printf("You put prob = %d, but it must be between 1 and 100.\n", prob);
		printf("(yes, there's always a chance!)\n");
		return (1);
	}

	prepare_text(prob);

//	create_new_stanza(prob);

	return 0;
}
