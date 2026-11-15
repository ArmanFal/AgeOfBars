# Age of Bars
15/11/2026

![Age of Bars gameplay](docs/images/gameplay.png)

A lane-based strategy prototype built in Unreal Engine 5 using C++ and Blueprints. Players deploy fantasy units and use their abilities to defend their tavern and destroy the opposing one highly inspired by the old Age Of War Flash Game some can relate to as "My child hood"

The project focuses on gameplay systems that let designers configure units, swap behaviours and test new gameplay ideas through Data Assets and Blueprint extensions so there's no limit of how many abilities you can include and how it can interact with the systems, as of now the unit collision detection is a bottleneck I aim to fix, because on high unit amounts like above 200 you can see noticable framedrops and especially with higher attack range attribute you can expect this to be very demanding. 

## Technical highlights

- Configure units and projectiles by Data Assets indexed by Gameplay Tags, **each data asset must have a unique gameplay tags or it will not be registered.**
- Using object pooling for units and projectiles, if you want to increase this adjust this value in MainGameMode Blueprint begin play.
- Implement attacks, abilities and impact responses through Blueprint behaviour classes, there are interfaces and events you can override for this.
- Coordinate lane membership, movement, combat and attributes through shared C++ foundations.
- Inspect runtime unit and projectile state through a reusable Slate debugging interface.

## Packaging
You cannot package the game yet as it stands due to the debugging interface and Blutility module, I plan to fix this later on, if you must you can completely remove or turn it into an editor only module.

## Explore the code

Start in `Source/AgeOfWar/Main`.
I plan to add documentation once the prototype is at a more stable version.

For the gameplay demonstrations, architecture explanations and implementation examples, visit my [portfolio breakdown](https://armanshportfolio.framer.website/ageofbars).
