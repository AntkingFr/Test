#include "Map.h"
#include "collision.h"
#include "Character.h"



Map::Map(void)
{
	
}



Map::Map(sf::Vector2f size, int height, int tile_sz)
{
	m_height=height;
	m_size=size;
	m_tile_sz=tile_sz;
	
	//intern_time=clock();

	//Allocation espace map

	m_map = (Tile***)malloc( m_height * sizeof(Tile**));
	for(int h=0; h<height; h++)
	{
		m_map[h] = (Tile**)malloc( m_size.x * sizeof(Tile*) );
		for(int i=0; i<m_size.x; i++)
		{
			m_map[h][i] = (Tile*)malloc(m_size.y * sizeof(Tile));
			for(int j=0; j<m_size.y; j++)
			{
				m_map[h][i][j].m_pos_text=sf::Vector2f(0,0);

				m_map[h][i][j].m_collisionable=true;
			}
		}
	}	

	this->update();
}


	
Map::~Map()
{
	for(int h=0; h<this->getHeight(); h++)
	{
		for(int i=0; i<this->getSize().x; i++)
		{
			if(m_map[h][i]!=NULL)
				free(m_map[h][i]);
		}

		if(m_map[h]!=NULL)
			free(m_map[h]);
	}	
	free(m_map);	
}


//Taille de la map
int Map::getHeight() const										{	return m_height;	}


//Hauteur de la map
sf::Vector2f Map::getSize() const								{	return m_size;		}


//Taille d'une tile
int Map::getTileSize() const									{	return m_tile_sz;	}


//Une tile de la map
Tile Map::getTile(int height, sf::Vector2i tile_pos) const 		
{	
	if(m_map!=NULL && m_map[height]!=NULL && m_map[height][tile_pos.x]!=NULL) 
		return m_map[height][tile_pos.x][tile_pos.y]; 
	else
	{
		std::cerr << "Erreur lors de la tentative de l'acquisition d'une tile, la map n'est peut être pas correctement chargée" << std::endl;
		return (Tile){sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	}
}

Tile Map::getTileFromCoords(int height, sf::Vector2f position) const
{
	if(m_map!=NULL && m_map[height]!=NULL && m_map[height][(int)(position.x/this->getTileSize())]!=NULL) 
		return m_map[height][(int)(position.x/this->getTileSize())][(int)(position.y/this->getTileSize())]; 
	else
	{
		std::cerr << "Erreur lors de la tentative de l'acquisition d'une tile, la map n'est peut être pas correctement chargée" << std::endl;	
		return (Tile){sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	}
}


void Map::setTile(Tile tile, sf::Vector2i position, int height)
{
	m_map[height][position.x][position.y]=tile;
}



void Map::setTexture(sf::Texture* texture)
{
	m_texture=texture;
}



void Map::update()
{
	m_time=clock();
	m_vertex.setPrimitiveType(sf::Quads);
	m_vertex.resize(this->getSize().x * this->getSize().y * this->getHeight() * 4);

	int index=0;	
	for(int h=0; h<this->getHeight(); h++)
	{
		for(int i=0; i<this->getSize().x; i++)
		{
			for(int j=0; j<this->getSize().y; j++)
			{
				index = h*this->getSize().x * this->getSize().y * 4 + i*this->getSize().y * 4 + j*4;

				m_vertex[index].position = sf::Vector2f(i*m_tile_sz, j*m_tile_sz);
				m_vertex[index+1].position = sf::Vector2f((i+1)*m_tile_sz, j*m_tile_sz);
				m_vertex[index+2].position = sf::Vector2f((i+1)*m_tile_sz, (j+1)*m_tile_sz);
				m_vertex[index+3].position = sf::Vector2f(i*m_tile_sz, (j+1)*m_tile_sz);

				m_vertex[index].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(0, 0);
				m_vertex[index+1].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, 0);
				m_vertex[index+2].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, m_tile_sz);
				m_vertex[index+3].texCoords= m_map[h][i][j].m_pos_text + sf::Vector2f(0, m_tile_sz);


				m_vertex[index].color.a = 255;
				m_vertex[index+1].color.a = 255;
				m_vertex[index+2].color.a = 255;
				m_vertex[index+3].color.a = 255;	
			}
		}	
	}
}


void Map::update_transparency(int chosen_height)
{
	m_vertex.setPrimitiveType(sf::Quads);
	m_vertex.resize(this->getSize().x * this->getSize().y * this->getHeight() * 4);

	int index=0;	
	for(int h=0; h<this->getHeight(); h++)
	{
		for(int i=0; i<this->getSize().x; i++)
		{
			for(int j=0; j<this->getSize().y; j++)
			{
				index = h*this->getSize().x * this->getSize().y * 4 + i*this->getSize().y * 4 + j*4;

				m_vertex[index].position = sf::Vector2f(i*m_tile_sz, j*m_tile_sz);
				m_vertex[index+1].position = sf::Vector2f((i+1)*m_tile_sz, j*m_tile_sz);
				m_vertex[index+2].position = sf::Vector2f((i+1)*m_tile_sz, (j+1)*m_tile_sz);
				m_vertex[index+3].position = sf::Vector2f(i*m_tile_sz, (j+1)*m_tile_sz);

				m_vertex[index].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(0, 0);
				m_vertex[index+1].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, 0);
				m_vertex[index+2].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, m_tile_sz);
				m_vertex[index+3].texCoords= m_map[h][i][j].m_pos_text + sf::Vector2f(0, m_tile_sz);

				if((int)(index/(this->getSize().x * this->getSize().y * 4)) != chosen_height)
				{
					m_vertex[index].color.a = 50;
					m_vertex[index+1].color.a = 50;
					m_vertex[index+2].color.a = 50;
					m_vertex[index+3].color.a = 50;
				}
				else
				{
					m_vertex[index].color.a = 255;
					m_vertex[index+1].color.a = 255;
					m_vertex[index+2].color.a = 255;
					m_vertex[index+3].color.a = 255;
				}

			}
		}	
	}
}


void Map::physics_objects()
{
	for(std::vector<Object*>::iterator obj1=m_objects.begin(); obj1!=m_objects.end(); obj1++)
	{
		
		//Physique des objets entre eux
		for(std::vector<Object*>::iterator obj2=obj1+1; obj2!=m_objects.end(); obj2++)
		{
			if((*obj1)->getType().compare("Character")==0 && (*obj2)->getType().compare("Character")==0)
				physics_characters((Character*)(*obj1), (Character*)(*obj2));
		}

		//Physique des objets avec les éléments de la map
		physics_character_map((Character*)(*obj1), this, 0);
		
	}
}


void Map::addObject(Object* object)
{
	m_objects.push_back(object);

	std::sort(m_objects.begin(), m_objects.end(), comparePosY);
}
	



/*
//Draw version simplifiée (sans perspective ou notion d'ordre d'affichage)
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	std::cout << "Temps :" << m_time << std::endl;
	states.transform *= getTransform();

	states.texture = m_texture;

	target.draw(m_vertex, states);
};
*/

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Permet de dessiner la map et les objets qui s'y trouvent dans un ordre dépendant de leur position. En effet, l'ordre d'affichage permet de définir un effet de perspective.
	states.transform *= getTransform();

	states.texture = m_texture;

	sf::VertexArray tmp_vertex;
	tmp_vertex.setPrimitiveType(sf::Quads);

	std::vector<Object*> obj_drawn;

	for(int j=0; j<this->getSize().y + this->getHeight() - 1; j++)
	{

		///////////////////////////////////
		//Dessine les objets		
		for(std::vector<Object*>::const_iterator it=m_objects.begin(); it!=m_objects.end(); it++)
		{
			
			bool obj_already_drawn=false;
	
			for(std::vector<Object*>::iterator it2=obj_drawn.begin(); it2!=obj_drawn.end(); it2++)
			{
				if( (*it)==(*it2) )
					obj_already_drawn = true;
			}


			if(!obj_already_drawn && ((*it)->getPosition().y + (*it)->getSize().y) < ( (j) * this->getTileSize()) + 1)
			{
				target.draw(*(*it));
				obj_drawn.push_back(*it);
			}
		}
		//
		////////////////////////////////////



		////////////////////////////////////
		// Dessine la map
		int tmp_size = this->getSize().x * 4;

		if(j>=this->getSize().y)
			tmp_size *= (this->getSize().y + this->getHeight()-1 - j);
		else if(j<this->getHeight())
			tmp_size *= (j+1);
		else
			tmp_size *= this->getHeight();

		tmp_vertex.resize(tmp_size);

		int tmp_j=0;

		for(int h=0; h < this->getHeight(); h++)
		{
			if(j-h>=0 && j-h < this->getSize().y)
			{
				for(int i=0; i< this->getSize().x; i++)
				{
					int index_tmp_vertex = tmp_j*this->getSize().x * 4 + i*4;
					int index_vertex = h*this->getSize().x * this->getSize().y * 4 + i*this->getSize().y * 4 + (j-h)*4;
					
					tmp_vertex[index_tmp_vertex] = m_vertex[index_vertex];
					tmp_vertex[index_tmp_vertex+1]=m_vertex[index_vertex+1];
					tmp_vertex[index_tmp_vertex+2]=m_vertex[index_vertex+2];
					tmp_vertex[index_tmp_vertex+3]=m_vertex[index_vertex+3];
				}

				tmp_j++;
			}
		}

		target.draw(tmp_vertex, states);
		//
		//////////////////////////////////

	}
};




bool comparePosY(Object* obj1, Object* obj2)
{ 
	return obj1->getPosition().y + obj1->getSize().y < obj2->getPosition().y + obj2->getSize().y;
}