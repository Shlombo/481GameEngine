#include "movingPlatform.h"

MovingPlatform::MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, int color) : GameObject(false, true, true, true, true, length, width) {
    this->type = type;
    setPosition(xPos, yPos);
    if(color == 0) {
        setFillColor(sf::Color::Blue);
    } else if(color == 1) {
        setFillColor(sf::Color::Green);
    } else if(color == 2) {
        setFillColor(sf::Color::Yellow);
    } else {
        setFillColor(sf::Color::Magenta);
    }
    this->boundUL = boundUL;
    this->boundDR = boundDR;
    if(this->getType() == "vertical") {
        this->velocityY = 1;
    } else if(this->getType() == "horizontal"){
        this->velocityX = 1;
    } else {
        this->scriptX = xPos;
        this->scriptY = yPos;
        this->guid = "scripted1";
    }
    
}

MovingPlatform::MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, std::string textureFile) : GameObject(false, true, true, true, true, length, width) {
    this->type = type;
    setPosition(xPos, yPos);
    if(!this->tex.loadFromFile(textureFile)) {
        setFillColor(sf::Color::Blue);
    } else {
        setTexture(&this->tex);
    }
    this->boundUL = boundUL;
    this->boundDR = boundDR;
    if(this->getType() == "vertical") {
        this->velocityY = 1;
    } else {
        this->velocityX = 1;
    }

}


void MovingPlatform::movement() {
    
    this->updateTime();
    if(this->getType() == "vertical") {
        if(this->getPosition().y < this->boundUL ) {
            this->velocityY = 1;
        } else if(this->getPosition().y > this->boundDR ) {
            this->velocityY = -1;
        }
        // std::cout << "MOVEUP" << "\n";
        this->move(0.f, (this->velocityY * this->deltaT) / 10);
    } else if(this->getType() == "horizontal") {
        if(this->getPosition().x < this->boundUL ) {
            this->velocityX = 1;
        } else if(this->getPosition().x > this->boundDR ) {
            this->velocityX = -1;
        }
        // std::cout << "MOVESIDE" << "\n";
        this->move(((this->velocityX * this->deltaT) / 10), 0.f);
    } else if(this->getType() == "scripted") {
        this->setPosition(this->scriptX, this->scriptY);
    }

}

void MovingPlatform::render(sf::RenderWindow *window) {
    if(window->hasFocus()) {
        window->setActive(true);
        window->draw(*this);
    }
    window->setActive(false);
}

void MovingPlatform::setTimeline(GameTime *timelineObj) {
    this->timeline = timelineObj;
    this->current_game_time = this->timeline->getTime();
    this->last_game_time = this->current_game_time;
}

void MovingPlatform::updateTime() {
    this->current_game_time = this->timeline->getTime();
    this->deltaT = this->current_game_time - this->last_game_time;
    this->last_game_time = this->current_game_time;
}

std::string MovingPlatform::getType() {
    return this->type;
}


// void MovingPlatform::setHasMovement(bool boolean) {
//     this->hasMovement = boolean;
// }

v8::Local<v8::Object> MovingPlatform::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("x", getGameObjectX, setGameObjectX));
	v.push_back(v8helpers::ParamContainer("y", getGameObjectY, setGameObjectY));
	v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));
    return v8helpers::exposeToV8(guid , this, v, isolate, context, context_name);
}

void MovingPlatform::setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<MovingPlatform*>(ptr)->scriptX = value->NumberValue();
}

void MovingPlatform::getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int x_val = static_cast<MovingPlatform*>(ptr)->scriptX;
	info.GetReturnValue().Set(x_val);
}


void MovingPlatform::setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<MovingPlatform*>(ptr)->scriptY = value->NumberValue();
}

void MovingPlatform::getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int y_val = static_cast<MovingPlatform*>(ptr)->scriptY;
	info.GetReturnValue().Set(y_val);
}

void MovingPlatform::setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
	static_cast<MovingPlatform*>(ptr)->guid = *utf8_str;
}

void MovingPlatform::getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	std::string guid = static_cast<MovingPlatform*>(ptr)->guid;
	v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), guid.c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(v8_guid);
}