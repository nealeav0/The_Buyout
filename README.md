# Team Serendipity: The Buyout
##### Welcome to our **Final Game Project**: _**The Buyout**_!
Our finalized, working code is stored on our `codefreeze` branch.
Our Alpha Report can be found here: https://docs.google.com/document/d/1JS10h2ct08d1U--IQM-fSsFgsuIOpkfjNbIW_d2dNGE/edit?usp=sharing

## Game Overview
Our game is a 3D RPG that has a dialogue system along with complex combat mechanics such as status effects, elemental damage, and skill trees. The game plotline is centered on the buyout of Jimbo Fisher (former Texas A&M football coach). The player will experience complex battles with varied enemies, plot driven gameplay via the dialogue system, and ends with Jimbo Fisher himself.

An outline/transcript of the dialogue can be found here: https://docs.google.com/document/d/1T4mgx-ijddAEV-dop8nFy1PdC_i_rgj1C3YNNTaEths/edit?usp=sharing

### Player Controls
| Key | Action |
| ------ | ------ |
| W | Forward |
| A | Left |
| S | Backward |
| D | Right |
| Mouse | Move to Pan Camera |
| P | Open Pause Menu |
| U | Open Ability Upgrades Menu |

> The above controls are only for the overworld. In combat, the player is can only click on the UI displayed. 

## Feature Implementations

### Game World
Our game world is a 3D overworld where players interact with dialogue volumes and enemies. Its design is Aggie-inspired to fit the storyline of our game.

### Gameplay Loop
The core gameplay loop of our game is that the player walks around in the overworld, encounters enemies, battles the enemies, receives XP/AP upon defeating them, levels up abilities, and battles stronger enemies. This all builds up to a final interaction with Jimbo Fisher which is the win condition for the game. We make this basic loop interesting with complex combat mechanics (such as status effects and elemental damage) as well as a skill tree for various types of abilities distributed amongst the three unique players.

###  Multiple States
There are many states throughout the game that affect gameplay. Some states include overworld vs battle; player turn vs enemy turn (in battle); pause menu vs in game (in overworld); alive vs dead (for each player/enemy). For instance, switching between the main menu’s gamemode to the overworld’s gamemode results in switching input mode. Similarly for transitions between the overworld and pause menu, or between the overworld’s gamemode and the battle gamemode.

### Player Interactions
In the overworld, our player character has basic walking and idle animations. In all of the menus, the player can see the highlighted buttons they are hovering over or make choices through clicking dialogue options. In the battle HUD, players are limited in their interaction choices with buttons that are enabled/disabled based on the gameplay.

### GUIs
The game can be split into three main parts/modes: the main menu, the battle scene, and the overworld. Each of these has its own UI elements that interface with the underlying game logic.

##### Main Menu
The main menu has very basic functionality, with the only features being the title of the game, a play button to begin the game / enter the overworld, a quit button to close the game, and a list of keyboard basic controls for the player. Of course, the player has no keyboard mobility and only has mouse-controls to interact with the UI.

##### Overworld
The overworld movement itself doesn’t require any UI as the player is just moving through the scene and interacting with enemies. The player controls the character via WASD and doesn’t require mouse input. There are three cases when we have implemented UI for the overworld: the dialogue system, ability upgrades, and the pause menu.

- The Ability Upgrade UI can be triggered by pressing “U” on the overworld. This will pause the game and bring up a list of player characters (it also enables cursor control). From here, the player can click on a character to bring up a list of abilities. Then, by clicking on an ability, you can see the required AP cost to upgrade/learn the ability. There is an upgrade button that will attempt to learn/upgrade the ability if you have enough ability points. Additionally, there is an exit button to return to the game.

- The pause menu also pauses the game and blurs the background. The player can toggle audio, resume the game, or quit. Once again, the player has mouse control when in this menu.

- The dialogue widget triggers once the player enters a certain part of the map. There is mouse control while interacting with the dialogue pop up. While the player is interacting with the dialogue portion of the game, gameplay is paused. Also, the responses from the system are based on the answers that the player chooses while interacting with the widget.

##### Battle HUD
The Battle UI is automatically displayed when the player is in battle mode, with only mouse-controls to interact with the UI. Its layout can be broken can be broken down into the following components:

- A Turn Label at the top that updates on every turn, displaying whether it is the player or enemy’s turn to move.

- Two EntityStats widgets, one on the left for player party members and one on the right for the enemy party members. They display each corresponding entity’s name, level, and health. This widget updates every move to reflect changes to each entity’s health.

    - The EntityStats widget itself consists of three EntityStatSlot widgets per each party’s entities.

- An Actions Bar at the bottom consisting of a Party and Escape button.

    - Pressing Escape allows the player to ‘run away’, leaving the game and transitioning back to the overworld.

    - When the Party button is pressed, a PartySelect widget is shown with 3 buttons corresponding to each party member.

    - When a party member is chosen, a dynamically updated ActionsSelect widget consisting of ActionButtons is shown and reflects all of that party member’s moves for each button. It is possible to change party members at this point, the shown actions will reflect these changes immediately.

        - When the player hovers over any of these ActionButtons (not Defend), the ability’s stats and description is shown in a tooltip widget.

    - Once an ability/ActionButton is selected, a dynamically updated TargetsSelect widget is shown if the player is allowed to choose a target for their move. For healing or support moves, a list of party members is shown. Similarly, for attack moves, a list of enemy members is shown. In the case of a multi-target move, the list does not show up at all and the move is immediately registered.

    - Otherwise, moves are only registered and sent to the BattleManager when a target is selected from TargetsSelect.

    - Any of these buttons can be disabled anytime as according to the gameplay. For instance, if an enemy is dead, the player can no longer choose that player as a target, so the respective button(s) is/are disabled. Similarly, if an ability is on cooldown, its button will be temporarily disabled, or if a party member is on cooldown (you can only play them once per player/enemy turn), that party member’s button from PartySelect is also temporarily disabled until the next turn.

- Four Buffs widgets that are initially hidden/empty on initialization, but show the buffs/debuffs for each entity above their position on the screen if they are affected by their opponent’s moves. It only shows the active buff stats, so if nothing is visible, that means the entities have not been affected. There is one Buffs widget for each enemy and the player only has one BuffStats widget above their head at all times, but it updates every time a party member is selected to reflect the corresponding stats.

    - These Buffs widgets consist of BuffStat widgets which actually reflect each buff type and its stat and they are added/removed dynamically throughout gameplay.

### Sound
Our game has two forms of sound: persistent background music and sound effects when the player interacts with some of the UI.
When the player starts the game, we begin playing the overworld background music, which continues playing through the level transition and into the overworld. Only when the player is in battle mode does the overworld music stop playing, with a different battle background music playing instead. If the player chooses to toggle the audio off, it mutes both the overworld and the battle music until the player toggles the audio on.
In the main, pause, and battle menus, there are sound effects when the player clicks on any button.

### Win/Lose Conditions
The core functionality of our game resides in the battle/combat system. Each time a player encounters an enemy, they engage in a battle and attempt to kill the enemies before being defeated themselves. When you win a battle, you are awarded experience/ability points. You are also working towards the final goal of achieving a $77.6 million buyout which will trigger a final dialogue and boss fight. Collecting all of the money for the buyout is the required win condition for the game.

### Advanced Feature - Advanced Software Systems (Combat + Skill Tree)
We really focused on developing a flexible and robust combat system for the advanced feature in our project. One part of this includes an ability skill tree where players have a multitude of abilities to learn and level up throughout the game. The player must make decisions on where to spend their ability points and thus which abilities to prioritize. Every ability has an array of AP (ability point) costs for each level and also stores its current/max level. Ability points are gained through battle, and abilities are upgraded in the overworld. Upgrading the abilities allows the player to fight stronger enemies and further level new abilities.

Combat is divided into distinct states: Begin Round, Player Turn, Player-to-Enemy, Enemy Turn, Enemy-to-Player, End Round. In these states, specific conditions are checked. For instance, burn and poison damage occurs only during the end of a side’s turn. Additionally, ability cooldowns are decremented at the end of each round. By keeping the combat loop into separate states, it offers greater flexibility for scripting.

Under the hood, Entities and Abilities are organized into structs that contain essential information opposed to using classes. Because pointers lose their connections during level changes, it was essential to have persistent storage for data. Furthermore, Player/Enemy stats and ability power is determined by their levels. Entities have “level 0” base stats which are used to calculate the stats used in combat. On the other hand, Abilities use their levels as indexes for their current power and AP upgrade threshold. 

Status Effects were another way to add complexity to combat. Although if given enough time, there would be more than 4 at the moment. Burns deal 100% fire magic damage. Poison deals 100% poison magic damage. Chill increases the damage of ice attacks by 50%, but fire damage removes the status.. Stun simply prevents an entity from taking an action. At a grander scale, status effects were meant to encourage player strategies as carefully executing a sequence of moves would be more effective than randomly choosing. For instance, by applying chill to an enemy, one could simply take advantage of the ice amplification to deal extra damage.

Elemental damage also was supposed to add more depth. Instead of simply attacking the enemies, it was also important to recognize their elemental strengths and weaknesses. If an enemy was immune to fire, then 100% fire abilities would not damage it. Conversely, if an enemy was weak to ice abilities, then the player should be encouraged to use ice abilities. Elements were meant to act as a gateway to player equipment, but that is beyond the scope of this game for now.

## Division of Labor

### Harini
- Created the dialogue system, including the dialogue widgets, and built the general flow of the dialogue.

### Ameya
- Worked on combat system, specifically on status effects and elemental damage types.
- Created ability upgrade UI for overworld and interfaced with skill tree/ability data

### Audrey
- Created the Main Menu, Battle HUD, Pause Menu, Audio along with their components and functionalities
- Worked on player controls, player and enemy behaviors in overworld and battle modes (player and enemy “animations”, spawning, camera animations, data between game modes and game instance)

### Neal
- Designed the combat loop, abilities, and stat system.
- Initialized all the abilities and stats of players and enemies.
- Initialized enemy spawn placements and enemy starting levels.
BattleManager and AbilityManager.

### Blake
- Story writing, narrative integration
- Map design and asset development
