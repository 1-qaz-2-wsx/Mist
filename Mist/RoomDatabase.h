#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Room.h"

// 前向声明，避免包含头文件
class ItemDatabase;
class EnemyDatabase;

class RoomDatabase {
public:
    /**
     * @brief 从JSON文件加载房间数据，并借助其他数据库来构建完整的房间
     * @param filename rooms.json 的路径
     * @param itemDB 指向 ItemDatabase 的指针
     * @param enemyDB 指向 EnemyDatabase 的指针
     * @return 是否加载成功
     */
    bool load(const std::string& filename, const ItemDatabase* itemDB, const EnemyDatabase* enemyDB);

    // 注意：我们将不再需要 getRoomTemplate，因为房间需要被实例化。
    // 我们直接提供一个创建实例的接口。

    /**
     * @brief 根据房间ID创建一个新的房间实例（深拷贝）
     * @param roomId 房间的唯一ID
     * @return 指向新Room实例的unique_ptr，找不到则为空
     */
    std::unique_ptr<Room> createInstance(unsigned int roomId) const;
    /**
     * @brief 获取数据库中所有房间模板的ID列表
     */
    std::vector<unsigned int> getAllRoomIds() const;

private:
    // 数据库现在存储的是房间的“模板”
    std::unordered_map<unsigned int, Room> roomTemplates_;
};