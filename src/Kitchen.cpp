#include "KitchenCatalog.hpp"

#include "Kitchen.hpp"

Kitchen::Kitchen(const KitchenCalatog &catalog, size_t ncook, int id, int port)
  : _catalog(catalog), _ncook(ncook)
{
}
