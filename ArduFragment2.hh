//holaaaaaaaaaaaaaa
//branch quilombo1
#ifndef sipmdaq_Overlays_ArduFragment_hh
#define sipmdaq_Overlays_ArduFragment_hh

#include "artdaq-core/Data/Fragment.hh"

#include <ostream>

namespace sipm
{
  class ArduFragment;

  std::ostream& operator <<(std::ostream&, ArduFragment const&);

}

class sipm::ArduFragment
{

public:

  struct Metadata {
    
    typedef uint32_t data_t;
    
    data_t num_serial_ardu: 16;
    data_t num_canales : 16;

    static size_t const tam_palabra = 1ul;
  };

  static_assert (sizeof(Metadata) == Metadata::tam_palabra * sizeof(Metadata::data_t), "ArduFragment::Metadata tamanho modificado");

  typedef uint8_t ardu_byte;

  struct Header {

    typedef uint32_t data_t;
    
    data_t per_conteo: 16;
    data_t canales_activos: 16;

    typedef uint32_t tam_fragmento_t;
    
    tam_fragmento_t tam_fragmento : 32;

    static size_t const tam_palabra = 2ul;

  };
 
  static_assert(sizeof(Header) == Header::tam_palabra * sizeof(Header::data_t), "ArduFragment::Header tamanho modificado");

  explicit ArduFragment(artdaq::Fragment const& f) : fragmento(f){}

  Header::tam_fragmento_t hdr_tam_fragmento() const {

    return header_()->tam_fragmento;

  }

  Header::data_t hdr_canales_activos() const {

    return header_()->canales_activos;

  }

  Header::data_t hdr_per_conteo() const { 

    return header_()->per_conteo;

  }

  static constexpr size_t hdr_tam_palabra() {

    return Header::tam_palabra;

  }

  size_t data_valor_total() const {

    return (hdr_tam_fragmento() - hdr_tam_palabra()) * data_por_palabra();

  }

  ardu_byte valor_data(size_t indice){

    if(indice >= data_valor_total()){return 0xff;}

    return inicioDeDatos()[indice];

  }

  ardu_byte const* inicioDeDatos() const { 

    return reinterpret_cast<ardu_byte const *>(header_() + 1);

  }

  ardu_byte const* finDeDatos() const {

    return inicioDeDatos() + data_valor_total();

  }

protected:

  static constexpr size_t data_por_palabra()
  {
    
    return sizeof(Header::data_t) / sizeof(ardu_byte);

  }

  Header const* header_() const{

    return reinterpret_cast<ArduFragment::Header const *>(fragmento.dataBeginBytes());
  }

private:

  artdaq::Fragment const& fragmento;

};

#endif

  

  

 





