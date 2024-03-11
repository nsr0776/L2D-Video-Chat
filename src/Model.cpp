#include "Model.hpp"
#include <cstring>
#include <exception>
#include <LApp/LAppPal.hpp>
#include <JsonConverter.hpp>
#include <LApp/LAppDefine.hpp>

#include <QDebug>

using namespace Csm;

Model::Model()
	: LAppModel() { }

Model::~Model() {
	_currentExpression = nullptr;
}


void Model::update(const nlohmann::json* data) {
	const csmFloat32 deltaTimeSeconds = LAppPal::GetDeltaTime();
	_userTimeSeconds += deltaTimeSeconds;

	// Expression
	_model->LoadParameters();
	if (data == nullptr) {
		// Case: No new facial data
		if (_currentExpression != nullptr) {
			// Replay the last obtained facial data
			//_expressionManager->StartMotionPriority(_currentExpression, false, 3);
		}
		else {
			// Do nothing
		}
	}
	else {
		// Case: New facial data obtained
		setExpression(data);
		_expressionManager->StartMotionPriority(_currentExpression, true, 3);	// Level3 is the highest 'Force' level
		//SetRandomExpression();
	}
	_model->SaveParameters();
	//////////////////////////////////////////////////////////////////////////

	_expressionManager->UpdateMotion(_model, deltaTimeSeconds);

	// Auto breathing
	if (_breath != nullptr)
	{
		_breath->UpdateParameters(_model, deltaTimeSeconds);
	}

	// Set pose
	if (_pose != nullptr)
	{
		_pose->UpdateParameters(_model, deltaTimeSeconds);
	}

	_model->Update();
}

void Model::setExpression(const nlohmann::json* data) {
	qDebug().noquote() << "[Model] Set new facial data";

	csmSizeInt size;
	csmByte* buffer;

	auto _ = JsonConverter::convert(data, &buffer, &size);

	_currentExpression = LoadExpression(buffer, size, "dummy");

	DeleteBuffer(buffer, "[Live expression data]");
}

