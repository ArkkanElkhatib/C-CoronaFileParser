#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Country Structure Used to Pair Cases and Name For Sorting
struct Country {
	char *name;
	unsigned int cases;
};


// Compare Function
int compare (const void * a, const void * b) {
	struct Country *countryA = (struct Country *)a;
	struct Country *countryB = (struct Country *)b;

	return (countryB->cases - countryA->cases);
	
}

int main () {
	
	setlocale(LC_NUMERIC, "");

	FILE *incomplete = NULL;
	FILE *ongoing = NULL;
	FILE *resolved = NULL;

	incomplete = fopen("incomplete.bin", "r");
	if (!incomplete) {
		printf("Error opening file\n");
		exit(-1);
	}	

	ongoing = fopen("ongoing.bin", "r");
	if (!ongoing) {
		printf("Error opening file\n");
		exit(-1);
	}

	resolved = fopen("resolved.bin", "r");
	if (!resolved) {
		printf("Error opening file\n");
		exit(-1);
	}

	unsigned short int incompleteCountriesNum;
	unsigned short int ongoingCountriesNum;
	unsigned short int resolvedCountriesNum;

	fread(&incompleteCountriesNum, sizeof(unsigned short int), 1, incomplete);
	fread(&ongoingCountriesNum, sizeof(unsigned short int), 1, ongoing);
	fread(&resolvedCountriesNum, sizeof(unsigned short int), 1, resolved);
	
	char lenCountryName;
	char lenLongestCountryName = -1;
	
	unsigned int totalCases;
	unsigned int totalDeaths;
	unsigned int totalRecov;
	unsigned int population;

	unsigned long populationWorld; 
	unsigned int totalCasesWorld;
	unsigned int totalDeathsWorld;
	unsigned int totalRecovWorld;

	char impact;

	
	// **  Reading the incomplete.bin file ** //
	printf("\nNumber of Countries in 'incomplete.bin': %d\n", incompleteCountriesNum);

	// Find longest country name in incomplete.bin
	for (int i = 0; i < incompleteCountriesNum; i++) {
		fread(&lenCountryName, sizeof(char), 1, incomplete);
		if (lenCountryName > lenLongestCountryName) {
			lenLongestCountryName = lenCountryName;
		}	
		fseek(incomplete, (sizeof(char) * lenCountryName) + (sizeof(unsigned int) * 4), SEEK_CUR);	
	}

	// Create string for storing country names
	// +1 length for the terminating character
	char countryNameIncomplete[lenLongestCountryName + 1];
	
	// Move File marker back to right after the 'unsigned short int' containing number of countries in file
	fseek(incomplete, sizeof(unsigned short int), SEEK_SET);

	// Printing Header
	printf("%30s\t\t%10s\t\t%10s\t\t%10s\t\t%10s\n", "Country", "Cases", "Deaths", "Recovered", "Population");	
	for (int i = 0; i < 130; i++) {
		printf("-");	
	}
	printf("\n");
	
	// Outputting and Computing Data
	for (unsigned short int i = 0; i < incompleteCountriesNum; i++) {
		fread(&lenCountryName, sizeof(char), 1, incomplete);	
		fread(countryNameIncomplete, sizeof(char) * lenCountryName, 1, incomplete);
		countryNameIncomplete[lenCountryName] = '\0';
		fread(&totalCases, sizeof(unsigned int), 1, incomplete);
		fread(&totalDeaths, sizeof(unsigned int), 1, incomplete);
		fread(&totalRecov, sizeof(unsigned int), 1, incomplete);
		fread(&population, sizeof(unsigned int), 1, incomplete);

		if (population != -1) {
			populationWorld += population;
		}

		if (totalCases != -1) {
			totalCasesWorld += totalCases;
		}

		if (totalDeaths != -1) {
			totalDeathsWorld += totalDeaths;
		}

		if (totalRecov != -1) {
			totalRecovWorld += totalRecov;
		}
	
		printf("%30s\t\t", countryNameIncomplete);
		totalCases == -1 ? printf("%10s\t\t", "--") : printf("%'10d\t\t", totalCases);
		totalDeaths == -1 ? printf("%10s\t\t", "--") : printf("%'10d\t\t", totalDeaths);
		totalRecov == -1 ? printf("%10s\t\t", "--") : printf("%'10d\t\t", totalRecov);
		population == -1 ? printf("%10s\n", "--") : printf("%'10d\n", population);
	}


	// ** Reading The ongoing.bin File ** //
	printf("\nNumber of Countries in 'ongoing.bin': %d\n", ongoingCountriesNum);

	// Computing longest country name in ongoing.bin
	lenLongestCountryName = -1;
	for (int i = 0; i < ongoingCountriesNum; i++) {
		fread(&lenCountryName, sizeof(char), 1, ongoing);
		if (lenCountryName > lenLongestCountryName) {
			lenLongestCountryName = lenCountryName;
		}	
		fseek(ongoing, (sizeof(char) * lenCountryName) + (sizeof(unsigned int) * 4), SEEK_CUR);	
	}
	
	char countryNameOngoing[lenLongestCountryName + 1];
	
	fseek(ongoing, sizeof(unsigned short int), SEEK_SET);

	unsigned short int countryCount = 0;

	// Allocate memory to store all the 'Country' struct to sort later
	struct Country *list = (struct Country *) malloc(sizeof(struct Country) * ongoingCountriesNum);

	printf("%30s\t\t%10s\t\t%10s\t\t%10s\t\t%10s\t\t%10s\n", "Country", "Cases", "Deaths", "Recovered", "Population", "Impact");	
	for (int i = 0; i < 148; i++) {
		printf("-");	
	}
	printf("\n");
		
	for (unsigned short int i = 0; i < ongoingCountriesNum; i++) {
		fread(&lenCountryName, sizeof(char), 1, ongoing);	
		fread(countryNameOngoing, sizeof(char) * lenCountryName, 1, ongoing);
		countryNameOngoing[lenCountryName] = '\0';
		fread(&totalCases, sizeof(unsigned int), 1, ongoing);
		fread(&totalDeaths, sizeof(unsigned int), 1, ongoing);
		fread(&totalRecov, sizeof(unsigned int), 1, ongoing);
		fread(&population, sizeof(unsigned int), 1, ongoing);
		fread(&impact, sizeof(char), 1, ongoing);

		populationWorld += population;
		totalCasesWorld += totalCases;
		totalDeathsWorld += totalDeaths;
		totalRecovWorld += totalRecov;

		// Creating a new struct to add to list
		struct Country newCountry;
		// Allocating memory for name
		newCountry.name = (char *) malloc((sizeof(char) * lenCountryName) + 1);
		strcpy(newCountry.name, countryNameOngoing);
		newCountry.cases = totalCases;	
		list[countryCount] = newCountry;
		countryCount++;
	
		printf("%30s\t\t%'10d\t\t%'10d\t\t%'10d\t\t%'10d\t\t%10c\n", countryNameOngoing, totalCases, totalDeaths, totalRecov, population, impact);
	}
	
	// ** Reading from 'resolved.bin' ** //	
	printf("\nNumber of Countries in 'resolved.bin': %d\n", resolvedCountriesNum);

	lenLongestCountryName = -1;
	for (int i = 0; i < resolvedCountriesNum; i++) {
		fread(&lenCountryName, sizeof(char), 1, resolved);
		if (lenCountryName > lenLongestCountryName) {
			lenLongestCountryName = lenCountryName;
		}	
		fseek(ongoing, (sizeof(char) * lenCountryName) + (sizeof(unsigned int) * 4), SEEK_CUR);	
	}
	
	char countryNameResolved[lenLongestCountryName + 1];
	
	fseek(resolved, sizeof(unsigned short int), SEEK_SET);

	printf("%30s\t\t%10s\t\t%10s\t\t%10s\t\t%10s\n", "Country", "Cases", "Deaths", "Recovered", "Population");	
	for (int i = 0; i < 130; i++) {
		printf("-");	
	}
	printf("\n");
		
	for (unsigned short int i = 0; i < resolvedCountriesNum; i++) {
		fread(&lenCountryName, sizeof(char), 1, resolved);	
		fread(countryNameResolved, sizeof(char) * lenCountryName, 1, resolved);
		countryNameResolved[lenCountryName] = '\0';
		fread(&totalDeaths, sizeof(unsigned int), 1, resolved);
		fread(&totalRecov, sizeof(unsigned int), 1, resolved);
		fread(&population, sizeof(unsigned int), 1, resolved);
		totalCases = totalDeaths + totalRecov;
	
		populationWorld += population;	
		totalCasesWorld += totalCases;
		totalDeathsWorld += totalDeaths;
		totalRecovWorld += totalRecov;
		
		printf("%30s\t\t%'10d\t\t%'10d\t\t%'10d\t\t%'10d\n", countryNameResolved, totalCases, totalDeaths, totalRecov, population);
	}


	printf("\nStatisics:\n");
	printf("Total World Population: %'ld\n", populationWorld);

	printf("\nTotal World Cases: %'d\n", totalCasesWorld);
	printf("\tPercentage of Population Infected: %0.2f%% \n", (totalCasesWorld / (double) populationWorld) * 100);

	printf("\nTotal World Deaths: %'d\n", totalDeathsWorld);
	printf("\tDeath Rate: %0.2f%% \n", (totalDeathsWorld / (double) totalCasesWorld) * 100); 	

	printf("\nTotal World Recovered: %'d\n", totalRecovWorld);
	printf("\tRecovery Rate: %0.2f%% \n", (totalRecovWorld / (double) totalCasesWorld) * 100);

	printf("\nOngoing Cases Worldwide: %'d\n", totalCasesWorld - totalDeathsWorld - totalRecovWorld);		
	printf("\tPercentage Of Cases Ongoing: %0.2f%% \n", ((totalCasesWorld - totalDeathsWorld - totalRecovWorld) / (double) totalCasesWorld) * 100);

	// Closing all the opened files	
	fclose(incomplete);
	fclose(ongoing);
	fclose(resolved);

	qsort(list, countryCount, sizeof(struct Country), compare);

	// Printing The Sorted List
	printf("%10s\t%10s\n", "Cases", "Country");
	for (int i = 0; i < 28; i++) {
		printf("-");
	}
	printf("\n");
	for (int i = 0; i < countryCount; i++) {
		printf("%'10d\t%10s\n", list[i].cases, list[i].name);
		free(list[i].name);
	}

	// Freeing The Allocated List
	free(list);
	return 0;
}
