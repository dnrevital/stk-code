
//  $Id: dust_cloud.cpp 1681 2008-04-09 13:52:48Z hikerstk $
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "smoke.hpp"

#include "graphics/material_manager.hpp"
#include "graphics/irr_driver.hpp"
#include "io/file_manager.hpp"
#include "karts/kart.hpp"
#include "utils/constants.hpp"

Smoke::Smoke(Kart* kart) : m_kart(kart)
{    
    m_node = irr_driver->addParticleNode();
    m_node->setParent(m_kart->getNode());
    const float particle_size = 0.5f;
    m_node->setPosition(core::vector3df(0, particle_size*0.5f, -m_kart->getKartLength()*0.5f));
    Material *m= material_manager->getMaterial("smoke.png");
    m->setMaterialProperties(&(m_node->getMaterial(0)));
    m_node->setMaterialTexture(0, m->getTexture());

    m_emitter = m_node->createPointEmitter(core::vector3df(0, 0, 0),   // velocity in m/ms
                                           5, 10,
                                           video::SColor(255,0,0,0),
                                           video::SColor(255,255,255,255),
                                           400, 400,
                                           20  // max angle
                                           );
    m_emitter->setMinStartSize(core::dimension2df(particle_size, particle_size));
    m_emitter->setMaxStartSize(core::dimension2df(particle_size, particle_size));
    m_node->setEmitter(m_emitter); // this grabs the emitter

    scene::IParticleAffector *af = m_node->createFadeOutParticleAffector();
    m_node->addAffector(af);
    af->drop();
}   // KartParticleSystem

//-----------------------------------------------------------------------------
/** Destructor, removes
 */
Smoke::~Smoke()
{
    irr_driver->removeNode(m_node);
}   // ~Smoke

//-----------------------------------------------------------------------------
void Smoke::update(float t)
{
    // No particles to emit, no need to change the speed
    if(m_emitter->getMinParticlesPerSecond()==0)
        return;
    // There seems to be no way to randomise the velocity for particles,
    // so we have to do this manually, by changing the default velocity.
    // Irrlicht expects velocity (called 'direction') in m/ms!!
    Vec3 dir(cos(DEGREE_TO_RAD(rand()%180))*0.001f,
             sin(DEGREE_TO_RAD(rand()%180))*0.001f,
             sin(DEGREE_TO_RAD(rand()%100))*0.001f);
    m_emitter->setDirection(dir.toIrrVector());
}   // update
//-----------------------------------------------------------------------------
void Smoke::setCreationRate(float f)
{
    m_emitter->setMinParticlesPerSecond(int(f));
    m_emitter->setMaxParticlesPerSecond(int(f));
}   // setCreationRate
