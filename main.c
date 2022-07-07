#include <stdio.h>
#include <string.h>

struct Book{
    int ID;
    char title[40];
    double price;
};

void print(const struct Book *book);

void print_n(const struct Book books[], int n);

int to_string(char *buffer, int buffersize, const struct Book *book);

int from_string(const char *buffer, struct Book *book);

int save(const char *filename, const struct Book *book);

int load(const char *filename, struct Book *book);

int save_n(const char *filename, const struct Book books[], int n);

int load_n(const char *filename, struct Book books[], int n);

void apply(struct Book books[], int n, void (*transform)(struct Book *));

void add_inflation_2021(struct Book *b); // increase price by 3.6%

void add_inflation_2022(struct Book *b); // increase price by 12.0%

int main(int argc, char *argv[]){
    struct Book book = {104, "Programming in C++", 10.99};
    struct Book books[] = {
        {133,"Programming in C++",19.99},
        {324,"Programming in C",29.99},
        {532,"Programming in D",39.99},
        {521,"Programming in Python",49.99},
        {104,"Programming in C#",59.99}
    };

    char buffer[100] ="\0";

    print(&book);
    print_n(books, 5);

    to_string(buffer, 100, &books[4]);

    printf("%s\n", buffer);

    save_n("test.txt", books, 5);

    return 0;
}

void add_inflation_2021(struct Book *book){
    book -> price *= 1.036;
}

void add_inflation_2022(struct Book *book){
    book -> price *= 1.12;
}

void apply(struct Book books[], int n, void (*transform)(struct Book *)){
    for(int i = 0; i < n; i++){
        transform(&books[i]);
    }
}

int load_n(const char *filename, struct Book books[], int n){
    int check = 0;
    for(int i = 0; i < n; i++){
        check = load(filename,&books[i]);
        if(check != 0){
            return check;
        }
    }
    return check;
}

int load(const char *filename, struct Book *book){
    FILE *file;
    char buffer[70];
    if(0!=(file = fopen(filename, "r"))){
        if(NULL != fgets(buffer,70,file)){
            from_string(buffer, book);
            fclose(file);
            return 0;
        } else {
            fclose(file);
            return 1;
        }
    } else return 1;
}

int save_n(const char *filename, const struct Book books[], int n){
    int check = 0;
    for(int i = 0; i < n; i++){
        check = save(filename,&books[i]);
        if(check != 0){
            return check;
        }
    }
    return check;
}

int save(const char *filename, const struct Book *book){
    FILE *file;
    if(0!=(file = fopen(filename,"a"))){
        char buffer[70] = "\0";
        if (0 == to_string(buffer,70,book)){
            strcat(buffer,"\n");
            fputs(buffer,file);
            fclose(file);
        } else {
            fclose(file);
            return 1;
        }
    } else {
        return 1;
    }
}

void print(const struct Book *book){
    printf("Book = [ ID=%d, title=\"%s\", price=%.2lf ]\n", book->ID,book->title,book->price);
}

void print_n(const struct Book books[], int n){
    for(int i = 0; i < n; i++){
        print(&books[i]);
    }
}

int to_string(char *buffer, int buffersize, const struct Book *book)
    //0 if no errors have occurred
    //1 if errors have occurred
{
    char tmp[100];
    int i = 0;
    int n = 0;
    int counter = 0;
    while (i < 3)
    {
        switch (i)
        {
        case 0:
            n = sprintf(tmp, "%d", book->ID);
            if (0 == n || n + 1 >  buffersize || 0 >= book->ID){
                printf("An error has occurred 1\n");
                return 1;
            } else {
                strcat(buffer, tmp);
                counter += n;
                *(buffer + counter) = ';';
            }
            break;
        case 1:
            n = sprintf(tmp, "%s", book->title);
            if (0 == n || n + counter + 1 > buffersize){
                printf("An error has occurred 2\n");
                return 1;
            } else {
                strcat(buffer, tmp);
                counter += n;
                *(buffer + counter+1) = ';';
            }
            break;
        case 2:
            n = sprintf(tmp, "%.2lf", book->price);
            if (0 == n || n + counter + 1 > buffersize || 0 > book->price){
                printf("An error has occurred 3\n");
                return 1;
            } else {
                strcat(buffer, tmp);
            }
            break;
        }
        i++;
    }    
    return 0;
}

int from_string(const char *buffer, struct Book *book){
    int id, check;
    double price;
    char title[40];

    check = scanf(buffer,"%i;%39[^;];%lf", &id,title,&price);

    if (check == 3){
        if (0 <= id){
            book->ID = id;
        } else return 1;
        if (0 != strcmp(title,"\0")){
            strcpy(book->title, title);
        } else return 1;
        if (0 <= price){
            book -> price = price;
        } else return 1;
    } else return 1;
}