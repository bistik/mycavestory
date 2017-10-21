#include "level.h"
#include "graphics.h"
#include "tinyxml2.h"

#include <SDL2/SDL.h>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
		_mapName(mapName),
		_spawnPoint(spawnPoint),
		_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	// parse tmx file

	XMLDocument doc;
	std::stringstream ss;
	ss << "content/maps/" << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	// get size of map
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	// load the <tileset>

	XMLElement* pTileset = mapNode->FirstChildElement("tileset");

	if (pTileset != NULL) {

		while (pTileset) {
			int firstgid;
			//const char* source = pTileset->Attribute("source");

			//char* path;

			std::stringstream ss;
			//ss << source;
			//ss << "../backgrounds/PrtCave.png";
			ss << "content/backgrounds/PrtCave.png";
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	//Loading <layers>
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			// loading <data>
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					// loading <tile>
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							// build each tile here
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								} else {
									break;
								}
							}

							//Get the tileset for this gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									tls = this->_tilesets.at(i);
									break;
								}
							}

							if (tls.FirstGid == -1) {
								// no tileset found
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								} else {
									break;
								}
							}

							//get position of tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							// compute position of tile in tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;
							int tsyy = 0;
							int amt = (gid / (tilesetWidth / tileWidth));
							tsyy = tileHeight * amt;
							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

							// build the tile and add to level tilelist
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;
							pTile = pTile->NextSiblingElement("tile");
						}
					}
					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	// parse the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup"); // <objectgroup>
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object"); // <object>
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			// other objectgroups go here with else if
			else if (ss.str() == "spawn points") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
									std::ceil(y) * globals::SPRITE_SCALE);
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			// get next
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}

}

void Level::update(int elapsedTime) {}

void Level::draw(Graphics &graphics) {
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}
