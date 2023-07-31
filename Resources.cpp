#include <map>
#include "Resources.h"
#include "SexyAppFramework/ResourceManager.h"

using namespace Sexy;

bool gNeedRecalcVariableToIdMap = false;

//0x474700
bool Sexy::ExtractResourcesByName(ResourceManager* theResourceManager, const char* theName)
{
	if (!strcmp(theName, "DelayLoad_Almanac"))					return ExtractDelayLoad_AlmanacResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_AwardScreen"))			return ExtractDelayLoad_AwardScreenResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Background1"))			return ExtractDelayLoad_Background1Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Background2"))			return ExtractDelayLoad_Background2Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Background3"))			return ExtractDelayLoad_Background3Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Background4"))			return ExtractDelayLoad_Background4Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Background5"))			return ExtractDelayLoad_Background5Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Background6"))			return ExtractDelayLoad_Background6Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_BackgroundUnsodded"))	return ExtractDelayLoad_BackgroundUnsoddedResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ChallengeScreen"))		return ExtractDelayLoad_ChallengeScreenResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Credits"))				return ExtractDelayLoad_CreditsResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_GreenHouseGarden"))	return ExtractDelayLoad_GreenHouseGardenResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_GreenHouseOverlay"))	return ExtractDelayLoad_GreenHouseOverlayResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_MushroomGarden"))		return ExtractDelayLoad_MushroomGardenResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Store"))				return ExtractDelayLoad_StoreResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_TreeOfWisdom"))		return ExtractDelayLoad_TreeOfWisdomResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ZombieFinalNote"))		return ExtractDelayLoad_ZombieFinalNoteResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ZombieNote"))			return ExtractDelayLoad_ZombieNoteResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ZombieNote1"))			return ExtractDelayLoad_ZombieNote1Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ZombieNote2"))			return ExtractDelayLoad_ZombieNote2Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ZombieNote3"))			return ExtractDelayLoad_ZombieNote3Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ZombieNote4"))			return ExtractDelayLoad_ZombieNote4Resources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_ZombieNoteHelp"))		return ExtractDelayLoad_ZombieNoteHelpResources(theResourceManager);
	else if (!strcmp(theName, "DelayLoad_Zombiquarium"))		return ExtractDelayLoad_ZombiquariumResources(theResourceManager);
	else if (!strcmp(theName, "Init"))							return ExtractInitResources(theResourceManager);
	else if (!strcmp(theName, "LoaderBar"))						return ExtractLoaderBarResources(theResourceManager);
	else if (!strcmp(theName, "LoadingFonts"))					return ExtractLoadingFontsResources(theResourceManager);
	else if (!strcmp(theName, "LoadingImages"))					return ExtractLoadingImagesResources(theResourceManager);
	else if (!strcmp(theName, "LoadingSounds"))					return ExtractLoadingSoundsResources(theResourceManager);
}

// DelayLoad_Almanac Resources
Image* Sexy::IMAGE_ALMANAC_CLOSEBUTTON;
Image* Sexy::IMAGE_ALMANAC_CLOSEBUTTONHIGHLIGHT;
Image* Sexy::IMAGE_ALMANAC_GROUNDDAY;
Image* Sexy::IMAGE_ALMANAC_GROUNDICE;
Image* Sexy::IMAGE_ALMANAC_GROUNDNIGHT;
Image* Sexy::IMAGE_ALMANAC_GROUNDNIGHTPOOL;
Image* Sexy::IMAGE_ALMANAC_GROUNDPOOL;
Image* Sexy::IMAGE_ALMANAC_GROUNDROOF;
Image* Sexy::IMAGE_ALMANAC_INDEXBACK;
Image* Sexy::IMAGE_ALMANAC_INDEXBUTTON;
Image* Sexy::IMAGE_ALMANAC_INDEXBUTTONHIGHLIGHT;
Image* Sexy::IMAGE_ALMANAC_PLANTBACK;
Image* Sexy::IMAGE_ALMANAC_PLANTCARD;
Image* Sexy::IMAGE_ALMANAC_ZOMBIEBACK;
Image* Sexy::IMAGE_ALMANAC_ZOMBIEBLANK;
Image* Sexy::IMAGE_ALMANAC_ZOMBIECARD;
Image* Sexy::IMAGE_ALMANAC_ZOMBIEWINDOW;
Image* Sexy::IMAGE_ALMANAC_ZOMBIEWINDOW2;

bool Sexy::ExtractDelayLoad_AlmanacResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ALMANAC_CLOSEBUTTON = aMgr.GetImageThrow("IMAGE_ALMANAC_CLOSEBUTTON");
		IMAGE_ALMANAC_CLOSEBUTTONHIGHLIGHT = aMgr.GetImageThrow("IMAGE_ALMANAC_CLOSEBUTTONHIGHLIGHT");
		IMAGE_ALMANAC_GROUNDDAY = aMgr.GetImageThrow("IMAGE_ALMANAC_GROUNDDAY");
		IMAGE_ALMANAC_GROUNDICE = aMgr.GetImageThrow("IMAGE_ALMANAC_GROUNDICE");
		IMAGE_ALMANAC_GROUNDNIGHT = aMgr.GetImageThrow("IMAGE_ALMANAC_GROUNDNIGHT");
		IMAGE_ALMANAC_GROUNDNIGHTPOOL = aMgr.GetImageThrow("IMAGE_ALMANAC_GROUNDNIGHTPOOL");
		IMAGE_ALMANAC_GROUNDPOOL = aMgr.GetImageThrow("IMAGE_ALMANAC_GROUNDPOOL");
		IMAGE_ALMANAC_GROUNDROOF = aMgr.GetImageThrow("IMAGE_ALMANAC_GROUNDROOF");
		IMAGE_ALMANAC_INDEXBACK = aMgr.GetImageThrow("IMAGE_ALMANAC_INDEXBACK");
		IMAGE_ALMANAC_INDEXBUTTON = aMgr.GetImageThrow("IMAGE_ALMANAC_INDEXBUTTON");
		IMAGE_ALMANAC_INDEXBUTTONHIGHLIGHT = aMgr.GetImageThrow("IMAGE_ALMANAC_INDEXBUTTONHIGHLIGHT");
		IMAGE_ALMANAC_PLANTBACK = aMgr.GetImageThrow("IMAGE_ALMANAC_PLANTBACK");
		IMAGE_ALMANAC_PLANTCARD = aMgr.GetImageThrow("IMAGE_ALMANAC_PLANTCARD");
		IMAGE_ALMANAC_ZOMBIEBACK = aMgr.GetImageThrow("IMAGE_ALMANAC_ZOMBIEBACK");
		IMAGE_ALMANAC_ZOMBIEBLANK = aMgr.GetImageThrow("IMAGE_ALMANAC_ZOMBIEBLANK");
		IMAGE_ALMANAC_ZOMBIECARD = aMgr.GetImageThrow("IMAGE_ALMANAC_ZOMBIECARD");
		IMAGE_ALMANAC_ZOMBIEWINDOW = aMgr.GetImageThrow("IMAGE_ALMANAC_ZOMBIEWINDOW");
		IMAGE_ALMANAC_ZOMBIEWINDOW2 = aMgr.GetImageThrow("IMAGE_ALMANAC_ZOMBIEWINDOW2");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_AwardScreen Resources
Image* Sexy::IMAGE_AWARDSCREEN_BACK;

bool Sexy::ExtractDelayLoad_AwardScreenResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_AWARDSCREEN_BACK = aMgr.GetImageThrow("IMAGE_AWARDSCREEN_BACK");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Background1 Resources
Image* Sexy::IMAGE_BACKGROUND1;
Image* Sexy::IMAGE_BACKGROUND1_GAMEOVER_INTERIOR_OVERLAY;
Image* Sexy::IMAGE_BACKGROUND1_GAMEOVER_MASK;

bool Sexy::ExtractDelayLoad_Background1Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND1 = aMgr.GetImageThrow("IMAGE_BACKGROUND1");
		IMAGE_BACKGROUND1_GAMEOVER_INTERIOR_OVERLAY = aMgr.GetImageThrow("IMAGE_BACKGROUND1_GAMEOVER_INTERIOR_OVERLAY");
		IMAGE_BACKGROUND1_GAMEOVER_MASK = aMgr.GetImageThrow("IMAGE_BACKGROUND1_GAMEOVER_MASK");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Background2 Resources
Image* Sexy::IMAGE_BACKGROUND2;
Image* Sexy::IMAGE_BACKGROUND2_GAMEOVER_INTERIOR_OVERLAY;
Image* Sexy::IMAGE_BACKGROUND2_GAMEOVER_MASK;

bool Sexy::ExtractDelayLoad_Background2Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND2 = aMgr.GetImageThrow("IMAGE_BACKGROUND2");
		IMAGE_BACKGROUND2_GAMEOVER_INTERIOR_OVERLAY = aMgr.GetImageThrow("IMAGE_BACKGROUND2_GAMEOVER_INTERIOR_OVERLAY");
		IMAGE_BACKGROUND2_GAMEOVER_MASK = aMgr.GetImageThrow("IMAGE_BACKGROUND2_GAMEOVER_MASK");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Background3 Resources
Image* Sexy::IMAGE_BACKGROUND3;
Image* Sexy::IMAGE_BACKGROUND3_GAMEOVER_INTERIOR_OVERLAY;
Image* Sexy::IMAGE_BACKGROUND3_GAMEOVER_MASK;

bool Sexy::ExtractDelayLoad_Background3Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND3 = aMgr.GetImageThrow("IMAGE_BACKGROUND3");
		IMAGE_BACKGROUND3_GAMEOVER_INTERIOR_OVERLAY = aMgr.GetImageThrow("IMAGE_BACKGROUND3_GAMEOVER_INTERIOR_OVERLAY");
		IMAGE_BACKGROUND3_GAMEOVER_MASK = aMgr.GetImageThrow("IMAGE_BACKGROUND3_GAMEOVER_MASK");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Background4 Resources
Image* Sexy::IMAGE_BACKGROUND4;
Image* Sexy::IMAGE_BACKGROUND4_GAMEOVER_INTERIOR_OVERLAY;
Image* Sexy::IMAGE_BACKGROUND4_GAMEOVER_MASK;
Image* Sexy::IMAGE_FOG;
Image* Sexy::IMAGE_FOG_SOFTWARE;

bool Sexy::ExtractDelayLoad_Background4Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND4 = aMgr.GetImageThrow("IMAGE_BACKGROUND4");
		IMAGE_BACKGROUND4_GAMEOVER_INTERIOR_OVERLAY = aMgr.GetImageThrow("IMAGE_BACKGROUND4_GAMEOVER_INTERIOR_OVERLAY");
		IMAGE_BACKGROUND4_GAMEOVER_MASK = aMgr.GetImageThrow("IMAGE_BACKGROUND4_GAMEOVER_MASK");
		IMAGE_FOG = aMgr.GetImageThrow("IMAGE_FOG");
		IMAGE_FOG_SOFTWARE = aMgr.GetImageThrow("IMAGE_FOG_SOFTWARE");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Background5 Resources
Image* Sexy::IMAGE_BACKGROUND5;
Image* Sexy::IMAGE_BACKGROUND5_GAMEOVER_MASK;

bool Sexy::ExtractDelayLoad_Background5Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND5 = aMgr.GetImageThrow("IMAGE_BACKGROUND5");
		IMAGE_BACKGROUND5_GAMEOVER_MASK = aMgr.GetImageThrow("IMAGE_BACKGROUND5_GAMEOVER_MASK");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Background6 Resources
Image* Sexy::IMAGE_BACKGROUND6BOSS;
Image* Sexy::IMAGE_BACKGROUND6_GAMEOVER_MASK;

bool Sexy::ExtractDelayLoad_Background6Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND6BOSS = aMgr.GetImageThrow("IMAGE_BACKGROUND6BOSS");
		IMAGE_BACKGROUND6_GAMEOVER_MASK = aMgr.GetImageThrow("IMAGE_BACKGROUND6_GAMEOVER_MASK");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_BackgroundUnsodded Resources
Image* Sexy::IMAGE_BACKGROUND1UNSODDED;
Image* Sexy::IMAGE_SOD1ROW;
Image* Sexy::IMAGE_SOD3ROW;

bool Sexy::ExtractDelayLoad_BackgroundUnsoddedResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND1UNSODDED = aMgr.GetImageThrow("IMAGE_BACKGROUND1UNSODDED");
		IMAGE_SOD1ROW = aMgr.GetImageThrow("IMAGE_SOD1ROW");
		IMAGE_SOD3ROW = aMgr.GetImageThrow("IMAGE_SOD3ROW");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ChallengeScreen Resources
Image* Sexy::IMAGE_CHALLENGE_BACKGROUND;
Image* Sexy::IMAGE_CHALLENGE_BLANK;
Image* Sexy::IMAGE_CHALLENGE_THUMBNAILS;
Image* Sexy::IMAGE_CHALLENGE_WINDOW;
Image* Sexy::IMAGE_CHALLENGE_WINDOW_HIGHLIGHT;
Image* Sexy::IMAGE_LOCK;
Image* Sexy::IMAGE_LOCK_OPEN;
Image* Sexy::IMAGE_SURVIVAL_THUMBNAILS;

bool Sexy::ExtractDelayLoad_ChallengeScreenResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_CHALLENGE_BACKGROUND = aMgr.GetImageThrow("IMAGE_CHALLENGE_BACKGROUND");
		IMAGE_CHALLENGE_BLANK = aMgr.GetImageThrow("IMAGE_CHALLENGE_BLANK");
		IMAGE_CHALLENGE_THUMBNAILS = aMgr.GetImageThrow("IMAGE_CHALLENGE_THUMBNAILS");
		IMAGE_CHALLENGE_WINDOW = aMgr.GetImageThrow("IMAGE_CHALLENGE_WINDOW");
		IMAGE_CHALLENGE_WINDOW_HIGHLIGHT = aMgr.GetImageThrow("IMAGE_CHALLENGE_WINDOW_HIGHLIGHT");
		IMAGE_LOCK = aMgr.GetImageThrow("IMAGE_LOCK");
		IMAGE_LOCK_OPEN = aMgr.GetImageThrow("IMAGE_LOCK_OPEN");
		IMAGE_SURVIVAL_THUMBNAILS = aMgr.GetImageThrow("IMAGE_SURVIVAL_THUMBNAILS");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Credits Resources
Font* Sexy::FONT_BRIANNETOD32;
Font* Sexy::FONT_BRIANNETOD32BLACK;
Image* Sexy::IMAGE_CREDITS_PLAYBUTTON;
Image* Sexy::IMAGE_CREDITS_ZOMBIENOTE;
Image* Sexy::IMAGE_REANIM_CREDITS_DISCOLIGHTS;
Image* Sexy::IMAGE_REANIM_CREDITS_FOGMACHINE;
Image* Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING1;
Image* Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING2;
Image* Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING3;
Image* Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING4;
Image* Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING5;
Image* Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_WINK;

bool Sexy::ExtractDelayLoad_CreditsResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		FONT_BRIANNETOD32 = aMgr.GetFontThrow("FONT_BRIANNETOD32");
		FONT_BRIANNETOD32BLACK = aMgr.GetFontThrow("FONT_BRIANNETOD32BLACK");
		IMAGE_CREDITS_PLAYBUTTON = aMgr.GetImageThrow("IMAGE_CREDITS_PLAYBUTTON");
		IMAGE_CREDITS_ZOMBIENOTE = aMgr.GetImageThrow("IMAGE_CREDITS_ZOMBIENOTE");
		IMAGE_REANIM_CREDITS_DISCOLIGHTS = aMgr.GetImageThrow("IMAGE_REANIM_CREDITS_DISCOLIGHTS");
		IMAGE_REANIM_CREDITS_FOGMACHINE = aMgr.GetImageThrow("IMAGE_REANIM_CREDITS_FOGMACHINE");
		IMAGE_REANIM_SUNFLOWER_HEAD_SING1 = aMgr.GetImageThrow("IMAGE_REANIM_SUNFLOWER_HEAD_SING1");
		IMAGE_REANIM_SUNFLOWER_HEAD_SING2 = aMgr.GetImageThrow("IMAGE_REANIM_SUNFLOWER_HEAD_SING2");
		IMAGE_REANIM_SUNFLOWER_HEAD_SING3 = aMgr.GetImageThrow("IMAGE_REANIM_SUNFLOWER_HEAD_SING3");
		IMAGE_REANIM_SUNFLOWER_HEAD_SING4 = aMgr.GetImageThrow("IMAGE_REANIM_SUNFLOWER_HEAD_SING4");
		IMAGE_REANIM_SUNFLOWER_HEAD_SING5 = aMgr.GetImageThrow("IMAGE_REANIM_SUNFLOWER_HEAD_SING5");
		IMAGE_REANIM_SUNFLOWER_HEAD_WINK = aMgr.GetImageThrow("IMAGE_REANIM_SUNFLOWER_HEAD_WINK");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_GreenHouseGarden Resources
Image* Sexy::IMAGE_BACKGROUND_GREENHOUSE;

bool Sexy::ExtractDelayLoad_GreenHouseGardenResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND_GREENHOUSE = aMgr.GetImageThrow("IMAGE_BACKGROUND_GREENHOUSE");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_GreenHouseOverlay Resources
Image* Sexy::IMAGE_BACKGROUND_GREENHOUSE_OVERLAY;

bool Sexy::ExtractDelayLoad_GreenHouseOverlayResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND_GREENHOUSE_OVERLAY = aMgr.GetImageThrow("IMAGE_BACKGROUND_GREENHOUSE_OVERLAY");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_MushroomGarden Resources
Image* Sexy::IMAGE_BACKGROUND_MUSHROOMGARDEN;

bool Sexy::ExtractDelayLoad_MushroomGardenResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BACKGROUND_MUSHROOMGARDEN = aMgr.GetImageThrow("IMAGE_BACKGROUND_MUSHROOMGARDEN");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Store Resources
Image* Sexy::IMAGE_STORE_AQUARIUMGARDENICON;
Image* Sexy::IMAGE_STORE_BACKGROUND;
Image* Sexy::IMAGE_STORE_BACKGROUNDNIGHT;
Image* Sexy::IMAGE_STORE_CAR;
Image* Sexy::IMAGE_STORE_CARCLOSED;
Image* Sexy::IMAGE_STORE_CARCLOSED_NIGHT;
Image* Sexy::IMAGE_STORE_CAR_NIGHT;
Image* Sexy::IMAGE_STORE_FIRSTAIDWALLNUTICON;
Image* Sexy::IMAGE_STORE_HATCHBACKOPEN;
Image* Sexy::IMAGE_STORE_MAINMENUBUTTON;
Image* Sexy::IMAGE_STORE_MAINMENUBUTTONDOWN;
Image* Sexy::IMAGE_STORE_MAINMENUBUTTONHIGHLIGHT;
Image* Sexy::IMAGE_STORE_MUSHROOMGARDENICON;
Image* Sexy::IMAGE_STORE_NEXTBUTTON;
Image* Sexy::IMAGE_STORE_NEXTBUTTONDISABLED;
Image* Sexy::IMAGE_STORE_NEXTBUTTONHIGHLIGHT;
Image* Sexy::IMAGE_STORE_PACKETUPGRADE;
Image* Sexy::IMAGE_STORE_PREVBUTTON;
Image* Sexy::IMAGE_STORE_PREVBUTTONDISABLED;
Image* Sexy::IMAGE_STORE_PREVBUTTONHIGHLIGHT;
Image* Sexy::IMAGE_STORE_PRICETAG;
Image* Sexy::IMAGE_STORE_PVZICON;
Image* Sexy::IMAGE_STORE_SIGN;
Image* Sexy::IMAGE_STORE_TREEOFWISDOMICON;

bool Sexy::ExtractDelayLoad_StoreResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_STORE_AQUARIUMGARDENICON = aMgr.GetImageThrow("IMAGE_STORE_AQUARIUMGARDENICON");
		IMAGE_STORE_BACKGROUND = aMgr.GetImageThrow("IMAGE_STORE_BACKGROUND");
		IMAGE_STORE_BACKGROUNDNIGHT = aMgr.GetImageThrow("IMAGE_STORE_BACKGROUNDNIGHT");
		IMAGE_STORE_CAR = aMgr.GetImageThrow("IMAGE_STORE_CAR");
		IMAGE_STORE_CARCLOSED = aMgr.GetImageThrow("IMAGE_STORE_CARCLOSED");
		IMAGE_STORE_CARCLOSED_NIGHT = aMgr.GetImageThrow("IMAGE_STORE_CARCLOSED_NIGHT");
		IMAGE_STORE_CAR_NIGHT = aMgr.GetImageThrow("IMAGE_STORE_CAR_NIGHT");
		IMAGE_STORE_FIRSTAIDWALLNUTICON = aMgr.GetImageThrow("IMAGE_STORE_FIRSTAIDWALLNUTICON");
		IMAGE_STORE_HATCHBACKOPEN = aMgr.GetImageThrow("IMAGE_STORE_HATCHBACKOPEN");
		IMAGE_STORE_MAINMENUBUTTON = aMgr.GetImageThrow("IMAGE_STORE_MAINMENUBUTTON");
		IMAGE_STORE_MAINMENUBUTTONDOWN = aMgr.GetImageThrow("IMAGE_STORE_MAINMENUBUTTONDOWN");
		IMAGE_STORE_MAINMENUBUTTONHIGHLIGHT = aMgr.GetImageThrow("IMAGE_STORE_MAINMENUBUTTONHIGHLIGHT");
		IMAGE_STORE_MUSHROOMGARDENICON = aMgr.GetImageThrow("IMAGE_STORE_MUSHROOMGARDENICON");
		IMAGE_STORE_NEXTBUTTON = aMgr.GetImageThrow("IMAGE_STORE_NEXTBUTTON");
		IMAGE_STORE_NEXTBUTTONDISABLED = aMgr.GetImageThrow("IMAGE_STORE_NEXTBUTTONDISABLED");
		IMAGE_STORE_NEXTBUTTONHIGHLIGHT = aMgr.GetImageThrow("IMAGE_STORE_NEXTBUTTONHIGHLIGHT");
		IMAGE_STORE_PACKETUPGRADE = aMgr.GetImageThrow("IMAGE_STORE_PACKETUPGRADE");
		IMAGE_STORE_PREVBUTTON = aMgr.GetImageThrow("IMAGE_STORE_PREVBUTTON");
		IMAGE_STORE_PREVBUTTONDISABLED = aMgr.GetImageThrow("IMAGE_STORE_PREVBUTTONDISABLED");
		IMAGE_STORE_PREVBUTTONHIGHLIGHT = aMgr.GetImageThrow("IMAGE_STORE_PREVBUTTONHIGHLIGHT");
		IMAGE_STORE_PRICETAG = aMgr.GetImageThrow("IMAGE_STORE_PRICETAG");
		IMAGE_STORE_PVZICON = aMgr.GetImageThrow("IMAGE_STORE_PVZICON");
		IMAGE_STORE_SIGN = aMgr.GetImageThrow("IMAGE_STORE_SIGN");
		IMAGE_STORE_TREEOFWISDOMICON = aMgr.GetImageThrow("IMAGE_STORE_TREEOFWISDOMICON");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_TreeOfWisdom Resources

bool Sexy::ExtractDelayLoad_TreeOfWisdomResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ZombieFinalNote Resources
Image* Sexy::IMAGE_ZOMBIE_FINAL_NOTE;

bool Sexy::ExtractDelayLoad_ZombieFinalNoteResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ZOMBIE_FINAL_NOTE = aMgr.GetImageThrow("IMAGE_ZOMBIE_FINAL_NOTE");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ZombieNote Resources
Image* Sexy::IMAGE_ZOMBIE_NOTE;

bool Sexy::ExtractDelayLoad_ZombieNoteResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ZOMBIE_NOTE = aMgr.GetImageThrow("IMAGE_ZOMBIE_NOTE");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ZombieNote1 Resources
Image* Sexy::IMAGE_ZOMBIE_NOTE1;

bool Sexy::ExtractDelayLoad_ZombieNote1Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ZOMBIE_NOTE1 = aMgr.GetImageThrow("IMAGE_ZOMBIE_NOTE1");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ZombieNote2 Resources
Image* Sexy::IMAGE_ZOMBIE_NOTE2;

bool Sexy::ExtractDelayLoad_ZombieNote2Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ZOMBIE_NOTE2 = aMgr.GetImageThrow("IMAGE_ZOMBIE_NOTE2");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ZombieNote3 Resources
Image* Sexy::IMAGE_ZOMBIE_NOTE3;

bool Sexy::ExtractDelayLoad_ZombieNote3Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ZOMBIE_NOTE3 = aMgr.GetImageThrow("IMAGE_ZOMBIE_NOTE3");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ZombieNote4 Resources
Image* Sexy::IMAGE_ZOMBIE_NOTE4;

bool Sexy::ExtractDelayLoad_ZombieNote4Resources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ZOMBIE_NOTE4 = aMgr.GetImageThrow("IMAGE_ZOMBIE_NOTE4");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_ZombieNoteHelp Resources
Image* Sexy::IMAGE_ZOMBIE_NOTE_HELP;

bool Sexy::ExtractDelayLoad_ZombieNoteHelpResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ZOMBIE_NOTE_HELP = aMgr.GetImageThrow("IMAGE_ZOMBIE_NOTE_HELP");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// DelayLoad_Zombiquarium Resources
Image* Sexy::IMAGE_AQUARIUM1;
Image* Sexy::IMAGE_WAVECENTER;
Image* Sexy::IMAGE_WAVESIDE;

bool Sexy::ExtractDelayLoad_ZombiquariumResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_AQUARIUM1 = aMgr.GetImageThrow("IMAGE_AQUARIUM1");
		IMAGE_WAVECENTER = aMgr.GetImageThrow("IMAGE_WAVECENTER");
		IMAGE_WAVESIDE = aMgr.GetImageThrow("IMAGE_WAVESIDE");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// Init Resources
Image* Sexy::IMAGE_BLANK;
Image* Sexy::IMAGE_PARTNER_LOGO;
Image* Sexy::IMAGE_POPCAP_LOGO;

bool Sexy::ExtractInitResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_BLANK = aMgr.GetImageThrow("IMAGE_BLANK");
		IMAGE_PARTNER_LOGO = aMgr.GetImageThrow("IMAGE_PARTNER_LOGO");
		IMAGE_POPCAP_LOGO = aMgr.GetImageThrow("IMAGE_POPCAP_LOGO");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// LoaderBar Resources
Font* Sexy::FONT_BRIANNETOD16;
Image* Sexy::IMAGE_LOADBAR_DIRT;
Image* Sexy::IMAGE_LOADBAR_GRASS;
Image* Sexy::IMAGE_PVZ_LOGO;
Image* Sexy::IMAGE_REANIM_SODROLLCAP;
Image* Sexy::IMAGE_TITLESCREEN;
int Sexy::SOUND_BUTTONCLICK;
int Sexy::SOUND_LOADINGBAR_FLOWER;
int Sexy::SOUND_LOADINGBAR_ZOMBIE;

bool Sexy::ExtractLoaderBarResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		FONT_BRIANNETOD16 = aMgr.GetFontThrow("FONT_BRIANNETOD16");
		IMAGE_LOADBAR_DIRT = aMgr.GetImageThrow("IMAGE_LOADBAR_DIRT");
		IMAGE_LOADBAR_GRASS = aMgr.GetImageThrow("IMAGE_LOADBAR_GRASS");
		IMAGE_PVZ_LOGO = aMgr.GetImageThrow("IMAGE_PVZ_LOGO");
		IMAGE_REANIM_SODROLLCAP = aMgr.GetImageThrow("IMAGE_REANIM_SODROLLCAP");
		IMAGE_TITLESCREEN = aMgr.GetImageThrow("IMAGE_TITLESCREEN");
		SOUND_BUTTONCLICK = aMgr.GetSoundThrow("SOUND_BUTTONCLICK");
		SOUND_LOADINGBAR_FLOWER = aMgr.GetSoundThrow("SOUND_LOADINGBAR_FLOWER");
		SOUND_LOADINGBAR_ZOMBIE = aMgr.GetSoundThrow("SOUND_LOADINGBAR_ZOMBIE");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// LoadingFonts Resources
Font* Sexy::FONT_BRIANNETOD12;
Font* Sexy::FONT_CONTINUUMBOLD14;
Font* Sexy::FONT_CONTINUUMBOLD14OUTLINE;
Font* Sexy::FONT_DWARVENTODCRAFT12;
Font* Sexy::FONT_DWARVENTODCRAFT15;
Font* Sexy::FONT_DWARVENTODCRAFT18;
Font* Sexy::FONT_DWARVENTODCRAFT18BRIGHTGREENINSET;
Font* Sexy::FONT_DWARVENTODCRAFT18GREENINSET;
Font* Sexy::FONT_DWARVENTODCRAFT18YELLOW;
Font* Sexy::FONT_DWARVENTODCRAFT24;
Font* Sexy::FONT_DWARVENTODCRAFT36BRIGHTGREENINSET;
Font* Sexy::FONT_DWARVENTODCRAFT36GREENINSET;
Font* Sexy::FONT_HOUSEOFTERROR16;
Font* Sexy::FONT_HOUSEOFTERROR20;
Font* Sexy::FONT_HOUSEOFTERROR28;
Image* Sexy::FONT_IMAGE_HOUSEOFTERROR28;
Font* Sexy::FONT_PICO129;
Font* Sexy::FONT_TINYBOLD;

bool Sexy::ExtractLoadingFontsResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		FONT_BRIANNETOD12 = aMgr.GetFontThrow("FONT_BRIANNETOD12");
		FONT_CONTINUUMBOLD14 = aMgr.GetFontThrow("FONT_CONTINUUMBOLD14");
		FONT_CONTINUUMBOLD14OUTLINE = aMgr.GetFontThrow("FONT_CONTINUUMBOLD14OUTLINE");
		FONT_DWARVENTODCRAFT12 = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT12");
		FONT_DWARVENTODCRAFT15 = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT15");
		FONT_DWARVENTODCRAFT18 = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT18");
		FONT_DWARVENTODCRAFT18BRIGHTGREENINSET = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT18BRIGHTGREENINSET");
		FONT_DWARVENTODCRAFT18GREENINSET = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT18GREENINSET");
		FONT_DWARVENTODCRAFT18YELLOW = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT18YELLOW");
		FONT_DWARVENTODCRAFT24 = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT24");
		FONT_DWARVENTODCRAFT36BRIGHTGREENINSET = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT36BRIGHTGREENINSET");
		FONT_DWARVENTODCRAFT36GREENINSET = aMgr.GetFontThrow("FONT_DWARVENTODCRAFT36GREENINSET");
		FONT_HOUSEOFTERROR16 = aMgr.GetFontThrow("FONT_HOUSEOFTERROR16");
		FONT_HOUSEOFTERROR20 = aMgr.GetFontThrow("FONT_HOUSEOFTERROR20");
		FONT_HOUSEOFTERROR28 = aMgr.GetFontThrow("FONT_HOUSEOFTERROR28");
		FONT_IMAGE_HOUSEOFTERROR28 = aMgr.GetImageThrow("FONT_IMAGE_HOUSEOFTERROR28");
		FONT_PICO129 = aMgr.GetFontThrow("FONT_PICO129");
		FONT_TINYBOLD = aMgr.GetFontThrow("FONT_TINYBOLD");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// LoadingImages Resources
Image* Sexy::IMAGE_ALMANAC;
Image* Sexy::IMAGE_ALMANAC_IMITATER;
Image* Sexy::IMAGE_AWARDPICKUPGLOW;
Image* Sexy::IMAGE_BEGHOULED_TWIST_OVERLAY;
Image* Sexy::IMAGE_BRAIN;
Image* Sexy::IMAGE_BUG_SPRAY;
Image* Sexy::IMAGE_BUNGEECORD;
Image* Sexy::IMAGE_BUNGEETARGET;
Image* Sexy::IMAGE_BUTTON_DOWN_LEFT;
Image* Sexy::IMAGE_BUTTON_DOWN_MIDDLE;
Image* Sexy::IMAGE_BUTTON_DOWN_RIGHT;
Image* Sexy::IMAGE_BUTTON_LEFT;
Image* Sexy::IMAGE_BUTTON_MIDDLE;
Image* Sexy::IMAGE_BUTTON_RIGHT;
Image* Sexy::IMAGE_CARKEYS;
Image* Sexy::IMAGE_CHOCOLATE;
Image* Sexy::IMAGE_COBCANNON_POPCORN;
Image* Sexy::IMAGE_COBCANNON_TARGET;
Image* Sexy::IMAGE_COINBANK;
Image* Sexy::IMAGE_CONVEYORBELT;
Image* Sexy::IMAGE_CONVEYORBELT_BACKDROP;
Image* Sexy::IMAGE_CRATER;
Image* Sexy::IMAGE_CRATER_FADING;
Image* Sexy::IMAGE_CRATER_ROOF_CENTER;
Image* Sexy::IMAGE_CRATER_ROOF_LEFT;
Image* Sexy::IMAGE_CRATER_WATER_DAY;
Image* Sexy::IMAGE_CRATER_WATER_NIGHT;
Image* Sexy::IMAGE_DIALOG_BIGBOTTOMLEFT;
Image* Sexy::IMAGE_DIALOG_BIGBOTTOMMIDDLE;
Image* Sexy::IMAGE_DIALOG_BIGBOTTOMRIGHT;
Image* Sexy::IMAGE_DIALOG_BOTTOMLEFT;
Image* Sexy::IMAGE_DIALOG_BOTTOMMIDDLE;
Image* Sexy::IMAGE_DIALOG_BOTTOMRIGHT;
Image* Sexy::IMAGE_DIALOG_CENTERLEFT;
Image* Sexy::IMAGE_DIALOG_CENTERMIDDLE;
Image* Sexy::IMAGE_DIALOG_CENTERRIGHT;
Image* Sexy::IMAGE_DIALOG_HEADER;
Image* Sexy::IMAGE_DIALOG_TOPLEFT;
Image* Sexy::IMAGE_DIALOG_TOPMIDDLE;
Image* Sexy::IMAGE_DIALOG_TOPRIGHT;
Image* Sexy::IMAGE_DIRTBIG;
Image* Sexy::IMAGE_DIRTSMALL;
Image* Sexy::IMAGE_DOOMSHROOM_EXPLOSION_BASE;
Image* Sexy::IMAGE_DUST_PUFFS;
Image* Sexy::IMAGE_EDITBOX;
Image* Sexy::IMAGE_FERTILIZER;
Image* Sexy::IMAGE_FLAGMETER;
Image* Sexy::IMAGE_FLAGMETERLEVELPROGRESS;
Image* Sexy::IMAGE_FLAGMETERPARTS;
Image* Sexy::IMAGE_ICE;
Image* Sexy::IMAGE_ICETRAP;
Image* Sexy::IMAGE_ICETRAP2;
Image* Sexy::IMAGE_ICETRAP_PARTICLES;
Image* Sexy::IMAGE_ICE_CAP;
Image* Sexy::IMAGE_ICE_SPARKLES;
Image* Sexy::IMAGE_ICON_POOLCLEANER;
Image* Sexy::IMAGE_ICON_RAKE;
Image* Sexy::IMAGE_ICON_ROOFCLEANER;
Image* Sexy::IMAGE_IMITATERCLOUDS;
Image* Sexy::IMAGE_IMITATERPUFFS;
Image* Sexy::IMAGE_IMITATERSEED;
Image* Sexy::IMAGE_IMITATERSEEDDISABLED;
Image* Sexy::IMAGE_MELONPULT_PARTICLES;
Image* Sexy::IMAGE_MINIGAME_TROPHY;
Image* Sexy::IMAGE_MONEYBAG;
Image* Sexy::IMAGE_MONEYBAG_HI_RES;
Image* Sexy::IMAGE_NIGHT_GRAVE_GRAPHIC;
Image* Sexy::IMAGE_OPTIONS_BACKTOGAMEBUTTON0;
Image* Sexy::IMAGE_OPTIONS_BACKTOGAMEBUTTON2;
Image* Sexy::IMAGE_OPTIONS_CHECKBOX0;
Image* Sexy::IMAGE_OPTIONS_CHECKBOX1;
Image* Sexy::IMAGE_OPTIONS_MENUBACK;
Image* Sexy::IMAGE_OPTIONS_SLIDERKNOB2;
Image* Sexy::IMAGE_OPTIONS_SLIDERSLOT;
Image* Sexy::IMAGE_PACKET_PLANTS;
Image* Sexy::IMAGE_PEA_PARTICLES;
Image* Sexy::IMAGE_PEA_SHADOWS;
Image* Sexy::IMAGE_PEA_SPLATS;
Image* Sexy::IMAGE_PHONOGRAPH;
Image* Sexy::IMAGE_PINATA;
Image* Sexy::IMAGE_PLANTSHADOW;
Image* Sexy::IMAGE_PLANTSHADOW2;
Image* Sexy::IMAGE_PLANTSPEECHBUBBLE;
Image* Sexy::IMAGE_POOL;
Image* Sexy::IMAGE_POOLSPARKLY;
Image* Sexy::IMAGE_POOL_BASE;
Image* Sexy::IMAGE_POOL_BASE_NIGHT;
Image* Sexy::IMAGE_POOL_CAUSTIC_EFFECT;
Image* Sexy::IMAGE_POOL_NIGHT;
Image* Sexy::IMAGE_POOL_SHADING;
Image* Sexy::IMAGE_POOL_SHADING_NIGHT;
Image* Sexy::IMAGE_POTATOMINE_PARTICLES;
Image* Sexy::IMAGE_PRESENT;
Image* Sexy::IMAGE_PRESENTOPEN;
Image* Sexy::IMAGE_PROJECTILECACTUS;
Image* Sexy::IMAGE_PROJECTILEPEA;
Image* Sexy::IMAGE_PROJECTILESNOWPEA;
Image* Sexy::IMAGE_PROJECTILE_STAR;
Image* Sexy::IMAGE_PUFFSHROOM_PUFF1;
Image* Sexy::IMAGE_RAIN;
Image* Sexy::IMAGE_REANIM_CABBAGEPULT_CABBAGE;
Image* Sexy::IMAGE_REANIM_COBCANNON_COB;
Image* Sexy::IMAGE_REANIM_COINGLOW;
Image* Sexy::IMAGE_REANIM_COIN_GOLD_DOLLAR;
Image* Sexy::IMAGE_REANIM_COIN_SILVER_DOLLAR;
Image* Sexy::IMAGE_REANIM_CORNPULT_BUTTER;
Image* Sexy::IMAGE_REANIM_CORNPULT_BUTTER_SPLAT;
Image* Sexy::IMAGE_REANIM_CORNPULT_KERNAL;
Image* Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH1;
Image* Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH4;
Image* Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH5;
Image* Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH6;
Image* Sexy::IMAGE_REANIM_DIAMOND;
Image* Sexy::IMAGE_REANIM_GARLIC_BODY2;
Image* Sexy::IMAGE_REANIM_GARLIC_BODY3;
Image* Sexy::IMAGE_REANIM_MELONPULT_MELON;
Image* Sexy::IMAGE_REANIM_POT_TOP_DARK;
Image* Sexy::IMAGE_REANIM_PUMPKIN_DAMAGE1;
Image* Sexy::IMAGE_REANIM_PUMPKIN_DAMAGE3;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_BUTTON;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_HIGHLIGHT;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_ALMANAC_SHADOW;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_BG;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_BUTTON;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_HIGHLIGHT;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_KEY_SHADOW;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_BUTTON;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_HIGHLIGHT;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_BUTTON;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_HIGHLIGHT;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_BUTTON;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_HIGHLIGHT;
Image* Sexy::IMAGE_REANIM_SELECTORSCREEN_WOODSIGN2_PRESS;
Image* Sexy::IMAGE_REANIM_STINKY_TURN3;
Image* Sexy::IMAGE_REANIM_TALLNUT_CRACKED1;
Image* Sexy::IMAGE_REANIM_TALLNUT_CRACKED2;
Image* Sexy::IMAGE_REANIM_TREE_BG;
Image* Sexy::IMAGE_REANIM_WALLNUT_BODY;
Image* Sexy::IMAGE_REANIM_WALLNUT_CRACKED1;
Image* Sexy::IMAGE_REANIM_WALLNUT_CRACKED2;
Image* Sexy::IMAGE_REANIM_WINTERMELON_PROJECTILE;
Image* Sexy::IMAGE_REANIM_ZENGARDEN_BUGSPRAY_BOTTLE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BALLOON_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_HAND;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_EYEGLOW_BLUE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_ICEBALL;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_MOUTHGLOW_BLUE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BUCKET1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BUCKET2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BUCKET3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_BUNGI_HEAD_SCARED;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE_WITHBALL;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_WITHBALL;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_SIDING_DAMAGE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CONE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CONE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_CONE3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_DANCER_INNERARM_HAND;
Image* Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT;
Image* Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_DOLPHINRIDER_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_FLAG1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_FLAG3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET;
Image* Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_HAND;
Image* Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_DUCKXING;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_FOOT2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2_REDEYE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_ZOMBIE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_HEAD_GROSSOUT;
Image* Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES4;
Image* Sexy::IMAGE_REANIM_ZOMBIE_IMP_ARM1_BONE;
Image* Sexy::IMAGE_REANIM_ZOMBIE_IMP_ARM2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_JACKBOX_BOX;
Image* Sexy::IMAGE_REANIM_ZOMBIE_JACKBOX_OUTERARM_LOWER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_HAND;
Image* Sexy::IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_LADDER_1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_LADDER_5;
Image* Sexy::IMAGE_REANIM_ZOMBIE_LADDER_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_MUSTACHE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_MUSTACHE3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_OUTERARM_HAND;
Image* Sexy::IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_LOWER;
Image* Sexy::IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_PAPER_MADHEAD;
Image* Sexy::IMAGE_REANIM_ZOMBIE_PAPER_PAPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_PAPER_PAPER3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_POGO_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICKHANDS2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_POLEVAULTER_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_SCREENDOOR1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_SCREENDOOR2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_SCREENDOOR3;
Image* Sexy::IMAGE_REANIM_ZOMBIE_SNORKLE_HEAD;
Image* Sexy::IMAGE_REANIM_ZOMBIE_SNORKLE_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_HAND;
Image* Sexy::IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_UPPER2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE1;
Image* Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE2;
Image* Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_WHEEL_FLAT;
Image* Sexy::IMAGE_ROCKSMALL;
Image* Sexy::IMAGE_SCARY_POT;
Image* Sexy::IMAGE_SEEDBANK;
Image* Sexy::IMAGE_SEEDCHOOSER_BACKGROUND;
Image* Sexy::IMAGE_SEEDCHOOSER_BUTTON;
Image* Sexy::IMAGE_SEEDCHOOSER_BUTTON2;
Image* Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW;
Image* Sexy::IMAGE_SEEDCHOOSER_BUTTON_DISABLED;
Image* Sexy::IMAGE_SEEDCHOOSER_BUTTON_GLOW;
Image* Sexy::IMAGE_SEEDCHOOSER_IMITATERADDON;
Image* Sexy::IMAGE_SEEDPACKETFLASH;
Image* Sexy::IMAGE_SEEDPACKETSILHOUETTE;
Image* Sexy::IMAGE_SEEDPACKET_LARGER;
Image* Sexy::IMAGE_SEEDS;
Image* Sexy::IMAGE_SELECTORSCREEN_ALMANAC;
Image* Sexy::IMAGE_SELECTORSCREEN_ALMANACHIGHLIGHT;
Image* Sexy::IMAGE_SELECTORSCREEN_HELP1;
Image* Sexy::IMAGE_SELECTORSCREEN_HELP2;
Image* Sexy::IMAGE_SELECTORSCREEN_LEVELNUMBERS;
Image* Sexy::IMAGE_SELECTORSCREEN_OPTIONS1;
Image* Sexy::IMAGE_SELECTORSCREEN_OPTIONS2;
Image* Sexy::IMAGE_SELECTORSCREEN_QUIT1;
Image* Sexy::IMAGE_SELECTORSCREEN_QUIT2;
Image* Sexy::IMAGE_SELECTORSCREEN_STORE;
Image* Sexy::IMAGE_SELECTORSCREEN_STOREHIGHLIGHT;
Image* Sexy::IMAGE_SELECTORSCREEN_ZENGARDEN;
Image* Sexy::IMAGE_SELECTORSCREEN_ZENGARDENHIGHLIGHT;
Image* Sexy::IMAGE_SHOVEL;
Image* Sexy::IMAGE_SHOVELBANK;
Image* Sexy::IMAGE_SHOVEL_HI_RES;
Image* Sexy::IMAGE_SLOTMACHINE_OVERLAY;
Image* Sexy::IMAGE_SNOWFLAKES;
Image* Sexy::IMAGE_SNOWPEA_PARTICLES;
Image* Sexy::IMAGE_SNOWPEA_PUFF;
Image* Sexy::IMAGE_SNOWPEA_SPLATS;
Image* Sexy::IMAGE_SPOTLIGHT;
Image* Sexy::IMAGE_SPOTLIGHT2;
Image* Sexy::IMAGE_STAR_PARTICLES;
Image* Sexy::IMAGE_STAR_SPLATS;
Image* Sexy::IMAGE_STORE_SPEECHBUBBLE;
Image* Sexy::IMAGE_STORE_SPEECHBUBBLE2;
Image* Sexy::IMAGE_SUNBANK;
Image* Sexy::IMAGE_SUNFLOWER_TROPHY;
Image* Sexy::IMAGE_TACO;
Image* Sexy::IMAGE_TOMBSTONES;
Image* Sexy::IMAGE_TOMBSTONE_MOUNDS;
Image* Sexy::IMAGE_TREEFOOD;
Image* Sexy::IMAGE_TROPHY;
Image* Sexy::IMAGE_TROPHY_HI_RES;
Image* Sexy::IMAGE_VASE_CHUNKS;
Image* Sexy::IMAGE_WALLNUTPARTICLESLARGE;
Image* Sexy::IMAGE_WALLNUTPARTICLESSMALL;
Image* Sexy::IMAGE_WALLNUT_BOWLINGSTRIPE;
Image* Sexy::IMAGE_WATERDROP;
Image* Sexy::IMAGE_WATERINGCAN;
Image* Sexy::IMAGE_WATERINGCANGOLD;
Image* Sexy::IMAGE_WATERPARTICLE;
Image* Sexy::IMAGE_WHITEPIXEL;
Image* Sexy::IMAGE_WHITEWATER;
Image* Sexy::IMAGE_WHITEWATER_SHADOW;
Image* Sexy::IMAGE_WINTERMELON_PARTICLES;
Image* Sexy::IMAGE_ZAMBONISMOKE;
Image* Sexy::IMAGE_ZENSHOPBUTTON;
Image* Sexy::IMAGE_ZENSHOPBUTTON_HIGHLIGHT;
Image* Sexy::IMAGE_ZEN_GARDENGLOVE;
Image* Sexy::IMAGE_ZEN_GOLDTOOLRETICLE;
Image* Sexy::IMAGE_ZEN_MONEYSIGN;
Image* Sexy::IMAGE_ZEN_NEED_ICONS;
Image* Sexy::IMAGE_ZEN_NEXTGARDEN;
Image* Sexy::IMAGE_ZEN_WHEELBARROW;
Image* Sexy::IMAGE_ZOMBIEBACKUPDANCERHEAD;
Image* Sexy::IMAGE_ZOMBIEBALLOONHEAD;
Image* Sexy::IMAGE_ZOMBIEBOBSLEDHEAD;
Image* Sexy::IMAGE_ZOMBIEDANCERHEAD;
Image* Sexy::IMAGE_ZOMBIEDIGGERARM;
Image* Sexy::IMAGE_ZOMBIEDIGGERHEAD;
Image* Sexy::IMAGE_ZOMBIEDOLPHINRIDERHEAD;
Image* Sexy::IMAGE_ZOMBIEFOOTBALLHEAD;
Image* Sexy::IMAGE_ZOMBIEFUTUREGLASSES;
Image* Sexy::IMAGE_ZOMBIEIMPHEAD;
Image* Sexy::IMAGE_ZOMBIEJACKBOXARM;
Image* Sexy::IMAGE_ZOMBIELADDERHEAD;
Image* Sexy::IMAGE_ZOMBIEPOGO;
Image* Sexy::IMAGE_ZOMBIEPOLEVAULTERHEAD;
Image* Sexy::IMAGE_ZOMBIEYETIHEAD;
Image* Sexy::IMAGE_ZOMBIE_BOBSLED1;
Image* Sexy::IMAGE_ZOMBIE_BOBSLED2;
Image* Sexy::IMAGE_ZOMBIE_BOBSLED3;
Image* Sexy::IMAGE_ZOMBIE_BOBSLED4;
Image* Sexy::IMAGE_ZOMBIE_BOBSLED_INSIDE;
Image* Sexy::IMAGE_ZOMBIE_BOSS_FIREBALL_GROUNDPARTICLES;
Image* Sexy::IMAGE_ZOMBIE_BOSS_FIREBALL_PARTICLES;
Image* Sexy::IMAGE_ZOMBIE_BOSS_ICEBALL_GROUNDPARTICLES;
Image* Sexy::IMAGE_ZOMBIE_BOSS_ICEBALL_PARTICLES;
Image* Sexy::IMAGE_ZOMBIE_NOTE_SMALL;
Image* Sexy::IMAGE_ZOMBIE_SEAWEED;
Image* Sexy::IMAGE_ZOMBOSS_PARTICLES;

bool Sexy::ExtractLoadingImagesResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		IMAGE_ALMANAC = aMgr.GetImageThrow("IMAGE_ALMANAC");
		IMAGE_ALMANAC_IMITATER = aMgr.GetImageThrow("IMAGE_ALMANAC_IMITATER");
		IMAGE_AWARDPICKUPGLOW = aMgr.GetImageThrow("IMAGE_AWARDPICKUPGLOW");
		IMAGE_BEGHOULED_TWIST_OVERLAY = aMgr.GetImageThrow("IMAGE_BEGHOULED_TWIST_OVERLAY");
		IMAGE_BRAIN = aMgr.GetImageThrow("IMAGE_BRAIN");
		IMAGE_BUG_SPRAY = aMgr.GetImageThrow("IMAGE_BUG_SPRAY");
		IMAGE_BUNGEECORD = aMgr.GetImageThrow("IMAGE_BUNGEECORD");
		IMAGE_BUNGEETARGET = aMgr.GetImageThrow("IMAGE_BUNGEETARGET");
		IMAGE_BUTTON_DOWN_LEFT = aMgr.GetImageThrow("IMAGE_BUTTON_DOWN_LEFT");
		IMAGE_BUTTON_DOWN_MIDDLE = aMgr.GetImageThrow("IMAGE_BUTTON_DOWN_MIDDLE");
		IMAGE_BUTTON_DOWN_RIGHT = aMgr.GetImageThrow("IMAGE_BUTTON_DOWN_RIGHT");
		IMAGE_BUTTON_LEFT = aMgr.GetImageThrow("IMAGE_BUTTON_LEFT");
		IMAGE_BUTTON_MIDDLE = aMgr.GetImageThrow("IMAGE_BUTTON_MIDDLE");
		IMAGE_BUTTON_RIGHT = aMgr.GetImageThrow("IMAGE_BUTTON_RIGHT");
		IMAGE_CARKEYS = aMgr.GetImageThrow("IMAGE_CARKEYS");
		IMAGE_CHOCOLATE = aMgr.GetImageThrow("IMAGE_CHOCOLATE");
		IMAGE_COBCANNON_POPCORN = aMgr.GetImageThrow("IMAGE_COBCANNON_POPCORN");
		IMAGE_COBCANNON_TARGET = aMgr.GetImageThrow("IMAGE_COBCANNON_TARGET");
		IMAGE_COINBANK = aMgr.GetImageThrow("IMAGE_COINBANK");
		IMAGE_CONVEYORBELT = aMgr.GetImageThrow("IMAGE_CONVEYORBELT");
		IMAGE_CONVEYORBELT_BACKDROP = aMgr.GetImageThrow("IMAGE_CONVEYORBELT_BACKDROP");
		IMAGE_CRATER = aMgr.GetImageThrow("IMAGE_CRATER");
		IMAGE_CRATER_FADING = aMgr.GetImageThrow("IMAGE_CRATER_FADING");
		IMAGE_CRATER_ROOF_CENTER = aMgr.GetImageThrow("IMAGE_CRATER_ROOF_CENTER");
		IMAGE_CRATER_ROOF_LEFT = aMgr.GetImageThrow("IMAGE_CRATER_ROOF_LEFT");
		IMAGE_CRATER_WATER_DAY = aMgr.GetImageThrow("IMAGE_CRATER_WATER_DAY");
		IMAGE_CRATER_WATER_NIGHT = aMgr.GetImageThrow("IMAGE_CRATER_WATER_NIGHT");
		IMAGE_DIALOG_BIGBOTTOMLEFT = aMgr.GetImageThrow("IMAGE_DIALOG_BIGBOTTOMLEFT");
		IMAGE_DIALOG_BIGBOTTOMMIDDLE = aMgr.GetImageThrow("IMAGE_DIALOG_BIGBOTTOMMIDDLE");
		IMAGE_DIALOG_BIGBOTTOMRIGHT = aMgr.GetImageThrow("IMAGE_DIALOG_BIGBOTTOMRIGHT");
		IMAGE_DIALOG_BOTTOMLEFT = aMgr.GetImageThrow("IMAGE_DIALOG_BOTTOMLEFT");
		IMAGE_DIALOG_BOTTOMMIDDLE = aMgr.GetImageThrow("IMAGE_DIALOG_BOTTOMMIDDLE");
		IMAGE_DIALOG_BOTTOMRIGHT = aMgr.GetImageThrow("IMAGE_DIALOG_BOTTOMRIGHT");
		IMAGE_DIALOG_CENTERLEFT = aMgr.GetImageThrow("IMAGE_DIALOG_CENTERLEFT");
		IMAGE_DIALOG_CENTERMIDDLE = aMgr.GetImageThrow("IMAGE_DIALOG_CENTERMIDDLE");
		IMAGE_DIALOG_CENTERRIGHT = aMgr.GetImageThrow("IMAGE_DIALOG_CENTERRIGHT");
		IMAGE_DIALOG_HEADER = aMgr.GetImageThrow("IMAGE_DIALOG_HEADER");
		IMAGE_DIALOG_TOPLEFT = aMgr.GetImageThrow("IMAGE_DIALOG_TOPLEFT");
		IMAGE_DIALOG_TOPMIDDLE = aMgr.GetImageThrow("IMAGE_DIALOG_TOPMIDDLE");
		IMAGE_DIALOG_TOPRIGHT = aMgr.GetImageThrow("IMAGE_DIALOG_TOPRIGHT");
		IMAGE_DIRTBIG = aMgr.GetImageThrow("IMAGE_DIRTBIG");
		IMAGE_DIRTSMALL = aMgr.GetImageThrow("IMAGE_DIRTSMALL");
		IMAGE_DOOMSHROOM_EXPLOSION_BASE = aMgr.GetImageThrow("IMAGE_DOOMSHROOM_EXPLOSION_BASE");
		IMAGE_DUST_PUFFS = aMgr.GetImageThrow("IMAGE_DUST_PUFFS");
		IMAGE_EDITBOX = aMgr.GetImageThrow("IMAGE_EDITBOX");
		IMAGE_FERTILIZER = aMgr.GetImageThrow("IMAGE_FERTILIZER");
		IMAGE_FLAGMETER = aMgr.GetImageThrow("IMAGE_FLAGMETER");
		IMAGE_FLAGMETERLEVELPROGRESS = aMgr.GetImageThrow("IMAGE_FLAGMETERLEVELPROGRESS");
		IMAGE_FLAGMETERPARTS = aMgr.GetImageThrow("IMAGE_FLAGMETERPARTS");
		IMAGE_ICE = aMgr.GetImageThrow("IMAGE_ICE");
		IMAGE_ICETRAP = aMgr.GetImageThrow("IMAGE_ICETRAP");
		IMAGE_ICETRAP2 = aMgr.GetImageThrow("IMAGE_ICETRAP2");
		IMAGE_ICETRAP_PARTICLES = aMgr.GetImageThrow("IMAGE_ICETRAP_PARTICLES");
		IMAGE_ICE_CAP = aMgr.GetImageThrow("IMAGE_ICE_CAP");
		IMAGE_ICE_SPARKLES = aMgr.GetImageThrow("IMAGE_ICE_SPARKLES");
		IMAGE_ICON_POOLCLEANER = aMgr.GetImageThrow("IMAGE_ICON_POOLCLEANER");
		IMAGE_ICON_RAKE = aMgr.GetImageThrow("IMAGE_ICON_RAKE");
		IMAGE_ICON_ROOFCLEANER = aMgr.GetImageThrow("IMAGE_ICON_ROOFCLEANER");
		IMAGE_IMITATERCLOUDS = aMgr.GetImageThrow("IMAGE_IMITATERCLOUDS");
		IMAGE_IMITATERPUFFS = aMgr.GetImageThrow("IMAGE_IMITATERPUFFS");
		IMAGE_IMITATERSEED = aMgr.GetImageThrow("IMAGE_IMITATERSEED");
		IMAGE_IMITATERSEEDDISABLED = aMgr.GetImageThrow("IMAGE_IMITATERSEEDDISABLED");
		IMAGE_MELONPULT_PARTICLES = aMgr.GetImageThrow("IMAGE_MELONPULT_PARTICLES");
		IMAGE_MINIGAME_TROPHY = aMgr.GetImageThrow("IMAGE_MINIGAME_TROPHY");
		IMAGE_MONEYBAG = aMgr.GetImageThrow("IMAGE_MONEYBAG");
		IMAGE_MONEYBAG_HI_RES = aMgr.GetImageThrow("IMAGE_MONEYBAG_HI_RES");
		IMAGE_NIGHT_GRAVE_GRAPHIC = aMgr.GetImageThrow("IMAGE_NIGHT_GRAVE_GRAPHIC");
		IMAGE_OPTIONS_BACKTOGAMEBUTTON0 = aMgr.GetImageThrow("IMAGE_OPTIONS_BACKTOGAMEBUTTON0");
		IMAGE_OPTIONS_BACKTOGAMEBUTTON2 = aMgr.GetImageThrow("IMAGE_OPTIONS_BACKTOGAMEBUTTON2");
		IMAGE_OPTIONS_CHECKBOX0 = aMgr.GetImageThrow("IMAGE_OPTIONS_CHECKBOX0");
		IMAGE_OPTIONS_CHECKBOX1 = aMgr.GetImageThrow("IMAGE_OPTIONS_CHECKBOX1");
		IMAGE_OPTIONS_MENUBACK = aMgr.GetImageThrow("IMAGE_OPTIONS_MENUBACK");
		IMAGE_OPTIONS_SLIDERKNOB2 = aMgr.GetImageThrow("IMAGE_OPTIONS_SLIDERKNOB2");
		IMAGE_OPTIONS_SLIDERSLOT = aMgr.GetImageThrow("IMAGE_OPTIONS_SLIDERSLOT");
		IMAGE_PACKET_PLANTS = aMgr.GetImageThrow("IMAGE_PACKET_PLANTS");
		IMAGE_PEA_PARTICLES = aMgr.GetImageThrow("IMAGE_PEA_PARTICLES");
		IMAGE_PEA_SHADOWS = aMgr.GetImageThrow("IMAGE_PEA_SHADOWS");
		IMAGE_PEA_SPLATS = aMgr.GetImageThrow("IMAGE_PEA_SPLATS");
		IMAGE_PHONOGRAPH = aMgr.GetImageThrow("IMAGE_PHONOGRAPH");
		IMAGE_PINATA = aMgr.GetImageThrow("IMAGE_PINATA");
		IMAGE_PLANTSHADOW = aMgr.GetImageThrow("IMAGE_PLANTSHADOW");
		IMAGE_PLANTSHADOW2 = aMgr.GetImageThrow("IMAGE_PLANTSHADOW2");
		IMAGE_PLANTSPEECHBUBBLE = aMgr.GetImageThrow("IMAGE_PLANTSPEECHBUBBLE");
		IMAGE_POOL = aMgr.GetImageThrow("IMAGE_POOL");
		IMAGE_POOLSPARKLY = aMgr.GetImageThrow("IMAGE_POOLSPARKLY");
		IMAGE_POOL_BASE = aMgr.GetImageThrow("IMAGE_POOL_BASE");
		IMAGE_POOL_BASE_NIGHT = aMgr.GetImageThrow("IMAGE_POOL_BASE_NIGHT");
		IMAGE_POOL_CAUSTIC_EFFECT = aMgr.GetImageThrow("IMAGE_POOL_CAUSTIC_EFFECT");
		IMAGE_POOL_NIGHT = aMgr.GetImageThrow("IMAGE_POOL_NIGHT");
		IMAGE_POOL_SHADING = aMgr.GetImageThrow("IMAGE_POOL_SHADING");
		IMAGE_POOL_SHADING_NIGHT = aMgr.GetImageThrow("IMAGE_POOL_SHADING_NIGHT");
		IMAGE_POTATOMINE_PARTICLES = aMgr.GetImageThrow("IMAGE_POTATOMINE_PARTICLES");
		IMAGE_PRESENT = aMgr.GetImageThrow("IMAGE_PRESENT");
		IMAGE_PRESENTOPEN = aMgr.GetImageThrow("IMAGE_PRESENTOPEN");
		IMAGE_PROJECTILECACTUS = aMgr.GetImageThrow("IMAGE_PROJECTILECACTUS");
		IMAGE_PROJECTILEPEA = aMgr.GetImageThrow("IMAGE_PROJECTILEPEA");
		IMAGE_PROJECTILESNOWPEA = aMgr.GetImageThrow("IMAGE_PROJECTILESNOWPEA");
		IMAGE_PROJECTILE_STAR = aMgr.GetImageThrow("IMAGE_PROJECTILE_STAR");
		IMAGE_PUFFSHROOM_PUFF1 = aMgr.GetImageThrow("IMAGE_PUFFSHROOM_PUFF1");
		IMAGE_RAIN = aMgr.GetImageThrow("IMAGE_RAIN");
		IMAGE_REANIM_CABBAGEPULT_CABBAGE = aMgr.GetImageThrow("IMAGE_REANIM_CABBAGEPULT_CABBAGE");
		IMAGE_REANIM_COBCANNON_COB = aMgr.GetImageThrow("IMAGE_REANIM_COBCANNON_COB");
		IMAGE_REANIM_COINGLOW = aMgr.GetImageThrow("IMAGE_REANIM_COINGLOW");
		IMAGE_REANIM_COIN_GOLD_DOLLAR = aMgr.GetImageThrow("IMAGE_REANIM_COIN_GOLD_DOLLAR");
		IMAGE_REANIM_COIN_SILVER_DOLLAR = aMgr.GetImageThrow("IMAGE_REANIM_COIN_SILVER_DOLLAR");
		IMAGE_REANIM_CORNPULT_BUTTER = aMgr.GetImageThrow("IMAGE_REANIM_CORNPULT_BUTTER");
		IMAGE_REANIM_CORNPULT_BUTTER_SPLAT = aMgr.GetImageThrow("IMAGE_REANIM_CORNPULT_BUTTER_SPLAT");
		IMAGE_REANIM_CORNPULT_KERNAL = aMgr.GetImageThrow("IMAGE_REANIM_CORNPULT_KERNAL");
		IMAGE_REANIM_CRAZYDAVE_MOUTH1 = aMgr.GetImageThrow("IMAGE_REANIM_CRAZYDAVE_MOUTH1");
		IMAGE_REANIM_CRAZYDAVE_MOUTH4 = aMgr.GetImageThrow("IMAGE_REANIM_CRAZYDAVE_MOUTH4");
		IMAGE_REANIM_CRAZYDAVE_MOUTH5 = aMgr.GetImageThrow("IMAGE_REANIM_CRAZYDAVE_MOUTH5");
		IMAGE_REANIM_CRAZYDAVE_MOUTH6 = aMgr.GetImageThrow("IMAGE_REANIM_CRAZYDAVE_MOUTH6");
		IMAGE_REANIM_DIAMOND = aMgr.GetImageThrow("IMAGE_REANIM_DIAMOND");
		IMAGE_REANIM_GARLIC_BODY2 = aMgr.GetImageThrow("IMAGE_REANIM_GARLIC_BODY2");
		IMAGE_REANIM_GARLIC_BODY3 = aMgr.GetImageThrow("IMAGE_REANIM_GARLIC_BODY3");
		IMAGE_REANIM_MELONPULT_MELON = aMgr.GetImageThrow("IMAGE_REANIM_MELONPULT_MELON");
		IMAGE_REANIM_POT_TOP_DARK = aMgr.GetImageThrow("IMAGE_REANIM_POT_TOP_DARK");
		IMAGE_REANIM_PUMPKIN_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_PUMPKIN_DAMAGE1");
		IMAGE_REANIM_PUMPKIN_DAMAGE3 = aMgr.GetImageThrow("IMAGE_REANIM_PUMPKIN_DAMAGE3");
		IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_BUTTON = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_BUTTON");
		IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_HIGHLIGHT = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_HIGHLIGHT");
		IMAGE_REANIM_SELECTORSCREEN_ALMANAC_SHADOW = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_ALMANAC_SHADOW");
		IMAGE_REANIM_SELECTORSCREEN_BG = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_BG");
		IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_BUTTON = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_BUTTON");
		IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_HIGHLIGHT = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_HIGHLIGHT");
		IMAGE_REANIM_SELECTORSCREEN_KEY_SHADOW = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_KEY_SHADOW");
		IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_BUTTON = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_BUTTON");
		IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_HIGHLIGHT = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_HIGHLIGHT");
		IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_BUTTON = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_BUTTON");
		IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_HIGHLIGHT = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_HIGHLIGHT");
		IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_BUTTON = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_BUTTON");
		IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_HIGHLIGHT = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_HIGHLIGHT");
		IMAGE_REANIM_SELECTORSCREEN_WOODSIGN2_PRESS = aMgr.GetImageThrow("IMAGE_REANIM_SELECTORSCREEN_WOODSIGN2_PRESS");
		IMAGE_REANIM_STINKY_TURN3 = aMgr.GetImageThrow("IMAGE_REANIM_STINKY_TURN3");
		IMAGE_REANIM_TALLNUT_CRACKED1 = aMgr.GetImageThrow("IMAGE_REANIM_TALLNUT_CRACKED1");
		IMAGE_REANIM_TALLNUT_CRACKED2 = aMgr.GetImageThrow("IMAGE_REANIM_TALLNUT_CRACKED2");
		IMAGE_REANIM_TREE_BG = aMgr.GetImageThrow("IMAGE_REANIM_TREE_BG");
		IMAGE_REANIM_WALLNUT_BODY = aMgr.GetImageThrow("IMAGE_REANIM_WALLNUT_BODY");
		IMAGE_REANIM_WALLNUT_CRACKED1 = aMgr.GetImageThrow("IMAGE_REANIM_WALLNUT_CRACKED1");
		IMAGE_REANIM_WALLNUT_CRACKED2 = aMgr.GetImageThrow("IMAGE_REANIM_WALLNUT_CRACKED2");
		IMAGE_REANIM_WINTERMELON_PROJECTILE = aMgr.GetImageThrow("IMAGE_REANIM_WINTERMELON_PROJECTILE");
		IMAGE_REANIM_ZENGARDEN_BUGSPRAY_BOTTLE = aMgr.GetImageThrow("IMAGE_REANIM_ZENGARDEN_BUGSPRAY_BOTTLE");
		IMAGE_REANIM_ZOMBIE_BALLOON_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BALLOON_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_HAND = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_HAND");
		IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_BOSS_EYEGLOW_BLUE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_EYEGLOW_BLUE");
		IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_BOSS_ICEBALL = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_ICEBALL");
		IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_BOSS_MOUTHGLOW_BLUE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_MOUTHGLOW_BLUE");
		IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_BUCKET1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BUCKET1");
		IMAGE_REANIM_ZOMBIE_BUCKET2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BUCKET2");
		IMAGE_REANIM_ZOMBIE_BUCKET3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BUCKET3");
		IMAGE_REANIM_ZOMBIE_BUNGI_HEAD_SCARED = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_BUNGI_HEAD_SCARED");
		IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL");
		IMAGE_REANIM_ZOMBIE_CATAPULT_POLE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CATAPULT_POLE");
		IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE");
		IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE_WITHBALL = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE_WITHBALL");
		IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_WITHBALL = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_WITHBALL");
		IMAGE_REANIM_ZOMBIE_CATAPULT_SIDING_DAMAGE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CATAPULT_SIDING_DAMAGE");
		IMAGE_REANIM_ZOMBIE_CONE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CONE1");
		IMAGE_REANIM_ZOMBIE_CONE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CONE2");
		IMAGE_REANIM_ZOMBIE_CONE3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_CONE3");
		IMAGE_REANIM_ZOMBIE_DANCER_INNERARM_HAND = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_DANCER_INNERARM_HAND");
		IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT");
		IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT2");
		IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT3");
		IMAGE_REANIM_ZOMBIE_DIGGER_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_DIGGER_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE");
		IMAGE_REANIM_ZOMBIE_DOLPHINRIDER_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_DOLPHINRIDER_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_FLAG1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_FLAG1");
		IMAGE_REANIM_ZOMBIE_FLAG3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_FLAG3");
		IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET");
		IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2");
		IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3");
		IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_HAND = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_HAND");
		IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_2");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_3");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_DUCKXING = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_DUCKXING");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_FOOT2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_FOOT2");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2_REDEYE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2_REDEYE");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2");
		IMAGE_REANIM_ZOMBIE_GARGANTUAR_ZOMBIE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_GARGANTUAR_ZOMBIE");
		IMAGE_REANIM_ZOMBIE_HEAD_GROSSOUT = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_HEAD_GROSSOUT");
		IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES1");
		IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES2");
		IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES3");
		IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES4 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES4");
		IMAGE_REANIM_ZOMBIE_IMP_ARM1_BONE = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_IMP_ARM1_BONE");
		IMAGE_REANIM_ZOMBIE_IMP_ARM2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_IMP_ARM2");
		IMAGE_REANIM_ZOMBIE_JACKBOX_BOX = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_JACKBOX_BOX");
		IMAGE_REANIM_ZOMBIE_JACKBOX_OUTERARM_LOWER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_JACKBOX_OUTERARM_LOWER2");
		IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_HAND = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_HAND");
		IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_LADDER_1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_LADDER_1");
		IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_LADDER_5 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_LADDER_5");
		IMAGE_REANIM_ZOMBIE_LADDER_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_LADDER_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_MUSTACHE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_MUSTACHE2");
		IMAGE_REANIM_ZOMBIE_MUSTACHE3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_MUSTACHE3");
		IMAGE_REANIM_ZOMBIE_OUTERARM_HAND = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_OUTERARM_HAND");
		IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_LOWER = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_LOWER");
		IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_PAPER_MADHEAD = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_PAPER_MADHEAD");
		IMAGE_REANIM_ZOMBIE_PAPER_PAPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_PAPER_PAPER2");
		IMAGE_REANIM_ZOMBIE_PAPER_PAPER3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_PAPER_PAPER3");
		IMAGE_REANIM_ZOMBIE_POGO_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_POGO_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE1");
		IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE2");
		IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE1");
		IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE2");
		IMAGE_REANIM_ZOMBIE_POGO_STICKHANDS2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_POGO_STICKHANDS2");
		IMAGE_REANIM_ZOMBIE_POLEVAULTER_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_POLEVAULTER_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_SCREENDOOR1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_SCREENDOOR1");
		IMAGE_REANIM_ZOMBIE_SCREENDOOR2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_SCREENDOOR2");
		IMAGE_REANIM_ZOMBIE_SCREENDOOR3 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_SCREENDOOR3");
		IMAGE_REANIM_ZOMBIE_SNORKLE_HEAD = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_SNORKLE_HEAD");
		IMAGE_REANIM_ZOMBIE_SNORKLE_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_SNORKLE_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_HAND = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_HAND");
		IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_UPPER2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_UPPER2");
		IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE1 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE1");
		IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE2 = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE2");
		IMAGE_REANIM_ZOMBIE_ZAMBONI_WHEEL_FLAT = aMgr.GetImageThrow("IMAGE_REANIM_ZOMBIE_ZAMBONI_WHEEL_FLAT");
		IMAGE_ROCKSMALL = aMgr.GetImageThrow("IMAGE_ROCKSMALL");
		IMAGE_SCARY_POT = aMgr.GetImageThrow("IMAGE_SCARY_POT");
		IMAGE_SEEDBANK = aMgr.GetImageThrow("IMAGE_SEEDBANK");
		IMAGE_SEEDCHOOSER_BACKGROUND = aMgr.GetImageThrow("IMAGE_SEEDCHOOSER_BACKGROUND");
		IMAGE_SEEDCHOOSER_BUTTON = aMgr.GetImageThrow("IMAGE_SEEDCHOOSER_BUTTON");
		IMAGE_SEEDCHOOSER_BUTTON2 = aMgr.GetImageThrow("IMAGE_SEEDCHOOSER_BUTTON2");
		IMAGE_SEEDCHOOSER_BUTTON2_GLOW = aMgr.GetImageThrow("IMAGE_SEEDCHOOSER_BUTTON2_GLOW");
		IMAGE_SEEDCHOOSER_BUTTON_DISABLED = aMgr.GetImageThrow("IMAGE_SEEDCHOOSER_BUTTON_DISABLED");
		IMAGE_SEEDCHOOSER_BUTTON_GLOW = aMgr.GetImageThrow("IMAGE_SEEDCHOOSER_BUTTON_GLOW");
		IMAGE_SEEDCHOOSER_IMITATERADDON = aMgr.GetImageThrow("IMAGE_SEEDCHOOSER_IMITATERADDON");
		IMAGE_SEEDPACKETFLASH = aMgr.GetImageThrow("IMAGE_SEEDPACKETFLASH");
		IMAGE_SEEDPACKETSILHOUETTE = aMgr.GetImageThrow("IMAGE_SEEDPACKETSILHOUETTE");
		IMAGE_SEEDPACKET_LARGER = aMgr.GetImageThrow("IMAGE_SEEDPACKET_LARGER");
		IMAGE_SEEDS = aMgr.GetImageThrow("IMAGE_SEEDS");
		IMAGE_SELECTORSCREEN_ALMANAC = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_ALMANAC");
		IMAGE_SELECTORSCREEN_ALMANACHIGHLIGHT = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_ALMANACHIGHLIGHT");
		IMAGE_SELECTORSCREEN_HELP1 = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_HELP1");
		IMAGE_SELECTORSCREEN_HELP2 = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_HELP2");
		IMAGE_SELECTORSCREEN_LEVELNUMBERS = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_LEVELNUMBERS");
		IMAGE_SELECTORSCREEN_OPTIONS1 = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_OPTIONS1");
		IMAGE_SELECTORSCREEN_OPTIONS2 = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_OPTIONS2");
		IMAGE_SELECTORSCREEN_QUIT1 = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_QUIT1");
		IMAGE_SELECTORSCREEN_QUIT2 = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_QUIT2");
		IMAGE_SELECTORSCREEN_STORE = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_STORE");
		IMAGE_SELECTORSCREEN_STOREHIGHLIGHT = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_STOREHIGHLIGHT");
		IMAGE_SELECTORSCREEN_ZENGARDEN = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_ZENGARDEN");
		IMAGE_SELECTORSCREEN_ZENGARDENHIGHLIGHT = aMgr.GetImageThrow("IMAGE_SELECTORSCREEN_ZENGARDENHIGHLIGHT");
		IMAGE_SHOVEL = aMgr.GetImageThrow("IMAGE_SHOVEL");
		IMAGE_SHOVELBANK = aMgr.GetImageThrow("IMAGE_SHOVELBANK");
		IMAGE_SHOVEL_HI_RES = aMgr.GetImageThrow("IMAGE_SHOVEL_HI_RES");
		IMAGE_SLOTMACHINE_OVERLAY = aMgr.GetImageThrow("IMAGE_SLOTMACHINE_OVERLAY");
		IMAGE_SNOWFLAKES = aMgr.GetImageThrow("IMAGE_SNOWFLAKES");
		IMAGE_SNOWPEA_PARTICLES = aMgr.GetImageThrow("IMAGE_SNOWPEA_PARTICLES");
		IMAGE_SNOWPEA_PUFF = aMgr.GetImageThrow("IMAGE_SNOWPEA_PUFF");
		IMAGE_SNOWPEA_SPLATS = aMgr.GetImageThrow("IMAGE_SNOWPEA_SPLATS");
		IMAGE_SPOTLIGHT = aMgr.GetImageThrow("IMAGE_SPOTLIGHT");
		IMAGE_SPOTLIGHT2 = aMgr.GetImageThrow("IMAGE_SPOTLIGHT2");
		IMAGE_STAR_PARTICLES = aMgr.GetImageThrow("IMAGE_STAR_PARTICLES");
		IMAGE_STAR_SPLATS = aMgr.GetImageThrow("IMAGE_STAR_SPLATS");
		IMAGE_STORE_SPEECHBUBBLE = aMgr.GetImageThrow("IMAGE_STORE_SPEECHBUBBLE");
		IMAGE_STORE_SPEECHBUBBLE2 = aMgr.GetImageThrow("IMAGE_STORE_SPEECHBUBBLE2");
		IMAGE_SUNBANK = aMgr.GetImageThrow("IMAGE_SUNBANK");
		IMAGE_SUNFLOWER_TROPHY = aMgr.GetImageThrow("IMAGE_SUNFLOWER_TROPHY");
		IMAGE_TACO = aMgr.GetImageThrow("IMAGE_TACO");
		IMAGE_TOMBSTONES = aMgr.GetImageThrow("IMAGE_TOMBSTONES");
		IMAGE_TOMBSTONE_MOUNDS = aMgr.GetImageThrow("IMAGE_TOMBSTONE_MOUNDS");
		IMAGE_TREEFOOD = aMgr.GetImageThrow("IMAGE_TREEFOOD");
		IMAGE_TROPHY = aMgr.GetImageThrow("IMAGE_TROPHY");
		IMAGE_TROPHY_HI_RES = aMgr.GetImageThrow("IMAGE_TROPHY_HI_RES");
		IMAGE_VASE_CHUNKS = aMgr.GetImageThrow("IMAGE_VASE_CHUNKS");
		IMAGE_WALLNUTPARTICLESLARGE = aMgr.GetImageThrow("IMAGE_WALLNUTPARTICLESLARGE");
		IMAGE_WALLNUTPARTICLESSMALL = aMgr.GetImageThrow("IMAGE_WALLNUTPARTICLESSMALL");
		IMAGE_WALLNUT_BOWLINGSTRIPE = aMgr.GetImageThrow("IMAGE_WALLNUT_BOWLINGSTRIPE");
		IMAGE_WATERDROP = aMgr.GetImageThrow("IMAGE_WATERDROP");
		IMAGE_WATERINGCAN = aMgr.GetImageThrow("IMAGE_WATERINGCAN");
		IMAGE_WATERINGCANGOLD = aMgr.GetImageThrow("IMAGE_WATERINGCANGOLD");
		IMAGE_WATERPARTICLE = aMgr.GetImageThrow("IMAGE_WATERPARTICLE");
		IMAGE_WHITEPIXEL = aMgr.GetImageThrow("IMAGE_WHITEPIXEL");
		IMAGE_WHITEWATER = aMgr.GetImageThrow("IMAGE_WHITEWATER");
		IMAGE_WHITEWATER_SHADOW = aMgr.GetImageThrow("IMAGE_WHITEWATER_SHADOW");
		IMAGE_WINTERMELON_PARTICLES = aMgr.GetImageThrow("IMAGE_WINTERMELON_PARTICLES");
		IMAGE_ZAMBONISMOKE = aMgr.GetImageThrow("IMAGE_ZAMBONISMOKE");
		IMAGE_ZENSHOPBUTTON = aMgr.GetImageThrow("IMAGE_ZENSHOPBUTTON");
		IMAGE_ZENSHOPBUTTON_HIGHLIGHT = aMgr.GetImageThrow("IMAGE_ZENSHOPBUTTON_HIGHLIGHT");
		IMAGE_ZEN_GARDENGLOVE = aMgr.GetImageThrow("IMAGE_ZEN_GARDENGLOVE");
		IMAGE_ZEN_GOLDTOOLRETICLE = aMgr.GetImageThrow("IMAGE_ZEN_GOLDTOOLRETICLE");
		IMAGE_ZEN_MONEYSIGN = aMgr.GetImageThrow("IMAGE_ZEN_MONEYSIGN");
		IMAGE_ZEN_NEED_ICONS = aMgr.GetImageThrow("IMAGE_ZEN_NEED_ICONS");
		IMAGE_ZEN_NEXTGARDEN = aMgr.GetImageThrow("IMAGE_ZEN_NEXTGARDEN");
		IMAGE_ZEN_WHEELBARROW = aMgr.GetImageThrow("IMAGE_ZEN_WHEELBARROW");
		IMAGE_ZOMBIEBACKUPDANCERHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEBACKUPDANCERHEAD");
		IMAGE_ZOMBIEBALLOONHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEBALLOONHEAD");
		IMAGE_ZOMBIEBOBSLEDHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEBOBSLEDHEAD");
		IMAGE_ZOMBIEDANCERHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEDANCERHEAD");
		IMAGE_ZOMBIEDIGGERARM = aMgr.GetImageThrow("IMAGE_ZOMBIEDIGGERARM");
		IMAGE_ZOMBIEDIGGERHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEDIGGERHEAD");
		IMAGE_ZOMBIEDOLPHINRIDERHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEDOLPHINRIDERHEAD");
		IMAGE_ZOMBIEFOOTBALLHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEFOOTBALLHEAD");
		IMAGE_ZOMBIEFUTUREGLASSES = aMgr.GetImageThrow("IMAGE_ZOMBIEFUTUREGLASSES");
		IMAGE_ZOMBIEIMPHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEIMPHEAD");
		IMAGE_ZOMBIEJACKBOXARM = aMgr.GetImageThrow("IMAGE_ZOMBIEJACKBOXARM");
		IMAGE_ZOMBIELADDERHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIELADDERHEAD");
		IMAGE_ZOMBIEPOGO = aMgr.GetImageThrow("IMAGE_ZOMBIEPOGO");
		IMAGE_ZOMBIEPOLEVAULTERHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEPOLEVAULTERHEAD");
		IMAGE_ZOMBIEYETIHEAD = aMgr.GetImageThrow("IMAGE_ZOMBIEYETIHEAD");
		IMAGE_ZOMBIE_BOBSLED1 = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOBSLED1");
		IMAGE_ZOMBIE_BOBSLED2 = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOBSLED2");
		IMAGE_ZOMBIE_BOBSLED3 = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOBSLED3");
		IMAGE_ZOMBIE_BOBSLED4 = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOBSLED4");
		IMAGE_ZOMBIE_BOBSLED_INSIDE = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOBSLED_INSIDE");
		IMAGE_ZOMBIE_BOSS_FIREBALL_GROUNDPARTICLES = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOSS_FIREBALL_GROUNDPARTICLES");
		IMAGE_ZOMBIE_BOSS_FIREBALL_PARTICLES = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOSS_FIREBALL_PARTICLES");
		IMAGE_ZOMBIE_BOSS_ICEBALL_GROUNDPARTICLES = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOSS_ICEBALL_GROUNDPARTICLES");
		IMAGE_ZOMBIE_BOSS_ICEBALL_PARTICLES = aMgr.GetImageThrow("IMAGE_ZOMBIE_BOSS_ICEBALL_PARTICLES");
		IMAGE_ZOMBIE_NOTE_SMALL = aMgr.GetImageThrow("IMAGE_ZOMBIE_NOTE_SMALL");
		IMAGE_ZOMBIE_SEAWEED = aMgr.GetImageThrow("IMAGE_ZOMBIE_SEAWEED");
		IMAGE_ZOMBOSS_PARTICLES = aMgr.GetImageThrow("IMAGE_ZOMBOSS_PARTICLES");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// LoadingSounds Resources
int Sexy::SOUND_AWOOGA;
int Sexy::SOUND_BALLOONINFLATE;
int Sexy::SOUND_BALLOON_POP;
int Sexy::SOUND_BASKETBALL;
int Sexy::SOUND_BIGCHOMP;
int Sexy::SOUND_BLEEP;
int Sexy::SOUND_BLOVER;
int Sexy::SOUND_BOING;
int Sexy::SOUND_BONK;
int Sexy::SOUND_BOSSBOULDERATTACK;
int Sexy::SOUND_BOSSEXPLOSION;
int Sexy::SOUND_BOWLING;
int Sexy::SOUND_BOWLINGIMPACT;
int Sexy::SOUND_BOWLINGIMPACT2;
int Sexy::SOUND_BUGSPRAY;
int Sexy::SOUND_BUNGEE_SCREAM;
int Sexy::SOUND_BUNGEE_SCREAM2;
int Sexy::SOUND_BUNGEE_SCREAM3;
int Sexy::SOUND_BUTTER;
int Sexy::SOUND_BUZZER;
int Sexy::SOUND_CERAMIC;
int Sexy::SOUND_CHERRYBOMB;
int Sexy::SOUND_CHIME;
int Sexy::SOUND_CHOMP;
int Sexy::SOUND_CHOMP2;
int Sexy::SOUND_CHOMPSOFT;
int Sexy::SOUND_COBLAUNCH;
int Sexy::SOUND_COFFEE;
int Sexy::SOUND_COIN;
int Sexy::SOUND_CRAZYDAVECRAZY;
int Sexy::SOUND_CRAZYDAVEEXTRALONG1;
int Sexy::SOUND_CRAZYDAVEEXTRALONG2;
int Sexy::SOUND_CRAZYDAVEEXTRALONG3;
int Sexy::SOUND_CRAZYDAVELONG1;
int Sexy::SOUND_CRAZYDAVELONG2;
int Sexy::SOUND_CRAZYDAVELONG3;
int Sexy::SOUND_CRAZYDAVESCREAM;
int Sexy::SOUND_CRAZYDAVESCREAM2;
int Sexy::SOUND_CRAZYDAVESHORT1;
int Sexy::SOUND_CRAZYDAVESHORT2;
int Sexy::SOUND_CRAZYDAVESHORT3;
int Sexy::SOUND_DANCER;
int Sexy::SOUND_DIAMOND;
int Sexy::SOUND_DIGGER_ZOMBIE;
int Sexy::SOUND_DIRT_RISE;
int Sexy::SOUND_DOLPHIN_APPEARS;
int Sexy::SOUND_DOLPHIN_BEFORE_JUMPING;
int Sexy::SOUND_DOOMSHROOM;
int Sexy::SOUND_EVILLAUGH;
int Sexy::SOUND_EXPLOSION;
int Sexy::SOUND_FERTILIZER;
int Sexy::SOUND_FINALFANFARE;
int Sexy::SOUND_FINALWAVE;
int Sexy::SOUND_FIREPEA;
int Sexy::SOUND_FLOOP;
int Sexy::SOUND_FROZEN;
int Sexy::SOUND_FUME;
int Sexy::SOUND_GARGANTUAR_THUMP;
int Sexy::SOUND_GARGANTUDEATH;
int Sexy::SOUND_GRASSSTEP;
int Sexy::SOUND_GRAVEBUSTERCHOMP;
int Sexy::SOUND_GRAVEBUTTON;
int Sexy::SOUND_GRAVESTONE_RUMBLE;
int Sexy::SOUND_GROAN;
int Sexy::SOUND_GROAN2;
int Sexy::SOUND_GROAN3;
int Sexy::SOUND_GROAN4;
int Sexy::SOUND_GROAN5;
int Sexy::SOUND_GROAN6;
int Sexy::SOUND_GULP;
int Sexy::SOUND_HATCHBACK_CLOSE;
int Sexy::SOUND_HATCHBACK_OPEN;
int Sexy::SOUND_HUGE_WAVE;
int Sexy::SOUND_HYDRAULIC;
int Sexy::SOUND_HYDRAULIC_SHORT;
int Sexy::SOUND_IGNITE;
int Sexy::SOUND_IGNITE2;
int Sexy::SOUND_IMP;
int Sexy::SOUND_IMP2;
int Sexy::SOUND_JACKINTHEBOX;
int Sexy::SOUND_JACK_SURPRISE;
int Sexy::SOUND_JACK_SURPRISE2;
int Sexy::SOUND_JALAPENO;
int Sexy::SOUND_JUICY;
int Sexy::SOUND_KERNELPULT;
int Sexy::SOUND_KERNELPULT2;
int Sexy::SOUND_LADDER_ZOMBIE;
int Sexy::SOUND_LAWNMOWER;
int Sexy::SOUND_LIGHTFILL;
int Sexy::SOUND_LIMBS_POP;
int Sexy::SOUND_LOSEMUSIC;
int Sexy::SOUND_LOWGROAN;
int Sexy::SOUND_LOWGROAN2;
int Sexy::SOUND_MAGNETSHROOM;
int Sexy::SOUND_MELONIMPACT;
int Sexy::SOUND_MELONIMPACT2;
int Sexy::SOUND_MINDCONTROLLED;
int Sexy::SOUND_MONEYFALLS;
int Sexy::SOUND_NEWSPAPER_RARRGH;
int Sexy::SOUND_NEWSPAPER_RARRGH2;
int Sexy::SOUND_NEWSPAPER_RIP;
int Sexy::SOUND_PAPER;
int Sexy::SOUND_PAUSE;
int Sexy::SOUND_PHONOGRAPH;
int Sexy::SOUND_PLANT;
int Sexy::SOUND_PLANT2;
int Sexy::SOUND_PLANTERN;
int Sexy::SOUND_PLANTGROW;
int Sexy::SOUND_PLANT_WATER;
int Sexy::SOUND_PLASTICHIT;
int Sexy::SOUND_PLASTICHIT2;
int Sexy::SOUND_POGO_ZOMBIE;
int Sexy::SOUND_POINTS;
int Sexy::SOUND_POLEVAULT;
int Sexy::SOUND_POOL_CLEANER;
int Sexy::SOUND_PORTAL;
int Sexy::SOUND_POTATO_MINE;
int Sexy::SOUND_PRIZE;
int Sexy::SOUND_PUFF;
int Sexy::SOUND_RAIN;
int Sexy::SOUND_READYSETPLANT;
int Sexy::SOUND_REVERSE_EXPLOSION;
int Sexy::SOUND_ROLL_IN;
int Sexy::SOUND_RVTHROW;
int Sexy::SOUND_SCREAM;
int Sexy::SOUND_SEEDLIFT;
int Sexy::SOUND_SHIELDHIT;
int Sexy::SOUND_SHIELDHIT2;
int Sexy::SOUND_SHOOP;
int Sexy::SOUND_SHOVEL;
int Sexy::SOUND_SIREN;
int Sexy::SOUND_SLOT_MACHINE;
int Sexy::SOUND_SLURP;
int Sexy::SOUND_SNOW_PEA_SPARKLES;
int Sexy::SOUND_SPLAT;
int Sexy::SOUND_SPLAT2;
int Sexy::SOUND_SPLAT3;
int Sexy::SOUND_SQUASH_HMM;
int Sexy::SOUND_SQUASH_HMM2;
int Sexy::SOUND_SUKHBIR;
int Sexy::SOUND_SUKHBIR2;
int Sexy::SOUND_SUKHBIR3;
int Sexy::SOUND_SUKHBIR4;
int Sexy::SOUND_SUKHBIR5;
int Sexy::SOUND_SUKHBIR6;
int Sexy::SOUND_SWING;
int Sexy::SOUND_TAP;
int Sexy::SOUND_TAP2;
int Sexy::SOUND_TAPGLASS;
int Sexy::SOUND_THROW;
int Sexy::SOUND_THROW2;
int Sexy::SOUND_THUNDER;
int Sexy::SOUND_VASE_BREAKING;
int Sexy::SOUND_WAKEUP;
int Sexy::SOUND_WATERING;
int Sexy::SOUND_WINMUSIC;
int Sexy::SOUND_YUCK;
int Sexy::SOUND_YUCK2;
int Sexy::SOUND_ZAMBONI;
int Sexy::SOUND_ZOMBAQUARIUM_DIE;
int Sexy::SOUND_ZOMBIESPLASH;
int Sexy::SOUND_ZOMBIE_ENTERING_WATER;
int Sexy::SOUND_ZOMBIE_FALLING_1;
int Sexy::SOUND_ZOMBIE_FALLING_2;

bool Sexy::ExtractLoadingSoundsResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager& aMgr = *theManager;
	try
	{
		SOUND_AWOOGA = aMgr.GetSoundThrow("SOUND_AWOOGA");
		SOUND_BALLOONINFLATE = aMgr.GetSoundThrow("SOUND_BALLOONINFLATE");
		SOUND_BALLOON_POP = aMgr.GetSoundThrow("SOUND_BALLOON_POP");
		SOUND_BASKETBALL = aMgr.GetSoundThrow("SOUND_BASKETBALL");
		SOUND_BIGCHOMP = aMgr.GetSoundThrow("SOUND_BIGCHOMP");
		SOUND_BLEEP = aMgr.GetSoundThrow("SOUND_BLEEP");
		SOUND_BLOVER = aMgr.GetSoundThrow("SOUND_BLOVER");
		SOUND_BOING = aMgr.GetSoundThrow("SOUND_BOING");
		SOUND_BONK = aMgr.GetSoundThrow("SOUND_BONK");
		SOUND_BOSSBOULDERATTACK = aMgr.GetSoundThrow("SOUND_BOSSBOULDERATTACK");
		SOUND_BOSSEXPLOSION = aMgr.GetSoundThrow("SOUND_BOSSEXPLOSION");
		SOUND_BOWLING = aMgr.GetSoundThrow("SOUND_BOWLING");
		SOUND_BOWLINGIMPACT = aMgr.GetSoundThrow("SOUND_BOWLINGIMPACT");
		SOUND_BOWLINGIMPACT2 = aMgr.GetSoundThrow("SOUND_BOWLINGIMPACT2");
		SOUND_BUGSPRAY = aMgr.GetSoundThrow("SOUND_BUGSPRAY");
		SOUND_BUNGEE_SCREAM = aMgr.GetSoundThrow("SOUND_BUNGEE_SCREAM");
		SOUND_BUNGEE_SCREAM2 = aMgr.GetSoundThrow("SOUND_BUNGEE_SCREAM2");
		SOUND_BUNGEE_SCREAM3 = aMgr.GetSoundThrow("SOUND_BUNGEE_SCREAM3");
		SOUND_BUTTER = aMgr.GetSoundThrow("SOUND_BUTTER");
		SOUND_BUZZER = aMgr.GetSoundThrow("SOUND_BUZZER");
		SOUND_CERAMIC = aMgr.GetSoundThrow("SOUND_CERAMIC");
		SOUND_CHERRYBOMB = aMgr.GetSoundThrow("SOUND_CHERRYBOMB");
		SOUND_CHIME = aMgr.GetSoundThrow("SOUND_CHIME");
		SOUND_CHOMP = aMgr.GetSoundThrow("SOUND_CHOMP");
		SOUND_CHOMP2 = aMgr.GetSoundThrow("SOUND_CHOMP2");
		SOUND_CHOMPSOFT = aMgr.GetSoundThrow("SOUND_CHOMPSOFT");
		SOUND_COBLAUNCH = aMgr.GetSoundThrow("SOUND_COBLAUNCH");
		SOUND_COFFEE = aMgr.GetSoundThrow("SOUND_COFFEE");
		SOUND_COIN = aMgr.GetSoundThrow("SOUND_COIN");
		SOUND_CRAZYDAVECRAZY = aMgr.GetSoundThrow("SOUND_CRAZYDAVECRAZY");
		SOUND_CRAZYDAVEEXTRALONG1 = aMgr.GetSoundThrow("SOUND_CRAZYDAVEEXTRALONG1");
		SOUND_CRAZYDAVEEXTRALONG2 = aMgr.GetSoundThrow("SOUND_CRAZYDAVEEXTRALONG2");
		SOUND_CRAZYDAVEEXTRALONG3 = aMgr.GetSoundThrow("SOUND_CRAZYDAVEEXTRALONG3");
		SOUND_CRAZYDAVELONG1 = aMgr.GetSoundThrow("SOUND_CRAZYDAVELONG1");
		SOUND_CRAZYDAVELONG2 = aMgr.GetSoundThrow("SOUND_CRAZYDAVELONG2");
		SOUND_CRAZYDAVELONG3 = aMgr.GetSoundThrow("SOUND_CRAZYDAVELONG3");
		SOUND_CRAZYDAVESCREAM = aMgr.GetSoundThrow("SOUND_CRAZYDAVESCREAM");
		SOUND_CRAZYDAVESCREAM2 = aMgr.GetSoundThrow("SOUND_CRAZYDAVESCREAM2");
		SOUND_CRAZYDAVESHORT1 = aMgr.GetSoundThrow("SOUND_CRAZYDAVESHORT1");
		SOUND_CRAZYDAVESHORT2 = aMgr.GetSoundThrow("SOUND_CRAZYDAVESHORT2");
		SOUND_CRAZYDAVESHORT3 = aMgr.GetSoundThrow("SOUND_CRAZYDAVESHORT3");
		SOUND_DANCER = aMgr.GetSoundThrow("SOUND_DANCER");
		SOUND_DIAMOND = aMgr.GetSoundThrow("SOUND_DIAMOND");
		SOUND_DIGGER_ZOMBIE = aMgr.GetSoundThrow("SOUND_DIGGER_ZOMBIE");
		SOUND_DIRT_RISE = aMgr.GetSoundThrow("SOUND_DIRT_RISE");
		SOUND_DOLPHIN_APPEARS = aMgr.GetSoundThrow("SOUND_DOLPHIN_APPEARS");
		SOUND_DOLPHIN_BEFORE_JUMPING = aMgr.GetSoundThrow("SOUND_DOLPHIN_BEFORE_JUMPING");
		SOUND_DOOMSHROOM = aMgr.GetSoundThrow("SOUND_DOOMSHROOM");
		SOUND_EVILLAUGH = aMgr.GetSoundThrow("SOUND_EVILLAUGH");
		SOUND_EXPLOSION = aMgr.GetSoundThrow("SOUND_EXPLOSION");
		SOUND_FERTILIZER = aMgr.GetSoundThrow("SOUND_FERTILIZER");
		SOUND_FINALFANFARE = aMgr.GetSoundThrow("SOUND_FINALFANFARE");
		SOUND_FINALWAVE = aMgr.GetSoundThrow("SOUND_FINALWAVE");
		SOUND_FIREPEA = aMgr.GetSoundThrow("SOUND_FIREPEA");
		SOUND_FLOOP = aMgr.GetSoundThrow("SOUND_FLOOP");
		SOUND_FROZEN = aMgr.GetSoundThrow("SOUND_FROZEN");
		SOUND_FUME = aMgr.GetSoundThrow("SOUND_FUME");
		SOUND_GARGANTUAR_THUMP = aMgr.GetSoundThrow("SOUND_GARGANTUAR_THUMP");
		SOUND_GARGANTUDEATH = aMgr.GetSoundThrow("SOUND_GARGANTUDEATH");
		SOUND_GRASSSTEP = aMgr.GetSoundThrow("SOUND_GRASSSTEP");
		SOUND_GRAVEBUSTERCHOMP = aMgr.GetSoundThrow("SOUND_GRAVEBUSTERCHOMP");
		SOUND_GRAVEBUTTON = aMgr.GetSoundThrow("SOUND_GRAVEBUTTON");
		SOUND_GRAVESTONE_RUMBLE = aMgr.GetSoundThrow("SOUND_GRAVESTONE_RUMBLE");
		SOUND_GROAN = aMgr.GetSoundThrow("SOUND_GROAN");
		SOUND_GROAN2 = aMgr.GetSoundThrow("SOUND_GROAN2");
		SOUND_GROAN3 = aMgr.GetSoundThrow("SOUND_GROAN3");
		SOUND_GROAN4 = aMgr.GetSoundThrow("SOUND_GROAN4");
		SOUND_GROAN5 = aMgr.GetSoundThrow("SOUND_GROAN5");
		SOUND_GROAN6 = aMgr.GetSoundThrow("SOUND_GROAN6");
		SOUND_GULP = aMgr.GetSoundThrow("SOUND_GULP");
		SOUND_HATCHBACK_CLOSE = aMgr.GetSoundThrow("SOUND_HATCHBACK_CLOSE");
		SOUND_HATCHBACK_OPEN = aMgr.GetSoundThrow("SOUND_HATCHBACK_OPEN");
		SOUND_HUGE_WAVE = aMgr.GetSoundThrow("SOUND_HUGE_WAVE");
		SOUND_HYDRAULIC = aMgr.GetSoundThrow("SOUND_HYDRAULIC");
		SOUND_HYDRAULIC_SHORT = aMgr.GetSoundThrow("SOUND_HYDRAULIC_SHORT");
		SOUND_IGNITE = aMgr.GetSoundThrow("SOUND_IGNITE");
		SOUND_IGNITE2 = aMgr.GetSoundThrow("SOUND_IGNITE2");
		SOUND_IMP = aMgr.GetSoundThrow("SOUND_IMP");
		SOUND_IMP2 = aMgr.GetSoundThrow("SOUND_IMP2");
		SOUND_JACKINTHEBOX = aMgr.GetSoundThrow("SOUND_JACKINTHEBOX");
		SOUND_JACK_SURPRISE = aMgr.GetSoundThrow("SOUND_JACK_SURPRISE");
		SOUND_JACK_SURPRISE2 = aMgr.GetSoundThrow("SOUND_JACK_SURPRISE2");
		SOUND_JALAPENO = aMgr.GetSoundThrow("SOUND_JALAPENO");
		SOUND_JUICY = aMgr.GetSoundThrow("SOUND_JUICY");
		SOUND_KERNELPULT = aMgr.GetSoundThrow("SOUND_KERNELPULT");
		SOUND_KERNELPULT2 = aMgr.GetSoundThrow("SOUND_KERNELPULT2");
		SOUND_LADDER_ZOMBIE = aMgr.GetSoundThrow("SOUND_LADDER_ZOMBIE");
		SOUND_LAWNMOWER = aMgr.GetSoundThrow("SOUND_LAWNMOWER");
		SOUND_LIGHTFILL = aMgr.GetSoundThrow("SOUND_LIGHTFILL");
		SOUND_LIMBS_POP = aMgr.GetSoundThrow("SOUND_LIMBS_POP");
		SOUND_LOSEMUSIC = aMgr.GetSoundThrow("SOUND_LOSEMUSIC");
		SOUND_LOWGROAN = aMgr.GetSoundThrow("SOUND_LOWGROAN");
		SOUND_LOWGROAN2 = aMgr.GetSoundThrow("SOUND_LOWGROAN2");
		SOUND_MAGNETSHROOM = aMgr.GetSoundThrow("SOUND_MAGNETSHROOM");
		SOUND_MELONIMPACT = aMgr.GetSoundThrow("SOUND_MELONIMPACT");
		SOUND_MELONIMPACT2 = aMgr.GetSoundThrow("SOUND_MELONIMPACT2");
		SOUND_MINDCONTROLLED = aMgr.GetSoundThrow("SOUND_MINDCONTROLLED");
		SOUND_MONEYFALLS = aMgr.GetSoundThrow("SOUND_MONEYFALLS");
		SOUND_NEWSPAPER_RARRGH = aMgr.GetSoundThrow("SOUND_NEWSPAPER_RARRGH");
		SOUND_NEWSPAPER_RARRGH2 = aMgr.GetSoundThrow("SOUND_NEWSPAPER_RARRGH2");
		SOUND_NEWSPAPER_RIP = aMgr.GetSoundThrow("SOUND_NEWSPAPER_RIP");
		SOUND_PAPER = aMgr.GetSoundThrow("SOUND_PAPER");
		SOUND_PAUSE = aMgr.GetSoundThrow("SOUND_PAUSE");
		SOUND_PHONOGRAPH = aMgr.GetSoundThrow("SOUND_PHONOGRAPH");
		SOUND_PLANT = aMgr.GetSoundThrow("SOUND_PLANT");
		SOUND_PLANT2 = aMgr.GetSoundThrow("SOUND_PLANT2");
		SOUND_PLANTERN = aMgr.GetSoundThrow("SOUND_PLANTERN");
		SOUND_PLANTGROW = aMgr.GetSoundThrow("SOUND_PLANTGROW");
		SOUND_PLANT_WATER = aMgr.GetSoundThrow("SOUND_PLANT_WATER");
		SOUND_PLASTICHIT = aMgr.GetSoundThrow("SOUND_PLASTICHIT");
		SOUND_PLASTICHIT2 = aMgr.GetSoundThrow("SOUND_PLASTICHIT2");
		SOUND_POGO_ZOMBIE = aMgr.GetSoundThrow("SOUND_POGO_ZOMBIE");
		SOUND_POINTS = aMgr.GetSoundThrow("SOUND_POINTS");
		SOUND_POLEVAULT = aMgr.GetSoundThrow("SOUND_POLEVAULT");
		SOUND_POOL_CLEANER = aMgr.GetSoundThrow("SOUND_POOL_CLEANER");
		SOUND_PORTAL = aMgr.GetSoundThrow("SOUND_PORTAL");
		SOUND_POTATO_MINE = aMgr.GetSoundThrow("SOUND_POTATO_MINE");
		SOUND_PRIZE = aMgr.GetSoundThrow("SOUND_PRIZE");
		SOUND_PUFF = aMgr.GetSoundThrow("SOUND_PUFF");
		SOUND_RAIN = aMgr.GetSoundThrow("SOUND_RAIN");
		SOUND_READYSETPLANT = aMgr.GetSoundThrow("SOUND_READYSETPLANT");
		SOUND_REVERSE_EXPLOSION = aMgr.GetSoundThrow("SOUND_REVERSE_EXPLOSION");
		SOUND_ROLL_IN = aMgr.GetSoundThrow("SOUND_ROLL_IN");
		SOUND_RVTHROW = aMgr.GetSoundThrow("SOUND_RVTHROW");
		SOUND_SCREAM = aMgr.GetSoundThrow("SOUND_SCREAM");
		SOUND_SEEDLIFT = aMgr.GetSoundThrow("SOUND_SEEDLIFT");
		SOUND_SHIELDHIT = aMgr.GetSoundThrow("SOUND_SHIELDHIT");
		SOUND_SHIELDHIT2 = aMgr.GetSoundThrow("SOUND_SHIELDHIT2");
		SOUND_SHOOP = aMgr.GetSoundThrow("SOUND_SHOOP");
		SOUND_SHOVEL = aMgr.GetSoundThrow("SOUND_SHOVEL");
		SOUND_SIREN = aMgr.GetSoundThrow("SOUND_SIREN");
		SOUND_SLOT_MACHINE = aMgr.GetSoundThrow("SOUND_SLOT_MACHINE");
		SOUND_SLURP = aMgr.GetSoundThrow("SOUND_SLURP");
		SOUND_SNOW_PEA_SPARKLES = aMgr.GetSoundThrow("SOUND_SNOW_PEA_SPARKLES");
		SOUND_SPLAT = aMgr.GetSoundThrow("SOUND_SPLAT");
		SOUND_SPLAT2 = aMgr.GetSoundThrow("SOUND_SPLAT2");
		SOUND_SPLAT3 = aMgr.GetSoundThrow("SOUND_SPLAT3");
		SOUND_SQUASH_HMM = aMgr.GetSoundThrow("SOUND_SQUASH_HMM");
		SOUND_SQUASH_HMM2 = aMgr.GetSoundThrow("SOUND_SQUASH_HMM2");
		SOUND_SUKHBIR = aMgr.GetSoundThrow("SOUND_SUKHBIR");
		SOUND_SUKHBIR2 = aMgr.GetSoundThrow("SOUND_SUKHBIR2");
		SOUND_SUKHBIR3 = aMgr.GetSoundThrow("SOUND_SUKHBIR3");
		SOUND_SUKHBIR4 = aMgr.GetSoundThrow("SOUND_SUKHBIR4");
		SOUND_SUKHBIR5 = aMgr.GetSoundThrow("SOUND_SUKHBIR5");
		SOUND_SUKHBIR6 = aMgr.GetSoundThrow("SOUND_SUKHBIR6");
		SOUND_SWING = aMgr.GetSoundThrow("SOUND_SWING");
		SOUND_TAP = aMgr.GetSoundThrow("SOUND_TAP");
		SOUND_TAP2 = aMgr.GetSoundThrow("SOUND_TAP2");
		SOUND_TAPGLASS = aMgr.GetSoundThrow("SOUND_TAPGLASS");
		SOUND_THROW = aMgr.GetSoundThrow("SOUND_THROW");
		SOUND_THROW2 = aMgr.GetSoundThrow("SOUND_THROW2");
		SOUND_THUNDER = aMgr.GetSoundThrow("SOUND_THUNDER");
		SOUND_VASE_BREAKING = aMgr.GetSoundThrow("SOUND_VASE_BREAKING");
		SOUND_WAKEUP = aMgr.GetSoundThrow("SOUND_WAKEUP");
		SOUND_WATERING = aMgr.GetSoundThrow("SOUND_WATERING");
		SOUND_WINMUSIC = aMgr.GetSoundThrow("SOUND_WINMUSIC");
		SOUND_YUCK = aMgr.GetSoundThrow("SOUND_YUCK");
		SOUND_YUCK2 = aMgr.GetSoundThrow("SOUND_YUCK2");
		SOUND_ZAMBONI = aMgr.GetSoundThrow("SOUND_ZAMBONI");
		SOUND_ZOMBAQUARIUM_DIE = aMgr.GetSoundThrow("SOUND_ZOMBAQUARIUM_DIE");
		SOUND_ZOMBIESPLASH = aMgr.GetSoundThrow("SOUND_ZOMBIESPLASH");
		SOUND_ZOMBIE_ENTERING_WATER = aMgr.GetSoundThrow("SOUND_ZOMBIE_ENTERING_WATER");
		SOUND_ZOMBIE_FALLING_1 = aMgr.GetSoundThrow("SOUND_ZOMBIE_FALLING_1");
		SOUND_ZOMBIE_FALLING_2 = aMgr.GetSoundThrow("SOUND_ZOMBIE_FALLING_2");
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// [Beta v0.1.1 Only]
Image* Sexy::IMAGE_GOOGLYEYE;
Image* Sexy::IMAGE_SQUIRREL;

bool (*gExtractResourcesByName)(Sexy::ResourceManager* theResourceManager, const char* theName);

void* gResources[(int)Sexy::ResourceId::RESOURCE_ID_MAX] =
{
	/* Init��0x69E6B0 */
	&Sexy::IMAGE_BLANK,
	&Sexy::IMAGE_POPCAP_LOGO,
	&Sexy::IMAGE_PARTNER_LOGO,
	/* LoaderBar��0x69E6BC */
	&Sexy::IMAGE_TITLESCREEN,
	&Sexy::IMAGE_LOADBAR_DIRT,
	&Sexy::IMAGE_LOADBAR_GRASS,
	&Sexy::IMAGE_PVZ_LOGO,
	&Sexy::IMAGE_REANIM_SODROLLCAP,
	&Sexy::FONT_BRIANNETOD16,
	&Sexy::SOUND_BUTTONCLICK,
	&Sexy::SOUND_LOADINGBAR_FLOWER,
	&Sexy::SOUND_LOADINGBAR_ZOMBIE,
	/* LoadingFonts��0x69E6E0 */
	&Sexy::FONT_IMAGE_HOUSEOFTERROR28,
	&Sexy::FONT_HOUSEOFTERROR28,
	&Sexy::FONT_HOUSEOFTERROR20,
	&Sexy::FONT_HOUSEOFTERROR16,
	&Sexy::FONT_TINYBOLD,
	&Sexy::FONT_CONTINUUMBOLD14,
	&Sexy::FONT_CONTINUUMBOLD14OUTLINE,
	&Sexy::FONT_DWARVENTODCRAFT12,
	&Sexy::FONT_DWARVENTODCRAFT15,
	&Sexy::FONT_DWARVENTODCRAFT18,
	&Sexy::FONT_DWARVENTODCRAFT18BRIGHTGREENINSET,
	&Sexy::FONT_DWARVENTODCRAFT18GREENINSET,
	&Sexy::FONT_DWARVENTODCRAFT18YELLOW,
	&Sexy::FONT_DWARVENTODCRAFT24,
	&Sexy::FONT_DWARVENTODCRAFT36BRIGHTGREENINSET,
	&Sexy::FONT_DWARVENTODCRAFT36GREENINSET,
	&Sexy::FONT_PICO129,
	&Sexy::FONT_BRIANNETOD12,
	/* LoadingImages��0x69E728 */
	&Sexy::IMAGE_EDITBOX,
	&Sexy::IMAGE_DIALOG_TOPLEFT,
	&Sexy::IMAGE_DIALOG_TOPMIDDLE,
	&Sexy::IMAGE_DIALOG_TOPRIGHT,
	&Sexy::IMAGE_DIALOG_CENTERLEFT,
	&Sexy::IMAGE_DIALOG_CENTERMIDDLE,
	&Sexy::IMAGE_DIALOG_CENTERRIGHT,
	&Sexy::IMAGE_DIALOG_BOTTOMLEFT,
	&Sexy::IMAGE_DIALOG_BOTTOMMIDDLE,
	&Sexy::IMAGE_DIALOG_BOTTOMRIGHT,
	&Sexy::IMAGE_DIALOG_BIGBOTTOMLEFT,
	&Sexy::IMAGE_DIALOG_BIGBOTTOMMIDDLE,
	&Sexy::IMAGE_DIALOG_BIGBOTTOMRIGHT,
	&Sexy::IMAGE_DIALOG_HEADER,
	&Sexy::IMAGE_BUTTON_LEFT,
	&Sexy::IMAGE_BUTTON_MIDDLE,
	&Sexy::IMAGE_BUTTON_RIGHT,
	&Sexy::IMAGE_BUTTON_DOWN_LEFT,
	&Sexy::IMAGE_BUTTON_DOWN_MIDDLE,
	&Sexy::IMAGE_BUTTON_DOWN_RIGHT,
	/* <-- Screens --> : 0x69E778 */
	&Sexy::IMAGE_SEEDCHOOSER_BACKGROUND,
	&Sexy::IMAGE_SEEDCHOOSER_BUTTON,
	&Sexy::IMAGE_SEEDCHOOSER_BUTTON_DISABLED,
	&Sexy::IMAGE_SEEDCHOOSER_BUTTON_GLOW,
	&Sexy::IMAGE_SEEDCHOOSER_BUTTON2,
	&Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW,
	&Sexy::IMAGE_SEEDCHOOSER_IMITATERADDON,
	&Sexy::IMAGE_SEEDS,
	&Sexy::IMAGE_SEEDPACKET_LARGER,
	&Sexy::IMAGE_SEEDPACKETSILHOUETTE,
	&Sexy::IMAGE_FLAGMETER,
	&Sexy::IMAGE_FLAGMETERLEVELPROGRESS,
	&Sexy::IMAGE_FLAGMETERPARTS,
	&Sexy::IMAGE_TROPHY,
	&Sexy::IMAGE_TROPHY_HI_RES,
	&Sexy::IMAGE_MINIGAME_TROPHY,
	&Sexy::IMAGE_ZOMBIE_NOTE_SMALL,
	&Sexy::IMAGE_TACO,
	&Sexy::IMAGE_CARKEYS,
	&Sexy::IMAGE_ALMANAC,
	&Sexy::IMAGE_ICON_POOLCLEANER,
	&Sexy::IMAGE_ICON_ROOFCLEANER,
	&Sexy::IMAGE_ICON_RAKE,
	&Sexy::IMAGE_BRAIN,
	&Sexy::IMAGE_MONEYBAG,
	&Sexy::IMAGE_MONEYBAG_HI_RES,
	&Sexy::IMAGE_CHOCOLATE,
	/* <-- Buttons and other UI --> : 0x69E7E4 */
	&Sexy::IMAGE_OPTIONS_BACKTOGAMEBUTTON0,
	&Sexy::IMAGE_OPTIONS_BACKTOGAMEBUTTON2,
	&Sexy::IMAGE_OPTIONS_CHECKBOX0,
	&Sexy::IMAGE_OPTIONS_CHECKBOX1,
	&Sexy::IMAGE_OPTIONS_MENUBACK,
	&Sexy::IMAGE_OPTIONS_SLIDERKNOB2,
	&Sexy::IMAGE_OPTIONS_SLIDERSLOT,
	&Sexy::IMAGE_SELECTORSCREEN_HELP1,
	&Sexy::IMAGE_SELECTORSCREEN_HELP2,
	&Sexy::IMAGE_SELECTORSCREEN_OPTIONS1,
	&Sexy::IMAGE_SELECTORSCREEN_OPTIONS2,
	&Sexy::IMAGE_SELECTORSCREEN_QUIT1,
	&Sexy::IMAGE_SELECTORSCREEN_QUIT2,
	&Sexy::IMAGE_SELECTORSCREEN_ALMANAC,
	&Sexy::IMAGE_SELECTORSCREEN_ALMANACHIGHLIGHT,
	&Sexy::IMAGE_SELECTORSCREEN_STORE,
	&Sexy::IMAGE_SELECTORSCREEN_STOREHIGHLIGHT,
	&Sexy::IMAGE_SELECTORSCREEN_LEVELNUMBERS,
	&Sexy::IMAGE_SELECTORSCREEN_ZENGARDEN,
	&Sexy::IMAGE_SELECTORSCREEN_ZENGARDENHIGHLIGHT,
	&Sexy::IMAGE_STORE_SPEECHBUBBLE,
	&Sexy::IMAGE_STORE_SPEECHBUBBLE2,

	/* <-- Misc small items --> : 0x69E83C */
	&Sexy::IMAGE_SHOVEL,
	&Sexy::IMAGE_SHOVEL_HI_RES,
	&Sexy::IMAGE_TOMBSTONES,
	&Sexy::IMAGE_TOMBSTONE_MOUNDS,
	&Sexy::IMAGE_NIGHT_GRAVE_GRAPHIC,
	&Sexy::IMAGE_CRATER,
	&Sexy::IMAGE_CRATER_FADING,
	&Sexy::IMAGE_CRATER_ROOF_CENTER,
	&Sexy::IMAGE_CRATER_ROOF_LEFT,
	&Sexy::IMAGE_CRATER_WATER_DAY,
	&Sexy::IMAGE_CRATER_WATER_NIGHT,
	&Sexy::IMAGE_COBCANNON_TARGET,
	&Sexy::IMAGE_COBCANNON_POPCORN,
	&Sexy::IMAGE_FERTILIZER,
	&Sexy::IMAGE_WATERINGCAN,
	&Sexy::IMAGE_WATERINGCANGOLD,
	&Sexy::IMAGE_PHONOGRAPH,
	&Sexy::IMAGE_BEGHOULED_TWIST_OVERLAY,
	&Sexy::IMAGE_ZEN_GOLDTOOLRETICLE,
	&Sexy::IMAGE_ZEN_GARDENGLOVE,
	&Sexy::IMAGE_ZEN_MONEYSIGN,
	&Sexy::IMAGE_ZEN_NEXTGARDEN,
	&Sexy::IMAGE_ZEN_WHEELBARROW,
	&Sexy::IMAGE_ZENSHOPBUTTON,
	&Sexy::IMAGE_ZENSHOPBUTTON_HIGHLIGHT,
	&Sexy::IMAGE_PRESENT,
	&Sexy::IMAGE_PRESENTOPEN,
	&Sexy::IMAGE_SUNFLOWER_TROPHY,
	&Sexy::IMAGE_SLOTMACHINE_OVERLAY,
	&Sexy::IMAGE_SCARY_POT,
	&Sexy::IMAGE_BUG_SPRAY,
	&Sexy::IMAGE_PLANTSPEECHBUBBLE,
	&Sexy::IMAGE_WATERDROP,
	&Sexy::IMAGE_TREEFOOD,
	&Sexy::IMAGE_PACKET_PLANTS,
	&Sexy::IMAGE_ZEN_NEED_ICONS,
	/* <-- Various strips --> : 0x69E8CC */
	&Sexy::IMAGE_PROJECTILEPEA,
	&Sexy::IMAGE_PROJECTILESNOWPEA,
	&Sexy::IMAGE_PROJECTILECACTUS,
	&Sexy::IMAGE_DIRTSMALL,
	&Sexy::IMAGE_DIRTBIG,
	&Sexy::IMAGE_ROCKSMALL,
	&Sexy::IMAGE_WATERPARTICLE,
	&Sexy::IMAGE_WHITEWATER,
	&Sexy::IMAGE_WHITEWATER_SHADOW,
	&Sexy::IMAGE_MELONPULT_PARTICLES,
	&Sexy::IMAGE_WINTERMELON_PARTICLES,
	&Sexy::IMAGE_PROJECTILE_STAR,
	/* <-- Background --> : 0x69E8FC */
	&Sexy::IMAGE_SEEDBANK,
	&Sexy::IMAGE_CONVEYORBELT_BACKDROP,
	&Sexy::IMAGE_CONVEYORBELT,
	&Sexy::IMAGE_SHOVELBANK,
	&Sexy::IMAGE_SUNBANK,
	&Sexy::IMAGE_COINBANK,
	&Sexy::IMAGE_POOL,
	&Sexy::IMAGE_POOL_NIGHT,
	&Sexy::IMAGE_POOL_BASE,
	&Sexy::IMAGE_POOL_BASE_NIGHT,
	&Sexy::IMAGE_POOL_SHADING,
	&Sexy::IMAGE_POOL_SHADING_NIGHT,
	&Sexy::IMAGE_POOL_CAUSTIC_EFFECT,
	&Sexy::IMAGE_PLANTSHADOW,
	&Sexy::IMAGE_PLANTSHADOW2,
	&Sexy::IMAGE_PEA_SHADOWS,
	&Sexy::IMAGE_WALLNUT_BOWLINGSTRIPE,
	&Sexy::IMAGE_ICE,
	&Sexy::IMAGE_ICE_CAP,
	&Sexy::IMAGE_ICE_SPARKLES,
	/* 0x69E94C */
	&Sexy::IMAGE_IMITATERSEED,
	&Sexy::IMAGE_ALMANAC_IMITATER,
	&Sexy::IMAGE_IMITATERSEEDDISABLED,
	&Sexy::IMAGE_ICETRAP,
	&Sexy::IMAGE_ICETRAP2,
	&Sexy::IMAGE_ICETRAP_PARTICLES,
	/* <-- Zombies --> : 0x69E964 */
	&Sexy::IMAGE_ZOMBIE_BOBSLED1,
	&Sexy::IMAGE_ZOMBIE_BOBSLED2,
	&Sexy::IMAGE_ZOMBIE_BOBSLED3,
	&Sexy::IMAGE_ZOMBIE_BOBSLED4,
	&Sexy::IMAGE_ZOMBIE_BOBSLED_INSIDE,
	&Sexy::IMAGE_BUNGEECORD,
	&Sexy::IMAGE_BUNGEETARGET,
	&Sexy::IMAGE_SPOTLIGHT,
	&Sexy::IMAGE_SPOTLIGHT2,
	/* 0x69E988 */
	&Sexy::IMAGE_WHITEPIXEL,
	&Sexy::IMAGE_ZOMBIEPOLEVAULTERHEAD,
	&Sexy::IMAGE_ZOMBIEFOOTBALLHEAD,
	&Sexy::IMAGE_POOLSPARKLY,
	&Sexy::IMAGE_WALLNUTPARTICLESSMALL,
	&Sexy::IMAGE_WALLNUTPARTICLESLARGE,
	&Sexy::IMAGE_PEA_SPLATS,
	&Sexy::IMAGE_STAR_PARTICLES,
	&Sexy::IMAGE_STAR_SPLATS,
	&Sexy::IMAGE_PEA_PARTICLES,
	&Sexy::IMAGE_SNOWPEA_SPLATS,
	&Sexy::IMAGE_SNOWPEA_PARTICLES,
	&Sexy::IMAGE_SNOWPEA_PUFF,
	&Sexy::IMAGE_SNOWFLAKES,
	&Sexy::IMAGE_POTATOMINE_PARTICLES,
	&Sexy::IMAGE_PUFFSHROOM_PUFF1,
	&Sexy::IMAGE_ZAMBONISMOKE,
	&Sexy::IMAGE_ZOMBIEDANCERHEAD,
	&Sexy::IMAGE_ZOMBIEBALLOONHEAD,
	&Sexy::IMAGE_ZOMBIEIMPHEAD,
	&Sexy::IMAGE_ZOMBIEDIGGERHEAD,
	&Sexy::IMAGE_ZOMBIEDIGGERARM,
	&Sexy::IMAGE_ZOMBIEDOLPHINRIDERHEAD,
	&Sexy::IMAGE_ZOMBIEPOGO,
	&Sexy::IMAGE_ZOMBIEBACKUPDANCERHEAD,
	&Sexy::IMAGE_ZOMBIEBOBSLEDHEAD,
	&Sexy::IMAGE_ZOMBIELADDERHEAD,
	&Sexy::IMAGE_ZOMBIEYETIHEAD,
	&Sexy::IMAGE_SEEDPACKETFLASH,
	&Sexy::IMAGE_ZOMBIEJACKBOXARM,
	&Sexy::IMAGE_IMITATERCLOUDS,
	&Sexy::IMAGE_IMITATERPUFFS,
	&Sexy::IMAGE_ZOMBIE_BOSS_FIREBALL_PARTICLES,
	&Sexy::IMAGE_ZOMBIE_BOSS_ICEBALL_PARTICLES,
	&Sexy::IMAGE_ZOMBIE_BOSS_FIREBALL_GROUNDPARTICLES,
	&Sexy::IMAGE_ZOMBIE_BOSS_ICEBALL_GROUNDPARTICLES,
	&Sexy::IMAGE_DOOMSHROOM_EXPLOSION_BASE,
	&Sexy::IMAGE_RAIN,
	&Sexy::IMAGE_VASE_CHUNKS,
	&Sexy::IMAGE_ZOMBOSS_PARTICLES,
	&Sexy::IMAGE_AWARDPICKUPGLOW,
	&Sexy::IMAGE_ZOMBIE_SEAWEED,
	&Sexy::IMAGE_PINATA,
	&Sexy::IMAGE_ZOMBIEFUTUREGLASSES,
	&Sexy::IMAGE_DUST_PUFFS,
	/* 0x69EA3C */
	&Sexy::IMAGE_REANIM_WALLNUT_BODY,
	&Sexy::IMAGE_REANIM_WALLNUT_CRACKED1,
	&Sexy::IMAGE_REANIM_WALLNUT_CRACKED2,
	&Sexy::IMAGE_REANIM_TALLNUT_CRACKED1,
	&Sexy::IMAGE_REANIM_TALLNUT_CRACKED2,
	&Sexy::IMAGE_REANIM_PUMPKIN_DAMAGE1,
	&Sexy::IMAGE_REANIM_PUMPKIN_DAMAGE3,
	&Sexy::IMAGE_REANIM_ZOMBIE_CONE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_CONE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_CONE3,
	&Sexy::IMAGE_REANIM_ZOMBIE_BUCKET1,
	&Sexy::IMAGE_REANIM_ZOMBIE_BUCKET2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BUCKET3,
	&Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT,
	&Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT2,
	&Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT3,
	&Sexy::IMAGE_REANIM_ZOMBIE_SCREENDOOR1,
	&Sexy::IMAGE_REANIM_ZOMBIE_SCREENDOOR2,
	&Sexy::IMAGE_REANIM_ZOMBIE_SCREENDOOR3,
	&Sexy::IMAGE_REANIM_ZOMBIE_FLAG1,
	&Sexy::IMAGE_REANIM_ZOMBIE_FLAG3,
	&Sexy::IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET,
	&Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2,
	&Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3,
	&Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_HAND,
	&Sexy::IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_LOWER,
	&Sexy::IMAGE_REANIM_ZOMBIE_PAPER_PAPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_PAPER_PAPER3,
	&Sexy::IMAGE_REANIM_ZOMBIE_PAPER_MADHEAD,
	&Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_ZAMBONI_WHEEL_FLAT,
	&Sexy::IMAGE_REANIM_COIN_SILVER_DOLLAR,
	&Sexy::IMAGE_REANIM_COIN_GOLD_DOLLAR,
	&Sexy::IMAGE_REANIM_DIAMOND,
	&Sexy::IMAGE_REANIM_COINGLOW,
	&Sexy::IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_HAND,
	&Sexy::IMAGE_REANIM_ZOMBIE_JACKSON_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_DANCER_INNERARM_HAND,
	&Sexy::IMAGE_REANIM_ZOMBIE_POLEVAULTER_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_OUTERARM_HAND,
	&Sexy::IMAGE_REANIM_ZOMBIE_BALLOON_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_IMP_ARM1_BONE,
	&Sexy::IMAGE_REANIM_ZOMBIE_IMP_ARM2,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_2,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_3,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_DUCKXING,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_ZOMBIE,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_FOOT2,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2_REDEYE,
	&Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE,
	&Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_DOLPHINRIDER_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_POGO_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_POGO_STICKHANDS2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_HAND,
	&Sexy::IMAGE_REANIM_ZOMBIE_JACKBOX_BOX,
	&Sexy::IMAGE_REANIM_ZOMBIE_JACKBOX_OUTERARM_LOWER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_SNORKLE_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BUNGI_HEAD_SCARED,
	&Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL,
	&Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE,
	&Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE,
	&Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_WITHBALL,
	&Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_POLE_DAMAGE_WITHBALL,
	&Sexy::IMAGE_REANIM_ZOMBIE_CATAPULT_SIDING_DAMAGE,
	&Sexy::IMAGE_REANIM_ZOMBIE_LADDER_1,
	&Sexy::IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_LADDER_5,
	&Sexy::IMAGE_REANIM_ZOMBIE_LADDER_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_ICEBALL,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE1,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_EYEGLOW_BLUE,
	&Sexy::IMAGE_REANIM_ZOMBIE_BOSS_MOUTHGLOW_BLUE,
	&Sexy::IMAGE_REANIM_CABBAGEPULT_CABBAGE,
	&Sexy::IMAGE_REANIM_CORNPULT_KERNAL,
	&Sexy::IMAGE_REANIM_CORNPULT_BUTTER,
	&Sexy::IMAGE_REANIM_CORNPULT_BUTTER_SPLAT,
	&Sexy::IMAGE_REANIM_MELONPULT_MELON,
	&Sexy::IMAGE_REANIM_WINTERMELON_PROJECTILE,
	&Sexy::IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE,
	&Sexy::IMAGE_REANIM_ZOMBIE_HEAD_GROSSOUT,
	&Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES1,
	&Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES2,
	&Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES3,
	&Sexy::IMAGE_REANIM_ZOMBIE_HEAD_SUNGLASSES4,
	&Sexy::IMAGE_REANIM_GARLIC_BODY2,
	&Sexy::IMAGE_REANIM_GARLIC_BODY3,
	&Sexy::IMAGE_REANIM_COBCANNON_COB,
	&Sexy::IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_UPPER2,
	&Sexy::IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_HAND,
	&Sexy::IMAGE_REANIM_ZOMBIE_SNORKLE_HEAD,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_BUTTON,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_ADVENTURE_HIGHLIGHT,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_BUTTON,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_STARTADVENTURE_HIGHLIGHT,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_BUTTON,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_SURVIVAL_HIGHLIGHT,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_BUTTON,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_CHALLENGES_HIGHLIGHT,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_BUTTON,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_VASEBREAKER_HIGHLIGHT,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_WOODSIGN2_PRESS,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_BG,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_ALMANAC_SHADOW,
	&Sexy::IMAGE_REANIM_SELECTORSCREEN_KEY_SHADOW,
	&Sexy::IMAGE_REANIM_TREE_BG,
	&Sexy::IMAGE_REANIM_POT_TOP_DARK,
	&Sexy::IMAGE_REANIM_ZENGARDEN_BUGSPRAY_BOTTLE,
	&Sexy::IMAGE_REANIM_STINKY_TURN3,
	&Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH1,
	&Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH4,
	&Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH5,
	&Sexy::IMAGE_REANIM_CRAZYDAVE_MOUTH6,
	&Sexy::IMAGE_REANIM_ZOMBIE_MUSTACHE2,
	&Sexy::IMAGE_REANIM_ZOMBIE_MUSTACHE3,
	/* LoadingSounds */
	&Sexy::SOUND_AWOOGA,
	&Sexy::SOUND_BLEEP,
	&Sexy::SOUND_BUZZER,
	&Sexy::SOUND_CHOMP,
	&Sexy::SOUND_CHOMP2,
	&Sexy::SOUND_CHOMPSOFT,
	&Sexy::SOUND_EVILLAUGH,
	&Sexy::SOUND_FLOOP,
	&Sexy::SOUND_FROZEN,
	&Sexy::SOUND_GULP,
	&Sexy::SOUND_GROAN,
	&Sexy::SOUND_GROAN2,
	&Sexy::SOUND_GROAN3,
	&Sexy::SOUND_GROAN4,
	&Sexy::SOUND_GROAN5,
	&Sexy::SOUND_GROAN6,
	&Sexy::SOUND_LOSEMUSIC,
	&Sexy::SOUND_MINDCONTROLLED,
	&Sexy::SOUND_PAUSE,
	&Sexy::SOUND_PLANT,
	&Sexy::SOUND_PLANT2,
	&Sexy::SOUND_POINTS,
	&Sexy::SOUND_SEEDLIFT,
	&Sexy::SOUND_SIREN,
	&Sexy::SOUND_SLURP,
	&Sexy::SOUND_SPLAT,
	&Sexy::SOUND_SPLAT2,
	&Sexy::SOUND_SPLAT3,
	&Sexy::SOUND_SUKHBIR,
	&Sexy::SOUND_SUKHBIR2,
	&Sexy::SOUND_SUKHBIR3,
	&Sexy::SOUND_SUKHBIR4,
	&Sexy::SOUND_SUKHBIR5,
	&Sexy::SOUND_SUKHBIR6,
	&Sexy::SOUND_TAP,
	&Sexy::SOUND_TAP2,
	&Sexy::SOUND_THROW,
	&Sexy::SOUND_THROW2,
	&Sexy::SOUND_BLOVER,
	&Sexy::SOUND_WINMUSIC,
	&Sexy::SOUND_LAWNMOWER,
	&Sexy::SOUND_BOING,
	&Sexy::SOUND_JACKINTHEBOX,
	&Sexy::SOUND_DIAMOND,
	&Sexy::SOUND_DOLPHIN_APPEARS,
	&Sexy::SOUND_DOLPHIN_BEFORE_JUMPING,
	&Sexy::SOUND_POTATO_MINE,
	&Sexy::SOUND_ZAMBONI,
	&Sexy::SOUND_BALLOON_POP,
	&Sexy::SOUND_THUNDER,
	&Sexy::SOUND_ZOMBIESPLASH,
	&Sexy::SOUND_BOWLING,
	&Sexy::SOUND_BOWLINGIMPACT,
	&Sexy::SOUND_BOWLINGIMPACT2,
	&Sexy::SOUND_GRAVEBUSTERCHOMP,
	&Sexy::SOUND_GRAVEBUTTON,
	&Sexy::SOUND_LIMBS_POP,
	&Sexy::SOUND_PLANTERN,
	&Sexy::SOUND_POGO_ZOMBIE,
	&Sexy::SOUND_SNOW_PEA_SPARKLES,
	&Sexy::SOUND_PLANT_WATER,
	&Sexy::SOUND_ZOMBIE_ENTERING_WATER,
	&Sexy::SOUND_ZOMBIE_FALLING_1,
	&Sexy::SOUND_ZOMBIE_FALLING_2,
	&Sexy::SOUND_PUFF,
	&Sexy::SOUND_FUME,
	&Sexy::SOUND_HUGE_WAVE,
	&Sexy::SOUND_SLOT_MACHINE,
	&Sexy::SOUND_COIN,
	&Sexy::SOUND_ROLL_IN,
	&Sexy::SOUND_DIGGER_ZOMBIE,
	&Sexy::SOUND_HATCHBACK_CLOSE,
	&Sexy::SOUND_HATCHBACK_OPEN,
	&Sexy::SOUND_KERNELPULT,
	&Sexy::SOUND_KERNELPULT2,
	&Sexy::SOUND_ZOMBAQUARIUM_DIE,
	&Sexy::SOUND_BUNGEE_SCREAM,
	&Sexy::SOUND_BUNGEE_SCREAM2,
	&Sexy::SOUND_BUNGEE_SCREAM3,
	&Sexy::SOUND_BUTTER,
	&Sexy::SOUND_JACK_SURPRISE,
	&Sexy::SOUND_JACK_SURPRISE2,
	&Sexy::SOUND_NEWSPAPER_RARRGH,
	&Sexy::SOUND_NEWSPAPER_RARRGH2,
	&Sexy::SOUND_NEWSPAPER_RIP,
	&Sexy::SOUND_SQUASH_HMM,
	&Sexy::SOUND_SQUASH_HMM2,
	&Sexy::SOUND_VASE_BREAKING,
	&Sexy::SOUND_POOL_CLEANER,
	&Sexy::SOUND_MAGNETSHROOM,
	&Sexy::SOUND_LADDER_ZOMBIE,
	&Sexy::SOUND_GARGANTUAR_THUMP,
	&Sexy::SOUND_BASKETBALL,
	&Sexy::SOUND_FIREPEA,
	&Sexy::SOUND_IGNITE,
	&Sexy::SOUND_IGNITE2,
	&Sexy::SOUND_READYSETPLANT,
	&Sexy::SOUND_DOOMSHROOM,
	&Sexy::SOUND_EXPLOSION,
	&Sexy::SOUND_FINALWAVE,
	&Sexy::SOUND_REVERSE_EXPLOSION,
	&Sexy::SOUND_RVTHROW,
	&Sexy::SOUND_SHIELDHIT,
	&Sexy::SOUND_SHIELDHIT2,
	&Sexy::SOUND_BOSSEXPLOSION,
	&Sexy::SOUND_CHERRYBOMB,
	&Sexy::SOUND_BONK,
	&Sexy::SOUND_SWING,
	&Sexy::SOUND_RAIN,
	&Sexy::SOUND_LIGHTFILL,
	&Sexy::SOUND_PLASTICHIT,
	&Sexy::SOUND_PLASTICHIT2,
	&Sexy::SOUND_JALAPENO,
	&Sexy::SOUND_BALLOONINFLATE,
	&Sexy::SOUND_BIGCHOMP,
	&Sexy::SOUND_MELONIMPACT,
	&Sexy::SOUND_MELONIMPACT2,
	&Sexy::SOUND_PLANTGROW,
	&Sexy::SOUND_SHOOP,
	&Sexy::SOUND_TAPGLASS,
	&Sexy::SOUND_JUICY,
	&Sexy::SOUND_COFFEE,
	&Sexy::SOUND_WAKEUP,
	&Sexy::SOUND_LOWGROAN,
	&Sexy::SOUND_LOWGROAN2,
	&Sexy::SOUND_PRIZE,
	&Sexy::SOUND_YUCK,
	&Sexy::SOUND_YUCK2,
	&Sexy::SOUND_GRASSSTEP,
	&Sexy::SOUND_SHOVEL,
	&Sexy::SOUND_COBLAUNCH,
	&Sexy::SOUND_WATERING,
	&Sexy::SOUND_POLEVAULT,
	&Sexy::SOUND_GRAVESTONE_RUMBLE,
	&Sexy::SOUND_DIRT_RISE,
	&Sexy::SOUND_FERTILIZER,
	&Sexy::SOUND_PORTAL,
	&Sexy::SOUND_BUGSPRAY,
	&Sexy::SOUND_SCREAM,
	&Sexy::SOUND_PAPER,
	&Sexy::SOUND_MONEYFALLS,
	&Sexy::SOUND_IMP,
	&Sexy::SOUND_IMP2,
	&Sexy::SOUND_HYDRAULIC_SHORT,
	&Sexy::SOUND_HYDRAULIC,
	&Sexy::SOUND_GARGANTUDEATH,
	&Sexy::SOUND_CERAMIC,
	&Sexy::SOUND_BOSSBOULDERATTACK,
	&Sexy::SOUND_CHIME,
	&Sexy::SOUND_CRAZYDAVESHORT1,
	&Sexy::SOUND_CRAZYDAVESHORT2,
	&Sexy::SOUND_CRAZYDAVESHORT3,
	&Sexy::SOUND_CRAZYDAVELONG1,
	&Sexy::SOUND_CRAZYDAVELONG2,
	&Sexy::SOUND_CRAZYDAVELONG3,
	&Sexy::SOUND_CRAZYDAVEEXTRALONG1,
	&Sexy::SOUND_CRAZYDAVEEXTRALONG2,
	&Sexy::SOUND_CRAZYDAVEEXTRALONG3,
	&Sexy::SOUND_CRAZYDAVECRAZY,
	&Sexy::SOUND_PHONOGRAPH,
	&Sexy::SOUND_DANCER,
	&Sexy::SOUND_FINALFANFARE,
	&Sexy::SOUND_CRAZYDAVESCREAM,
	&Sexy::SOUND_CRAZYDAVESCREAM2,
	/* DelayLoad_Background1 */
	&Sexy::IMAGE_BACKGROUND1,
	&Sexy::IMAGE_BACKGROUND1_GAMEOVER_INTERIOR_OVERLAY,
	&Sexy::IMAGE_BACKGROUND1_GAMEOVER_MASK,
	/* DelayLoad_BackgroundUnsodded */
	&Sexy::IMAGE_BACKGROUND1UNSODDED,
	&Sexy::IMAGE_SOD1ROW,
	&Sexy::IMAGE_SOD3ROW,
	/* DelayLoad_Background2 */
	&Sexy::IMAGE_BACKGROUND2,
	&Sexy::IMAGE_BACKGROUND2_GAMEOVER_INTERIOR_OVERLAY,
	&Sexy::IMAGE_BACKGROUND2_GAMEOVER_MASK,
	/* DelayLoad_Background3 */
	&Sexy::IMAGE_BACKGROUND3,
	&Sexy::IMAGE_BACKGROUND3_GAMEOVER_INTERIOR_OVERLAY,
	&Sexy::IMAGE_BACKGROUND3_GAMEOVER_MASK,
	/* DelayLoad_Background4 */
	&Sexy::IMAGE_BACKGROUND4,
	&Sexy::IMAGE_BACKGROUND4_GAMEOVER_INTERIOR_OVERLAY,
	&Sexy::IMAGE_BACKGROUND4_GAMEOVER_MASK,
	&Sexy::IMAGE_FOG,
	&Sexy::IMAGE_FOG_SOFTWARE,
	/* DelayLoad_Background5 */
	&Sexy::IMAGE_BACKGROUND5,
	&Sexy::IMAGE_BACKGROUND5_GAMEOVER_MASK,
	/* DelayLoad_Background6 */
	&Sexy::IMAGE_BACKGROUND6BOSS,
	&Sexy::IMAGE_BACKGROUND6_GAMEOVER_MASK,
	/* DelayLoad_MushroomGarden */
	&Sexy::IMAGE_BACKGROUND_MUSHROOMGARDEN,
	/* DelayLoad_GreenHouseGarden */
	&Sexy::IMAGE_BACKGROUND_GREENHOUSE,
	/* DelayLoad_GreenHouseOverlay */
	&Sexy::IMAGE_BACKGROUND_GREENHOUSE_OVERLAY,
	/* DelayLoad_Zombiquarium */
	&Sexy::IMAGE_AQUARIUM1,
	&Sexy::IMAGE_WAVECENTER,
	&Sexy::IMAGE_WAVESIDE,
	/* DelayLoad_TreeOfWisdom */
	/* DelayLoad_AwardScreen */
	&Sexy::IMAGE_AWARDSCREEN_BACK,
	/* DelayLoad_ChallengeScreen */
	&Sexy::IMAGE_CHALLENGE_BACKGROUND,
	&Sexy::IMAGE_CHALLENGE_WINDOW,
	&Sexy::IMAGE_CHALLENGE_WINDOW_HIGHLIGHT,
	&Sexy::IMAGE_CHALLENGE_BLANK,
	&Sexy::IMAGE_CHALLENGE_THUMBNAILS,
	&Sexy::IMAGE_SURVIVAL_THUMBNAILS,
	&Sexy::IMAGE_LOCK,
	&Sexy::IMAGE_LOCK_OPEN,
	/* DelayLoad_Almanac */
	&Sexy::IMAGE_ALMANAC_INDEXBACK,
	&Sexy::IMAGE_ALMANAC_PLANTBACK,
	&Sexy::IMAGE_ALMANAC_ZOMBIEBACK,
	&Sexy::IMAGE_ALMANAC_PLANTCARD,
	&Sexy::IMAGE_ALMANAC_ZOMBIECARD,
	&Sexy::IMAGE_ALMANAC_ZOMBIEWINDOW,
	&Sexy::IMAGE_ALMANAC_ZOMBIEWINDOW2,
	&Sexy::IMAGE_ALMANAC_ZOMBIEBLANK,
	&Sexy::IMAGE_ALMANAC_GROUNDDAY,
	&Sexy::IMAGE_ALMANAC_GROUNDNIGHT,
	&Sexy::IMAGE_ALMANAC_GROUNDPOOL,
	&Sexy::IMAGE_ALMANAC_GROUNDNIGHTPOOL,
	&Sexy::IMAGE_ALMANAC_GROUNDROOF,
	&Sexy::IMAGE_ALMANAC_GROUNDICE,
	&Sexy::IMAGE_ALMANAC_CLOSEBUTTON,
	&Sexy::IMAGE_ALMANAC_CLOSEBUTTONHIGHLIGHT,
	&Sexy::IMAGE_ALMANAC_INDEXBUTTON,
	&Sexy::IMAGE_ALMANAC_INDEXBUTTONHIGHLIGHT,
	/* DelayLoad_Store */
	&Sexy::IMAGE_STORE_BACKGROUND,
	&Sexy::IMAGE_STORE_BACKGROUNDNIGHT,
	&Sexy::IMAGE_STORE_CAR,
	&Sexy::IMAGE_STORE_CAR_NIGHT,
	&Sexy::IMAGE_STORE_CARCLOSED,
	&Sexy::IMAGE_STORE_CARCLOSED_NIGHT,
	&Sexy::IMAGE_STORE_HATCHBACKOPEN,
	&Sexy::IMAGE_STORE_SIGN,
	&Sexy::IMAGE_STORE_MAINMENUBUTTON,
	&Sexy::IMAGE_STORE_MAINMENUBUTTONDOWN,
	&Sexy::IMAGE_STORE_MAINMENUBUTTONHIGHLIGHT,
	&Sexy::IMAGE_STORE_NEXTBUTTON,
	&Sexy::IMAGE_STORE_NEXTBUTTONHIGHLIGHT,
	&Sexy::IMAGE_STORE_NEXTBUTTONDISABLED,
	&Sexy::IMAGE_STORE_PREVBUTTON,
	&Sexy::IMAGE_STORE_PREVBUTTONHIGHLIGHT,
	&Sexy::IMAGE_STORE_PREVBUTTONDISABLED,
	&Sexy::IMAGE_STORE_PRICETAG,
	&Sexy::IMAGE_STORE_PACKETUPGRADE,
	&Sexy::IMAGE_STORE_MUSHROOMGARDENICON,
	&Sexy::IMAGE_STORE_AQUARIUMGARDENICON,
	&Sexy::IMAGE_STORE_TREEOFWISDOMICON,
	&Sexy::IMAGE_STORE_FIRSTAIDWALLNUTICON,
	&Sexy::IMAGE_STORE_PVZICON,
	/* DelayLoad_ZombieNote */
	&Sexy::IMAGE_ZOMBIE_NOTE,
	/* DelayLoad_ZombieNote1 */
	&Sexy::IMAGE_ZOMBIE_NOTE1,
	/* DelayLoad_ZombieNote2 */
	&Sexy::IMAGE_ZOMBIE_NOTE2,
	/* DelayLoad_ZombieNote3 */
	&Sexy::IMAGE_ZOMBIE_NOTE3,
	/* DelayLoad_ZombieNote4 */
	&Sexy::IMAGE_ZOMBIE_NOTE4,
	/* DelayLoad_ZombieFinalNote */
	&Sexy::IMAGE_ZOMBIE_FINAL_NOTE,
	/* DelayLoad_ZombieNoteHelp */
	&Sexy::IMAGE_ZOMBIE_NOTE_HELP,
	/* DelayLoad_Credits */
	&Sexy::FONT_BRIANNETOD32,
	&Sexy::FONT_BRIANNETOD32BLACK,
	&Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING1,
	&Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING2,
	&Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING3,
	&Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING4,
	&Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_SING5,
	&Sexy::IMAGE_REANIM_SUNFLOWER_HEAD_WINK,
	&Sexy::IMAGE_REANIM_CREDITS_DISCOLIGHTS,
	&Sexy::IMAGE_REANIM_CREDITS_FOGMACHINE,
	&Sexy::IMAGE_CREDITS_ZOMBIENOTE,
	&Sexy::IMAGE_CREDITS_PLAYBUTTON,
};

Sexy::Image* Sexy::GetImageById(ResourceId theId)
{
	return *(Sexy::Image**)gResources[(int)theId];
}

Sexy::Font* Sexy::GetFontById(ResourceId theId)
{
	return *(Sexy::Font**)gResources[(int)theId];
}

int Sexy::GetSoundById(ResourceId theId)
{
	return *(int*)gResources[(int)theId];
}

Image*& Sexy::GetImageRefById(ResourceId theId)
{
	return *(Image**)gResources[(int)theId];
}

Font*& Sexy::GetFontRefById(ResourceId theId)
{
	return *(Font**)gResources[(int)theId];
}

int& Sexy::GetSoundRefById(ResourceId theId)
{
	return *(int*)gResources[(int)theId];
}

Sexy::ResourceId Sexy::GetIdByImage(Image* theImage)
{
	return GetIdByVariable(theImage);
}

Sexy::ResourceId Sexy::GetIdByFont(Font* theFont)
{
	return GetIdByVariable(theFont);
}

Sexy::ResourceId Sexy::GetIdBySound(int theSound)
{
	return GetIdByVariable((void*)theSound);
}

//0x47FBC0
Sexy::ResourceId Sexy::GetIdByVariable(void* theVariable)
{
	static std::map<int, int> aMap;

	if (gNeedRecalcVariableToIdMap)
	{
		gNeedRecalcVariableToIdMap = false;
		aMap.clear();
		for (int i = 0; i < (int)ResourceId::RESOURCE_ID_MAX; i++)
			aMap[*(int*)gResources[i]] = i;
	}

	auto anIter = aMap.find((int)theVariable);
	return anIter == aMap.end() ? ResourceId::RESOURCE_ID_MAX : (ResourceId)anIter->second;
}
/*
	*aMap.end() => __asm 
	{ 
		mov eax,[aMap+04]
	}
	anIter == aMap.end() => __asm
	{ 
		mov edi,[aMap+04]  ; edi = *aMap.end()
		mov ebx,[anIter]   ; ebx = *anIter
		cmp ebx,edi        ; this �� other �� ��==���������Ƚ� *this �� *other �Ƿ���ͬ
	}
*/