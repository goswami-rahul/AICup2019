#include "MyStrategy.hpp"

MyStrategy::MyStrategy() {}

double distanceSqr(Vec2Double a, Vec2Double b) {
  return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

Vec2Float getDebugPos(const Vec2Double &pos, const Vec2Double &size) {
  return Vec2Float(pos.x - size.x / 2, pos.y);
}

Vec2Double operator * (const Vec2Double &lhs, const double &rhs) {
  return Vec2Double(lhs.x * rhs, lhs.y * rhs);
}

const ColorFloat RED   (1, 0, 0, 1);
const ColorFloat GREEN (0, 1, 0, 1);
const ColorFloat BLUE  (0, 0, 1, 1);

UnitAction MyStrategy::getAction(const Unit &unit, const Game &game,
                                 Debug &debug) {
  const Unit *nearestEnemy = nullptr;
  for (const Unit &other : game.units) {
    if (other.playerId != unit.playerId) {
      if (nearestEnemy == nullptr ||
          distanceSqr(unit.position, other.position) <
              distanceSqr(unit.position, nearestEnemy->position)) {
        nearestEnemy = &other;
      }
    }
  }
  const LootBox *nearestWeapon = nullptr;
  for (const LootBox &lootBox : game.lootBoxes) {
    if (std::dynamic_pointer_cast<Item::Weapon>(lootBox.item)) {
      if (nearestWeapon == nullptr ||
          distanceSqr(unit.position, lootBox.position) <
              distanceSqr(unit.position, nearestWeapon->position)) {
        nearestWeapon = &lootBox;
      }
    }
  }
  Vec2Double targetPos = unit.position;
  Vec2Double targetSize = unit.size;
  if (unit.weapon == nullptr && nearestWeapon != nullptr) {
    targetPos = nearestWeapon->position;
    targetSize = nearestWeapon->size;
  } else if (nearestEnemy != nullptr) {
    targetPos = nearestEnemy->position;
    targetSize = nearestEnemy->size;
  }

  {
    debug.draw(CustomData::Log(
      std::string("Target pos: ") + targetPos.toString()));
    debug.draw(CustomData::Rect(
      getDebugPos(targetPos, targetSize), 
      Vec2Float(targetSize.x, targetSize.y),
      RED
    ));
  }

  Vec2Double aim = Vec2Double(0, 0);
  if (nearestEnemy != nullptr) {
    aim = Vec2Double(nearestEnemy->position.x - unit.position.x,
                     nearestEnemy->position.y - unit.position.y);
  }
  bool jump = targetPos.y > unit.position.y;
  if (targetPos.x > unit.position.x &&
      game.level.tiles[size_t(unit.position.x + 1)][size_t(unit.position.y)] ==
          Tile::WALL) {
    jump = true;
  }
  if (targetPos.x < unit.position.x &&
      game.level.tiles[size_t(unit.position.x - 1)][size_t(unit.position.y)] ==
          Tile::WALL) {
    jump = true;
  }
  UnitAction action;
  action.velocity = targetPos.x - unit.position.x;
  if (action.velocity >= 0.0) {
    action.velocity = std::max(action.velocity, game.properties.unitMaxHorizontalSpeed);
  } else {
    action.velocity = std::min(action.velocity, -game.properties.unitMaxHorizontalSpeed);
  }
  action.jump = jump;
  action.jumpDown = !action.jump;
  action.aim = aim;
  action.shoot = true;
  action.reload = false;
  action.swapWeapon = true;
  action.plantMine = false;
  return action;
}