void print_menu() {
    printf("Hole-fitting Algorithms")
    printf("-----------------------")
    printf("1) Enter parameters");
    printf("2) Allocate memory for a block using First-fit");
    printf("3) Allocate memory for a block using Best-fit");
    printf("4) Deallocate memory for block");
    printf("5) Defragment memory");
    printf("6) Exit");
}

int main() {
    print_menu();
    return 0;
}