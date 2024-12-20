#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib> 
#include <ctime>   
#include <stack>  
#include <random> 
using namespace std;

// �޴� ������ Ʋ
struct Menu {
    string name;
    double price;
};

// �Ϸ� �ĺ�
int totalCost;

// ������ ���� ����
bool dataModified = false;

// ������ �޴� ����
stack<Menu> modifiedMenus;

// ���õ� �޴���
Menu selectedBreakfast, selectedLunch, selectedDinner;

// [ �Լ� 1 ] : ���Ͽ� ���� ���� ���� 
void saveStoresToFile(const unordered_map<string, vector<Menu>>& storeMap, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error : ���� ���� ����" << endl;
        return;
    }

    for (const auto& storePair : storeMap) {
        const string& storeName = storePair.first;
        const vector<Menu>& menus = storePair.second;

        file << storeName << endl;
        file << menus.size() << endl;
        for (const auto& menu : menus) {
            file << menu.name << " " << menu.price << endl;
        }
    }

    file.close();
}

// [ �Լ� 2 ] : ���Ͽ��� ���� ���� ��������
unordered_map<string, vector<Menu>> loadStoresFromFile(const string& filename) {
    unordered_map<string, vector<Menu>> storeMap;
    ifstream file(filename);
    if (!file) {
        cerr << "Error : ���� ���� ����" << endl;
        return storeMap;
    }

    while (!file.eof()) {
        string storeName;
        size_t menuCount;

        getline(file, storeName);
        if (storeName.empty()) break;

        file >> menuCount;
        file.ignore();

        vector<Menu> menus;
        for (size_t i = 0; i < menuCount; ++i) {
            Menu menu;
            file >> menu.name >> menu.price;
            file.ignore();
            menus.push_back(menu);
        }

        storeMap[storeName] = menus;
    }

    file.close();
    dataModified = false;
    return storeMap;
}

// [ �Լ� 3 ] : ���� �޴� �����ֱ�
void displayCurrentMenus() {
    cout << "\n�ѤѤѤѤѤ����� �޴��ѤѤѤѤѤ�\n";
    cout << "1. ��ħ : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    cout << "2. ���� : " << selectedLunch.name << " ($" << selectedLunch.price << ")\n";
    cout << "3. ���� : " << selectedDinner.name << " ($" << selectedDinner.price << ")\n";
    cout << "�ѤѤѤѤѤ����� �޴��ѤѤѤѤѤ�\n";
}
// [ �Լ� 4 ] : ���� �޴� ���� ����
void changeMenu(unordered_map<string, vector<Menu>>& storeMap, int mealTime) {
    vector<Menu> allMenus;

    // ��� �޴� ���
    for (const auto& storePair : storeMap) {
        const vector<Menu>& menus = storePair.second;
        allMenus.insert(allMenus.end(), menus.begin(), menus.end());
    }

    // ����ִ��� üũ
    if (allMenus.empty()) {
        cout << "Error : �޴� ����\n";
        return;
    }

    // ����
    random_device rd;
    mt19937 g(rd());
    shuffle(allMenus.begin(), allMenus.end(), g);

    // �޴� ����
    Menu selectedMenu = allMenus.front();

    // ����� �Է¿� ���� �޴� ����. �� �� ����Ǵ� �޴� �����ʹ� ���ÿ� �����.
    if (mealTime == 1) {
        cout << "��ħ : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedBreakfast = selectedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else if (mealTime == 2) {
        cout << "���� : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedLunch = selectedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else if (mealTime == 3) {
        cout << "���� : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedDinner = selectedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else {
        cout << "Error : �߸��� �Է� \n";
    }
}

// [ �Լ� 5 ] : ���� ���� �ʱ�ȭ
void resetAllMenus(unordered_map<string, vector<Menu>>& storeMap) {
    cout << "\n ���� �ʱ�ȭ \n";
    storeMap.clear();
}

// [ �Լ� 6 ] : ���� ���� �߰�
void addMenu(unordered_map<string, vector<Menu>>& storeMap) {
    string storeName, menuName;
    double menuPrice;

    cout << "���� �̸� �Է� : ";
    cin.ignore();
    getline(cin, storeName);

    cout << "�޴� �̸� �Է� : ";
    getline(cin, menuName);

    cout << "�޴� ���� �Է� : ";
    cin >> menuPrice;

    auto& menus = storeMap[storeName];
    auto it = find_if(menus.begin(), menus.end(), [&menuName](const Menu& menu) {
        return menu.name == menuName;
        });

    if (it == menus.end()) {
        menus.push_back({ menuName, menuPrice });
        dataModified = true;
        cout << "�޴� �߰� ���� \n";
    }
    else {
        cout << "�޴� �߰� ���� \n";
    }
}

// [ �Լ� 7 ] : ���� ���� �����ֱ�
void displayStoresAndMenus(const unordered_map<string, vector<Menu>>& storeMap) {
    if (storeMap.empty()) {
        cout << "����� �����Ͱ� �����ϴ�. \n";
        return;
    }

    cout << "\n�ѤѰ��� ����Ʈ �Ѥ� \n";
    int index = 1;
    unordered_map<int, string> indexToStore;
    for (const auto& storePair : storeMap) {
        const string& storeName = storePair.first;
        cout << index << ". " << storeName << endl;
        indexToStore[index] = storeName;
        ++index;
    }

    cout << "�ѤѰ��� ����Ʈ �Ѥ� \n";
    cout << "\nȮ�� �� ���� ��ȣ �Է� :  ";
    int storeIndex;
    cin >> storeIndex;

    if (indexToStore.find(storeIndex) == indexToStore.end()) {
        cout << "Error : �߸��� �Է� \n";
        return;
    }

    const string& selectedStoreName = indexToStore[storeIndex];
    const auto& menus = storeMap.at(selectedStoreName);
    cout << "\n�Ѥ�" << selectedStoreName << "�� �޴� ����Ʈ�Ѥ�\n";
    for (const auto& menu : menus) {
        cout << "- " << menu.name << " ($" << menu.price << ")\n";
    }
    cout << "�Ѥ�" << selectedStoreName << "�� �޴� ����Ʈ�Ѥ�\n";
}

// [ �Լ� 8 ] : Undo
void applyChangesFromStack(int mealTime) {
    if (modifiedMenus.empty()) {
        cout << "Error : ������ ������� \n";
        return;
    }

    auto modifiedMenu = modifiedMenus.top();
    modifiedMenus.pop();

    if (mealTime == 1) {
        cout << "��ħ : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedBreakfast = modifiedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else if (mealTime == 2) {
        cout << "���� : " << selectedLunch.name << " ($" << selectedLunch.price << ") - > ";
        modifiedMenus.push(selectedLunch);
        selectedLunch = modifiedMenu;
        cout << " " << selectedLunch.name << " ($" << selectedLunch.price << ")\n";
    }
    else if (mealTime == 3) {
        cout << "���� : " << selectedDinner.name << " ($" << selectedDinner.price << ") - > ";
        modifiedMenus.push(selectedDinner);
        selectedDinner = modifiedMenu;
        cout << " " << selectedDinner.name << " ($" << selectedDinner.price << ")\n";
    }
    else {
        cout << "Error : �߸��� �Է�\n";
    }

    cout << "Undo ���� \n";
}
// [ �Լ� 9 ] : ��� �ٽ� �̱� 
void randomizeMenus(unordered_map<string, vector<Menu>>& storeMap) {
    vector<Menu> allMenus;
    int totalPrice = 0;

    // ��� �޴� ���
    for (const auto& storePair : storeMap) {
        const vector<Menu>& menus = storePair.second;
        allMenus.insert(allMenus.end(), menus.begin(), menus.end());
    }

    srand(time(0));
    random_shuffle(allMenus.begin(), allMenus.end());

    modifiedMenus.push(selectedBreakfast);
    modifiedMenus.push(selectedLunch);
    modifiedMenus.push(selectedDinner);

    // �޴��� �����ϵ�, totalCost�� ���� �ʵ��� �ջ��ϱ�
    selectedBreakfast = selectedLunch = selectedDinner = { "", 0.0 }; // �ʱ�ȭ
    for (const auto& menu : allMenus) {
        if (totalPrice + menu.price <= totalCost) {
            if (selectedBreakfast.name.empty()) {
                selectedBreakfast = menu;
                totalPrice += menu.price;
            }
            else if (selectedLunch.name.empty()) {
                selectedLunch = menu;
                totalPrice += menu.price;
            }
            else if (selectedDinner.name.empty()) {
                selectedDinner = menu;
                totalPrice += menu.price;
            }
        }
    }
}
// [ �Լ� 10 ] : ��� �ٽ� �̱� 
void modifyMeals(unordered_map<string, vector<Menu>>& storeMap) {
    cout << "\n���� �ɼ��� �����Ͻÿ�. \n";
    cout << "1. ��ħ ���� ����\n";
    cout << "2. ���� ���� ����\n";
    cout << "3. ���� ���� ����\n";
    cout << "4. ���� ���� �ʱ�ȭ\n";
    cout << "5. Undo\n";
    cout << "6. ��� �ٽ� �̱�\n";
    cout << "�ɼ� ���� : ";

    int mealChoice;
    cin >> mealChoice;

    if (mealChoice == 1) {
        changeMenu(storeMap, 1);
    }
    else if (mealChoice == 2) {
        changeMenu(storeMap, 2);
    }
    else if (mealChoice == 3) {
        changeMenu(storeMap, 3);
    }
    else if (mealChoice == 4) {
        resetAllMenus(storeMap);
    }
    else if (mealChoice == 5) {
        int mealTime;
        cout << "�ð� ���� : (1. ��ħ 2. ���� 3. ����) ";
        cin >> mealTime;
        applyChangesFromStack(mealTime);
    }
    else if (mealChoice == 6) {
        randomizeMenus(storeMap);
    }
    else {
        cout << "Error : �߸��� �Է�\n";
    }
}

// [ �Լ� 11 ] : �Ϸ� �ĺ� �Է�
void setTotalCost() {
    int cost;
    cout << "�Ϸ� �ĺ� �Է��Ͻÿ�. ";
    cin >> cost;
    totalCost = cost;
}
int main() {
    string filename = "stores.txt";
    unordered_map<string, vector<Menu>> storeMap;

    // Load initial data from file
    storeMap = loadStoresFromFile(filename);

    while (true) {
        cout << "\n�ѤѸ��� ȭ��Ѥ�\n";
        cout << "0. �Ϸ� �ĺ� �Է�\n";
        cout << "1. ���� ���õ� �޴� Ȯ��\n";
        cout << "2. �޴� ����\n";
        cout << "3. ���� �߰�\n";
        cout << "4. ���� ���� Ȯ��\n";
        cout << "5. Exit (������ ����)\n";
        cout << "�ѤѸ��� ȭ��Ѥ�\n";
        cout << "�ɼ� �Է� : ";

        int choice;
        cin >> choice;
        if (choice == 0) {
            setTotalCost();
        }
        else if (choice == 1) {
            displayCurrentMenus();
        }
        else if (choice == 2) {
            modifyMeals(storeMap);
        }
        else if (choice == 3) {
            addMenu(storeMap);
        }
        else if (choice == 4) {
            displayStoresAndMenus(storeMap);
        }
        else if (choice == 5) {
            if (dataModified) {
                saveStoresToFile(storeMap, filename);
            }
            cout << "���α׷� ����\n";
            break;
        }
        else {
            cout << "Error : �߸��� �Է�\n";
        }
    }

    return 0;
}
