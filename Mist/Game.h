#pragma once
#include <memory>
#include <string>
#include <vector>

//�н��ߣ�����������֮���ͨ�š������ǳ�ʼ����ϵͳ��������Ϸѭ����
//ǰ������������ѭ������
class Player;
class Map;
class BattleSystem;
class Room;
class ItemDatabase;
class EnemyDatabase;
class RoomDatabase;

class Game
{

public:
	// --- ����/�����������������ڹ��� ---
	Game();
	~Game();

	// ��ֹ�����͸�ֵ����ΪGame�ǵ�����Դ������
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	// --- ��Ϸ�����̿��� ---

	// ��ʼ��������Ϸ���ݺ���ϵͳ
	bool initialize(const std::string& dataFolderPath);

	// ������������Ϸ��ѭ��
	void run();

public:
	// ----------------------------------------------------------------
	// �н��߽ӿ� (Mediator Interface)
	// ----------------------------------------------------------------
	// ��Щ�����Ǹ���ͬ���ࡱ(��Player, BattleSystem)���õģ��������н��߱����¼����������

	/**
	 * @brief ��һ��ս������ʱ����BattleSystem���ô˺���
	 * @param playerWon ����Ƿ�ʤ��
	 * @param lootItemIds ս��Ʒ��Ʒ��ID�б�
	 */
	void notifyBattleFinished(bool playerWon, const std::vector<unsigned int>& lootItemIds);



private:
    // ----------------------------------------------------------------
    // �ڲ��߼������̴��� (Private Helper Methods)
    // ----------------------------------------------------------------
    // ��Щ��Game���ڲ�����������Ϸ�߼���˽�к�����

    // ��Ϸ��ѭ������ run() ����
    void gameLoop();

    // ������ҵ�����
    void processPlayerInput();

    // ��ʾ��ǰ�������Ϣ
    void displayCurrentState() const;

    // �л�������ڵķ��䣬���������뷿����߼�
    void enterRoom(unsigned int newRoomId);

    // --- ���ָ����� ---
    void handleMoveCommand(const std::string& direction);
    void handleLookCommand() const;
    void handleAttackCommand(const std::string& targetName);
    void handleTakeCommand(const std::string& itemName);
    void handleInventoryCommand() const;
    void handleHelpCommand() const;
    void handleQuitCommand();

    // --- �ڲ��¼����� ---

    // �ڽ���һ���·���󣬼���Ƿ��е��˲�����ս��
    void checkForEnemiesAndInitiateBattle();

    // ����ս��������ĸ������ˣ����Ƴ����ˡ�������Ʒ��
    void processBattleAftermath(bool playerWon, const std::vector<unsigned int>& lootItemIds);


private:
    // ----------------------------------------------------------------
    // ��Ա���� (Member Variables)
    // ----------------------------------------------------------------
    // �н��߳���������ϵͳ������Ȩ��ʹ��unique_ptr�����Զ������ڴ档

    // --- ������Ϸ���� ---
    std::unique_ptr<Player> player_;
    std::unique_ptr<Map> map_;

    // --- ��Ϸ��ϵͳ ---
    std::unique_ptr<BattleSystem> battleSystem_;

    // --- ���ݹ����� ---
    std::unique_ptr<ItemDatabase> itemDB_;
    std::unique_ptr<EnemyDatabase> enemyDB_;
    std::unique_ptr<RoomDatabase> roomDB_;

    // --- ��Ϸ״̬ ---
    bool isRunning_ = false; // ������Ϸ��ѭ���Ƿ����
};
};


