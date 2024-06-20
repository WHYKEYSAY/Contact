#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 1000
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 20
#define MAX_ADDR_LEN 100

typedef struct {
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char address[MAX_ADDR_LEN];
} Contact;

Contact contacts[MAX_CONTACTS];
int contact_count = 0;

void display_main_menu() {
    printf("\n\n====================================\n\n");
    printf("请选择操作：\n");
    printf("1. 展示所有联系人\n");
    printf("2. 查找联系人\n");
    printf("3. 添加联系人\n");
    printf("4. 更新联系人\n");
    printf("5. 删除联系人\n");
    printf("6. 通过文件更新联系人列表\n");
    printf("7. 退出系统\n");
}

void display_contacts(int start) {
    int end = start + 20 < contact_count ? start + 20 : contact_count;
    printf("%-10s %-15s %-20s\n", "名字", "电话", "地址");
    for (int i = start; i < end; i++) {
        printf("%-10s %-15s %-20s\n", contacts[i].name, contacts[i].phone, contacts[i].address);
    }
    if (end < contact_count) {
        printf("\n按'Enter'来展示剩下的联系人\n");
        getchar();
        display_contacts(end);
    } else {
        printf("\n按'q'回到主界面\n");
        getchar();
    }
}

void add_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("通讯录已满，无法添加新联系人。\n");
        return;
    }
    printf("名字: ");
    scanf("%49s", contacts[contact_count].name);
    getchar(); // Consume the newline character

    printf("电话: ");
    scanf("%19s", contacts[contact_count].phone);
    getchar(); // Consume the newline character

    printf("地址: ");
    scanf("%99s", contacts[contact_count].address);
    getchar(); // Consume the newline character

    contact_count++;
    printf("\n新增联系人信息：\n");
    printf("名字: %s 电话: %s 地址: %s\n", contacts[contact_count-1].name, contacts[contact_count-1].phone, contacts[contact_count-1].address);
    printf("按'q'回到主界面\n");
    getchar();
}

void update_contact() {
    char name[MAX_NAME_LEN];
    printf("请输入要更新的联系人名字: ");
    scanf("%49s", name);
    getchar(); // Consume the newline character

    for (int i = 0; i < contact_count; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            printf("新电话号码: ");
            scanf("%19s", contacts[i].phone);
            getchar(); // Consume the newline character

            printf("新地址: ");
            scanf("%99s", contacts[i].address);
            getchar(); // Consume the newline character

            printf("\n更新后的联系人信息：\n");
            printf("名字: %s 电话: %s 地址: %s\n", contacts[i].name, contacts[i].phone, contacts[i].address);
            printf("按'q'回到主界面\n");
            getchar();
            return;
        }
    }
    printf("未找到联系人\n");
    printf("按'q'回到主界面\n");
    getchar();
}

void delete_contact() {
    char name[MAX_NAME_LEN];
    printf("请输入要删除的联系人名字: ");
    scanf("%49s", name);
    getchar(); // Consume the newline character

    for (int i = 0; i < contact_count; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            for (int j = i; j < contact_count - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            contact_count--;
            printf("联系人已删除。\n");
            printf("按'q'回到主界面\n");
            getchar();
            return;
        }
    }
    printf("未找到联系人\n");
    printf("按'q'回到主界面\n");
    getchar();
}

void search_contact() {
    int choice;
    char query[MAX_ADDR_LEN];
    printf("1. 按照名字查找\n");
    printf("2. 按照电话号码查找\n");
    printf("3. 按照地址查找\n");
    printf("请选择: ");
    scanf("%d", &choice);
    getchar(); // Consume newline character

    printf("请输入查询内容: ");
    scanf("%99s", query);
    getchar(); // Consume newline character

    for (int i = 0; i < contact_count; i++) {
        if ((choice == 1 && strcmp(contacts[i].name, query) == 0) ||
            (choice == 2 && strcmp(contacts[i].phone, query) == 0) ||
            (choice == 3 && strcmp(contacts[i].address, query) == 0)) {
            printf("名字: %s 电话: %s 地址: %s\n", contacts[i].name, contacts[i].phone, contacts[i].address);
        }
    }
    printf("按'q'回到主界面\n");
    getchar();
}

void update_contacts_from_file() {
    char filename[MAX_NAME_LEN];
    printf("请上传文件: ");
    scanf("%49s", filename);
    getchar(); // Consume newline character

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("文件上传失败。\n");
        printf("按'q'回到主界面\n");
        getchar();
        return;
    }

    while (fscanf(file, "%49[^,],%19[^,],%99[^\n]\n", contacts[contact_count].name, contacts[contact_count].phone, contacts[contact_count].address) != EOF) {
        contact_count++;
        if (contact_count >= MAX_CONTACTS) break;
    }
    fclose(file);

    printf("上传成功! 联系人列表已更新。\n");
    printf("按'q'回到主界面\n");
    getchar();
}

int main() {
    int choice;
    while (1) {
        display_main_menu();
        printf("请选择操作: ");
        scanf("%d", &choice);
        getchar();  // Consume newline character

        switch (choice) {
            case 1:
                display_contacts(0);
                break;
            case 2:
                search_contact();
                break;
            case 3:
                add_contact();
                break;
            case 4:
                update_contact();
                break;
            case 5:
                delete_contact();
                break;
            case 6:
                update_contacts_from_file();
                break;
            case 7:
                exit(0);
                break;
            default:
                printf("无效选择，请重新选择。\n");
        }
    }
    return 0;
}
