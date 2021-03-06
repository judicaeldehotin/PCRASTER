/*!
\note
Do not edit, generated from libs/pcrxml/generate.py
*/
#ifndef INCLUDED_STDDEFX
#include "stddefx.h"
#define INCLUDED_STDDEFX
#endif
#ifndef INCLUDED_PCRGENXML_LODINGS
#include "pcrgenxml_lodings.h"
#define INCLUDED_PCRGENXML_LODINGS
#endif

#ifndef INCLUDED_QDOM
#include <qdom.h>
#define INCLUDED_QDOM
#endif



#ifndef INCLUDED_PCRXML_CHILDELEMENTVISITOR
#include "pcrxml_childelementvisitor.h"
#define INCLUDED_PCRXML_CHILDELEMENTVISITOR
#endif

const std::string pcrxml::Lodings::d_elementName("Lodings");
//! ctor
pcrxml::Lodings::Lodings(const QDomElement& element):Element(element,d_elementName)
 ,dataExtend(nullptr)
 {
  try {
   ChildElementVisitor v(element);

   // required element
   v.checkRequiredChild("DataExtend");
   dataExtend = new DataExtend(v.processChild());
   // + repeated element
   v.checkRequiredChild("LodingName");
   while(v.currentChildEq("LodingName"))
     lodingName.push_back(new LodingName(v.processChild()));
  } catch (...) { clean(); throw; }
 }
pcrxml::Lodings::Lodings():Element()
 
 {
 }
const std::string& pcrxml::Lodings::elementName() const
{
 return d_elementName;
}
//! dtor
pcrxml::Lodings::~Lodings()
{
 clean();
}
//! clean
void pcrxml::Lodings::clean()
{
 delete dataExtend;dataExtend=nullptr;
 for(auto & i : lodingName) delete i;
lodingName.clear();
}
//! copy ctor
pcrxml::Lodings::Lodings(const Lodings& src):
pcrxml::Element(src)
{
 dataExtend=new DataExtend(*(src.dataExtend));
 for(auto i : src.lodingName) lodingName.push_back(new LodingName(*i));
}
//! assignment operator
pcrxml::Lodings& pcrxml::Lodings::operator=(const Lodings& src)
{
 if(this != &src)
 {
   clean(); PRECOND(false);
  dataExtend=new DataExtend(*(src.dataExtend));
  for(auto i : src.lodingName) lodingName.push_back(new LodingName(*i));
 }
return *this;
}
void pcrxml::Lodings::fill(QDomElement el) const
{
 if (dataExtend) dataExtend->appendTo(el);
 for(auto i : lodingName) i->appendTo(el);
}
