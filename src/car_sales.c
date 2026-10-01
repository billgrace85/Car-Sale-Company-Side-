//declaring the libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

//defining the constants
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 30
#define DISCOUNT_PERCENTAGE 0.20f
#define MENU_OPTION_VIEW_CARS 'a'
#define MENU_OPTION_BUY_CAR 'b'
#define MENU_OPTION_VIEW_SALES 'c'
#define MENU_OPTION_FEEDBACK 'd'
#define MENU_OPTION_EXIT 'x'
#define MENU_OPTION_VIEW_FEEDBACK 'v'

#define CSV_FILE "billgrace.csv"
//using struct
typedef struct {
    char model[20];
    float price;
    unsigned short year;
    unsigned short stock;
} Car;

char customerNames[201];

typedef struct {
    char name[201];
    unsigned short age;
    bool discountGiven;
    float discountValue;
    unsigned short carsBought;
    unsigned short carModelBought;
    int feedbackRating;
    char feedbackComment[201];
    struct tm purchaseDate;
} Customer;
 // 5 car arryas
Car cars[] = {
        {"Lambo", 50000, 2022, 5},
        {"Mercedes", 45000, 2021, 4},
        {"RR", 40000, 2020, 3},
        {"Tesla", 60000, 2023, 2},
        {"Ford", 25000, 2019, 6}
};

Customer* customers = NULL; //  dynamically allocated customer array
unsigned short numberOfCars = 5;
unsigned short numberOfSales = 0;
unsigned short salesCapacity = 0;

//clearing the buffer
void clearBuffer() {
    while (getchar() != '\n');
}

bool isValidName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return false;
        }
    }
    return true;
}


//get the message from the user.
char getCharFromConsole(char* message) {
    char userInput;
    printf("%s", message);
    while (scanf(" %c", &userInput) != 1) {
        printf("Error : Please enter Correct Input: ");
        clearBuffer();
    }
    clearBuffer();
    return userInput;
}
//get the int value from the user
unsigned short getUnsignedShortFromConsole(char* message) {
    unsigned short userInput;
    printf("%s", message);
    while (scanf("%hu", &userInput) != 1 || userInput <= 0) {
        printf("Error: Enter a Valid Number ");
        clearBuffer();
    }
    clearBuffer();
    return userInput;
}

void getStringFromConsole(char* message, char* str) {
    // prompt user
    printf("%s", message);
    // as we're now working with the pointer str, we don't need & here
    scanf(" %[^\n]", str);
    clearBuffer();
}

//  Dynamically allocate
void ensureSalesCapacity() {
    if (numberOfSales >= salesCapacity) {
        salesCapacity = (salesCapacity == 0) ? 10 : salesCapacity * 2;
        Customer* temp = realloc(customers, salesCapacity * sizeof(Customer));
        if (temp == NULL) {
            printf("Memory allocation failed. Exiting...\n");
            exit(EXIT_FAILURE);
        }
        customers = temp;
    }
}
//reading the file
void readSalesDataFromFile() {
    FILE *file = fopen(CSV_FILE, "r");
    if (file == NULL) {
        printf("Error opening file for reading sales data: %s\n", strerror(errno));
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file);
    numberOfSales = 0;

    while (fgets(line, sizeof(line), file)) {
        Customer cust;
        char carModel[20], discountStr[10], dateStr[20];

        sscanf(line, "%200[^,],%hu,%19[^,],%9[^,],%f,%hu,%d,%200[^,],%19[^\n]",
               cust.name, &cust.age, carModel, discountStr, &cust.discountValue,
               &cust.carsBought, &cust.feedbackRating, cust.feedbackComment, dateStr);

        cust.discountGiven = strcmp(discountStr, "Yes") == 0;

        int modelFound = -1;
        for (int i = 0; i < numberOfCars; i++) {
            if (strcmp(cars[i].model, carModel) == 0) {
                modelFound = i;
                break;
            }
        }
        if (modelFound == -1) continue;

        cust.carModelBought = modelFound;

        int year, month, day;
        if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) == 3) {
            cust.purchaseDate.tm_year = year - 1900;
            cust.purchaseDate.tm_mon = month - 1;
            cust.purchaseDate.tm_mday = day;
        } else {
            continue;
        }

        ensureSalesCapacity();
        customers[numberOfSales++] = cust;
    }

    fclose(file);
}
//loading the data from file
void loadSalesDataFromFile() {
    readSalesDataFromFile();
    if (numberOfSales > 0) {
        printf("Previous sales data loaded successfully.\n");
    } else {
        printf("No previous sales data available.\n");
    }
}
//saving the data from file
void saveSalesDataToFile() {
    if (numberOfSales == 0) {
        printf("No sales data to save.\n");
        return;
    }

    FILE *file = fopen(CSV_FILE, "w");
    if (file == NULL) {
        printf("Error saving sales data.\n");
        return;
    }

    fprintf(file, "Name,Age,CarModel,DiscountGiven,DiscountValue,CarsBought,FeedbackRating,FeedbackComment,PurchaseDate\n");

    for (int i = 0; i < numberOfSales; i++) {
        fprintf(file, "%s,%hu,%s,%s,%.2f,%hu,%d,%s,%d-%02d-%02d\n",
                customers[i].name,
                customers[i].age,
                cars[customers[i].carModelBought].model,
                customers[i].discountGiven ? "Yes" : "No",
                customers[i].discountValue,
                customers[i].carsBought,
                customers[i].feedbackRating,
                customers[i].feedbackComment,
                customers[i].purchaseDate.tm_year + 1900,
                customers[i].purchaseDate.tm_mon + 1,
                customers[i].purchaseDate.tm_mday
        );
    }

    fclose(file);
    printf("Sales data saved successfully.\n");
}
 //getting feedback from the customer.
void menu_customerFeedback() {
    if (numberOfSales == 0) {
        printf("No sales made yet. Please make a purchase first.\n");
        return;
    }

    printf("Enter the sale number to leave feedback (1-%d): ", numberOfSales);
    unsigned short saleChoice = getUnsignedShortFromConsole("");

    if (saleChoice < 1 || saleChoice > numberOfSales) {
        printf("Invalid choice.\n");
        return;
    }

    saleChoice--;
    printf("Enter your feedback for SALE %d:\n", saleChoice + 1);
    printf("Rating (1-5): ");
    customers[saleChoice].feedbackRating = getUnsignedShortFromConsole("");

    while (customers[saleChoice].feedbackRating < 1 || customers[saleChoice].feedbackRating > 5) {
        printf("Invalid rating. Enter 1 to 5: ");
        customers[saleChoice].feedbackRating = getUnsignedShortFromConsole("");
    }

    printf("Feedback comment: ");
    scanf(" %[^\n]", customers[saleChoice].feedbackComment);
    clearBuffer();

    printf("Thank you for your feedback!\n");
}
//viewing the customer feedback
void view_customerFeedback() {
    if (numberOfSales == 0) {
        printf("No sales data available.\n");
        return;
    }

    printf("\nCustomer Feedback:\n");
    for (int i = 0; i < numberOfSales; i++) {
        printf("SALE %d | %s | Rating: %d | Comment: %s\n",
               i + 1,
               customers[i].name,
               customers[i].feedbackRating,
               customers[i].feedbackComment);
    }
}
//using og quick sort for desending order of years
int partition(Car cars[], int low, int high) {
    int pivot = cars[high].year;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (cars[j].year > pivot) {
            i++;
            Car temp = cars[i];
            cars[i] = cars[j];
            cars[j] = temp;
        }
    }
    Car temp = cars[i + 1];
    cars[i + 1] = cars[high];
    cars[high] = temp;
    return i + 1;
}

void quickSort(Car cars[], int low, int high) {
    if (low < high) {
        int pi = partition(cars, low, high);
        quickSort(cars, low, pi - 1);
        quickSort(cars, pi + 1, high);
    }
}
//for getting the currect data anf time
void getCurrentDate(struct tm *purchaseDate) {
    time_t currentTime = time(NULL);
    *purchaseDate = *localtime(&currentTime);
}
//view the cars stocks
void menu_viewCarsStock() {
    Car sortedCars[5];
    memcpy(sortedCars, cars, sizeof(cars));
    quickSort(sortedCars, 0, numberOfCars - 1);

    printf("\nAVAILABLE CARS STOCKS:\n");
    for (int i = 0; i < numberOfCars; i++) {
        printf("%-3d %-10s %-6s %-6d | %-8s %-2d | %-10s %-8.2fGBP\n",
               i + 1, sortedCars[i].model, "-", sortedCars[i].year,
               "Stock:", sortedCars[i].stock, "Price:", sortedCars[i].price);

    }
}
//buying the car
void menu_buyCar() {

        if (numberOfSales >= 1000) {
            printf("Max sales limit reached.\n");
            return;
        }

        Car sortedCars[5];
        int carIndexMap[5]; // Map sorted index back to original index
        for (int i = 0; i < numberOfCars; i++) {
            sortedCars[i] = cars[i];
            carIndexMap[i] = i;
        }

        // Sort sortedCars and update index mapping
        for (int i = 0; i < numberOfCars - 1; i++) {
            for (int j = i + 1; j < numberOfCars; j++) {
                if (sortedCars[i].year < sortedCars[j].year) {
                    Car tempCar = sortedCars[i];
                    sortedCars[i] = sortedCars[j];
                    sortedCars[j] = tempCar;

                    int tempIdx = carIndexMap[i];
                    carIndexMap[i] = carIndexMap[j];
                    carIndexMap[j] = tempIdx;
                }
            }
        }

        printf("Buy Car:\n");
        for (int i = 0; i < numberOfCars; i++) {
            printf("%-3d %-10s %-6s %-6d | %-8s %-2d | %-10s %-8.2fGBP\n",
                   i + 1, sortedCars[i].model, "-", sortedCars[i].year,
                   "Stock:", sortedCars[i].stock, "Price:", sortedCars[i].price);

        }

        unsigned short carChoice = getUnsignedShortFromConsole("Select a car (1-5): ");
        if (carChoice < 1 || carChoice > numberOfCars) {
            printf("Invalid car selection.\n");
            return;
        }

        int actualCarIndex = carIndexMap[carChoice - 1];

        if (cars[actualCarIndex].stock == 0) {
            printf("Out of stock.\n");
            return;
        }

        ensureSalesCapacity();

        // Get customer name and ensure it is valid
        do {
            getStringFromConsole("What is your name? Name: ", customers[numberOfSales].name);

            // Check if the name is valid (no numbers or invalid characters)
            if (!isValidName(customers[numberOfSales].name)) {
                printf("Invalid name. Please use only letters and spaces.\n");
            }
        } while (!isValidName(customers[numberOfSales].name));

        unsigned short age = getUnsignedShortFromConsole("Enter your age: ");
        unsigned short qty = getUnsignedShortFromConsole("How many cars to buy: ");

        if (qty > cars[actualCarIndex].stock) {
            printf("Insufficient stock.\n");
            return;
        }

        int rating = getUnsignedShortFromConsole("Rate our service (1-5): ");
        while (rating < 1 || rating > 5) {
            rating = getUnsignedShortFromConsole("Invalid rating. Enter 1-5: ");
        }

        customers[numberOfSales].feedbackRating = rating;
        printf("Leave a comment: ");
        scanf(" %[^\n]", customers[numberOfSales].feedbackComment);
        clearBuffer(); // Ensure the buffer is cleared after taking feedback comment.

        if (age >= DISCOUNT_MIN_AGE && age <= DISCOUNT_MAX_AGE) {
            customers[numberOfSales].discountGiven = true;
            customers[numberOfSales].discountValue = DISCOUNT_PERCENTAGE * cars[actualCarIndex].price;
            printf("Discount applied!\n");
        } else {
            customers[numberOfSales].discountGiven = false;
            customers[numberOfSales].discountValue = 0;
        }

        customers[numberOfSales].age = age;
        customers[numberOfSales].carModelBought = actualCarIndex;
        customers[numberOfSales].carsBought = qty;
        getCurrentDate(&customers[numberOfSales].purchaseDate);
        cars[actualCarIndex].stock -= qty;

        numberOfSales++;
        printf("Thank you for your purchase!\n");
        saveSalesDataToFile();
    }


//calculating the revenue
    float calculateRevenueForSale(int saleIndex) {
    unsigned short model = customers[saleIndex].carModelBought;
    float price = cars[model].price;
    float discount = customers[saleIndex].discountGiven ? customers[saleIndex].discountValue : 0;
    return customers[saleIndex].carsBought * (price - discount);
}
//sorting the revenue in descending order
void sortSalesByRevenue() {
    for (int i = 0; i < numberOfSales - 1; i++) {
        for (int j = i + 1; j < numberOfSales; j++) {
            if (calculateRevenueForSale(i) < calculateRevenueForSale(j)) {
                Customer temp = customers[i];
                customers[i] = customers[j];
                customers[j] = temp;
            }
        }
    }
}
//viewing the sales
void menu_viewSales() {
    if (numberOfSales == 0) {
        printf("No sales available.\n");
        return;
    }

    sortSalesByRevenue();
    printf("\nSales Report:\n");

    for (int i = 0; i < numberOfSales; i++) {
        float revenue = calculateRevenueForSale(i);
        printf("SALE %d || %s || Age: %hu || Bought: %hu x %s || Discount: %s | Discount Value: %.2f || Date: %d-%02d-%02d\n",
               i + 1,
               customers[i].name,
               customers[i].age,
               customers[i].carsBought,
               cars[customers[i].carModelBought].model,
               customers[i].discountGiven ? "Yes" : "No",
               customers[i].discountValue,
               customers[i].purchaseDate.tm_year + 1900,
               customers[i].purchaseDate.tm_mon + 1,
               customers[i].purchaseDate.tm_mday);
        printf("Revenue: %.2f GBP\n", revenue);
    }

}
//our main function
int main() {
    loadSalesDataFromFile();  //for loading the data

    char choice;
    do {
        printf("\n----------- MENU -----------\n");
        printf("%c. View Available Cars\n", MENU_OPTION_VIEW_CARS);
        printf("%c. Buy Car\n", MENU_OPTION_BUY_CAR);
        printf("%c. View Sales\n", MENU_OPTION_VIEW_SALES);
        printf("%c. View Feedback\n", MENU_OPTION_VIEW_FEEDBACK);
        printf("%c. Exit\n", MENU_OPTION_EXIT);
        printf("----------------------------\n");
        choice = tolower(getCharFromConsole("Enter your choice: "));

        switch (choice) {
            case MENU_OPTION_VIEW_CARS:
                menu_viewCarsStock();
                break;
            case MENU_OPTION_BUY_CAR:
                menu_buyCar();
                break;
            case MENU_OPTION_VIEW_SALES:
                menu_viewSales();
                break;
            case MENU_OPTION_VIEW_FEEDBACK:
                view_customerFeedback();
                break;
            case MENU_OPTION_EXIT:
                saveSalesDataToFile();
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while (choice != MENU_OPTION_EXIT);

    free(customers); //  Clean up dynamically allocated memory
    return 0;
}
