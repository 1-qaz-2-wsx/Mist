// Game.cpp

#include "Game.h"

// ----------------------------------------------------------------
// �������С�ͬ���ࡱ�͹������ͷ�ļ�
// ----------------------------------------------------------------
// ��.cpp�ļ��У�������Ҫ����������Ҫ���н���������������塣
#include "Player.h"
#include "Map.h"
#include "BattleSystem.h"
#include "Room.h"
#include "ItemDatabase.h"
#include "EnemyDatabase.h"
#include "RoomDatabase.h"

#include <iostream>
#include <string>
#include <sstream> // ���ڽ����ַ�������

// --- ����/�������� ---

Game::Game() {
    // ���캯�����Ա���Ϊ�գ������ĳ�ʼ���� initialize() �н���
    // unique_ptr Ĭ�ϻ��ʼ��Ϊ nullptr
}

// ��������������.cpp��ʵ�֣���Ϊͷ�ļ���ֻ��ǰ������
// �������������������������������壬֪�������ȷ���� unique_ptr
Game::~Game() {
}

// --- ��Ϸ�����̿��� ---

bool Game::initialize(const std::string& dataFolderPath) {
    std::cout << "���ڳ�ʼ����Ϸ..." << std::endl;

    try {
        // 1. �������������ݿ� (˳�����Ҫ����Ϊ����ϵͳ��������)
        itemDB_ = std::make_unique<ItemDatabase>();
        itemDB_->load(dataFolderPath + "/items.json");

        enemyDB_ = std::make_unique<EnemyDatabase>();
        enemyDB_->load(dataFolderPath + "/enemies.json");

        roomDB_ = std::make_unique<RoomDatabase>();
        roomDB_->load(dataFolderPath + "/rooms.json");

        // 2. ������ͼ����������
        map_ = std::make_unique<Map>();
        map_->buildFromDatabase(roomDB_.get()); // ʹ��.get()��unique_ptr��ȡ��ָ��

        // 3. ������Ҳ����ó�ʼλ��
        player_ = std::make_unique<Player>(); // �������Player���캯�������ó�ʼ����
        player_->setCurrentRoomId(map_->getStartingRoomId());

        // 4. ����ս��ϵͳ
        battleSystem_ = std::make_unique<BattleSystem>();

        // 5. [�ؼ�] ���н���(Game* this)ע����Ҫ����ͬ����
        // ע��: ����Ҫ�� BattleSystem �� Player ������� setMediator(Game* game) ����
        battleSystem_->setMediator(this);
        // player_->setMediator(this); // ���PlayerҲ��Ҫ�ص�Game��Ҳ�������

    }
    catch (const std::exception& e) {
        // ����κβ���ʧ�ܣ������ļ��Ҳ�����JSON��ʽ���󣩣����ʼ��ʧ��
        std::cerr << "��ʼ��ʧ��: " << e.what() << std::endl;
        return false;
    }

    isRunning_ = true;
    std::cout << "��ʼ���ɹ���" << std::endl;
    return true;
}

void Game::run() {
    if (!isRunning_) {
        std::cout << "��Ϸδ��ʼ����" << std::endl;
        return;
    }

    std::cout << "\n��ӭ���� MUD ���磡" << std::endl;
    std::cout << "���� 'help' �鿴����ָ�" << std::endl;

    // ���������ʼ������߼�
    enterRoom(player_->getCurrentRoomId());

    while (isRunning_) {
        gameLoop();
    }

    std::cout << "��л���棬�ټ���" << std::endl;
}


// --- �н��߽ӿ�ʵ�� ---

void Game::notifyBattleFinished(bool playerWon, const std::vector<unsigned int>& lootItemIds) {
    std::cout << "\n[ϵͳ] ս��������" << std::endl;
    // ս�������󣬵����ڲ��߼������������
    processBattleAftermath(playerWon, lootItemIds);
}


// --- �ڲ��߼������̴��� ---

void Game::gameLoop() {
    std::cout << "\n> "; // ��ʾ������ʾ��
    processPlayerInput();
}

void Game::processPlayerInput() {
    std::string line;
    std::getline(std::cin, line);

    // ʹ�� stringstream �����ɷָ�����Ͳ���
    std::stringstream ss(line);
    std::string command;
    ss >> command;

    // ������תΪСд������Ƚ�
    for (char& c : command) {
        c = tolower(c);
    }

    // ��������ַ�����ͬ�Ĵ�����
    if (command == "go" || command == "move") {
        std::string direction;
        ss >> direction;
        handleMoveCommand(direction);
    }
    else if (command == "look" || command == "l") {
        handleLookCommand();
    }
    else if (command == "attack") {
        std::string target;
        ss >> target;
        handleAttackCommand(target);
    }
    else if (command == "take" || command == "get") {
        std::string item;
        ss >> item;
        handleTakeCommand(item);
    }
    else if (command == "inventory" || command == "inv" || command == "i") {
        handleInventoryCommand();
    }
    else if (command == "help") {
        handleHelpCommand();
    }
    else if (command == "quit" || command == "exit") {
        handleQuitCommand();
    }
    else {
        std::cout << "δָ֪����� 'help' �鿴������" << std::endl;
    }
}

void Game::displayCurrentState() const {
    Room* currentRoom = map_->getRoom(player_->getCurrentRoomId());
    if (!currentRoom) {
        std::cerr << "������ҵ�ǰ������Ч��" << std::endl;
        return;
    }

    // ����Room�Լ��ķ�������ʾ��Ϣ (����Room���ְ��)
    currentRoom->display();
}

void Game::enterRoom(unsigned int newRoomId) {
    player_->setCurrentRoomId(newRoomId);
    std::cout << "\n----------------------------------------" << std::endl;
    handleLookCommand(); // �����·�����Զ�ִ�� "look"

    // ��鷿�����Ƿ��е��ˣ������ܴ���ս��
    checkForEnemiesAndInitiateBattle();
}


// --- ���ָ����� ---

void Game::handleMoveCommand(const std::string& direction) {
    if (direction.empty()) {
        std::cout << "��Ҫ���ĸ������ߣ�(north, south, east, west...)" << std::endl;
        return;
    }

    unsigned int currentRoomId = player_->getCurrentRoomId();
    Room* currentRoom = map_->getRoom(currentRoomId);

    // ��Mapϵͳ��ѯ������Ϣ
    int nextRoomId = currentRoom->getExit(direction);

    if (nextRoomId != -1) { // -1 ����û�г���
        std::cout << "���� " << direction << " ������ȥ..." << std::endl;
        enterRoom(static_cast<unsigned int>(nextRoomId));
    }
    else {
        std::cout << "�������û��·��" << std::endl;
    }
}

void Game::handleLookCommand() const {
    displayCurrentState();
}

void Game::handleAttackCommand(const std::string& targetName) {
    if (targetName.empty()) {
        std::cout << "��Ҫ����˭��" << std::endl;
        return;
    }
    // TODO: ʵ�ֹ����߼�
    // 1. ��ȡ��ǰ����
    // 2. �ڷ���ĵ����б��в������� targetName �ĵ���
    // 3. ����ҵ������� battleSystem_->startBattle(player_.get(), enemy);
    // 4. ���û�ҵ�����ʾ���
    std::cout << "�㳢�Թ��� " << targetName << "����ս��ϵͳ��δʵ�֡�" << std::endl;
}

void Game::handleTakeCommand(const std::string& itemName) {
    if (itemName.empty()) {
        std::cout << "��Ҫʰȡʲô��" << std::endl;
        return;
    }
    // TODO: ʵ��ʰȡ�߼�
    // 1. ��ȡ��ǰ����
    // 2. �ڷ������Ʒ�б��в������� itemName ����Ʒ
    // 3. ����ҵ����ӷ����Ƴ�����Ʒ������ӵ���ҵ���Ʒ��
    // 4. ���û�ҵ�����ʾ���
    std::cout << "�㳢��ʰȡ " << itemName << "����ʰȡϵͳ��δʵ�֡�" << std::endl;
}

void Game::handleInventoryCommand() const {
    // TODO: ����player_->displayInventory();
    std::cout << "��ı����ǿյģ���Ʒ��ϵͳ��δʵ�֣���" << std::endl;
}

void Game::handleHelpCommand() const {
    std::cout << "--- ����ָ�� ---\n"
        << "go [direction] - ��ָ�������ƶ� (e.g., go north)\n"
        << "look           - �鿴��ǰ����\n"
        << "attack [name]  - ����ָ������\n"
        << "take [item]    - ʰȡ���ϵ���Ʒ\n"
        << "inventory      - �鿴��ı���\n"
        << "quit           - �뿪��Ϸ\n"
        << "-----------------" << std::endl;
}

void Game::handleQuitCommand() {
    std::cout << "��ȷ��Ҫ�뿪��Ϸ��(yes/no)" << std::endl;
    std::string confirmation;
    std::cin >> confirmation;
    // ����cin�Ļ�����
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirmation == "yes" || confirmation == "y") {
        isRunning_ = false;
    }
    else {
        std::cout << "���������ð�ա�" << std::endl;
    }
}


// --- �ڲ��¼����� ---

void Game::checkForEnemiesAndInitiateBattle() {
    // TODO: ����ս�������߼�
    // 1. ��ȡ��ǰ����
    // 2. ��鷿�����Ƿ��е���
    // 3. ����У�������Ƴ��Զ�����ս�������������ѡ�񹥻�
    //    �����������ȼ�����������е��˾��Զ���ս
    //    battleSystem_->startBattle(player_.get(), room->getEnemies()[0]);

    // Ŀǰֻ��һ��ռλ��
    Room* currentRoom = map_->getRoom(player_->getCurrentRoomId());
    if (currentRoom && !currentRoom->getEnemies().empty()) {
        std::cout << "�������˵��ˣ�(ս��ϵͳ��ʵ��)" << std::endl;
    }
}

void Game::processBattleAftermath(bool playerWon, const std::vector<unsigned int>& lootItemIds) {
    if (playerWon) {
        std::cout << "��ȡ����ʤ����" << std::endl;
        // TODO: ����ս��Ʒ
        // 1. ��ȡ��ǰ����
        // 2. ��շ�����ĵ��� (����ֻ�Ƴ������ܵ��Ǹ�)
        // 3. ���� lootItemIds���� itemDB_ ��ȡ��Ʒԭ��
        // 4. ����Ʒʵ����ӵ�����ĵ�����
        // 5. ��ʾ������Ϣ
    }
    else {
        std::cout << "�㱻������... ��Ϸ������" << std::endl;
        isRunning_ = false; // �����������Ϸ����
    }
}