/*!
\note
Do not edit, generated from libs/pcrxml/generate.py
*/
#ifndef INCLUDED_PCRGENXML_AREAMAPDTD
#define INCLUDED_PCRGENXML_AREAMAPDTD


#ifndef INCLUDED_PCRGENXML_RASTERMASK
#include "pcrgenxml_rastermask.h"
#define INCLUDED_PCRGENXML_RASTERMASK
#endif



#ifndef INCLUDED_PCRGENXML_RASTERSPACE
#include "pcrgenxml_rasterspace.h"
#define INCLUDED_PCRGENXML_RASTERSPACE
#endif



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
class AreaMapDTD : public Element {
private:

 //! clean up
 void clean();
 //! name of the element
 static const std::string d_elementName;

 AreaMapDTD& operator=(const AreaMapDTD&);

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
 AreaMapDTD(const QDomElement& element);
 
 //! default ctor, initialize with no subelement and no attributes
 AreaMapDTD();
 //! Copy constructor.
 AreaMapDTD(const AreaMapDTD&);
 //! dtor
 ~AreaMapDTD() override;
 //! element name
 const std::string& elementName()const override;

 //! child element
 RasterSpace *rasterSpace{nullptr};
 //! child element
 RasterMask *rasterMask{nullptr};
};
} // namespace

#endif
