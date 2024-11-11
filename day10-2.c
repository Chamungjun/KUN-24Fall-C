#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    VIP = 1,
    GOLD,
    SILVER,
    BRONZE
} Rank;

typedef struct Customer {
    char* customerName;
    Rank rank;
    int order_amount;
    int point;
    struct Customer* prev;
    struct Customer* next;
} Customer;

Customer* createCustomer(char* name, Rank rank, int order_amount, int point);
void insertCustomer(Customer** head, char* name, Rank rank, int order_amount, int point);
void deleteCustomer(Customer** head, char* name);
void updateCustomer(Customer* head, char* name, Rank rank, int order_amount, int point);
void printCustomerList(Customer* head);
void freeCustomerList(Customer* head);

int main() {
    Customer* head = NULL;
    int choice;
    char name[50];
    int rank, order_amount, point;

    while (1) {
        printf("\n1. 고객 추가\n");
        printf("2. 고객 삭제\n");
        printf("3. 고객 수정\n");
        printf("4. 전체 리스트 출력\n");
        printf("5. 종료\n");
        printf("input : ");
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("고객 이름: ");
            scanf_s("%49s", name, (unsigned)_countof(name));
            printf("고객 등급 (1:VIP, 2:GOLD, 3:SILVER, 4:BRONZE): ");
            scanf_s("%d", &rank);
            printf("전체 주문량: ");
            scanf_s("%d", &order_amount);
            printf("포인트: ");
            scanf_s("%d", &point);
            insertCustomer(&head, name, rank, order_amount, point);
            break;

        case 2:
            printf("삭제할 고객 이름: ");
            scanf_s("%49s", name, (unsigned)_countof(name));
            deleteCustomer(&head, name);
            break;

        case 3:
            printf("수정할 고객 이름: ");
            scanf_s("%49s", name, (unsigned)_countof(name));
            printf("새로운 등급 (1:VIP, 2:GOLD, 3:SILVER, 4:BRONZE): ");
            scanf_s("%d", &rank);
            printf("새로운 전체 주문량: ");
            scanf_s("%d", &order_amount);
            printf("새로운 포인트: ");
            scanf_s("%d", &point);
            updateCustomer(head, name, rank, order_amount, point);
            break;

        case 4:
            printCustomerList(head);
            break;

        case 5:
            freeCustomerList(head);
            return 0;

        default:
            printf("잘못된 입력입니다.\n");
        }
    }
}

Customer* createCustomer(char* name, Rank rank, int order_amount, int point) {
    Customer* newCustomer = (Customer*)malloc(sizeof(Customer));
    if (newCustomer == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }

    newCustomer->customerName = (char*)malloc(strlen(name) + 1);
    if (newCustomer->customerName == NULL) {
        printf("메모리 할당 실패\n");
        free(newCustomer);
        exit(1);
    }

    strcpy_s(newCustomer->customerName, strlen(name) + 1, name);
    newCustomer->rank = rank;
    newCustomer->order_amount = order_amount;
    newCustomer->point = point;
    newCustomer->prev = NULL;
    newCustomer->next = NULL;
    return newCustomer;
}

void insertCustomer(Customer** head, char* name, Rank rank, int order_amount, int point) {
    Customer* newCustomer = createCustomer(name, rank, order_amount, point);

    if (*head == NULL) {
        *head = newCustomer;
        return;
    }

    Customer* current = *head;
    while (current != NULL) {
        if (current->rank > rank ||
            (current->rank == rank && current->order_amount < order_amount) ||
            (current->rank == rank && current->order_amount == order_amount && current->point < point)) {
            break;
        }
        current = current->next;
    }
    \
        if (current == *head) {
            newCustomer->next = *head;
            (*head)->prev = newCustomer;
            *head = newCustomer;
        }
        else if (current == NULL) {
            Customer* tail = *head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = newCustomer;
            newCustomer->prev = tail;
        }
        else {
            newCustomer->prev = current->prev;
            newCustomer->next = current;

            if (current->prev != NULL) {
                current->prev->next = newCustomer;
            }
            current->prev = newCustomer;
        }
}

void deleteCustomer(Customer** head, char* name) {
    if (head == NULL || *head == NULL) return;

    Customer* current = *head;
    while (current && strcmp(current->customerName, name) != 0) {
        current = current->next;
    }

    if (!current) {
        printf("고객을 찾을 수 없습니다.\n");
        return;
    }

    if (current->prev) current->prev->next = current->next;
    if (current->next) current->next->prev = current->prev;
    if (current == *head) *head = current->next;

    free(current->customerName);
    free(current);
}

void updateCustomer(Customer* head, char* name, Rank rank, int order_amount, int point) {
    Customer* current = head;
    while (current && strcmp(current->customerName, name) != 0) {
        current = current->next;
    }

    if (!current) {
        printf("고객을 찾을 수 없습니다.\n");
        return;
    }

    current->rank = rank;
    current->order_amount = order_amount;
    current->point = point;
}

void printCustomerList(Customer* head) {
    Customer* current = head;
    while (current) {
        printf("이름: %s, 등급: %d, 주문량: %d, 포인트: %d\n",
            current->customerName, current->rank, current->order_amount, current->point);
        current = current->next;
    }
}

void freeCustomerList(Customer* head) {
    Customer* current = head;
    while (current) {
        Customer* temp = current;
        current = current->next;
        free(temp->customerName);
        free(temp);
    }
}
