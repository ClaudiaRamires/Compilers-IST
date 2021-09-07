#ifndef __FIR_TARGETS_SYMBOL_H__
#define __FIR_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace fir {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;
    std::vector<std::shared_ptr<fir::symbol>> _args;
    std::string _name;
    long _value; // hack!
    int _offset = 0;
    bool _function;
    bool _forward = false;


  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, long value, bool function, bool forward = false) :
        _type(type), _name(name), _value(value), _function(function),_forward(forward) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }

    void set_type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }

    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }

    int offset() const {
      return _offset;
    }
    void set_offset(int offset) {
      _offset = offset;
    }

     void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }
    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const {
      return _argument_types.size();
    }

    std::shared_ptr<fir::symbol>  argument(size_t ax) const {
      return _args.at(ax); 
    }

    bool isFunction() const {
      return _function;
    }

    bool forward() const {
    return _forward;
  }

  };

  inline auto make_symbol(bool constant, std::shared_ptr<cdk::basic_type> type, const std::string &name,
                         bool function, bool forward = false) {
    return std::make_shared<symbol>( type, name,constant, function, forward);
  }

} // fir

#endif
