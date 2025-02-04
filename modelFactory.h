#pragma once
#include "main.h"
#include <map>
#include <string>
#include <memory>
#include "modelRenderer.h"

class ModleFactory
{
private:
	std::map<std::string, std::shared_ptr<ModelRenderer>> m_ModelContainer;

	std::shared_ptr<ModelRenderer> CreateModel(std::string filename)
	{
		
	}


};