#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define EXIT_CODE 1
#define PRODUCTS 100
#define ING 5
#define BAR 13
#define MAX 100
#define MAX2 50
#define MAXPRICE 10

//Checks if the given file is opened successfully
FILE *fopenCheck(char *filename, char *mode) {
    FILE *f = fopen(filename, mode);
    if (f != NULL) return f;
    fprintf(stderr, "Can't open %s.\n", filename);
    exit(EXIT_CODE);
    
    return 0;
}

void barHTML(FILE *HTML, char barcode[]) {
    fprintf(HTML, "%s</span></div></body></html>", barcode);
}

//Removes commas from the end of a string
void comma(char name[], int length) {
    for (int i = 0; i < length; i++) {
        if (name[i] == ',') name[i] = '\0';
        else;
    }
}

//Checks is the given ingredient is an allergen
bool allergen(char ingredient[]) {
    if (strcmp(ingredient, "Butter,") == 0) return true;
    else if (strcmp(ingredient, "Sugar,") == 0) return true;
    else return false;
}

//Checks if the ingredient field has been left empty
bool empty(char ingredient[]) {
    if (strcmp(ingredient, ",") == 0) return true;
    else return false;
}

//Removes underscores from the prosuct name (Allows products to have multiple words in their names)
void underscore(char string[], int length) {
    for (int i = 0; i < length; i++) {
        if (string[i] == '_') string[i] = ' ';
        else;
    }
}

//Writes the ingredients to the HTML document. If the given ingredient is an
//allergen then it is written in bold and if the field has been left empty
//then it is not included in teh HTML document.
void ingHTML(FILE *HTML, char ingredient[]) {
    underscore(ingredient, strlen(ingredient));
    bool allergy = allergen(ingredient);
    bool null = empty(ingredient);
    if (null == true);
    else if (allergy == true) {
        fprintf(HTML, "<b>%s </b>", ingredient);
    }
    else fprintf(HTML, "%s ", ingredient);
}

//Calls the ingHTML function on each ingredient. Then removes the comma on the end
void ingredients(FILE *HTML, char ing1[], char ing2[], char ing3[], char ing4[], char ing5[]) {
    ingHTML(HTML, ing1);
    ingHTML(HTML, ing2);
    ingHTML(HTML, ing3);
    ingHTML(HTML, ing4);
    ingHTML(HTML, ing5);
    fseek(HTML, -2, SEEK_CUR);
    fprintf(HTML, "</p><span style=\"font-family: 'Libre Barcode 39 Extended Text'; font-size: 55px;\">");
}

//Writes the price of teh product to the HTML file
void priceHTML(FILE *HTML, char price[]) {
    int length = strlen(price);
    comma(price, length);
    fprintf(HTML, "%s</h1><p style=\"font-size: 20px\">Ingredients:</p><p style=\"font-size: 20px\">", price);
}

//Writes the name of the product to the HTML file
void name(FILE *HTML, char name[]) {
    int length = strlen(name);
    comma(name, length);
    fprintf(HTML, "%s</h1><h1>", name);
}

//Creates the HTML file for each product
void productHTML(char htmlFile[], char productName[], char price[], char ing1[], char ing2[], char ing3[], char ing4[], char ing5[], char barcode[]) {
    FILE *product = fopenCheck(htmlFile, "w");
    fprintf(product, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\" /><title>%s", productName);
    fseek(product, -1, SEEK_CUR);
    fprintf(product, "</title><style>div {text-align: center;}</style></head><body><link rel=\"preconnect\" href=\"https://fonts.googleapis.com\"><link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin><link href=\"https://fonts.googleapis.com/css2?family=Libre+Barcode+39+Extended+Text&display=swap\" rel=\"stylesheet\"><div><h1>");
    name(product, productName);
    priceHTML(product, price);
    ingredients(product, ing1, ing2, ing3, ing4, ing5);
    barHTML(product, barcode);
    fclose(product);
}

//Adds product to index.html file
void indexHTML(FILE *index, char htmlFile[], char productName[]) {
    fprintf(index, "<li><a href=\"./%s\">%s</a></li>", htmlFile, productName);
}

//Writes the HTML file for a given row of the database
void writeHTML(char productName[], char price[], char ing1[], char ing2[], char ing3[], char ing4[], char ing5[], char barcode[], FILE *index) {
    int length = strlen(productName);
    char htmlFile[length + 10];
    underscore(productName, length);
    for (int i = 0; i < length; i++) {
        if (productName[i] != ',') {
            htmlFile[i] = productName[i];
        }
        else {
            htmlFile[i] = '.';
            htmlFile[i + 1] = 'h';
            htmlFile[i + 2] = 't';
            htmlFile[i + 3] = 'm';
            htmlFile[i + 4] = 'l';
            htmlFile[i + 5] = '\0'; 
            break;
        }
    }
    //printf("%s %s", productName, htmlFile);
    productHTML(htmlFile, productName, price, ing1, ing2, ing3, ing4, ing5, barcode);
    indexHTML(index, htmlFile, productName);
}

//Closes open tags in index.html
void indexEnd(FILE *index) {
    fprintf(index, "</ul><br /><hr width=\"100%%;\" color=\"black\" size=\"2\"></Body></html>");
}

//seperates rows into individual variables
void seperate(FILE *in, FILE *index) {
    char line[MAX], productName[MAX2], price[MAXPRICE], ing[ING][MAX2], barcode[BAR];
    fgets(line, MAX, in);
    while (! feof(in)) {
        sscanf(line, "%s %s %s %s %s %s %s %s", productName, price, ing[0], ing[1], ing[2], ing[3], ing[4], barcode);
        //printf("name = %s, price = %s, ing1 = %s, ing2 = %s, ing3 = %s, ing4 = %s, ing5 = %s, bar = %s\n", productName, price, ing[0], ing[1], ing[2], ing[3], ing[4], barcode);
        writeHTML(productName, price, ing[0], ing[1], ing[2], ing[3], ing[4], barcode, index);
        fgets(line, MAX, in);
    }
    sscanf(line, "%s %s %s %s %s %s %s %s", productName, price, ing[0], ing[1], ing[2], ing[3], ing[4], barcode);
    writeHTML(productName, price, ing[0], ing[1], ing[2], ing[3], ing[4], barcode, index);
    indexEnd(index);
    fclose(in);
}

//Adds the header information and some of the body to the index.html file
void indexHeader(FILE *index) {
    fprintf(index, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\" /><style>div {text-align: center;} li {display: block;width: 20%%;float: left;}</style><title>WVB Labels</title></head><Body><div><h1>Labels:</h1></div><hr width=\"100%%;\" color=\"black\" size=\"2\"><ul>");
}

//Opens .csv file and seperates rows into individual arrays each consisting of length 4 
void processFile(char *filename) {
    FILE *label = fopenCheck(filename, "r");
    FILE *index = fopenCheck("index.html", "w");

    indexHeader(index);
    seperate(label, index);
}

int main(int n, char *args[n]) {
    processFile(args[1]);
    printf("Done\n");
}
