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

// 메뉴 데이터 틀
struct Menu {
    string name;
    double price;
};

// 하루 식비
int totalCost;

// 데이터 변경 유무
bool dataModified = false;

// 버려진 메뉴 스택
stack<Menu> modifiedMenus;

// 선택된 메뉴들
Menu selectedBreakfast, selectedLunch, selectedDinner;

// [ 함수 1 ] : 파일에 가게 정보 저장 
void saveStoresToFile(const unordered_map<string, vector<Menu>>& storeMap, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error : 파일 열기 실패" << endl;
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

// [ 함수 2 ] : 파일에서 가게 정보 가져오기
unordered_map<string, vector<Menu>> loadStoresFromFile(const string& filename) {
    unordered_map<string, vector<Menu>> storeMap;
    ifstream file(filename);
    if (!file) {
        cerr << "Error : 파일 열기 실패" << endl;
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

// [ 함수 3 ] : 현재 메뉴 보여주기
void displayCurrentMenus() {
    cout << "\nㅡㅡㅡㅡㅡㅡ현재 메뉴ㅡㅡㅡㅡㅡㅡ\n";
    cout << "1. 아침 : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    cout << "2. 점심 : " << selectedLunch.name << " ($" << selectedLunch.price << ")\n";
    cout << "3. 저녁 : " << selectedDinner.name << " ($" << selectedDinner.price << ")\n";
    cout << "ㅡㅡㅡㅡㅡㅡ현재 메뉴ㅡㅡㅡㅡㅡㅡ\n";
}
// [ 함수 4 ] : 현재 메뉴 단일 변경
void changeMenu(unordered_map<string, vector<Menu>>& storeMap, int mealTime) {
    vector<Menu> allMenus;

    // 모든 메뉴 담기
    for (const auto& storePair : storeMap) {
        const vector<Menu>& menus = storePair.second;
        allMenus.insert(allMenus.end(), menus.begin(), menus.end());
    }

    // 비어있는지 체크
    if (allMenus.empty()) {
        cout << "Error : 메뉴 부족\n";
        return;
    }

    // 셔플
    random_device rd;
    mt19937 g(rd());
    shuffle(allMenus.begin(), allMenus.end(), g);

    // 메뉴 결정
    Menu selectedMenu = allMenus.front();

    // 사용자 입력에 따른 메뉴 변경. 이 때 변경되는 메뉴 데이터는 스택에 저장됨.
    if (mealTime == 1) {
        cout << "아침 : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedBreakfast = selectedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else if (mealTime == 2) {
        cout << "점심 : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedLunch = selectedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else if (mealTime == 3) {
        cout << "저녁 : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedDinner = selectedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else {
        cout << "Error : 잘못된 입력 \n";
    }
}

// [ 함수 5 ] : 가게 정보 초기화
void resetAllMenus(unordered_map<string, vector<Menu>>& storeMap) {
    cout << "\n 가게 초기화 \n";
    storeMap.clear();
}

// [ 함수 6 ] : 가게 정보 추가
void addMenu(unordered_map<string, vector<Menu>>& storeMap) {
    string storeName, menuName;
    double menuPrice;

    cout << "가게 이름 입력 : ";
    cin.ignore();
    getline(cin, storeName);

    cout << "메뉴 이름 입력 : ";
    getline(cin, menuName);

    cout << "메뉴 가격 입력 : ";
    cin >> menuPrice;

    auto& menus = storeMap[storeName];
    auto it = find_if(menus.begin(), menus.end(), [&menuName](const Menu& menu) {
        return menu.name == menuName;
        });

    if (it == menus.end()) {
        menus.push_back({ menuName, menuPrice });
        dataModified = true;
        cout << "메뉴 추가 성공 \n";
    }
    else {
        cout << "메뉴 추가 실패 \n";
    }
}

// [ 함수 7 ] : 가게 정보 보여주기
void displayStoresAndMenus(const unordered_map<string, vector<Menu>>& storeMap) {
    if (storeMap.empty()) {
        cout << "저장된 데이터가 없습니다. \n";
        return;
    }

    cout << "\nㅡㅡ가게 리스트 ㅡㅡ \n";
    int index = 1;
    unordered_map<int, string> indexToStore;
    for (const auto& storePair : storeMap) {
        const string& storeName = storePair.first;
        cout << index << ". " << storeName << endl;
        indexToStore[index] = storeName;
        ++index;
    }

    cout << "ㅡㅡ가게 리스트 ㅡㅡ \n";
    cout << "\n확인 할 가게 번호 입력 :  ";
    int storeIndex;
    cin >> storeIndex;

    if (indexToStore.find(storeIndex) == indexToStore.end()) {
        cout << "Error : 잘못된 입력 \n";
        return;
    }

    const string& selectedStoreName = indexToStore[storeIndex];
    const auto& menus = storeMap.at(selectedStoreName);
    cout << "\nㅡㅡ" << selectedStoreName << "의 메뉴 리스트ㅡㅡ\n";
    for (const auto& menu : menus) {
        cout << "- " << menu.name << " ($" << menu.price << ")\n";
    }
    cout << "ㅡㅡ" << selectedStoreName << "의 메뉴 리스트ㅡㅡ\n";
}

// [ 함수 8 ] : Undo
void applyChangesFromStack(int mealTime) {
    if (modifiedMenus.empty()) {
        cout << "Error : 스택이 비어있음 \n";
        return;
    }

    auto modifiedMenu = modifiedMenus.top();
    modifiedMenus.pop();

    if (mealTime == 1) {
        cout << "아침 : " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ") - > ";
        modifiedMenus.push(selectedBreakfast);
        selectedBreakfast = modifiedMenu;
        cout << " " << selectedBreakfast.name << " ($" << selectedBreakfast.price << ")\n";
    }
    else if (mealTime == 2) {
        cout << "점심 : " << selectedLunch.name << " ($" << selectedLunch.price << ") - > ";
        modifiedMenus.push(selectedLunch);
        selectedLunch = modifiedMenu;
        cout << " " << selectedLunch.name << " ($" << selectedLunch.price << ")\n";
    }
    else if (mealTime == 3) {
        cout << "저녁 : " << selectedDinner.name << " ($" << selectedDinner.price << ") - > ";
        modifiedMenus.push(selectedDinner);
        selectedDinner = modifiedMenu;
        cout << " " << selectedDinner.name << " ($" << selectedDinner.price << ")\n";
    }
    else {
        cout << "Error : 잘못된 입력\n";
    }

    cout << "Undo 성공 \n";
}
// [ 함수 9 ] : 모두 다시 뽑기 
void randomizeMenus(unordered_map<string, vector<Menu>>& storeMap) {
    vector<Menu> allMenus;
    int totalPrice = 0;

    // 모든 메뉴 담기
    for (const auto& storePair : storeMap) {
        const vector<Menu>& menus = storePair.second;
        allMenus.insert(allMenus.end(), menus.begin(), menus.end());
    }

    srand(time(0));
    random_shuffle(allMenus.begin(), allMenus.end());

    modifiedMenus.push(selectedBreakfast);
    modifiedMenus.push(selectedLunch);
    modifiedMenus.push(selectedDinner);

    // 메뉴를 선택하되, totalCost를 넘지 않도록 합산하기
    selectedBreakfast = selectedLunch = selectedDinner = { "", 0.0 }; // 초기화
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
// [ 함수 10 ] : 모두 다시 뽑기 
void modifyMeals(unordered_map<string, vector<Menu>>& storeMap) {
    cout << "\n변경 옵션을 선택하시오. \n";
    cout << "1. 아침 단일 변경\n";
    cout << "2. 점심 단일 변경\n";
    cout << "3. 저녁 단일 변경\n";
    cout << "4. 가게 정보 초기화\n";
    cout << "5. Undo\n";
    cout << "6. 모두 다시 뽑기\n";
    cout << "옵션 선택 : ";

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
        cout << "시간 선택 : (1. 아침 2. 점심 3. 저녁) ";
        cin >> mealTime;
        applyChangesFromStack(mealTime);
    }
    else if (mealChoice == 6) {
        randomizeMenus(storeMap);
    }
    else {
        cout << "Error : 잘못된 입력\n";
    }
}

// [ 함수 11 ] : 하루 식비 입력
void setTotalCost() {
    int cost;
    cout << "하루 식비를 입력하시오. ";
    cin >> cost;
    totalCost = cost;
}
int main() {
    string filename = "stores.txt";
    unordered_map<string, vector<Menu>> storeMap;

    // Load initial data from file
    storeMap = loadStoresFromFile(filename);

    while (true) {
        cout << "\nㅡㅡ메인 화면ㅡㅡ\n";
        cout << "0. 하루 식비 입력\n";
        cout << "1. 현재 선택된 메뉴 확인\n";
        cout << "2. 메뉴 변경\n";
        cout << "3. 가게 추가\n";
        cout << "4. 가게 정보 확인\n";
        cout << "5. Exit (데이터 저장)\n";
        cout << "ㅡㅡ메인 화면ㅡㅡ\n";
        cout << "옵션 입력 : ";

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
            cout << "프로그램 종료\n";
            break;
        }
        else {
            cout << "Error : 잘못된 입력\n";
        }
    }

    return 0;
}
