
/******************************************************************************
 *
 *	Lusíadas!
 *
 *	1. Generate N files, each corresponding to a stanza. DONE
 *	PARALLEL (TO DO)
 *	2. At each whitespace, generate a random number r between 1 and 100
 *	3. There is a "p" chance that a string "aaaa" will be added at each whitespace
 *	4. If r < p, add "aaaa" at whitespace
 *	5. Do this for all blocks and save 
 *	END PARALLEL
 *	6. Get files together and reconstruct Os Lusíadas
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lusiadas.h"

int prob;
int total_stanzas;
int nsic[NC + 1];

void prepare_files() {

	/***
	 *
	 * This function will read files containing the "cantos" of the
	 * Lusíadas and will save a copy of each stanza in a different
	 * file named "/path/to/old/cantoM_stanzaN.txt", where M is the
	 * "canto" number and N is the stanza number.
	 *
	 * It will also save another copy of the files in
	 * "/path/to/new/new_cantoM_stanzaN.txt".
	 * 
	***/

	int lnumber, stanza, canto;
	int c;
	char line[80];
	char stanza_filename[70], canto_filename[70], new_stanza_filename[70];
	FILE *stanza_file, *canto_file, *new_stanza_file;

//	void old_create_new_stanza(FILE *sfile, char sfname[70]);

	for (canto = 0; canto < NC; canto++) {
		sprintf(canto_filename,
			"/home/gustavo/lusiadas/cantos/canto%02d.txt", canto + 1);
		canto_file = fopen(canto_filename, "r");

		while ((c = getc(canto_file)) != EOF) {
//		while (!feof(canto_file)) {
			fscanf(canto_file, "%d\n", &stanza);
			sprintf(stanza_filename,
				"/home/gustavo/lusiadas/stanzas/old/canto%02d_stanza%04d.txt",
					canto + 1, stanza);
			stanza_file = fopen(stanza_filename, "w");

			// create new files
			sprintf(new_stanza_filename,
				"/home/gustavo/lusiadas/stanzas/new/new_canto%02d_stanza%04d.txt",
					canto + 1, stanza);
			new_stanza_file = fopen(new_stanza_filename, "w+");

			for (lnumber = 0; lnumber < LIS; lnumber ++) {
				fgets(line, 80, canto_file);
				fprintf(stanza_file, "%s", line);
				fprintf(new_stanza_file, "%s", line);
			}

//			old_create_new_stanza(new_stanza_file, new_stanza_filename);

			fclose(stanza_file);
			fclose(new_stanza_file);
		}

		fclose(canto_file);
		nsic[canto] = stanza;
		total_stanzas += nsic[canto];
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

	number = rand() % 100 + 1;

	return number;
}

/*

	// This fails because it will overwrite whatever is after ' '
	// Must write a function that reads line by line and replaces


void old_create_new_stanza(FILE *sfile) {

	int c, number;
	
	fseek(sfile, 0, SEEK_SET);
	while ((c = getc(sfile)) != EOF) {
		if (c == ' ') {
			number = rand_num();
			if (number <= prob) {
				fprintf(sfile, "aaaa ");
			}
		}
	}
}
*/

void create_new_stanza() {

	/***
	 *
	 * taken from here:
	 * http://see-programming.blogspot.com.br/2013/07/c-program-to-replace-word-in-file.html
	 * 
	 * Must fix probability.
	 *
	***/

	FILE *sf, *fp2;
	char sfname[100],  temp[] = "temp.txt";
	char line[100];
	char space[] = " ", replace[] = ", aaaaa, ";
	char *ptr1, *ptr2;
	
	int canto, stanza, number;

	for (canto = 0; canto < NC; canto++) {
		for (stanza = 0; stanza < nsic[canto]; stanza++) {
			// get file
			sprintf(sfname,
				"/home/gustavo/lusiadas/stanzas/new/new_canto%02d_stanza%04d.txt",
					canto + 1, stanza + 1);
			sf = fopen(sfname, "r");
			fp2 = fopen(temp, "w");

			while (!feof(sf)) {
				strcpy(line, "\0");

				// read line by line from the input file
				fgets(line, 100, sf);

				if (strstr(line, space)) {
					number = rand_num();
					if (number <= prob) {
						ptr2 = line;
						while (ptr1 = strstr(ptr2, space)) {

							// letters present before the word to be replaced
							while (ptr2 != ptr1) {
								fputc(*ptr2, fp2);
								ptr2++;
							}

							// skip the word to be replaced
							ptr1 += strlen(space);
							fprintf(fp2, "%s", replace);
							ptr2 = ptr1;
						}
	
						// characters present after the word to be replaced
						while (*ptr2 != '\0') {
							fputc(*ptr2, fp2);
							ptr2++;
						}
					} else {
						fputs(line, fp2);
					}


				} else {
					// current scanned line doesn't have the word to be replaced
					fputs(line, fp2);
				}
			}

			// close the opened files
			fclose(sf);
			fclose(fp2);

			// remove the input file
			remove(sfname);
			// rename temporary file name to input file name
			rename(temp, sfname);
		}
	}
}

void create_new_canto() {

}


int main()
{
	extern int prob;
	extern int total_stanzas;
	extern int nsic[NC + 1];

	srand(time(NULL));
	total_stanzas = 0;

	prob = 7;
	if (prob < 1 || prob > 100) {
		printf("You put prob = %d, but it must be between 1 and 100.\n", prob);
		printf("(yes, there's always a chance!)\n");
		return (1);
	}

	prepare_files();
	create_new_stanza();
//	create_new_canto();
//	create_new_lusiadas();

	return 0;
}
