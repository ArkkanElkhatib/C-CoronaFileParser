#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function Declarations
char fexists(char *fileName);
unsigned int convertStringToInt(char *str);

int main() {

	// Read File 
	FILE *fid = NULL;

	// Write Files
	FILE *ongoing = NULL;
	FILE *resolved = NULL;
	FILE *incomplete = NULL;
		
	
	char *readFileName = "pandemic.tsv";

	// If 'readFileName' does not exist'	
	if (!fexists(readFileName)) {
		printf("The File, '%s', does not exist\n", readFileName);
	}

	// The file does exist
	printf("The File, '%s', does exist\n", readFileName);

	// Opening the files
	fid = fopen(readFileName, "r");
	if (!fid) {
		printf("Error: Could not open file\n");
		exit(-1);
	}

	ongoing = fopen("ongoing.bin", "w");
	if (!ongoing) {
		printf("Error: Could not open file\n");
		exit(-1);
	}

	resolved = fopen("resolved.bin", "w");
	if (!resolved) {
		printf("Error: Could not open file\n");
		exit(-1);
	}

	incomplete = fopen("incomplete.bin", "w");
	if (!incomplete) {
		printf("Error: Could not open file\n");
		exit(-1);
	}

	// Len of the line just read
	long lenLine;
		
	// Stored so we can easily skip header line during second traversal
	long afterHeader; 

	// Removing The Header Line
	fscanf(fid, "%*[^\n]\n");

	afterHeader = ftell(fid);


	// Declaring and Initializing Variable
	int longestLine = -1;
	int longestCountryName = -1;	
	int lineCount = 0;
	long beforeName;
	long afterName;
	long lenCurName;
	long lineStart;
	long lineEnd;

	while (!feof(fid)) {
		// Reads in the current number, assigns it to 'num' for debugging, reads in tab char but doesn't store it
		lineStart = ftell(fid);
		fscanf(fid, "%*d %*[\t]");

		// Stores file marker before country name, reads countries name (doesn't store)
		beforeName = ftell(fid);
		fscanf(fid, "%*[^\t]");

		// Stores file marker after country name, computes length of country name as difference
		afterName = ftell(fid);
		long lenCurName = afterName - beforeName;

		// Set the longest country name to lenCurName if it is longer
		if (lenCurName > longestCountryName) {
			longestCountryName = lenCurName;
		}

		

		// Loops until the next line and sets to null terminator character, end of file also breaks
		fscanf(fid, "%*[^\n]");

		// If the length of the current line is longest line so far, set it to longestLine
		if (lenLine > longestLine) {
			longestLine = lenLine;
		}		

		// Increment Count
		lineCount++;
		lineEnd = ftell(fid);
		lenLine = lineEnd - lineStart;	
		// Debugging output
		fscanf(fid, "\n");
	}

	// Declaring and Initializing Variables
	char countryName[longestCountryName + 1];
	char tempData[longestLine + 1];
	unsigned int totalCases;
	unsigned int totalDeaths;
	unsigned int totalRecov;
	unsigned int activeCases;
	unsigned int population;
	char temp;
	char count = 0;		
	double impactCalc;
	char impact;
	char countryNameLen;

	// Initializing Count Variables
	unsigned short int ongoingCount = 0;
	unsigned short int resolvedCount = 0;
	unsigned short int incompleteCount = 0;

	// Sets The File Marker Back To Right After Header 
	fseek(fid, afterHeader, SEEK_SET);

while (!feof(fid)) {
	
		count = 0;
		// Reading name of country
		fscanf(fid, "%*d %[^\t]", countryName);
		
		countryNameLen = strlen(countryName);		

		// ** Total Cases ** //
		// When one tab is consumed, we must be at the spot for 'Total Cases'
		while (count < 1) {
			fscanf(fid, "%c", &temp);
			if (temp == '\t') {
				count++;
			}
		}
		
		// Read next character to see what's next, and move back to before character
		fscanf(fid, "%c", &temp);
		fseek(fid, -1, SEEK_CUR);
		// If temp == '\t', then col must be empty 
		if (temp == '\t') {
			totalCases = -1;
		} else {
			// Set pointer back and read value
			fscanf(fid, "%[^\t]", tempData);
			if (strcmp(tempData, "N/A") == 0) {
				totalCases = -1;
			} else {
				totalCases = convertStringToInt(tempData);
			}
		}
		// Reset Variables
		temp = '\0';
		count = 0;

		// ** Total Deaths ** //	
		// When two tabs are consumed, we must be at the spot for 'Total Deaths'
		while (count < 2) {
			fscanf(fid, "%c", &temp);
			if (temp == '\t') {
				count++;
			}
		}

		// Read next character to see what's next, and move back to before character
		fscanf(fid, "%c", &temp);
		fseek(fid, -1, SEEK_CUR);
		// If temp == '\t', then col must be empty 
		if (temp == '\t') {
			totalDeaths = -1;
		} else {
			// Set pointer back and read value
			fscanf(fid, "%[^\t]", tempData);
			if (strcmp(tempData, "N/A") == 0) {
				totalDeaths = -1;
			} else {
				totalDeaths = convertStringToInt(tempData);
			}
		}
		// Reset Variables
		temp = '\0';
		count = 0;

		// ** Total Recovered ** // 
		// When two tabs are consumed, we must be at the spot for 'Total Recovered'
		while (count < 2) {
			fscanf(fid, "%c", &temp);
			if (temp == '\t') {
				count++;
			}
		}
		
		// Read next character to see what's next, and move back to before character
		fscanf(fid, "%c", &temp);
		fseek(fid, -1, SEEK_CUR);
		// If temp == '\t', then col must be empty 
		if (temp == '\t') {
			totalRecov = -1;
		} else {
			// Set pointer back and read value
			fscanf(fid, "%[^\t]", tempData);
			if (strcmp(tempData, "N/A") == 0) {
				totalRecov = -1;
			} else {
				totalRecov = convertStringToInt(tempData);
			}
		}
		// Reset Variables
		temp = '\0';
		count = 0;

		// When one tab is consumed, we must be at the spot for 'Active Cases'
		while (count < 1) {
			fscanf(fid, "%c", &temp);
			if (temp == '\t') {
				count++;
			}
		}
		
		// Read next character to see what's next, and move back to before character
		fscanf(fid, "%c", &temp);
		fseek(fid, -1, SEEK_CUR);
		// If temp == '\t', then col must be empty 
		if (temp == '\t') {
			activeCases = -1;
		} else {
			// Set pointer back and read value
			fscanf(fid, "%[^\t]", tempData);
			if (strcmp(tempData, "N/A") == 0) {
				activeCases = -1;
			} else {
				activeCases =  convertStringToInt(tempData);
			}
		}
		// Reset Variables
		temp = '\0';
		count = 0;


		// ** Population ** // 
		// When six tabs are consumed, we must be at the spot for 'Population'
		while (count < 6) {
			fscanf(fid, "%c", &temp);
			if (temp == '\t') {
				count++;
			}
		}
		
		// Read next character to see what's next, and move back to before character
		fscanf(fid, "%c", &temp);
		fseek(fid, -1, SEEK_CUR);
		// If temp == '\t', then col must be empty 
		if (temp == '\t') {
			population = -1;
			// Next Line
			fscanf(fid, "%*[^\n]\n");
		} else {
			// Set pointer back and read value
			fscanf(fid, "%[^\n]", tempData);
			if (strcmp(tempData, "N/A") == 0) {
				population = -1;
			} else {
				population = convertStringToInt(tempData);
			}
			// Next Line
			fscanf(fid, "\n");
		}
		// Blank Populations are read as 0
		if (population == 0) {
			population = -1;
		}
		// Reset Variables
		temp = '\0';
		count = 0;

		// Filling in missing information that we have enough data to determine
		if (totalCases == 0) {
			totalRecov = 0;
			totalDeaths = 0;
			activeCases = 0;
		} else if (totalCases == activeCases) {
			totalRecov = 0;
			totalDeaths = 0;
		} else if (totalCases == totalRecov + activeCases) {
			totalDeaths = 0;
		} else if (totalCases == totalDeaths + activeCases) {
			totalRecov = 0;	
		}
 
		// Choosing whether the data represent data for 'incomplete', 'resolved', or 'ongoing'
		if (totalCases == -1 || totalRecov == -1 || totalDeaths == -1 || population == -1) {
			// ** Writing to the incomplete file ** //
			incompleteCount++;
			// Set file marker back to the top of the file 
			rewind(incomplete);	
			// Write the updated incomplete count 
			fwrite(&incompleteCount, sizeof(unsigned short int), 1, incomplete);
			// Set file marker back to the end of the file
			fseek(incomplete, 0, SEEK_END);
			fwrite(&countryNameLen, sizeof(char), 1, incomplete);
			fwrite(countryName, sizeof(char) * countryNameLen, 1, incomplete);			
			fwrite(&totalCases, sizeof(unsigned int), 1, incomplete);
			fwrite(&totalDeaths, sizeof(unsigned int), 1, incomplete);
			fwrite(&totalRecov, sizeof(unsigned int), 1, incomplete);
			fwrite(&population, sizeof(unsigned int), 1, incomplete);
	
		} else if (totalCases == totalRecov + totalDeaths) {
			// ** Writing to the resolved file ** //
			
			// Increment the count by 1
			resolvedCount++;

			
			// Set file marker back to the top of the file
			rewind(resolved);
			
			// Write the updated country count
			fwrite(&resolvedCount, sizeof(unsigned short int), 1, resolved);
	
			// Set file marker back to the end of the file
			fseek(resolved, 0, SEEK_END);
				
			// Writing the country data (Ignoring writing of total cases as it can be computed)
			fwrite(&countryNameLen, sizeof(char), 1, resolved);
			fwrite(countryName, sizeof(char) * countryNameLen, 1, resolved);			
			fwrite(&totalDeaths, sizeof(unsigned int), 1, resolved);
			fwrite(&totalRecov, sizeof(unsigned int), 1, resolved);
			fwrite(&population, sizeof(unsigned int), 1, resolved);
		
		} else {
			// ** Writing to the ongoing file ** //
			
			// Increment the count by 1
			ongoingCount++;

			
			// Set file marker back to the top of the file
			rewind(ongoing);
			
			// Write the updated country count
			fwrite(&ongoingCount, sizeof(unsigned short int), 1, ongoing);
	
			// Set file marker back to the end of the file
			fseek(ongoing, 0, SEEK_END);
		
			impactCalc = (double) totalCases / (double) population;	
			if (impactCalc <= 0.0005) {
				impact = 'L';
			} else if (impactCalc >= 0.003) {
				impact = 'H';
			} else {
				impact = 'M';
			}
				
			// Writing the country data (Ignoring writing of total cases as it can be computed)
			fwrite(&countryNameLen, sizeof(char), 1, ongoing);
			fwrite(countryName, sizeof(char) * countryNameLen, 1, ongoing);			
			fwrite(&totalCases, sizeof(unsigned int), 1, ongoing);
			fwrite(&totalDeaths, sizeof(unsigned int), 1, ongoing);
			fwrite(&totalRecov, sizeof(unsigned int), 1, ongoing);
			fwrite(&population, sizeof(unsigned int), 1, ongoing);
			fwrite(&impact, sizeof(char), 1, ongoing);
		}
	}	

	// Debugging output
	printf("Line Count: %d\n", lineCount);
	printf("Final Counts:\n\tIncomplete: %d\n\tResolved: %d\n\tOngoing: %d\n", incompleteCount, resolvedCount, ongoingCount);

	// Closing The Reading File 
	fclose(fid);

	// Closing All The Write Files
	fclose(ongoing);
	fclose(resolved);
	fclose(incomplete);

	return 0;
}

char fexists(char *fileName) {
	// Declaring Variables
	FILE *fp;
	int rc = 0;

	// Open the file
	fp = fopen(fileName, "r");

	// If the file opened
	if (fp) {
		rc = 1;
		fclose(fp);
	}

	return (rc);
}

unsigned int convertStringToInt(char *str) {

	// Declaring and Initialzing Variable
	unsigned int number;
	char newStr[strlen(str)];

	// If the first character is a ' " ', copy the string to temp variable without quotation marks
	// Else, just copy the string over as is
	if (strncmp("\"", str, 1) == 0) {
		strncpy(newStr, str + 1, strlen(str) - 2);
		newStr[strlen(str) - 2] = '\0';
	} else {
		strcpy(newStr , str);
	}

	char curChar = 0;

	// Iterate over string, if the character is a ' , ', move all the proceeding characters forward to fill in the gap	
	while (newStr[curChar] != '\0') {
		if (newStr[curChar] == ',') {
			for (int i = 0; i < strlen(newStr); i++) {
				newStr[curChar + i] = newStr[curChar + 1 + i];
			}
		} else {
			curChar++;
		}
	}

	// Convert string to a unsigned int
	number = strtoul(newStr, 0L, 10);
	
	return number;
}
