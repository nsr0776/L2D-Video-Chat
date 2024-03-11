#ifndef MODEL_HPP_ 
#define MODEL_HPP_

#include "LApp/LAppModel.hpp"
#include <nlohmann/json.hpp>


class Model : public LAppModel {
public:
	Model();
	virtual ~Model();
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

public:
	void update(const nlohmann::json* data);
	//void initialize(ICubismModelSetting* setting)

private:
	void setExpression(const nlohmann::json* data);

private:
	Csm::ACubismMotion* _currentExpression = nullptr;

};

#endif