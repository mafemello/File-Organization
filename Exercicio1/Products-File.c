//Program that register products in a binary file.

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char name [50];
	char brand [50];
	char ean13 [13]; // Product's code
	float price;
} Product;

int main (void) {

	FILE* fp; // Pointer that indicates the file
	Product* some_product = (Product*) malloc(sizeof(Product));
	Product* content = (Product*)malloc(sizeof(Product));
	int op; // Menu
	int num_products = 0; // Number of products that are going to be added. Chosen by user.

	// Opening the file to read and write.
	fp = fopen ("products.txt", "w+");

	// Initializing the MENU
	do {		
		printf ("Choose what you want to do: \n");
		printf ("Press (1) to record data of many products. \nPress (2) to recover the data of all products.\nPress (3) to leave.\n");
		scanf ("%d", &op);
		
		switch (op) {
			case 1: 
				printf ("\nOk, you want to save new products in the file.\nHow many products do you want to add?\n");
				scanf ("%d", &num_products);

				for (int i = 0; i < num_products; i ++) {
					printf ("What's the name of product number %d?\n", i+1);
					scanf ("%s", some_product->name);
					fwrite (some_product->name, sizeof(char), 50, fp);

					printf ("What's the brand of product number %d?\n", i+1);
					scanf ("%s", some_product->brand);
					fwrite (some_product->brand, sizeof(char), 50, fp);

					printf ("What's the code of product number %d?\n", i+1);
					scanf ("%s", some_product->ean13);
					fwrite (some_product->ean13, sizeof(char), 13, fp);

					printf ("What's the price of product number %d?\n", i+1);
					scanf ("%f", &some_product->price);
					fwrite (&some_product->price, sizeof(float), 1, fp);
					printf("\n");
				}
				break;


			case 2:
				// Sets the pointer to the beginning of the file so it can print everything.
				fseek (fp, 0, SEEK_SET);
				
				if (num_products == 0) {
					printf ("\nError. The file is empty. You have to add products\n\n");
				}

				printf ("\nLet's see what is in the file: \n");
				while (fread (content->name, sizeof(char), 50, fp) != 0) {	
					printf ("Name: \t");
					printf ("%s\n", content->name);
					
					printf ("Brand: \t");
					fread (content->brand, sizeof(char), 50, fp);
					printf ("%s\n", content->brand);
					
					printf ("Code: \t");
					fread (content->ean13, sizeof(char), 13, fp);
					printf ("%s\n", content->ean13);
					
					printf ("Price: \t");
					fread (&content->price, sizeof(float), 1, fp);
					printf ("%.4f\n", content->price);
					printf("\n");

				}
					printf("\n");
				break;

			case 3:
				printf ("Program finished.\n");
				break;
			default:
				printf ("\nIncorrect entry. Please try again.\n\n");
				break;
			}

	} while (op != 3);


	// Closes the file
	fclose (fp);

	return 0;
}
