\# Land of Souls



An action RPG built in Unreal Engine 5, using the \*\*Gameplay Ability System (GAS)\*\* as the core framework for all combat, character stats, ability management, and game state communication. Developed by \*\*Noctifuge\*\*.



All C++ types use the `LOS` prefix (\*Land of Souls\*) to keep the codebase clean and collision-free across a large project.



> \*\*Status: Active Development\*\* — Core systems including GAS integration, the full damage pipeline, character hierarchy, input routing, weapon equip/unequip, and the animation framework are functional. Enemy AI, ranged combat, additional weapon types, and full UI polish are still being built out. Architecture documented here reflects the current state of the codebase.



\---



\## Table of Contents



\- \[What Is GAS](#what-is-gas)

\- \[The Five Core Pillars of GAS](#the-five-core-pillars-of-gas)

\- \[How GAS Is Integrated Into Land of Souls](#how-gas-is-integrated-into-land-of-souls)

\- \[Project Structure](#project-structure)

\- \[Complete File Reference](#complete-file-reference)

\- \[System Deep Dives](#system-deep-dives)

&#x20; - \[LOS Types — Enums, Structs, Tags, Debug](#los-types--enums-structs-tags-debug)

&#x20; - \[Character Hierarchy](#character-hierarchy)

&#x20; - \[Interfaces](#interfaces)

&#x20; - \[Ability System](#ability-system)

&#x20; - \[Attribute Set and Damage Pipeline](#attribute-set-and-damage-pipeline)

&#x20; - \[Combat Components](#combat-components)

&#x20; - \[Weapon System](#weapon-system)

&#x20; - \[Input System](#input-system)

&#x20; - \[UI Components and Widgets](#ui-components-and-widgets)

&#x20; - \[StartUp Data Assets](#startup-data-assets)

&#x20; - \[Animation System](#animation-system)

&#x20; - \[Function Library](#function-library)

&#x20; - \[Game Mode and Controller](#game-mode-and-controller)

\- \[Design Patterns Used](#design-patterns-used)

\- \[Gameplay Tag Reference](#gameplay-tag-reference)

\- \[Getting Started](#getting-started)



\---



\## What Is GAS



The \*\*Gameplay Ability System\*\* is a first-party Unreal Engine plugin originally built for \*Fortnite\*. It solves a problem that every action game eventually runs into — how do you make systems like combat, health, input, animations, and UI talk to each other cleanly as the project grows?



Without GAS, the typical result looks something like this spread across your character class:



```cpp

if (bIsAttacking \&\& !bIsStunned \&\& !bIsDead \&\& HasEnoughStamina() \&\& !bIsInHitReact)

{

&#x20;   Target->Health -= Damage;

&#x20;   PlayMontage(...);

&#x20;   UpdateHealthBar();

&#x20;   // ... and five more things

}

```



Every new system you add breaks something else. GAS replaces this with a well-defined ability lifecycle, tag-based state communication, and a data-driven effect system so each system only does its own job and reacts to what it needs through tags and delegates.



\---



\## The Five Core Pillars of GAS



Everything in the codebase maps back to one or more of these.



\*\*1. Ability System Component (ASC)\*\* — Lives on the character and owns everything: abilities, active effects, attributes, and gameplay tags. Every activation, query, and effect application goes through it. In this project it is `ULOSAbilitySystemComponent`.



\*\*2. Gameplay Abilities (GA)\*\* — Each ability (`ULOSGameplayAbility` and subclasses) is a self-contained class that defines when it can activate (via tag requirements), what it does, and when it ends. Abilities are granted to the ASC at runtime from data assets, not hard-coded into characters.



\*\*3. Gameplay Effects (GE)\*\* — The only way to modify attributes. Instant effects fire once (damage), Duration effects tick for a set time (DoT), Infinite effects persist until removed (passive buffs). All configured as data assets in the editor.



\*\*4. Gameplay Attributes\*\* — `FGameplayAttributeData` properties inside `ULOSAttributeSet`. GAS captures them during effect calculations, and `PostGameplayEffectExecute` is the single place any attribute change is intercepted and reacted to.



\*\*5. Gameplay Tags\*\* — Hierarchical string identifiers (`Parent.Child.Grandchild`) that replace boolean flags entirely. Instead of `bIsAttacking`, a tag `Player.Ability.Attack.Light.Sword` is active on the ASC. Instead of `bIsDead`, the tag `Shared.Status.Dead` is applied. Abilities declare their requirements and blockers using tags, so state logic lives in data, not code.



\---



\## How GAS Is Integrated Into Land of Souls



\### Owner and Avatar are the same actor

In `ALOSBaseCharacter::PossessedBy`:

```cpp

LOSAbilitySystemComponent->InitAbilityActorInfo(this, this);

```

Both owner and avatar point to the character. This is the correct single-player setup. A networked game would place the ASC on the PlayerState, but that is unnecessary complexity here.



\### Abilities are granted from data assets, not code

Every character has a `TSoftObjectPtr<UDataAsset\_StartUpDataBase> CharacterStartUpData`. On possession, `GiveToAbilitySystemComponent()` runs on that asset and grants every ability and applies every startup effect. Adding or removing an ability from a character requires no code change — just editing the data asset.



\### Input routes to abilities through tags

When the player presses a button, `ULOSInputComponent` fires a callback with a `FGameplayTag`. That tag hits `ULOSAbilitySystemComponent::OnAbilityInputPressed`, which scans all activatable abilities for one whose dynamic spec source tags match, and calls `TryActivateAbility`. No input handler ever references an ability class directly.



\### Weapon abilities are granted on equip, removed on unequip

When the hero equips a sword, `GrantHeroWeaponAbility` on the ASC grants each ability from the weapon's `FLOSHeroWeaponData.DefaultWeaponAbilities` array, stores the returned `FGameplayAbilitySpecHandle` values in `ALOSHeroWeapon::GrantedAbilitySpecHandles`, and the input tag for each ability is embedded in the spec's dynamic tags. On unequip, those handles are passed to `RemoveGrantedHeroWeaponAbility` and cleared. The hero can never use sword abilities when no sword is equipped.



\### Damage flows through an Execution Calculation

The hero's attack ability calls `MakeHeroDamageEffectSpecHandle` which builds a `FGameplayEffectSpecHandle` with `SetByCaller` magnitudes for base damage, attack type, and combo count. That spec is applied to the target's ASC. The Gameplay Effect runs `UGEExecCalc\_DamageTaken`, which captures `AttackPower` from the attacker and `DefensePower` from the target, applies combo scaling, and writes the final number into the `DamageTaken` meta-attribute. `PostGameplayEffectExecute` on `ULOSAttributeSet` then deducts it from `CurrentHealth` and tells the UI.



\---



\## Project Structure



```

Source/LandOfSouls/

├── Public/                          # Headers (.h) — API contracts

│   ├── AbilitySystem/

│   │   ├── Abilities/

│   │   │   ├── LOSGameplayAbility.h

│   │   │   ├── LOSHeroGameplayAbility.h

│   │   │   └── LOSEnemyGameplayAbility.h

│   │   ├── GEExecCalc/

│   │   │   └── GEExecCalc\_DamageTaken.h

│   │   ├── LOSAbilitySystemComponent.h

│   │   └── LOSAttributeSet.h

│   ├── AnimInstances/

│   │   ├── Hero/

│   │   │   ├── LOSHeroAnimInstance.h

│   │   │   └── LOSHeroLinkedAnimLayer.h

│   │   ├── LOSBaseAnimInstance.h

│   │   └── LOSCharacterAnimInstance.h

│   ├── Characters/

│   │   ├── LOSBaseCharacter.h

│   │   ├── LOSHeroCharacter.h

│   │   └── LOSEnemyCharacter.h

│   ├── Components/

│   │   ├── Combat/

│   │   │   ├── PawnCombatComponent.h

│   │   │   ├── HeroCombatComponent.h

│   │   │   └── EnemyCombatComponent.h

│   │   ├── Input/

│   │   │   └── LOSInputComponent.h

│   │   ├── UI/

│   │   │   ├── PawnUIComponent.h

│   │   │   ├── HeroUIComponent.h

│   │   │   └── EnemyUIComponent.h

│   │   └── PawnExtensionComponentBase.h

│   ├── Controllers/

│   │   └── LOSPlayerController.h

│   ├── DataAssets/

│   │   ├── Input/

│   │   │   └── DataAsset\_InputConfig.h

│   │   └── StartUpData/

│   │       ├── DataAsset\_StartUpDataBase.h

│   │       ├── DataAsset\_HeroStartUpData.h

│   │       └── DataAsset\_EnemyStartUpData.h

│   ├── GameModes/

│   │   └── LOSBaseGameMode.h

│   ├── Interfaces/

│   │   ├── PawnCombatInterface.h

│   │   └── PawnUIInterface.h

│   ├── Items/Weapons/

│   │   ├── LOSWeaponBase.h

│   │   └── LOSHeroWeapon.h

│   ├── LOS Types/

│   │   ├── LOSEnumtypes.h

│   │   └── LOSStructTypes.h

│   ├── Widgets/

│   │   └── LOSWidgetBase.h

│   ├── LOSDebugHelper.h

│   ├── LOSFunctionLibrary.h

│   └── LOSGameplayTags.h

│

└── Private/                         # Implementations (.cpp) — mirrors Public structure

```



\---



\## Complete File Reference



\### Public/ — 37 headers



| File | Purpose |

|---|---|

| `LOSEnumtypes.h` | `ELOSConfirmType`, `ELOSValidType`, `ELOSSuccessType` — output enums for Blueprint exec pins |

| `LOSStructTypes.h` | `FLOSHeroAbilitySet`, `FLOSHeroWeaponData` — shared structs used across the ability and weapon systems |

| `LOSGameplayTags.h` | Extern declarations for every gameplay tag in the project |

| `LOSDebugHelper.h` | Static `Debug::Print` helpers — on-screen text and `UE\_LOG` output |

| `LOSFunctionLibrary.h` | Static Blueprint-callable helpers for ASC queries, tag management, and interface lookups |

| `LOSAbilitySystemComponent.h` | Custom ASC — input routing, weapon ability grant and revoke |

| `LOSAttributeSet.h` | All attributes — health, rage, attack power, defense power, damage taken |

| `LOSGameplayAbility.h` | Base ability — activation policy, effect application, custom tag requirement override |

| `LOSHeroGameplayAbility.h` | Hero ability base — cached hero/controller refs, damage spec builder |

| `LOSEnemyGameplayAbility.h` | Enemy ability base — cached enemy ref and combat component access |

| `GEExecCalc\_DamageTaken.h` | Execution calculation — damage formula with combo scaling |

| `LOSBaseAnimInstance.h` | Root anim instance — common ancestor for the entire animation tree |

| `LOSCharacterAnimInstance.h` | Character anim — `GroundSpeed`, `bHasAcceleration`, threadsafe update |

| `LOSHeroAnimInstance.h` | Hero anim — idle elapsed time, `bShouldEnterRelaxState`, configurable relax threshold |

| `LOSHeroLinkedAnimLayer.h` | Linked layer base — back-reference to the primary hero anim instance |

| `LOSBaseCharacter.h` | Root character — owns ASC and AttributeSet, implements all three interfaces |

| `LOSHeroCharacter.h` | Player character — camera boom, Enhanced Input setup, hero-specific components |

| `LOSEnemyCharacter.h` | AI character — auto-possess, async startup data loading |

| `PawnCombatComponent.h` | Base combat — weapon registry (`TMap`), equipped weapon tag, collision toggle, `EToggleDamageType` |

| `HeroCombatComponent.h` | Hero combat — GAS event dispatch on hit, weapon damage curve query |

| `EnemyCombatComponent.h` | Enemy combat — stub, infrastructure inherited, ready for AI combat logic |

| `LOSInputComponent.h` | Enhanced input bridge — `BindNativeInputAction` and `BindAbilityInputAction` template functions |

| `PawnExtensionComponentBase.h` | Component base — `GetOwningPawn<T>()` and `GetOwningController<T>()` with static\_assert enforcement |

| `PawnUIComponent.h` | Base UI component — `FOnPercenteChaingedDelegate`, `OnCurrentHealthChanged` |

| `HeroUIComponent.h` | Hero UI component — `OnCurrentRageChanged` delegate |

| `EnemyUIComponent.h` | Enemy UI component — inherits health delegate, ready for enemy-specific UI events |

| `LOSPlayerController.h` | Player controller — stub ready for expansion |

| `DataAsset\_InputConfig.h` | Input data asset — mapping context, `FLOSInputActionConfig` arrays for native and ability inputs |

| `DataAsset\_StartUpDataBase.h` | Base startup data — passive abilities, reactive abilities, startup gameplay effects |

| `DataAsset\_HeroStartUpData.h` | Hero startup data — input-tagged ability sets via `FLOSHeroAbilitySet` array |

| `DataAsset\_EnemyStartUpData.h` | Enemy startup data — array of enemy combat ability classes |

| `LOSBaseGameMode.h` | Game mode — stub |

| `PawnCombatInterface.h` | `IPawnCombatInterface` — pure virtual `GetCombatComponent()` |

| `PawnUIInterface.h` | `IPawnUIInterface` — pure virtual `GetPawnUIComponent()`, default-null `GetHeroUIComponent()` and `GetEnemyUIComponent()` |

| `LOSWeaponBase.h` | Weapon actor base — static mesh, box collision, `FOnTargedIntractableDelegate` hit and pull delegates |

| `LOSHeroWeapon.h` | Hero weapon — `FLOSHeroWeaponData`, granted ability spec handle storage |

| `LOSWidgetBase.h` | Widget base — self-initializes via `IPawnUIInterface`, Blueprint-implementable init events |



\### Private/ — 35 implementations



| File | What is implemented |

|---|---|

| `LOSGameplayTags.cpp` | All `UE\_DEFINE\_GAMEPLAY\_TAG` definitions |

| `LOSStructTypes.cpp` | `FLOSHeroAbilitySet::IsValid()` |

| `LOSFunctionLibrary.cpp` | ASC lookup, tag add/remove/check, combat component retrieval via interface |

| `LOSAbilitySystemComponent.cpp` | `OnAbilityInputPressed`, `GrantHeroWeaponAbility`, `RemoveGrantedHeroWeaponAbility` |

| `LOSAttributeSet.cpp` | `PostGameplayEffectExecute` — clamping, UI broadcast, `Shared.Status.Dead` tag on zero health |

| `LOSGameplayAbility.cpp` | `OnGiveAbility` (OnGiven policy), `EndAbility` (OnGiven self-clear), effect application, tag requirement lambdas |

| `LOSHeroGameplayAbility.cpp` | Cached hero and controller getters, `MakeHeroDamageEffectSpecHandle` |

| `LOSEnemyGameplayAbility.cpp` | Cached enemy character getter, combat component getter |

| `GEExecCalc\_DamageTaken.cpp` | `FLOSDamageCapture` struct, `Execute\_Implementation` with combo scaling |

| `LOSBaseCharacter.cpp` | Constructor (ASC + AttributeSet creation, tick disabled, decals off), `PossessedBy`, interface stubs |

| `LOSHeroCharacter.cpp` | Camera boom, input binding, `PossessedBy` (synchronous startup load), movement and ability input handlers |

| `LOSEnemyCharacter.cpp` | Movement config, `InItEnemyStartUpData` (async load via `UAssetManager`) |

| `LOSCharacterAnimInstance.cpp` | `NativeInitializeAnimation`, `NativeThreadSafeUpdateAnimation` |

| `LOSHeroAnimInstance.cpp` | Idle timer accumulation, `bShouldEnterRelaxState` threshold check |

| `LOSHeroLinkedAnimLayer.cpp` | `GetHeroAnimInstance` via `GetOwningComponent()->GetAnimInstance()` |

| `LOSBaseAnimInstance.cpp` | Empty — base class only |

| `PawnCombatComponent.cpp` | `RegisterSpawnedWeapon`, `GetCharacterCarriedWeaponByTag`, `ToggleWeaponCollision` |

| `HeroCombatComponent.cpp` | `OnHitTargetActor` (overlap guard, two GAS events), weapon tag and damage queries |

| `EnemyCombatComponent.cpp` | Empty — stub |

| `LOSInputComponent.cpp` | Empty — template implementations live in the header |

| `PawnExtensionComponentBase.cpp` | Empty — template implementations live in the header |

| `PawnUIComponent.cpp` | Empty — delegate declared and usable from header |

| `HeroUIComponent.cpp` | Empty — delegate declared in header |

| `EnemyUIComponent.cpp` | Empty — inherits from base |

| `LOSPlayerController.cpp` | Empty — stub |

| `DataAsset\_InputConfig.cpp` | `FindNativeInputActionByTag` linear scan |

| `DataAsset\_StartUpDataBase.cpp` | `GiveToAbilitySystemComponent`, `GrantAbilities` |

| `DataAsset\_HeroStartUpData.cpp` | Override — embeds input tags into ability specs before granting |

| `DataAsset\_EnemyStartUpData.cpp` | Override — grants `EnemyCombatAbilities` array without input tags |

| `LOSWeaponBase.cpp` | Constructor (mesh, collision box, delegate bindings), `OnCollisionBoxBeginOverlap`, `OnCollisionBoxEndOverlap` |

| `LOSHeroWeapon.cpp` | `AssignGrantedAbilitySpecHandle`, `GetGrantedAbilitySpecHandles` |

| `LOSStructTypes.cpp` | `FLOSHeroAbilitySet::IsValid()` |

| `LOSWidgetBase.cpp` | `NativeOnInitialized`, `InitEnemyCreatedWidget` |

| `PawnCombatInterface.cpp` | Empty — pure virtual interface |

| `PawnUIInterface.cpp` | Default implementations — `GetHeroUIComponent()` and `GetEnemyUIComponent()` return nullptr |

| `LOSBaseGameMode.cpp` | Empty — stub |



\---



\## System Deep Dives



\### LOS Types — Enums, Structs, Tags, Debug



These four files are the foundation everything else builds on.



\*\*`LOSEnumtypes.h`\*\* defines three output enums used exclusively for `ExpandEnumAsExecs` on Blueprint functions — giving Blueprint nodes separate execution output pins instead of a single flow pin. `ELOSConfirmType` (Yes/No) is used for tag checks, `ELOSValidType` (Valid/Invalid) for component lookups, and `ELOSSuccessType` (Successful/Failed) for effect application. None of these appear in any logic — they exist purely to make Blueprint graphs cleaner to read.



\*\*`LOSStructTypes.h`\*\* defines two structs. `FLOSHeroAbilitySet` pairs a `TSubclassOf<ULOSHeroGameplayAbility>` with a `FGameplayTag` and has an `IsValid()` method that checks both fields. `FLOSHeroWeaponData` is the more important one — it holds the weapon's linked anim layer class (`WeaponAnimLayerToLink`), an input mapping context for this weapon's inputs (`WeaponInputMappingContext`), an array of `FLOSHeroAbilitySet` for the weapon's abilities (`DefaultWeaponAbilities`), and the weapon's damage as a `FScalableFloat` curve (`WeaponBaseDamage`). This struct makes each hero weapon self-describing — the equip ability reads everything it needs from here.



\*\*`LOSGameplayTags.h/.cpp`\*\* declares and defines every gameplay tag in the project inside a `LOSGameplayTags` C++ namespace using `UE\_DECLARE\_GAMEPLAY\_TAG\_EXTERN` in the header and `UE\_DEFINE\_GAMEPLAY\_TAG` in the `.cpp`. This approach gives compile-time safety (a typo is a compile error, not a silent runtime failure) and a single authoritative file for every tag the game uses. See \[Gameplay Tag Reference](#gameplay-tag-reference) for the full list.



\*\*`LOSDebugHelper.h`\*\* is a pure header with two overloaded static functions in a `Debug` namespace. `Debug::Print(FString, FColor, int32 Key)` prints to screen via `GEngine->AddOnScreenDebugMessage` and logs via `UE\_LOG`. `Debug::Print(FString Title, float Value, int32 Key, FColor)` formats a float with a label. The optional `Key` parameter allows a message to overwrite a specific on-screen slot rather than stacking — critical for values that change every frame. There is no `.cpp` because there is nothing to compile; it is included directly wherever needed.



\---



\### Character Hierarchy



\*\*`ALOSBaseCharacter`\*\* is the root of all characters. The constructor creates `ULOSAbilitySystemComponent` and `ULOSAttributeSet` as subobjects, sets `bCanEverTick = false` and `bStartWithTickEnabled = false` (none of the core systems need tick — they are event-driven), and disables decals on the mesh. In `PossessedBy`, `InitAbilityActorInfo(this, this)` initializes the ASC with this actor as both owner and avatar. An `ensureMsgf` fires in development if `CharacterStartUpData` is null — catching a common designer oversight before it becomes a runtime crash. The class implements `IAbilitySystemInterface` so GAS can find the ASC from any actor reference, and stubs `IPawnCombatInterface` and `IPawnUIInterface` with null returns that subclasses override.



\*\*`ALOSHeroCharacter`\*\* builds the full player character on top of the base. The constructor configures a spring arm at 200cm with a 55cm Y and 65cm Z socket offset, attaches the follow camera, sets movement to orient-to-movement with a 500 deg/s rotation rate and 600cm/s walk speed, and creates `HeroCombatComponent` and `HeroUIComponent`. In `SetupPlayerInputComponent`, the `UEnhancedInputLocalPlayerSubsystem` is fetched from the local player, the mapping context from `InputConfigDataAsset` is added at priority 0, and `ULOSInputComponent` binds move and look natively and all ability actions with a single `BindAbilityInputAction` call. Startup data is loaded synchronously with `CharacterStartUpData.LoadSynchronous()` — acceptable for a hero character whose data asset will always be resident.



\*\*`ALOSEnemyCharacter`\*\* sets `AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned` so any AI controller works for both level-placed and runtime-spawned enemies. Movement is slower at 300cm/s with a 180 deg/s rotation rate. The critical difference from the hero is `InItEnemyStartUpData`, which uses `UAssetManager::GetStreamableManager().RequestAsyncLoad` with a lambda callback. The lambda captures `this` and, when the load completes, calls `GiveToAbilitySystemComponent` on the loaded data. This is the right pattern for enemies — there can be dozens in a level and you never want startup data loads to stall the game thread.



\---



\### Interfaces



Both follow Unreal's two-class interface pattern.



\*\*`IPawnCombatInterface`\*\* has one pure virtual method: `GetCombatComponent() const = 0`. Any actor that exposes a combat component implements this. `ULOSFunctionLibrary::NativeGetPawnCombatComponentFromActor` casts any `AActor\*` to this interface and calls `GetCombatComponent()`. Abilities and systems that need the combat component never know if they are talking to a hero or an enemy.



\*\*`IPawnUIInterface`\*\* has one pure virtual method (`GetPawnUIComponent() const = 0`) and two virtual methods with default implementations that return nullptr — `GetHeroUIComponent()` and `GetEnemyUIComponent()`. This is deliberate. The base character does not need to implement the hero or enemy variants. Code that calls `PawnUIInterface->GetHeroUIComponent()` is safe on any character — it returns nullptr for enemies. `ULOSAttributeSet` caches a `TWeakInterfacePtr<IPawnUIInterface>` so it can broadcast UI events without knowing the concrete character type.



\---



\### Ability System



\*\*`ULOSAbilitySystemComponent`\*\* adds three methods to the engine ASC.



`OnAbilityInputPressed(FGameplayTag InInputTag)` iterates all activatable abilities and calls `TryActivateAbility` on any spec whose `DynamicSpecSourceTags` contain the exact input tag. Dynamic source tags (rather than static asset tags on the class) mean the same ability class can be bound to a different input on a different weapon just by granting it with a different tag.



`GrantHeroWeaponAbility` creates a `FGameplayAbilitySpec` for each entry in the `FLOSHeroAbilitySet` array, sets the avatar as source object and the input tag in `DynamicSpecSourceTags`, calls `GiveAbility`, and stores the returned `FGameplayAbilitySpecHandle` in the output array. Those handles are the key to clean revocation on unequip.



`RemoveGrantedHeroWeaponAbility` iterates the handles, calls `ClearAbility` on each valid one, and empties the array. Called during weapon unequip — zero ability leaks.



\---



\*\*`ULOSGameplayAbility`\*\* is the base for every ability. The most important addition is `ELOSAbilityActivationPolicy`:



```cpp

enum class ELOSAbilityActivationPolicy : uint8

{

&#x20;   OnTriggered,  // activates on input or gameplay event

&#x20;   OnGiven       // activates immediately when granted to the ASC

};

```



`OnGiven` is used for always-on abilities — death handlers, hit-react listeners, passive buffs that should activate the moment the character exists. In `OnGiveAbility`, if the policy is `OnGiven` and the spec is not already active, `TryActivateAbility` fires immediately. In `EndAbility`, if `OnGiven`, `ClearAbility` removes the spec from the ASC — a one-shot passive does not need to remain in the activatable list.



`NativeApplyEffectSpecHandleToTarget` resolves the target's ASC, asserts validity, and calls `ApplyGameplayEffectSpecToTarget`. `BP\_ApplyEffectSpecHandleToTarget` wraps this with an `ELOSSuccessType` output for Blueprint exec pins.



`DoesAbilitySatisfyTagRequirements` is a full custom override of the engine's check. Two lambdas separately collect blocked tags and missing required tags into `OptionalRelevantTags`. This gives precise failure reasons for debugging and enables clear UI feedback about why an ability did not activate.



\---



\*\*`ULOSHeroGameplayAbility`\*\* lazily caches `ALOSHeroCharacter` and `ALOSPlayerController` via `TWeakObjectPtr` — weak pointers because abilities can outlive the character in edge cases and a weak pointer becomes invalid cleanly. Both getters check validity before returning.



`MakeHeroDamageEffectSpecHandle` builds a `FGameplayEffectContextHandle` with the ability, avatar as source object, and avatar as both instigator and effect causer. It calls `MakeOutgoingSpec`, then sets two `SetByCaller` magnitudes: `Shared.SetByCaller.BaseDamage` with the weapon damage at the current ability level, and either `Player.SetByCaller.AttackType.Light` or `Player.SetByCaller.AttackType.Heavy` with the combo count. The spec is self-contained — the damage calculation receives everything it needs without reaching outside.



\*\*`ULOSEnemyGameplayAbility`\*\* mirrors the hero base. It lazily caches `ALOSEnemyCharacter` and provides `GetEnemyCombatComponentFromActorInfo()` by chaining through the cached character. Enemy abilities inherit from this and get direct access to their character and combat component with no manual casting needed.



\---



\### Attribute Set and Damage Pipeline



\*\*`ULOSAttributeSet`\*\* defines seven attributes with `ATTRIBUTE\_ACCESSORS`, which generates a property getter, value getter, value setter, and value initializer per attribute:



| Attribute | Category | Role |

|---|---|---|

| `CurrentHealth` | Health | Active HP — clamped 0 to MaxHealth |

| `MaxHealth` | Health | HP cap |

| `CurrentRage` | Rage | Active rage — clamped 0 to MaxRage |

| `MaxRage` | Rage | Rage cap |

| `AttackPower` | Damage | Source multiplier in the damage formula |

| `DefensePower` | Damage | Target divisor in the damage formula |

| `DamageTaken` | Damage | \*\*Meta-attribute\*\* — receives computed damage, has no persistent value |



All attributes initialize to `1.f`. Real starting values come from startup gameplay effects applied on possession.



`DamageTaken` is a meta-attribute — a write-only landing pad for the execution calculation. It has no meaning between frames. `PostGameplayEffectExecute` intercepts it and applies the value as a health reduction, keeping all damage-to-health logic in one place.



`PostGameplayEffectExecute` lazily caches a `TWeakInterfacePtr<IPawnUIInterface>` on first call and asserts it is valid. Then it handles three cases. On `CurrentHealth` change: clamps to \[0, MaxHealth] and broadcasts `OnCurrentHealthChanged` with a normalized float. On `CurrentRage` change: clamps and broadcasts `OnCurrentRageChanged` on `HeroUIComponent` if it exists (null-checked since enemies have no rage). On `DamageTaken` change: subtracts from `CurrentHealth`, clamps, rebroadcasts health to the UI, and if health hits exactly zero, calls `ULOSFunctionLibrary::AddGameplayTagToActorIfNone` with `Shared.Status.Dead` — marking the actor dead through the tag system rather than calling death logic directly from the attribute set.



\---



\*\*`UGEExecCalc\_DamageTaken`\*\* is the damage formula. The file-local `FLOSDamageCapture` struct registers three attribute captures via `DECLARE\_ATTRIBUTE\_CAPTUREDEF` and `DEFINE\_ATTRIBUTE\_CAPTUREDEF`: `AttackPower` from the source, `DefensePower` from the target, and `DamageTaken` from the target. All three are added to `RelevantAttributesToCapture` in the constructor.



`Execute\_Implementation` reads from the spec via `SetByCallerTagMagnitudes`: `Shared.SetByCaller.BaseDamage` as `BaseDamage`, `Player.SetByCaller.AttackType.Light` as `UsedLightAttackComboCount`, `Player.SetByCaller.AttackType.Heavy` as `UsedHeavyAttackComboCount`.



Combo scaling is applied before the formula:

\- Light combo: `BaseDamage \*= (ComboCount - 1) \* 0.05 + 1.0` — 5% per additional hit

\- Heavy combo: `BaseDamage \*= ComboCount \* 0.15 + 1.0` — 15% per hit



Final formula:

```

FinalDamage = BaseDamage \* AttackPower / DefensePower

```



Written as an `Override` modifier to `DamageTaken`. `PostGameplayEffectExecute` picks it up and deducts it from health.



\---



\### Combat Components



\*\*`UPawnExtensionComponentBase`\*\* provides two template helpers with compile-time enforcement. `GetOwningPawn<T>()` uses `static\_assert(TPointerIsConvertibleFromTo<T, APawn>::Value)` — if `T` does not derive from `APawn`, the code will not compile. `GetOwningController<T>()` chains through the pawn to `GetController<T>()` with the same enforcement. This eliminates boilerplate and prevents accidental misuse in every component that extends this class.



\*\*`UPawnCombatComponent`\*\* owns the weapon registry — a `TMap<FGameplayTag, ALOSWeaponBase\*>` called `CharacterCarriedWeaponMap`. Weapons are identified by tag, not index. `CurrentEquippedWeaponTag` tracks which is currently equipped. `OverlappedActors` tracks which actors have been hit in the current swing.



`RegisterSpawnedWeapon` asserts the tag is not already registered, stores the weapon, binds its `OnWeaponHitTarget` and `OnWeaponPulledFromTarget` delegates to the component's virtual handlers, and optionally sets the weapon as the current equipped weapon.



`ToggleWeaponCollision` enables or disables the hitbox. The `EToggleDamageType` enum (`CurrentEquippedWeapon`, `LeftHand`, `RightHand`) allows future expansion — only `CurrentEquippedWeapon` is handled today. On disable, `OverlappedActors` is cleared so the next swing starts fresh.



\*\*`UHeroCombatComponent`\*\* adds the GAS integration. `OnHitTargetActor` checks `OverlappedActors` (same target, same swing = skip), adds the actor to `OverlappedActors`, then sends two events via `UAbilitySystemBlueprintLibrary::SendGameplayEventToActor`:

\- `Shared.Event.MeleeHit` with the hit actor as target — the attack ability waits for this to apply the damage effect

\- `Player.Event.HitPause` — triggers the hit pause / frame-freeze ability



`OnWeaponPulledFromTargetActor` also sends `Player.Event.HitPause`. This is marked for refactoring in the code — it is a copy from the hit handler and should likely be its own event.



`GetHeroCurrentEquippedWeaponDamageAtLevel` returns `HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel)` — damage scales with ability level via the curve.



\*\*`UEnemyCombatComponent`\*\* is an empty stub. All weapon registration and collision infrastructure is inherited from `UPawnCombatComponent` and ready to use. Enemy-specific hit logic will be added as AI combat develops.



\---



\### Weapon System



\*\*`ALOSWeaponBase`\*\* is a minimal actor with a `UStaticMeshComponent` as root and a `UBoxComponent` hitbox. Both have collision disabled on construction. Two single-cast delegates are the entire external interface:



```cpp

FOnTargedIntractableDelegate OnWeaponHitTarget;

FOnTargedIntractableDelegate OnWeaponPulledFromTarget;

```



`OnCollisionBoxBeginOverlap` gets the weapon's instigator pawn via `GetInstigator<APawn>()`, confirms the overlapping actor is a different pawn (self-hit prevention), and fires `OnWeaponHitTarget.ExecuteIfBound`. `OnCollisionBoxEndOverlap` mirrors it with `OnWeaponPulledFromTarget`. The weapon knows nothing about GAS, combat components, or character types — it just fires delegates.



\*\*`ALOSHeroWeapon`\*\* adds `FLOSHeroWeaponData HeroWeaponData` — the struct holding the linked anim layer class, input mapping context, default weapon abilities, and damage curve. It also holds `TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles`. The equip ability calls `AssignGrantedAbilitySpecHandle` to store handles returned by `GrantHeroWeaponAbility`. The unequip ability calls `GetGrantedAbilitySpecHandles` to retrieve them for revocation. The weapon actor is the natural owner for these handles — they should live exactly as long as the weapon is equipped.



\---



\### Input System



\*\*`ULOSInputComponent`\*\* extends `UEnhancedInputComponent`. Both methods are template functions in the header — no `.cpp` logic needed.



`BindNativeInputAction<UserObject, CallbackFunc>` calls `FindNativeInputActionByTag` on the config, then `BindAction` if the action is found. Used for move and look — two actions that call character methods directly.



`BindAbilityInputAction<UserObject, CallbackFunc>` iterates `InInputConfig->AbilityInputActions`, validates each entry, and binds `ETriggerEvent::Started` and `ETriggerEvent::Completed` to the provided press and release callbacks with the entry's `FGameplayTag` as the trailing parameter. One call from `SetupPlayerInputComponent` sets up every ability input in the game. Adding a new ability input requires no code change — only a new row in the data asset.



\*\*`DataAsset\_InputConfig`\*\* holds a `UInputMappingContext\*`, a `TArray<FLOSInputActionConfig> NativeInputActions` for move/look, and a `TArray<FLOSInputActionConfig> AbilityInputActions` for all ability bindings. `FLOSInputActionConfig` pairs a `FGameplayTag` with a `UInputAction\*` and has `IsValid()`. `FindNativeInputActionByTag` scans `NativeInputActions` linearly.



\---



\### UI Components and Widgets



\*\*`UPawnUIComponent`\*\* declares:

```cpp

DECLARE\_DYNAMIC\_MULTICAST\_DELEGATE\_OneParam(FOnPercenteChaingedDelegate, float, NewPercent);

UPROPERTY(BlueprintAssignable)

FOnPercenteChaingedDelegate OnCurrentHealthChanged;

```

This single delegate broadcasting a normalized 0–1 float is all any health bar needs. The attribute set broadcasts here; widgets bind here. No widget ever touches the attribute set directly.



\*\*`UHeroUIComponent`\*\* adds `OnCurrentRageChanged` using the same delegate type. Only the hero has rage, so this stays on the hero-specific component. `PostGameplayEffectExecute` null-checks for `UHeroUIComponent` before broadcasting — the same attribute set works on enemies without crashing.



\*\*`UEnemyUIComponent`\*\* inherits the health delegate from the base with no additions. It exists as a distinct class so the `IPawnUIInterface::GetEnemyUIComponent()` path can initialize enemy widgets, and future enemy-specific UI events can be added without touching the base.



\*\*`ULOSWidgetBase`\*\* is the base for all in-game widgets. `NativeOnInitialized` casts the owning player pawn to `IPawnUIInterface` and calls the Blueprint-implementable event `BP\_OnOwningHeroUIComponentInitialized` with the hero UI component if available. `InitEnemyCreatedWidget(AActor\* OwningEnemyActor)` is a `BlueprintCallable` function for enemy health bar widgets — it casts the enemy actor to `IPawnUIInterface`, asserts `GetEnemyUIComponent()` is valid, and calls `BP\_OnOwningEnemyUIComponentInitialized`. Widget blueprints override these events to bind to the delegates they care about. No widget class ever needs to know what type of character it is attached to.



\---



\### StartUp Data Assets



\*\*`UDataAsset\_StartUpDataBase`\*\* has three arrays: `ActivateOnGivenAbilities` (granted with `OnGiven` policy — HitReact, Death), `ReActiveAbilities` (granted normally, activated by input or event), and `StartUpGameplayEffects` (applied instantly to set initial attribute values). `GiveToAbilitySystemComponent` processes all three. The private `GrantAbilities` method creates a `FGameplayAbilitySpec` for each class, sets the avatar as source object and the level, and calls `GiveAbility`. Effects use `ApplyGameplayEffectToSelf` with `MakeEffectContext()` — this is how `CurrentHealth = MaxHealth` on spawn works.



\*\*`UDataAsset\_HeroStartUpData`\*\* overrides `GiveToAbilitySystemComponent`, calls `Super`, then processes `HeroStartUpAbilitiesSets`. Each `FLOSHeroAbilitySet` entry has its input tag embedded in the spec's `DynamicSpecSourceTags` before `GiveAbility` — this is what enables the input-to-ability tag routing system.



\*\*`UDataAsset\_EnemyStartUpData`\*\* overrides `GiveToAbilitySystemComponent`, calls `Super`, then iterates `EnemyCombatAbilities` granting each ability class without input tags. Enemy abilities are triggered by AI or gameplay events.



\---



\### Animation System



\*\*`ULOSBaseAnimInstance`\*\* is an empty root. Exists so the entire tree has a common ancestor.



\*\*`ULOSCharacterAnimInstance`\*\* caches `ALOSBaseCharacter\*` and `UCharacterMovementComponent\*` in `NativeInitializeAnimation`. All per-frame updates happen in `NativeThreadSafeUpdateAnimation` — the worker-thread variant that avoids game-thread hitches during heavy scenes. `GroundSpeed` uses `GetVelocity().Size2D()` to ignore vertical velocity from jumps. `bHasAcceleration` uses `SizeSquared2D() > 0` — a squared comparison avoids a sqrt every frame.



\*\*`ULOSHeroAnimInstance`\*\* adds the idle/relax system. When `bHasAcceleration` is true, `IdleElapsedTime` resets to 0 and `bShouldEnterRelaxState` is false. When false, `IdleElapsedTime` accumulates. Once it crosses `EnterRelaxStateThreshold` (5 seconds by default, configurable per asset), `bShouldEnterRelaxState` flips true. The anim Blueprint blends from a combat-ready idle to a relaxed idle after the player has stood still long enough.



\*\*`ULOSHeroLinkedAnimLayer`\*\* is a linked anim layer base for the hero. Linked layers in UE5 allow swapping complete animation sets at runtime — the hero can link a sword layer on equip and unlink it on unequip without changing the main anim graph. `GetHeroAnimInstance()` is marked `BlueprintThreadSafe` so the linked layer's anim graph can safely read `GroundSpeed`, `bHasAcceleration`, and `bShouldEnterRelaxState` from the primary instance while running on the worker thread.



\---



\### Function Library



\*\*`ULOSFunctionLibrary`\*\* provides six static helpers.



`NativeGetLOSASCFromActor` wraps `UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent` with `CastChecked<ULOSAbilitySystemComponent>` — callers get the project subclass directly without a second cast.



`AddGameplayTagToActorIfNone` checks `HasMatchingGameplayTag` before `AddLooseGameplayTag` — prevents double-adding and log spam. `RemoveGameplayTagFromActorIfFound` mirrors it.



`NativeDoesActorHasTag` returns a bool for C++. `BP\_DoesActorHaveTag` exposes it to Blueprint with `ELOSConfirmType` and `ExpandEnumAsExecs` — Yes/No execution pins.



`NativeGetPawnCombatComponentFromActor` validates the actor, casts to `IPawnCombatInterface`, and returns `GetCombatComponent()`. Returns nullptr safely if the interface is not implemented. `BP\_GetPawnCombatComponentFromActor` exposes this with `ELOSValidType` exec pins.



\---



\### Game Mode and Controller



\*\*`ALOSBaseGameMode`\*\* extends `AGameModeBase` — empty stub. Exists so the project has its own game mode type to expand without touching the engine base class.



\*\*`ALOSPlayerController`\*\* extends `APlayerController` — empty stub. Ready for input mode management, pause logic, or HUD access as the project grows.



\---



\## Design Patterns Used



| Pattern | Where | Why |

|---|---|---|

| \*\*Interface-based polymorphism\*\* | `IPawnCombatInterface`, `IPawnUIInterface` | Any system can query any actor for combat or UI data without knowing its concrete type |

| \*\*Tag-based state machine\*\* | All gameplay state, input routing, ability blocking, death | Replaces boolean flags; abilities declare requirements in data, not code |

| \*\*Data-driven ability loadout\*\* | `DataAsset\_StartUpDataBase` hierarchy | Adding or removing abilities requires no code — only data asset edits |

| \*\*Lazy cached weak references\*\* | Hero/enemy ability bases, `ULOSAttributeSet` UI interface | Avoids repeated casts in hot paths; weak pointers are safe if targets are destroyed mid-ability |

| \*\*Meta-attribute damage pattern\*\* | `DamageTaken` + `GEExecCalc\_DamageTaken` | All incoming damage converges at one intercept point |

| \*\*SetByCaller for runtime values\*\* | Base damage, combo count | Dynamic values are embedded in the effect spec without custom effect subclasses |

| \*\*Execution calculation\*\* | `GEExecCalc\_DamageTaken` | Captures live attribute values at execution time; formula is isolated and testable |

| \*\*Template component base\*\* | `UPawnExtensionComponentBase` | Type-safe pawn and controller access with `static\_assert` enforcement |

| \*\*Linked anim layers\*\* | `ULOSHeroLinkedAnimLayer` | Swap complete animation sets on equip/unequip without changing the main anim graph |

| \*\*Async asset loading for AI\*\* | `ALOSEnemyCharacter::InItEnemyStartUpData` | Many enemies can spawn without stalling the game thread |

| \*\*Delegate-driven UI\*\* | `UPawnUIComponent`, `UHeroUIComponent` | Widgets bind to events on the component; the attribute set never references a widget class |

| \*\*Spec handle lifetime on weapon\*\* | `ALOSHeroWeapon::GrantedAbilitySpecHandles` | Handles live on the weapon — naturally revoked when the weapon is unequipped |



\---



\## Gameplay Tag Reference



All tags are defined in `LOSGameplayTags.h/.cpp`.



```

InputTag.\*

&#x20; InputTag.Move                          — WASD / stick movement

&#x20; InputTag.Look                          — Mouse / right stick camera

&#x20; InputTag.EquipSword                    — Equip sword input

&#x20; InputTag.UnequipSword                  — Unequip sword input

&#x20; InputTag.LightAttack.Sword             — Light attack input

&#x20; InputTag.HeavyAttack.Sword             — Heavy attack input



Player.\*

&#x20; Player.Ability.Equip.Sword             — Asset tag on equip sword ability

&#x20; Player.Ability.Unequip.Sword           — Asset tag on unequip sword ability

&#x20; Player.Ability.Attack.Light.Sword      — Asset tag on light attack ability

&#x20; Player.Ability.Attack.Heavy.Sword      — Asset tag on heavy attack ability

&#x20; Player.Ability.HitPause                — Asset tag on hit pause ability

&#x20; Player.Weapon.Sword                    — Identifies the sword in the weapon registry

&#x20; Player.Event.Equip.Sword               — Fired by equip anim notify

&#x20; Player.Event.Unequip.Sword             — Fired by unequip anim notify

&#x20; Player.Event.HitPause                  — Fired on weapon hit and weapon pull

&#x20; Player.Event.Combo.ChainWindow         — Fired when a combo chain window opens

&#x20; Player.Status.JumpToFinisher           — Active during finisher jump state

&#x20; Player.SetByCaller.AttackType.Light    — Magnitude channel: light combo count

&#x20; Player.SetByCaller.AttackType.Heavy    — Magnitude channel: heavy combo count



Enemy.\*

&#x20; Enemy.Ability.Melee                    — Asset tag for enemy melee abilities

&#x20; Enemy.Ability.Ranged                   — Asset tag for enemy ranged abilities

&#x20; Enemy.Weapon                           — Identifies enemy weapon in weapon registry



Shared.\*

&#x20; Shared.Ability.HitReact                — Hit react ability — used by hero and enemies

&#x20; Shared.Ability.Death                   — Death ability — used by hero and enemies

&#x20; Shared.Event.MeleeHit                  — Sent by HeroCombatComponent on weapon contact

&#x20; Shared.Event.HitReact                  — Sent to trigger a hit react

&#x20; Shared.SetByCaller.BaseDamage          — Magnitude channel: raw weapon damage value

&#x20; Shared.Status.Dead                     — Applied when CurrentHealth reaches 0

```



\---



\## Getting Started



\### Requirements



\- Unreal Engine 5.1 or newer

\- Plugins enabled in `LandOfSouls.uproject`:

&#x20; - `GameplayAbilities`

&#x20; - `EnhancedInput`



\### Setup



1\. Clone the repository.

2\. Right-click `LandOfSouls.uproject` → \*Generate Visual Studio project files\*.

3\. Build `Development Editor` configuration in Visual Studio.

4\. Open the project in the Unreal Editor.



\### Adding a New Hero Ability



1\. Create a Blueprint class inheriting `ULOSHeroGameplayAbility`.

2\. Set `AbilityActivationPolicy` — `OnTriggered` for input-driven, `OnGiven` for passive.

3\. Configure `ActivationRequiredTags` or `ActivationBlockedTags` as needed.

4\. Open the hero's `DataAsset\_HeroStartUpData`.

5\. Add an entry to `HeroStartUpAbilitiesSets` with the ability class and its input tag.

6\. Ensure `DataAsset\_InputConfig` has a `UInputAction` mapped to that same input tag.

7\. The ability is granted on possession and activates when the input fires. No code changes required.



\### Adding a New Enemy Type



1\. Create a Blueprint inheriting `ALOSEnemyCharacter`.

2\. Create a `DataAsset\_EnemyStartUpData` for it.

3\. Populate `EnemyCombatAbilities` with the enemy's ability classes.

4\. Add a `StartUpGameplayEffects` entry to initialize the enemy's health and stats.

5\. Assign the data asset to `CharacterStartUpData` on the Blueprint defaults.

6\. The enemy initializes its ASC fully on possession — whether placed in the level or spawned at runtime.



\### Adding a New Attribute



1\. Add a `FGameplayAttributeData` to `ULOSAttributeSet` with the `ATTRIBUTE\_ACCESSORS` macro.

2\. Initialize it to `1.f` in the constructor.

3\. Handle the change case in `PostGameplayEffectExecute` if it should trigger a side effect.

4\. Create a startup `GameplayEffect` that sets its base value and add it to the relevant startup data assets.



\### Adding a New Weapon Type



1\. Create a Blueprint inheriting `ALOSHeroWeapon`.

2\. Assign the mesh and set the collision box extents.

3\. Fill in `HeroWeaponData`:

&#x20;  - `WeaponAnimLayerToLink` — the linked anim layer Blueprint for this weapon's locomotion

&#x20;  - `WeaponInputMappingContext` — the IMC with this weapon's attack inputs

&#x20;  - `DefaultWeaponAbilities` — one `FLOSHeroAbilitySet` per ability (light attack, heavy attack, etc.)

&#x20;  - `WeaponBaseDamage` — a curve table row defining damage per ability level

4\. The equip ability links the anim layer, adds the input context, grants the weapon abilities, and stores the handles. The unequip ability reverses all of this cleanly.



\---



\*© Noctifuge — All Rights Reserved\*

