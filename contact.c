#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_CONTACTS 1000
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 20
#define MAX_ADDR_LEN 100
#define CONTACTS_FILE "contacts.csv"  // 联系人数据文件名

typedef struct {
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char address[MAX_ADDR_LEN];
} Contact;

Contact contacts[MAX_CONTACTS];
int contact_count = 0;  // 当前联系人数量

// 清空输入缓冲区
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 检查文件是否为CSV格式
int is_csv_file(const char *filename) {
    const char *extension = strrchr(filename, '.');
    if (extension == NULL) {
        return 0;  // 没有找到文件扩展名
    }
    return strcasecmp(extension, ".csv") == 0;
}

// 根据名字查找联系人
Contact *find_contact_by_name(const char *name) {
    // 去除名字中的前后空白字符
    char trimmed_name[MAX_NAME_LEN];
    int j = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isspace((unsigned char) name[i])) {
            trimmed_name[j++] = name[i];
        }
    }
    trimmed_name[j] = '\0';

    for (int i = 0; i < contact_count; i++) {
        if (strcasecmp(contacts[i].name, trimmed_name) == 0) {
            return &contacts[i];
        }
    }
    return NULL;
}

// 显示单个联系人的信息
void display_contact(const Contact *contact) {
    printf("名字: %s 电话: %s 地址: %s\n", contact->name, contact->phone, contact->address);
}

// 显示主菜单
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

// 分页显示联系人列表
void display_contacts(int start) {
    int end = start + 20 < contact_count ? start + 20 : contact_count;
    printf("%-10s %-15s %-20s\n", "名字", "电话", "地址");
    for (int i = start; i < end; i++) {
        printf("%-10s %-15s %s\n", contacts[i].name, contacts[i].phone, contacts[i].address);
    }
    if (end < contact_count) {
        printf("\n按'Enter'来展示剩下的联系人\n");
        clear_input_buffer();
        getchar();
        display_contacts(end);
    } else {
        printf("\n按任意键回到主界面\n");
        clear_input_buffer();
        getchar();
    }
}

// 添加新联系人
void add_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("通讯录已满，无法添加新联系人。\n");
        return;
    }
    printf("名字: ");
    scanf("%49s", contacts[contact_count].name);
    clear_input_buffer();

    printf("电话: ");
    scanf("%19s", contacts[contact_count].phone);
    clear_input_buffer();

    printf("地址: ");
    scanf("%99[^\n]", contacts[contact_count].address);
    clear_input_buffer();

    printf("\n新增联系人信息：\n");
    display_contact(&contacts[contact_count]);
    contact_count++;

    printf("按任意键回到主界面\n");
    clear_input_buffer();
    getchar();
}

// 更新现有联系人信息
void update_contact() {
    char name[MAX_NAME_LEN];
    printf("请输入要更新的联系人名字: ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';

    Contact *contact = find_contact_by_name(name);
    if (contact == NULL) {
        printf("未找到联系人\n");
        printf("按任意键回到主界面\n");
        clear_input_buffer();
        getchar();
        return;
    }

    printf("新电话号码: ");
    fgets(contact->phone, MAX_PHONE_LEN, stdin);
    contact->phone[strcspn(contact->phone, "\n")] = '\0';

    printf("新地址: ");
    fgets(contact->address, MAX_ADDR_LEN, stdin);
    contact->address[strcspn(contact->address, "\n")] = '\0';

    printf("\n更新后的联系人信息：\n");
    display_contact(contact);

    printf("按任意键回到主界面\n");
    clear_input_buffer();
    getchar();
}

// 删除联系人
void delete_contact() {
    char name[MAX_NAME_LEN];
    printf("请输入要删除的联系人名字: ");
    scanf("%49s", name);
    clear_input_buffer();

    int found = 0;
    for (int i = 0; i < contact_count; i++) {
        if (strcasecmp(contacts[i].name, name) == 0) {
            for (int j = i; j < contact_count - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            contact_count--;
            printf("联系人已删除。\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("未找到联系人\n");
    }

    printf("按任意键回到主界面\n");
    clear_input_buffer();
    getchar();
}

// 搜索联系人
void search_contact() {
    int choice;
    char query[MAX_ADDR_LEN];
    printf("1. 按照名字查找\n");
    printf("2. 按照电话号码查找\n");
    printf("3. 按照地址查找\n");
    printf("请选择: ");
    scanf("%d", &choice);
    clear_input_buffer();

    printf("请输入查询内容: ");
    fgets(query, MAX_ADDR_LEN, stdin);
    query[strcspn(query, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < contact_count; i++) {
        switch (choice) {
            case 1:
                if (strcasecmp(contacts[i].name, query) == 0) {
                    display_contact(&contacts[i]);
                    found = 1;
                }
                break;
            case 2:
                if (strcmp(contacts[i].phone, query) == 0) {
                    display_contact(&contacts[i]);
                    found = 1;
                }
                break;
            case 3:
                if (strcmp(contacts[i].address, query) == 0) {
                    display_contact(&contacts[i]);
                    found = 1;
                }
                break;
            default:
                printf("无效选择，请重新选择。\n");
                break;
        }
    }
    if (!found) {
        printf("未找到联系人\n");
    }

    printf("按任意键回到主界面\n");
    clear_input_buffer();
    getchar();
}

// 从文件更新联系人列表
void update_contacts_from_file() {
    char filename[MAX_NAME_LEN];
    printf("请上传文件: ");
    scanf("%49s", filename);
    clear_input_buffer();

    if (!is_csv_file(filename)) {
        printf("对不起，目前仅支持 .csv 文件，请重新上传。\n");
        printf("按任意键回到主界面\n");
        clear_input_buffer();
        getchar();
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("文件上传失败，无法打开文件 %s。\n", filename);
        printf("按任意键回到主界面\n");
        clear_input_buffer();
        getchar();
        return;
    }

    char line[256];
    int first_line = 1; // 跳过第一行（标题行）
    while (fgets(line, sizeof(line), file)) {
        if (first_line) {
            first_line = 0;
            continue; // 跳过标题行
        }

        char *name = strtok(line, ",");
        char *phone = strtok(NULL, ",");
        char *address = strtok(NULL, "\n");

        if (name && phone && address) {
            // 去除字段中的前后空白
            // 去除名字中的前后空白
            while (isspace((unsigned char)*name)) name++;
            char *end = name + strlen(name) - 1;
            while (end > name && isspace((unsigned char)*end)) end--;
            end[1] = '\0';

            Contact *existing_contact = find_contact_by_name(name);
            if (existing_contact != NULL) {
                strncpy(existing_contact->phone, phone, MAX_PHONE_LEN - 1);
                existing_contact->phone[MAX_PHONE_LEN - 1] = '\0';
                strncpy(existing_contact->address, address, MAX_ADDR_LEN - 1);
                existing_contact->address[MAX_ADDR_LEN - 1] = '\0';
            } else if (contact_count < MAX_CONTACTS) {
                strncpy(contacts[contact_count].name, name, MAX_NAME_LEN - 1);
                contacts[contact_count].name[MAX_NAME_LEN - 1] = '\0';
                strncpy(contacts[contact_count].phone, phone, MAX_PHONE_LEN - 1);
                contacts[contact_count].phone[MAX_PHONE_LEN - 1] = '\0';
                strncpy(contacts[contact_count].address, address, MAX_ADDR_LEN - 1);
                contacts[contact_count].address[MAX_ADDR_LEN - 1] = '\0';
                contact_count++;
            }
        }
    }
    fclose(file);

    printf("上传成功! 联系人列表已更新。\n");
    printf("按任意键回到主界面\n");
    clear_input_buffer();
    getchar();
}

// 将联系人数据保存到文件
void save_contacts_to_file() {
    FILE *file = fopen(CONTACTS_FILE, "w");
    if (file == NULL) {
        printf("保存联系人数据失败，无法打开文件 %s。\n", CONTACTS_FILE);
        printf("按任意键回到主界面\n");
        clear_input_buffer();
        getchar();
        return;
    }

    fprintf(file, "名字,电话,地址\n");
    for (int i = 0; i < contact_count; i++) {
        fprintf(file, "%s,%s,%s\n", contacts[i].name, contacts[i].phone, contacts[i].address);
    }

    fclose(file);
    printf("联系人数据保存成功到文件 %s。\n", CONTACTS_FILE);
    printf("按任意键回到主界面\n");
    clear_input_buffer();
    getchar();
}

// 从文件加载联系人数据
void load_contacts_from_file() {
    FILE *file = fopen(CONTACTS_FILE, "r");
    if (file == NULL) {
        printf("加载联系人数据失败，无法打开文件 %s。\n", CONTACTS_FILE);
        printf("按任意键回到主界面\n");
        clear_input_buffer();
        getchar();
        return;
    }

    char line[256];
    int first_line = 1; // 跳过第一行（标题行）
    contact_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (first_line) {
            first_line = 0;
            continue; // 跳过标题行
        }

        char *name = strtok(line, ",");
        char *phone = strtok(NULL, ",");
        char *address = strtok(NULL, "\n");

        if (name && phone && address && contact_count < MAX_CONTACTS) {
            strncpy(contacts[contact_count].name, name, MAX_NAME_LEN - 1);
            contacts[contact_count].name[MAX_NAME_LEN - 1] = '\0';
            strncpy(contacts[contact_count].phone, phone, MAX_PHONE_LEN - 1);
            contacts[contact_count].phone[MAX_PHONE_LEN - 1] = '\0';
            strncpy(contacts[contact_count].address, address, MAX_ADDR_LEN - 1);
            contacts[contact_count].address[MAX_ADDR_LEN - 1] = '\0';
            contact_count++;
        }
    }

    fclose(file);
    printf("联系人数据加载成功。\n");
    printf("按任意键回到主界面\n");
    clear_input_buffer();
    getchar();
}

// 主函数
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // 设置输出为UTF-8（Windows特有）
#endif

    int choice;
    while (1) {
        display_main_menu();
        printf("请选择操作: ");
        scanf("%d", &choice);
        clear_input_buffer();

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
                save_contacts_to_file();
                exit(0);
                break;
            default:
                printf("无效选择，请重新选择。\n");
                break;
        }
    }

    return 0;
}
