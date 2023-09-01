#include "stdafx.h"
#include "IntegrityManager.h"
IntegrityManager g_IntegrityManager;
IntegrityManager* GetIntegrityManager() {
	return &g_IntegrityManager;
}