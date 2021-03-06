/*!
\note
Do not edit, generated from libs/pcrxml/generate.py
*/
#ifndef INCLUDED_STDDEFX
#include "stddefx.h"
#define INCLUDED_STDDEFX
#endif
#ifndef INCLUDED_PCRGENXML_EXCHANGEMODEL
#include "pcrgenxml_exchangemodel.h"
#define INCLUDED_PCRGENXML_EXCHANGEMODEL
#endif

#ifndef INCLUDED_QDOM
#include <qdom.h>
#define INCLUDED_QDOM
#endif



#ifndef INCLUDED_PCRXML_CHILDELEMENTVISITOR
#include "pcrxml_childelementvisitor.h"
#define INCLUDED_PCRXML_CHILDELEMENTVISITOR
#endif

const std::string pcrxml::ExchangeModel::d_elementName("exchangeModel");
//! ctor
pcrxml::ExchangeModel::ExchangeModel(const QDomElement& element):Element(element,d_elementName)
 ,id(element,"id",false)
 ,ioStrategy(element,"ioStrategy",false)
 ,description(element,"description",false)
 ,integerTimer(nullptr)
 ,areaMapDTD(nullptr)
 {
  try {
   ChildElementVisitor v(element);

   // optional element
   if(v.currentChildEq("integerTimer"))
     integerTimer = new IntegerTimer(v.processChild());
   // optional element
   if(v.currentChildEq("areaMapDTD"))
     areaMapDTD = new AreaMapDTD(v.processChild());
   // * repeated element
   while(v.currentChildEq("exchangeItem"))
     exchangeItem.push_back(new ExchangeItem(v.processChild()));
  } catch (...) { clean(); throw; }
 }
pcrxml::ExchangeModel::ExchangeModel():Element()
 
 {
 }
const std::string& pcrxml::ExchangeModel::elementName() const
{
 return d_elementName;
}
//! dtor
pcrxml::ExchangeModel::~ExchangeModel()
{
 clean();
}
//! clean
void pcrxml::ExchangeModel::clean()
{
 delete integerTimer;integerTimer=nullptr;
 delete areaMapDTD;areaMapDTD=nullptr;
 for(auto & i : exchangeItem) delete i;
exchangeItem.clear();
}
//! copy ctor
pcrxml::ExchangeModel::ExchangeModel(const ExchangeModel& src):
pcrxml::Element(src)
,id(src.id)
,ioStrategy(src.ioStrategy)
,description(src.description)
{
 integerTimer= (src.integerTimer) ? new IntegerTimer(*(src.integerTimer)): nullptr;
 areaMapDTD= (src.areaMapDTD) ? new AreaMapDTD(*(src.areaMapDTD)): nullptr;
 for(auto i : src.exchangeItem) exchangeItem.push_back(new ExchangeItem(*i));
}
//! assignment operator
pcrxml::ExchangeModel& pcrxml::ExchangeModel::operator=(const ExchangeModel& src)
{
 if(this != &src)
 {
   clean(); PRECOND(false);
  integerTimer= (src.integerTimer) ? new IntegerTimer(*(src.integerTimer)): nullptr;
  areaMapDTD= (src.areaMapDTD) ? new AreaMapDTD(*(src.areaMapDTD)): nullptr;
  for(auto i : src.exchangeItem) exchangeItem.push_back(new ExchangeItem(*i));
 }
return *this;
}
void pcrxml::ExchangeModel::fill(QDomElement el) const
{
 id.addToElement(el,"id");
 ioStrategy.addToElement(el,"ioStrategy");
 description.addToElement(el,"description");
 if (integerTimer) integerTimer->appendTo(el);
 if (areaMapDTD) areaMapDTD->appendTo(el);
 for(auto i : exchangeItem) i->appendTo(el);
}
