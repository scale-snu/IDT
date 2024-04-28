#include <iostream>
#include <iomanip>
#include <string.h>
#include "Piraha.hpp"
#include <stdlib.h>
#include <sstream>
#include "Cactus/cctk_core.h"
#include "cctk_Parameter.h"
#include <map>
#include <math.h>
#include <algorithm>
#include <limits>
#include <fstream>

namespace cctki_piraha {

#define VAR(X) " " #X "=" << X

extern "C" int CCTK_ParameterFilename(int len, char *filename);

/**
 * This holds the structure required for parsing
 * a Cactus par file.
 */
static smart_ptr<Grammar> par_file_grammar = new Grammar();

static std::string mklower(std::string& in) {
	std::string s = in;
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

enum ValueType { PIR_STRING,PIR_INT,PIR_REAL,PIR_BOOL,PIR_VOID };

std::string get_parfile() {
    char path[500];
    CCTK_ParameterFilename(500, path);
    char *value = strrchr (path, '/');
    if (value == NULL) {
        value = path;
    } else {
        value++;
    }
    int n = strlen(value);
    if(n > 4 && strcmp(value+n-4,".par")==0) {
    	value[n-4] = '\0';
    }
    std::string s = value;
    return s;
}

std::ostream& operator<<(std::ostream& o,const ValueType& vt) {
	if(vt == PIR_STRING)
		o << "STRING";
	else if(vt == PIR_INT)
		o << "INT";
	else if(vt == PIR_REAL)
		o << "REAL";
	else if(vt == PIR_BOOL)
		o << "BOOL";
	else if(vt == PIR_VOID)
		o << "VOID";
	else
		o << "UNDEF";
	return o;
}

std::string current_thorn;

/**
 * The Value class is something like a union. It holds
 * integer, real, and string data and has a type field
 * that identifies which field is currently valid.
 */
struct Value {
	/** This field holds the parse tree element associated with this Value. */
	smart_ptr<Group> hold;
	double ddata;
	int idata;
	std::string sdata;
	ValueType type;
	Value(smart_ptr<Group> g) : hold(g), ddata(0), idata(0), sdata(), type(PIR_VOID) {}
	~Value() {}
	/**
	 * Create a string representation of the Value.
	 */
	std::string copy() {
		assert(type != PIR_VOID);
		if(type == PIR_STRING) {
			return sdata;
		} else if(type == PIR_BOOL) {
			std::string s = idata ? "yes" : "no";
			return s;
		} else {
			std::ostringstream o;
			if(type == PIR_REAL) {
				o << std::setprecision(15) << ddata;
			} else {
				o << idata;
			}
			return o.str();
		}
	}
	/**
	 * Check to see if something is a bool and throw an error
	 * if it's not.
	 */
	void checkBool() {
		if(type != PIR_BOOL) {
			std::ostringstream msg;
			msg << "Does not evaluate to a boolean: " << hold->substring();
			std::string par = get_parfile();
			CCTK_Error(hold->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
		}
	}
	/**
	 * Compares with another Value, and throws a CCTK_Error
	 * if the comparison doesn't make sense.
	 */
	bool equals(smart_ptr<Value> v) {
		if(type == PIR_BOOL && v->type == PIR_BOOL) {
			return idata == v->idata;
		} else if(type == PIR_INT && v->type == PIR_INT) {
			return idata == v->idata;
		} else if(type == PIR_INT && v->type == PIR_REAL) {
			return idata == v->ddata;
		} else if(type == PIR_REAL && v->type == PIR_INT) {
			return ddata == v->idata;
		} else if(type == PIR_REAL && v->type == PIR_REAL) {
			return ddata == v->ddata;
		} else if(type == PIR_STRING && v->type == PIR_STRING) {
			return sdata == v->sdata;
		}
		std::ostringstream msg;
		msg << "Cannot compare " << type << " and " << v->type << std::endl;
		std::string par = get_parfile();
		CCTK_Error(hold->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
	        return false;
	}
	/**
	 * Return a double value, whether the underlying
	 * quantity is integer or real.
	 */
	double doubleValue() {
		if(type == PIR_REAL)
			return ddata;
		else if(type == PIR_INT)
			return idata;
		std::ostringstream msg;
		msg << "Cannot convert " << type << " to double." << std::endl;
		std::string par = get_parfile();
		CCTK_Error(hold->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
		return 0;
	}
	bool intOrDouble() {
		return type == PIR_INT || type == PIR_REAL;
	}
	/**
	 * This function converts a double to a real, but
	 * only if this can be done without loss of precision.
	 */
	void integerize() {
		if(type == PIR_REAL) {
			idata = ddata;
			if(idata == ddata) {
				type = PIR_INT;
			}
		}
	}
	void booleanize(smart_ptr<Group> gr) {
		if(type == PIR_STRING) {
			std::string s = mklower(sdata);
			if(s == "yes" || s == "true") {
				idata = 1;
				type = PIR_BOOL;
			} else if(s == "no" || s == "false") {
				idata = 0;
				type = PIR_BOOL;
			}
		} else if(type == PIR_INT) {
			/// Steven R. Brandt would like to remove this
			/// particular auto-conversion
			if(idata == 1 || idata == 0) {
				type = PIR_BOOL;
				std::ostringstream msg;
				msg << "Boolean variable is set with integer: " << gr->substring() << std::endl;
				std::string par = get_parfile();
				CCTK_Warn(1,gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
			}
		}
	}
};

std::ostream& operator<<(std::ostream& o,const smart_ptr<Value>& val) {
	if(!val.valid()) {
		return o << "NULL";
	}
	o << "(" << val->type << ")";
	if(val->type == PIR_REAL) {
		o << val->ddata;
	} else if(val->type == PIR_INT) {
		o << val->idata;
	} else if(val->type == PIR_BOOL) {
		o << (val->idata ? "true" : "false");
	} else if(val->type == PIR_STRING) {
		o << "\"" << val->sdata << "\"";
	} else {
		o << val->hold->substring();
	}
	return o;
}

typedef std::map<std::string,std::map<std::string,smart_ptr<Value> > >::iterator th_iter;
typedef std::map<std::string,smart_ptr<Value> >::iterator nm_iter;

smart_ptr<Value> eval_expr(std::string inp);

// If the value was already defined in this parameter file, look
// it up in the map. Otherwise, get it from Cactus.
smart_ptr<Value> find_val(smart_ptr<Group> gr,std::string thorn,std::string name) {
	smart_ptr<Value> ret = new Value(gr);
    int type;
    const void *result;
    result = CCTK_ParameterGet(name.c_str(),thorn.c_str(),&type);
    if(result == NULL) {
		std::ostringstream msg;
		msg << "Undefined or inaccessible variable: " << thorn << "::" << name << std::endl;
		std::string par = get_parfile();
		CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
    }
    
    switch(type) {
        case PARAMETER_REAL:
            ret->type = PIR_REAL;
            ret->ddata = *(const CCTK_REAL*)result;
            break;
        case PARAMETER_INT:
            ret->type = PIR_INT;
            ret->idata = *(const CCTK_INT*)result;
            break;
        case PARAMETER_BOOLEAN:
            ret->type = PIR_BOOL;
            ret->idata = *(const CCTK_INT*)result;
            break;
        case PARAMETER_STRING:
        case PARAMETER_SENTENCE:
        case PARAMETER_KEYWORD:
            {
                ret->type = PIR_STRING;
                const char *s = *(const char **)result;
                ret->sdata += s;
            }
            break;
        default:
		    std::ostringstream msg;
		    msg << "Unexpected type result from ParameterGet=" << type << std::endl;
		    std::string par = get_parfile();
		    CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
    }
    //std::cout << "Piraha: GET: " << thorn << "::" << name << "=" << ret << std::endl;
	return ret;
}

smart_ptr<Value> lookup_var(smart_ptr<Group> gr) {
	smart_ptr<Value> ret;
	if(gr->group(0)->getPatternName() == "env") {
		const char *env = getenv(gr->group(0)->group(0)->substring().c_str());
		if(env != NULL) {
			ret = new Value(gr);
			ret->type = PIR_STRING;
			ret->sdata = env;
		}
	} else if(gr->group(0)->substring() == "parfile") {
		ret = new Value(gr);
		ret->type = PIR_STRING;
		ret->sdata = get_parfile();
	} else if(gr->group(0)->substring() == "pi") {
		ret = new Value(gr);
		ret->type = PIR_REAL;
		ret->ddata = 4.0*atan2(1.0,1.0);
	}
	if(!ret.valid()) {
		std::ostringstream msg;
		msg << "Unknown variable: " << gr->substring() << std::endl;
		std::string par = get_parfile();
		CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
	}
	return ret;
}


std::string string_reparser(std::string s) {
	smart_ptr<Matcher> m = new Matcher(par_file_grammar,"stringparser",s.c_str(),s.length());
	if(m->matches()) {
		std::string out = "";
		for(int i=0;i < m->groupCount(); i++) {
			std::string pn = m->group(i)->getPatternName();
			if(pn == "any" || pn == "name") {
				out += m->group(i)->substring();
			} else if(pn == "stringcomment") {
				;
			} else {
				smart_ptr<Value> val = lookup_var(m->group(i));
				out += val->copy();
			}
		}
		return out;
	} else {
		return s;
	}
}

/**
 * The meval() function takes any node within
 * the parse tree and creates a Value object
 * from it. It is, therefore, designed to be
 * used recursively.
 **/
smart_ptr<Value> meval(smart_ptr<Group> gr) {
	assert(gr.valid());
	std::string pn = gr->getPatternName();
	smart_ptr<Value> ret = new Value(gr);
	if(pn == "num") {
		std::string s = gr->substring();
		s = mklower(s);
		ret->ddata = atof(s.c_str());
		ret->idata = ret->ddata;
		if(ret->idata == ret->ddata && (s.find('.') == std::string::npos))
			ret->type = PIR_INT;
		else
			ret->type = PIR_REAL;
	} else if(pn == "paren" || pn == "parindex") {
		return meval(gr->group(0));
	} else if(pn == "func") {
		std::string fn = gr->group(0)->substring();
		fn = mklower(fn);
		smart_ptr<Value> val = meval(gr->group(1));
		if(val->type == PIR_REAL || val->type == PIR_INT) {
			if(fn == "trunc") {
				val->ddata = trunc(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "floor") {
				val->ddata = floor(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "ceil") {
				val->ddata = ceil(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "sqrt") {
				val->ddata = sqrt(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "atan") {
				val->ddata = atan(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "sin") {
				val->ddata = sin(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "cos") {
				val->ddata = cos(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "tan") {
				val->ddata = tan(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "exp") {
				val->ddata = exp(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "log") {
				val->ddata = log(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "abs") {
				val->ddata = fabs(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "acos") {
				val->ddata = acos(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "asin") {
				val->ddata = asin(val->doubleValue());
				val->type = PIR_REAL;
				return val;
			} else if(fn == "bool") {
				if(val->type == PIR_REAL) {
					val->idata = val->ddata;
				}
				val->type = PIR_BOOL;
				return val;
			} else if(fn == "int") {
				if(val->type == PIR_REAL) {
					val->idata = val->ddata;
					val->type = PIR_INT;
				}
				return val;
			} else if(fn == "real") {
				if(val->type == PIR_INT) {
					val->ddata = val->idata;
					val->type = PIR_REAL;
				}
				return val;
			}
		} else if(val->type == PIR_BOOL) {
			if(fn == "int") {
				val->type = PIR_INT;
				return val;
			} else if(fn == "real") {
				val->type = PIR_REAL;
				val->ddata = val->idata;
				return val;
			}
		}
		std::ostringstream msg;
		msg << "Unknown func: " << fn << "(" << val->type << ")" << std::endl;
		std::string par = get_parfile();
		CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
	} else if(pn == "name"||pn == "dname") {
		std::string s = gr->substring();
		s = mklower(s);
		if(s == "nan") {
			ret->ddata = NAN;
			ret->type = PIR_REAL;
		} else if(s == "inf") {
			ret->ddata = INFINITY;
			ret->type = PIR_REAL;
		} else if(s == "no" || s == "false") {
			ret->type = PIR_BOOL;
			ret->idata = 0;
		} else if(s == "yes" || s == "true") {
			ret->type = PIR_BOOL;
			ret->idata = 1;
		} else {
			ret->type = PIR_STRING;
			ret->sdata = gr->substring();
		}
		return ret;
	} else if(pn == "par") {
		std::string thorn = gr->group(0)->substring();
		std::string name = gr->group(1)->substring();
		if(gr->groupCount() == 3) {
			std::ostringstream vn;
			smart_ptr<Value> index = meval(gr->group(2));
			if(index->type == PIR_INT) {
				std::stringstream o;
				o << name << "[" << index->idata << "]";
				o << std::flush;
				std::string keyi = o.str();
				ret = find_val(gr,thorn,keyi);
				return ret;
			}
		} else {
			ret = find_val(gr,thorn,name);
			return ret;
		}
		std::ostringstream msg;
		msg << "Unknown par: " << thorn << "::" << name << std::endl;
		std::string par = get_parfile();
		CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
	} else if(pn == "var") {
		ret = lookup_var(gr);
	} else if(pn == "value") {
		if(gr->groupCount()==2) {
			std::string unop = gr->group(0)->substring();
			ret = meval(gr->group(1));
			if(unop == "-") {
				if(ret->type == PIR_INT) {
					ret->idata = -ret->idata;
				} else if(ret->type == PIR_REAL) {
					ret->ddata = -ret->ddata;
				} else {
					std::ostringstream msg;
					msg << "Unknown operation: " << unop << ret->type << std::endl;
					std::string par = get_parfile();
					CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else if(unop == "!") {
				if(ret->type == PIR_BOOL) {
					ret->idata = !ret->idata;
				} else {
					std::ostringstream msg;
					msg << "Unknown operation: " << unop << ret->type << std::endl;
					std::string par = get_parfile();
					CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else {
				std::ostringstream msg;
				msg << "Unknown operation: " << unop << ret->type << std::endl;
				std::string par = get_parfile();
				CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
			}
		} else {
			return meval(gr->group(0));
		}
	} else if(pn == "quot") {
		ret->type = PIR_STRING;
		ret->sdata = string_reparser(gr->group(0)->substring());
	} else if(pn == "inquot") {
		ret->type = PIR_STRING;
		ret->sdata = gr->substring();
	} else if(pn == "expr") {
		if(gr->groupCount()==1)
			return meval(gr->group(0));
		smart_ptr<Value> v1 = meval(gr->group(0));
		smart_ptr<Value> v2 = meval(gr->group(1));
		v1->checkBool();
		v2->checkBool();
		ret->type = PIR_BOOL;
		ret->idata = v1->idata || v2->idata;
	} else if(pn == "powexpr") {
		if(gr->groupCount()==1)
			return meval(gr->group(0));
		smart_ptr<Value> v1 = meval(gr->group(0));
		smart_ptr<Value> v2 = meval(gr->group(1));
		ret->type = PIR_REAL;
		ret->ddata = pow(v1->doubleValue(),v2->doubleValue());
	} else if(pn == "andexpr") {
		if(gr->groupCount()==1)
			return meval(gr->group(0));
		smart_ptr<Value> v1 = meval(gr->group(0));
		smart_ptr<Value> v2 = meval(gr->group(1));
		v1->checkBool();
		v2->checkBool();
		ret->type = PIR_BOOL;
		ret->idata = v1->idata && v2->idata;
	} else if(pn == "eqexpr") {
		if(gr->groupCount()==1)
			return meval(gr->group(0));
		smart_ptr<Value> v1 = meval(gr->group(0));
		std::string eqop = gr->group(1)->substring();
		smart_ptr<Value> v2 = meval(gr->group(2));
		ret->type = PIR_BOOL;
		if(eqop == "==") {
			ret->idata = v1->equals(v2);
		} else if(eqop == "!=") {
			ret->idata = !v1->equals(v2);
		} else {
			std::ostringstream msg;
			msg << "Unknown equality operator: " << eqop << std::endl;
			std::string par = get_parfile();
			CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
		}
	} else if(pn == "compexpr") {
		if(gr->groupCount()==1)
			return meval(gr->group(0));
		smart_ptr<Value> v1 = meval(gr->group(0));
		if(gr->groupCount()>0) {
			std::string compop = gr->group(1)->substring();
			smart_ptr<Value> v2 = meval(gr->group(2));
			double d1 = v1->doubleValue();
			double d2 = v2->doubleValue();
			ret->type = PIR_BOOL;
			if(compop == "<") {
				ret->idata = (d1 < d2);
				return ret;
			} else if(compop == ">") {
				ret->idata = (d1 > d2);
				return ret;
			} else if(compop == "<=") {
				ret->idata = (d1 <= d2);
				return ret;
			} else if(compop == ">=") {
				ret->idata = (d1 >= d2);
				return ret;
			}
			std::ostringstream msg;
			msg << "Unknown comparison operator: " << compop << std::endl;
			std::string par = get_parfile();
			CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
		}
	} else if(pn == "aexpr") {
		if(gr->groupCount()==1)
			return meval(gr->group(0));
		smart_ptr<Value> v1 = meval(gr->group(0));
		for(int i=1;i+1<gr->groupCount();i+=2) {
			std::string addop = gr->group(i)->substring();
			smart_ptr<Value> v2 = meval(gr->group(i+1));
			assert(v2.valid());
			if(v1->type == PIR_INT && v2->type == PIR_INT) {
				ret->type = PIR_INT;
				if(addop == "+") {
					ret->idata = v1->idata + v2->idata;
				} else if(addop == "-") {
					ret->idata = v1->idata - v2->idata;
				} else {
					std::ostringstream msg;
					msg << "Unknown add operator: " << addop << std::endl;
					std::string par = get_parfile();
					CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else if(v1->type == PIR_STRING && v2->type == PIR_STRING) {
				ret->type = PIR_STRING;
				if(addop == "+") {
					ret->sdata = v1->sdata + v2->sdata;
				} else {
					std::ostringstream msg;
                    ret->sdata = v1->sdata + addop + v2->sdata;
					msg << "Unknown add operator: " << addop << std::endl;
                    msg << "Interpreting as literal string with value '" << ret->sdata << "'" << std::endl;
					std::string par = get_parfile();
					CCTK_Warn(1,gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else if(v1->intOrDouble() && v2->intOrDouble()) {
				ret->type = PIR_REAL;
				if(addop == "+") {
					ret->ddata = v1->doubleValue()+v2->doubleValue();
				} else if(addop == "-") {
					ret->ddata = v1->doubleValue()-v2->doubleValue();
				} else {
					std::ostringstream msg;
					msg << "Unknown add operator: " << addop << std::endl;
					std::string par = get_parfile();
					CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else {
				std::ostringstream msg;
				msg << "Unknown operation: " << v1->type;
				msg << " " << addop << v2->type << std::endl;
				std::string par = get_parfile();
				CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
			}
			v1 = ret;
		}
	} else if(pn == "mexpr") {
		if(gr->groupCount()==1)
			return meval(gr->group(0));
		smart_ptr<Value> v1 = meval(gr->group(0));
		for(int i=1;i+1<gr->groupCount();i+=2) {
			std::string mulop = gr->group(i)->substring();
			smart_ptr<Value> v2 = meval(gr->group(i+1));
			if(v1->type == PIR_INT && v2->type == PIR_INT) {
				ret->type = PIR_INT;
				if(mulop == "*") {
					ret->idata = v1->idata * v2->idata;
				} else if(mulop == "/") {
					ret->idata = v1->idata / v2->idata;
				} else if(mulop == "%") {
					ret->idata = v1->idata % v2->idata;
				} else {
					std::ostringstream msg;
					msg << "Unknown mul operator: " << v1->type << mulop << v2->type << std::endl;
					std::string par = get_parfile();
					CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else if(v1->type == PIR_STRING && v2->type == PIR_INT) {
				ret->type = PIR_STRING;
				if(mulop == "*") {
					ret->sdata = "";
					for(int i=0;i<v2->idata;i++)
						ret->sdata += v1->sdata;
				} else {
					std::ostringstream msg;
					msg << "Unknown mul operator: " << v1->type << mulop << v2->type << std::endl;
					std::string par = get_parfile();
					CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else if(v1->intOrDouble() && v2->intOrDouble()) {
				ret->type = PIR_REAL;
				if(mulop == "*") {
					ret->ddata = v1->doubleValue()*v2->doubleValue();
				} else if(mulop == "/") {
					ret->ddata = v1->doubleValue()/v2->doubleValue();
				} else {
					std::ostringstream msg;
					msg << "Unknown mul operator: " << v1->type << mulop << v2->type << std::endl;
					std::string par = get_parfile();
					CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
				}
			} else if(v1->type == PIR_STRING && v2->type == PIR_STRING) {
				std::ostringstream msg;
                ret->type = PIR_STRING;
                ret->sdata = v1->sdata + mulop + v2->sdata;
				msg << "Unknown operation: " << v1->type << " " << mulop << v2->type << std::endl;
                msg << "Interpreting as literal string with value '" << ret->sdata << "'" << std::endl;
				std::string par = get_parfile();
				CCTK_Warn(1,gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
			} else {
				std::ostringstream msg;
				msg << "Unknown operation: " << v1->type << " " << mulop << v2->type << std::endl;
				std::string par = get_parfile();
				CCTK_Error(gr->line(),par.c_str(),current_thorn.c_str(),msg.str().c_str());
			}
			v1 = ret;
		}
	} else {
		std::ostringstream msg;
		std::string par = get_parfile();
		msg << "Pattern not handled[" << gr->getPatternName() << "]=" << gr->substring() <<
				" at " << gr->line() << " in " << par << std::endl;
		CCTK_Error(__LINE__,__FILE__,"piraha",msg.str().c_str());
	}
	return ret;
}


smart_ptr<Value> eval_expr(std::string input) {
	smart_ptr<Matcher> m = new Matcher(par_file_grammar,"eval",input.c_str());
	smart_ptr<Value> ret;
	if(m->matches()) {
		ret = meval(m->group(0));
	}
	return ret;
}

void check_types(const char *thorn,int line,ValueType v,int t) {
	bool ok = false;
	if(v == PIR_STRING && (t == PARAMETER_STRING || t == PARAMETER_KEYWORD))
		ok = true;
	else if(v == PIR_INT && t == PARAMETER_INT)
		ok = true;
	else if(v == PIR_BOOL && t == PARAMETER_BOOLEAN)
		ok = true;
	else if((v == PIR_INT || v == PIR_REAL) && t == PARAMETER_REAL)
		ok = true;
	if(!ok) {
		std::string par = get_parfile();
		std::ostringstream msg;
		msg << "Parameter type mismatch ";
		switch(t) {
		case PARAMETER_BOOLEAN:
			msg << "BOOL";
			break;
		case PARAMETER_INT:
			msg << "INT";
			break;
		case PARAMETER_KEYWORD:
			msg << "KEYWORD";
			break;
		case PARAMETER_STRING:
			msg << "STRING";
			break;
		case PARAMETER_REAL:
			msg << "REAL";
			break;
		default:
			msg << "type(" << t << ")";
			break;
		}
		msg << " != " << v << std::endl;
		CCTK_Error(line,par.c_str(),thorn,msg.str().c_str());
	}
}

extern "C" int cctk_PirahaParser(const char *buffer,unsigned long buffersize,int (*set_function)(const char *, const char *, int)) {
	const char *par_file_src =
			"skipper = ([ \\t\\r\\n]|\\#.*)*\n"
			"# comment\n"
			"skipeol = ([ \\t\\r]|\\#.*)*($|\\n)\n"
			"any = [^]\n"
			"stringcomment = #.*\n"
			"stringparser = ^({stringcomment}|{var}|{name}|{any})*$\n"

			"# Note that / occurs in some par files. It is my\n"
			"# feeling that this should require quote marks.\n"

			"name = [a-zA-Z][a-zA-Z0-9_]*\n"
			"dname = [0-9][a-zA-Z_]{2,}\n"
			"inquot = ({var}|\\\\.|[^\\\\\"])*\n"
			"fname = \\.?/[-\\./0-9a-zA-Z_]+\n"
			"quot = \"{inquot}\"|{fname}\n"
			"num = ([0-9]+(\\.[0-9]*|)|\\.[0-9]+)([ed][+-]?[0-9]+|)\n"
			"env = ENV\\{{name}\\}\n"
			"var = \\$({env}|{name}|\\{{name}\\})\n"

			"powexpr = {value}( \\*\\* {value})?\n"
			"mulop = [*/%]\n"
			"mexpr = {powexpr}( {mulop} {powexpr})*\n"
			"addop = [+-]\n"
			"aexpr = {mexpr}( {addop} {mexpr})*\n"
			"compop = [<>]=?\n"
			"compexpr = {aexpr}( {compop} {aexpr})?\n"
			"eqop = [!=]=\n"
			"eqexpr = {compexpr}( {eqop} {eqexpr})?\n"
			"andexpr = {eqexpr}( && {eqexpr})?\n"
			"expr = {andexpr}( \\|\\| {andexpr})?\n"
			"eval = {expr}\n"

			"paren = \\( {expr} \\)\n"
			"par = {name} :: {name}( {parindex})?\n"
			"func = {name} \\( {expr} \\)\n"
			"array = \\[ {expr}( , {expr})* \\]\n"

			"value = {unop}?({par}|{func}|{paren}|{dname}|{num}|{quot}|{name}|{var})\n"
			"unop = [-!]\n"

			"int = [0-9]+\n"
			"index = \\[ {int} \\]\n"
			"parindex = \\[ {expr} \\]\n"
			"active = (?i:ActiveThorns)\n"
			"set = ({active} = ({quot}|{name})|{par}( {index}|) = ({array}|\\+?{expr})){-skipeol}\n"
            "desc = !DESC {quot}\n"
			"file = ^( ({desc}|{set}|{active}) )*$";
    //std::ofstream peg("/tmp/par.peg");
    //peg << par_file_src;
    //peg.close();

	compileFile(par_file_grammar,par_file_src,strlen(par_file_src));

	std::string active;
    smart_ptr<Matcher> m2 = new Matcher(par_file_grammar,"file",buffer,buffersize);
    if(m2->matches()) {
    	int line = -1;
    	for(int i=0;i<m2->groupCount();i++) {
    		smart_ptr<Group> gr = m2->group(i);
    		if(gr->group(0)->getPatternName() == "active") {
    			smart_ptr<Value> smv = meval(gr->group(1));
    			std::string val = smv->copy();
    			active += val;
    			active += ' ';
    			line = gr->line();
    		}
    	}
    	set_function("ActiveThorns",active.c_str(),line);
    	for(int i=0;i<m2->groupCount();i++) {
    		smart_ptr<Group> gr = m2->group(i);
    		if(gr->getPatternName() != "set")
    			continue;
    		smart_ptr<Group> par = gr->group("par");
    		if(par.valid()) {
    			// add value->quot->inquot
    			std::string key;
    			std::string thorn = par->group("name",0)->substring();
    			std::string name  = par->group("name",1)->substring();
    			key += thorn;
    			key += "::";
    			key += name;
				const cParamData *data = CCTK_ParameterData(name.c_str(),thorn.c_str());

    			smart_ptr<Group> index = par->group("parindex");
    			if(index.valid()) {
    				key += '[';
    				smart_ptr<Value> vv = meval(index);
    				if(vv->type != PIR_INT) {
    					std::ostringstream msg;
    					std::string par = get_parfile();
    					msg << "bad index " << vv << std::endl;
    					CCTK_Error(index->line(),par.c_str(),thorn.c_str(),msg.str().c_str());
    				}
    				std::string vvstr = vv->copy();
    				key += vvstr;
    				key += ']';
    			}

    			std::string val;
    			smart_ptr<Group> aexpr = gr->group("expr");
    			if(aexpr.valid()) {
    				current_thorn = thorn;
    				smart_ptr<Value> smv = meval(aexpr);
					val = smv->copy();
    				assert(smv.valid());
    				smv->integerize();
    				if(data != NULL) {
        				if(data->type == PARAMETER_REAL)
        					smv->integerize();
        				if(data->type == PARAMETER_BOOLEAN)
        					smv->booleanize(gr);
    					check_types(thorn.c_str(),aexpr->line(),smv->type,data->type);
    				}
    				set_function(
    						strdup(key.c_str()),
    						strdup(val.c_str()),
    						gr->group(0)->line());
    			} else {
    				smart_ptr<Group> arr = gr->group("array");
    				for(int i=0;i<arr->groupCount();i++) {
    					aexpr = arr->group(i);
    					std::ostringstream keyi;
    					keyi << key << '[' << i << ']';
    					smart_ptr<Value> smv = meval(aexpr);
    					val = smv->copy();
        				if(data != NULL) {
            				if(data->type == PARAMETER_REAL)
            					smv->integerize();
            				if(data->type == PARAMETER_BOOLEAN)
            					smv->booleanize(gr);
        					check_types(thorn.c_str(),aexpr->line(),smv->type,data->type);
        				}
        				set_function(
        						strdup(keyi.str().c_str()),
        						strdup(val.c_str()),
        						aexpr->line());
    				}
    			}
    		}
    	}
    } else {
        // TODO: Fix this
    	std::cout << "ERROR IN PARAMETER FILE:" << std::endl;
    	m2->showError();
        return 1;
    }
    return 0;
}

}
