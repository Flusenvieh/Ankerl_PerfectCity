# Game Idea
3rd Person PvPvE Hero Shooter

Goal: one controllable character with basic abilities + one very simple AI enemy, sync over multiple clients, at most basic UI 

(no collectibles, no equipped weapons -> abilities are per hero, no balancing, infinite reloading)

Useful Cheats: AbilitySystem.Debug.BasicHud, AbilitySystem.DebugAbilityTags

## Controls
 - WASD - Move
 - Mouse - Look
 - Left Mouse Button - Primary Fire
 - Left Shift - Heal
 - F - Melee
 - R - Reload
 ## Testing
 Manually connecting to IP (needed for standalone build multiplayer) caused issues (both in test build & editor), so best way to test for now is in Editor:
 - Open Content/Maps/GameMap in Editor
 -  Play Settings (3 dots next to Play in Editor button)
	 - Number of Players -> 2 (probably best for testing)
	 - Net Mode ->Play as Listen Server
	 - (Advanced Settings) 
		 - (Multiplayer Options/Run Under One Process -> False) (solved some issues for me, but loads slower)
		 
Play now opens a client with a server (in background) + additional clients (if specified) that are already connected to server
# Code
All code files + most classes have the prefix "PC" (initials of project).

Most classes separate declarations (public) and implementation (private) as is common practice, with the exception of two that contain basic definitions: PCGameplayTags (defines GameplayTags to use in C++ code) and PCLogChannels (defines categories for logging that are used throughout the project).
## Character + Player
PCCharacter contains a base character class (APCCharacter) that can be posessed both by AI and players. It holds stats for health, grants initial gameplay abilities and effects, as well as the AbilitySystemComponent necessary to handle those. For characters intended to be posessed by players, the derived class in PCPlayerCharacter adds camera and input support, as well as the combat attribute set. PlayerController only applies the MappingContext with keybindings.
## Attributes
AttributeSetHelpers contains a macro from a comment in AttributeSet.h (Engine) that generates basic getters and setters for individual Attributes within a Set. 
There is a base AttributeSet class that overrides functions called before attributes are changed by effects to call a function intended to be overriden by child classes for clamping attributes. It also declares a general delegate to be triggered when specific attributes change.

For now, two actual sets are defined: one that handles health stats (PCHealthAttributeSet) that is intended to be used for all entities (players & enemies) that can receive damage, and one for combat (PCCombatAttributeSet, currently handles only ammo) that was intended for player characters only at this stage.
## Input
Contains two DataAssets that map GameplayTags for InputActions and GameplayAbilities to bind/grant when initializing the Character.
## GameplayAbility
Remnant from experimenting with custom logic for Ability Costs, kept in for future extension (e.g. items) as well as potential for composing costs in the ability itself instead of a highly specialized GameplayEffect.
# Mechanics
## Abilities - Ghoul
Death blocks all abilities + movement, look + heal are exception.
### Primary Weapon: Simple Shot
(Planned as Spit Attack)

Fires a hitscan projectile that consumes ammo, dealing damage on hit. Raycast through reticle (camera pos -> cam forward), then cross-check from firing position (head, both are debug visualized). If not enough ammo, trigger reload instead.
### Offensive Ability: Melee
Punch that hits enemies within a certain radius (AoE) of the fist (green gizmo/debug sphere). Has cooldown.
### Defensive Ability: Heal
(Ghouls are sturdy, so death is just a minor inconvenience)

Heal over time, with cooldown. Can revive player.
### Reload
Reload all ammo. Blocks all other abilities.

## AI - Skelly
Skelly patrols randomly selected points in his range until being hit - then he chases after the player, hitting them with a simple Melee GameplayAbility when in range. When losing LOS for x seconds, he forgets the player and resumes patrolling. Despawns on death, no respawn.
## Multiplayer/Replication
Currently, the Host is both Server & Client with the option of other Clients connecting to it. Unreal handles replication of movement, GameplayAbilities are set to "Local Predicted" so it executes on the Client and Server (the latter should be Authority, but there are probably checks missing to ensure this). Attributes in AttributeSets have ReplicateUsing attributes, the specified callbacks use a macro to set the value.
## Input/Ability triggering
(Probably the most modular/data-driven input system I have ever programmed)

Every Character grants itself GameplayAbilities that can be identified and triggered with their GameplayTag, doesn't matter if they are a Player or AI. InputActions are bound to a function (APCPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)) that tries to invoke a GameplayAbility with a specific GameplayTag, AI can use a Task that takes the GameplayTask as parameter.

# Known Issues
- Auto-reload doesn't get triggered
- Missing Animations
- Death State sometimes bugged
- Skelly's vision seems bugged (doesn't forget player after losing LOS, so resumes chase after restoring LOS)
## Future ideas/plans
Short-term:
- UI! - Health bars, Status effects, Cooldowns,...
- Initial GameplayEffect to override hardcoded initial stat values
- Animation-driven abilities - play montage that trigger gameplay effect responsible for hit detection

Mid-term:
- Dedicated Server - matchmaking, improve performance (cosmetic effects on clients only), Authority independent from singular Player/Host
- Skelly respawn

Long-Term:
- Adjust player character for AI possession (e.g. Training Area)
- Make "contract" for stats via interface (e.g. IDamageable) - when dealing with non-humanoid pawns or multiple base characters, guarantees certain attribute sets while keeping hierarchies flat
