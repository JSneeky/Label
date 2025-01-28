#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define EXIT_CODE 1
#define ING 10
#define BAR 13
#define MAX 100
#define MAX2 50
#define MAXPRICE 10
#define COMMA -2

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

//Removes underscores from the name (Allows products and ingredients to have multiple words in their names)
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
//Places ingredients into a 2D array for concisenss
void ingredient(FILE *HTML, char *ingredients[]) {
    for (int i = 0; i < 5; i++) {
        ingHTML(HTML, ingredients[i]);
    }
    fseek(HTML, COMMA, SEEK_CUR);
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
void productHTML(char htmlFile[], char productName[], char price[], char *ingredients[], char barcode[]) {
    FILE *product = fopenCheck(htmlFile, "w");
    fprintf(product, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\" /><title>%s", productName);
    fseek(product, -1, SEEK_CUR);
    fprintf(product, "</title><style>div {text-align: center;}</style></head><body><link rel=\"preconnect\" href=\"https://fonts.googleapis.com\"><link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin><link href=\"https://fonts.googleapis.com/css2?family=Libre+Barcode+39+Extended+Text&display=swap\" rel=\"stylesheet\"><div><h1>");
    name(product, productName);
    priceHTML(product, price);
    ingredient(product, ingredients);
    barHTML(product, barcode);
    fclose(product);
}

//Adds product to index.html file
void indexHTML(FILE *index, char htmlFile[], char productName[]) {
    fprintf(index, "<li><a href=\"./%s\"><button>%s</button></a></li>", htmlFile, productName);
}

//Writes the HTML file for a given row of the database
void writeHTML(char productName[], char price[], char *ingredients[], char barcode[], FILE *index) {
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
    productHTML(htmlFile, productName, price, ingredients, barcode);
    indexHTML(index, htmlFile, productName);
}

//Closes open tags in index.html
void indexEnd(FILE *index) {
    fprintf(index, "</ul></Body></html>");
}

//seperates collumns into individual variables
void seperate(FILE *in, FILE *index) {
    char line[MAX], productName[MAX2], price[MAXPRICE], ing[ING][MAX2], barcode[BAR];
    fgets(line, MAX, in);
    while (! feof(in)) {
        sscanf(line, "%s %s %s %s %s %s %s %s %s %s %s %s %s", productName, price, ing[0], ing[1], ing[2], ing[3], ing[4], ing[5], ing[6], ing[7], ing[8], ing[9], barcode);
        char *ingredients[] = {ing[0], ing[1], ing[2], ing[3], ing[4], ing[5], ing[6], ing[7], ing[8], ing[9], ing[10]};
        writeHTML(productName, price, ingredients, barcode, index);
        fgets(line, MAX, in);
    }
    sscanf(line, "%s %s %s %s %s %s %s %s %s %s %s %s %s", productName, price, ing[0], ing[1], ing[2], ing[3], ing[4], ing[5], ing[6], ing[7], ing[8], ing[9], barcode);
    char *ingredients[] = {ing[0], ing[1], ing[2], ing[3], ing[4], ing[5], ing[6], ing[7], ing[8], ing[9], ing[10]};
    writeHTML(productName, price, ingredients, barcode, index);
    indexEnd(index);
    fclose(in);
}

//Adds the header information and some of the body to the index.html file
void indexHeader(FILE *index) {
    fprintf(index, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\" /><style>div {text-align: center;} li {display: block;width: 20%%;float: left;} button {padding:5px 10px; color:#000; border-radius:12px; background-color:#91ab66;}</style><title>WVB Labels</title></head><body><a href=\"https://github.com/JSneeky/Label\"><button style=\"position:10px 10px;\">Github</button></a><div><h1>Labels</h1></div><hr width=\"100%%;\" color=\"black\" size=\"2\"><ul>");
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
    return 0;
}
