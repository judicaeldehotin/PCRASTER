/*!
\note
Do not edit, generated from libs/pcrxml/generate.py
*/
#ifndef INCLUDED_PCRGENXML_TIMER
#define INCLUDED_PCRGENXML_TIMER


#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif



#ifndef INCLUDED_PCRXML_ELEMENT
#include "pcrxml_element.h"
#define INCLUDED_PCRXML_ELEMENT
#endif


class QDomNode;
namespace pcrxml {
 //! Do not edit, generated from PCRaster dtd/schema pcrxml::Element has detailed documentation
class Timer : public Element {
private:

 //! clean up
 void clean();
 //! name of the element
 static const std::string d_elementName;

 Timer& operator=(const Timer&);

 //! in support of toDomElement and toDom
  void       fill(QDomElement el) const override;
public:
 static const char* tagName() {
   return d_elementName.c_str();
 }
 //! ctor
 /*! \throws
        com::BadStreamFormat if xml is not valid
 */
 Timer(const QDomElement& element);
 
 //! default ctor, initialize with no subelement and no attributes
 Timer();
 //! Copy constructor.
 Timer(const Timer&);
 //! dtor
 ~Timer() override;
 //! element name
 const std::string& elementName()const override;

};
} // namespace

#endif
