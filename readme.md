The file LabelMaker.c takes a .csv file as input and creates html files as output. The html files 
consist of an index.html which is a automatically created home page with buttons that links to the 
individual created label pages for each product.

Input:
./LabelMaker FILE.csv

LabelMaker.c can take any file type as input as long as the content of the file is formatted as such:
ProductName, Price, Ingredient 1, Ingredient 2, Ingredient 3, Ingredient 4, Ingredient 5, Barcode

Important Information:
> The maximum number of ingredients that a product can have is 5.
> The maximum length of a barcode is 12 digits.
> If a product or ingredient has a name that includes more than one word, the words must be joined 
  together with an underscore, e.g. Sea_Salt.
> The maximum size for all fields is 50 characters.
> Allergens will automatically be bolded.
> All ingredient fields must be filled. E.g. if a product only has 3 ingredients then the last two 
  fields must be left blank but should appear in the input file as , ,.

LabelMaker.c contains detailed comments to explain its functionality further. 
