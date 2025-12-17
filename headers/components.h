#ifndef COMPONENT_H
#define COMPONENT_H

#define MAX_NAME_LEN 64        // max length of names
#define MAX_TYPE_LEN 4         // max length of type name
#define MAX_ATTACK_COUNT 4     // max number of attacks per bot
#define MAX_COMPONENT_COUNT 4  // max number of compenents attached to a bot
#define MAX_STATS_COUNT 8      // max number of stats assigned per component

typedef struct {
    char name[MAX_NAME_LEN]; // e.g., "attack", "defense"
    int value;               // numeric stat value
} Stat;

typedef struct {
    char name[MAX_NAME_LEN]; // name of the attack
    int dmg;                 // base damage of attack
} Attack;

typedef struct {
    char name[MAX_NAME_LEN];     // string containing component name (e.g. "Electrified Arm", "X-Ray eye", etc.)
    char type[MAX_TYPE_LEN];     // string containing component type (e.g. "eye", "processor", "arm", etc.)
    int lvl;                     // level of component, i.e. the times it has been upgraded
    Stat stats[MAX_STATS_COUNT]; // array of stats (e.g. "durability", "weight", "strength", "speed", etc.)
    Attack attacks[MAX_ATTACK_COUNT]; // array of all attacks allowed for a component
} Component;

typedef struct {
    char name[MAX_NAME_LEN];                   // name of bot
    Component components[MAX_COMPONENT_COUNT]; // list of all components attached onto a bot
    Attack attacks[MAX_ATTACK_COUNT];          // list of all attacks
    Stat stats[MAX_STATS_COUNT];               // sum of all stats of a bot
} Bot;

#endif
