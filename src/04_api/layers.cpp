#include "layers.h"

gameLayersManager::gameLayersManager() : m_Idx(0) {
}

gameLayersManager::~gameLayersManager() {
	for ( gameLayer *l : m_Layers ) {
		l->dettach();
		delete l;
	}
}

void gameLayersManager::pushLayer(gameLayer *l) {
	if (l) {
		m_Layers.emplace(m_Layers.begin() + m_Idx, l);
		l->attach();
		m_Idx++;
	}
}

void gameLayersManager::popLayer(gameLayer *l) {
	// gameLayer *itr = std::find(m_Layers.begin(), m_Layers.begin() + m_Idx, l);
	auto itr = std::find(m_Layers.begin(), m_Layers.begin() + m_Idx, l);

	if (itr != m_Layers.begin() + m_Idx) {  // l is not the last/invalid pointr-to-layer
		// itr->dettach();
		l->dettach();
		
		m_Layers.erase(itr);
		m_Idx--;
	}
}
