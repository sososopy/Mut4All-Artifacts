# 1 "../src/Entity.cpp" 1
#if 0 /* expanded by -frewrite-includes */
#include "Entity.hpp"
#endif /* expanded by -frewrite-includes */
# 1 "../src/Entity.hpp" 1
/// Entities are persos, monsters and objects that can be destroyed or collected
/**
 * Multiple entities cannot be on the same square at the same moment.
 * All the entities can be moved but Coin and BonusCoin
 */

#ifndef ENTITY_HPP
#define ENTITY_HPP

#if 0 /* expanded by -frewrite-includes */
#include "Element.hpp"
#endif /* expanded by -frewrite-includes */
# 1 "../src/Element.hpp" 1
/// Element are all the physical elements that are drawed on the screen
/**
 * Derived classes: Entity, Mechanism and Tile.
 * It has a Position, and a method to know if it blocks the way.
 */

#ifndef ELEMENT_HPP
#define ELEMENT_HPP

class Element {
private:
	const unsigned id;
	//const unsigned width;
	unsigned x;
	unsigned y;
public:
	Element(unsigned id, unsigned col, unsigned row/*, unsigned width*/);
	/** Do nothing by default */
	void update();
	void set_position(unsigned x, unsigned y);
	unsigned get_id();
};

#endif // ELEMENT_HPP
# 11 "../src/Entity.hpp" 2

/**
 * N, S, E, W stands NORTH, SOUTH, EAST, WEST
 * Used in Entity.
 */
enum direction_t {
	NONE = 0,
	N = 1,
	S = 2,
	E = 3,
	W = 4
};

class Entity: public Element {
private:
	bool destroyed;
	direction_t direction;
public:
	using Element::Element;
	bool is_destroyed();
};

#endif // ENTITY_HPP
# 2 "../src/Entity.cpp" 2

Entity::Entity(unsigned id, unsigned col, unsigned row): Element(id, col, row) {
	this->direction = NONE;
}

bool Entity::is_destroyed() {
	return this->destroyed;
}

void Entity::update() {
	if(this->direction != NONE) {
		unsigned new_x = this->x + 1;
		unsigned new_y = this->y + 1;
		
		if(new_x % 100 != 50 && new_y % 100 != 50) {
			this->x = new_x;
			this->y = new_y;
		}
	}
}
