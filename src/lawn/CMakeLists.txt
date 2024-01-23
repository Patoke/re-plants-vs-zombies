target_sources(${PROJECT_NAME} PRIVATE
    Board.cpp
	LawnMower.cpp
	GameObject.cpp
	SeedPacket.cpp
	CutScene.cpp
	Challenge.cpp
	CursorObject.cpp
	Coin.cpp
	Projectile.cpp
	Plant.cpp
	Zombie.cpp
	ToolTipWidget.cpp
	ZenGarden.cpp
	MessageWidget.cpp
	LawnCommon.cpp
	GridItem.cpp
)

add_subdirectory(system)
add_subdirectory(widget)
