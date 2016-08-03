
// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

class Glyph {
public:
	Glyph();
	virtual void A();
	virtual void B(); ;
	virtual void Draw();
};

class MonoGlyph : public Glyph {
public:
	MonoGlyph(Glyph* component);
	virtual void B(); 
	virtual void Draw();
private:
	Glyph* _component;
};

class Border : public MonoGlyph {
public:
	Border(MonoGlyph* monoGlyph);
	virtual void Draw();
};

class Scroller : public MonoGlyph {
public:
	Scroller(MonoGlyph* monoGlyph);
	virtual void Draw();
};

Glyph::Glyph() {
}

void Glyph::A() {
	lib.rs232.Print("Glyph:A();\r\n");
	B();
}

void Glyph::B() {
	lib.rs232.Print("Glyph:B();\r\n");
}

void Glyph::Draw() {
	lib.rs232.Print("Glyph:Draw();\r\n");
}

MonoGlyph::MonoGlyph(Glyph* component) : _component(component) {
}

void MonoGlyph::B() {
	lib.rs232.Print("MonoGlyph:B();\r\n");
	_component->B();
}

void MonoGlyph::Draw() {
	_component->Draw();
}

Border::Border(MonoGlyph* monoGlyph) : MonoGlyph(monoGlyph) {
}

void Border::Draw() {
	lib.rs232.Print("Border:Draw();\r\n");
	MonoGlyph::Draw();
}

Scroller::Scroller(MonoGlyph* monoGlyph) : MonoGlyph(monoGlyph) {
}

void Scroller::Draw() {
	lib.rs232.Print("Scroller:Draw();\r\n");
	MonoGlyph::Draw();
}
	
#if 1

U32 Items::TI007()
{
	Glyph glyph;
	MonoGlyph monoGlyph(&glyph);
	Border border(&monoGlyph);
	Scroller scroller(&border);

	scroller.A();
	lib.rs232.Print("\r\n\r\n");
	scroller.Draw();

	
	return TI_SUCCESS;
}
#else
U32 Items::TI007()
{	
	return TI_SUCCESS;
}
#endif	// #if 0
