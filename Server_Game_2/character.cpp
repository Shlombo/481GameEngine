#include "character.h"
#include "event.h"
#include "eventManager.h"

Character::Character(float length, float width, float xPos, float yPos, int id, GameTime *timelineObj, EventManager *man, ScriptManager *scriptMan) : GameObject(true, true, true, true, true, length, width) {
    setPosition(xPos, yPos);
    setFillColor(sf::Color::Red);
    this->velocityX = 0;
    this->velocityY = 0;
    this->speed = 0.25;
    this->left = false;
    this->right = false;
    this->scriptX = xPos;
    this->scriptY = yPos;
    this->guid = "paddle";
    this->setId(id);
    this->setTimeline(timelineObj);
    this->setManager(man);
    this->setScriptManager(scriptMan);
    
}

void Character::input() {
    this->velocityX = 0;
    updateTime();
    if(left && right) {
        //Do nothing
    } else if(left) {
        this->scriptManager->runOne("raise_left", false);
        manager->raiseScriptedEvent(this->timeline->getTime());
    } else if(right) {
        this->scriptManager->runOne("raise_right", false);
        manager->raiseScriptedEvent(this->timeline->getTime());
    }
}

void Character::render(sf::RenderWindow *window) {
    if(window->hasFocus()) {
        window->setActive(true);
        window->draw(*this);
    }
    window->setActive(false);
}


void Character::movement() {
    //For non scripted
    // this->move(this->velocityX, this->velocityY);

    //For scripted
    this->setPosition(this->scriptX, this->scriptY);
}

void Character::setTimeline(GameTime *timelineObj) {
    this->timeline = timelineObj;
    this->current_game_time = this->timeline->getTime();
    this->last_game_time = this->current_game_time;
}

void Character::updateTime() {
    this->current_game_time = this->timeline->getTime();
    this->deltaT = this->current_game_time - this->last_game_time;
    this->last_game_time = this->current_game_time;
}

void Character::setManager(EventManager *man) {
    this->manager = man;
}

void Character::setScriptManager(ScriptManager *man) {
    this->scriptManager = man;
}

v8::Local<v8::Object> Character::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("x", getGameObjectX, setGameObjectX));
	v.push_back(v8helpers::ParamContainer("y", getGameObjectY, setGameObjectY));
	v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));
    return v8helpers::exposeToV8(guid , this, v, isolate, context, context_name);
}

void Character::setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<Character*>(ptr)->scriptX = value->NumberValue();
}

void Character::getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int x_val = static_cast<Character*>(ptr)->scriptX;
	info.GetReturnValue().Set(x_val);
}


void Character::setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<Character*>(ptr)->scriptY = value->NumberValue();
}

void Character::getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int y_val = static_cast<Character*>(ptr)->scriptY;
	info.GetReturnValue().Set(y_val);
}

void Character::setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
	static_cast<Character*>(ptr)->guid = *utf8_str;
}

void Character::getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	std::string guid = static_cast<Character*>(ptr)->guid;
	v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), guid.c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(v8_guid);
}