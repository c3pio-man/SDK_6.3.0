#ifndef _JSON_PARSER_UTILS_H_
#define _JSON_PARSER_UTILS_H_
#include <stdint.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_util.h>
#include <string>
#include <vector>
#include "common_utilities/pblog.h"
#include "common_utilities/pb_error.h"
#include <stack>
#include <inkview.h>

namespace pocketbook
{
namespace utilities
{

class JsonParser
{
    json_object* root_obj_;
    json_object* cur_obj_;
    std::stack<json_object*> obj_stack_;

public:
    JsonParser():root_obj_(NULL),cur_obj_(NULL){}

    int Init(const std::string& json)
    {
        root_obj_ = json_tokener_parse(json.c_str());
        PB_FRETURN_IF(is_error(root_obj_), PB_FAILED);
        cur_obj_ = root_obj_;
        return is_error(root_obj_);
    }

    int Init(const std::vector<char>& json)
    {
        if (json.empty()) {
            return 1;
        }
        struct json_tokener* tok = json_tokener_new();
        root_obj_ = json_tokener_parse_ex(tok, &json[0], json.size());
        if(tok->err != json_tokener_success) {
            json_tokener_free(tok);
            root_obj_ = NULL;
            return PB_FAILED;
        }
        json_tokener_free(tok);
        PB_FRETURN_IF(is_error(root_obj_), PB_FAILED);
        cur_obj_ = root_obj_;
        return is_error(root_obj_);
    }

    json_object* get_subobject(const char * key	) {
        json_object * o;
        if (!json_object_object_get_ex(cur_obj_, key, &o)) return NULL;
        return o;
    }

    int Parse(const std::string& key, std::string* value)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;
        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        *value = json_object_get_string(o);
        return 0;
    }

    int Parse(const std::string& key, int* value)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;

        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        *value = json_object_get_int(o);
        return 0;
    }

    int Parse(const std::string& key, WIFI_SECURITY* value)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;

        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        *value = (WIFI_SECURITY)json_object_get_int(o);
        return 0;
    }

    int Parse(const std::string &key, std::vector<std::string> *array)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;
        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        array_list* list = json_object_get_array(o);
        if (!list) return PB_INVAL;
        obj_stack_.push(cur_obj_);
        for (int i = 0; i < list->length; ++i)
        {
            cur_obj_ = static_cast<json_object*>(list->array[i]);
            std::string result = json_object_get_string(cur_obj_);
            if (!result.empty())
                array->push_back(result);
        }
        cur_obj_ = obj_stack_.top();
        obj_stack_.pop();
        return 0;
    }

    int Parse(const std::string &key, std::vector<std::string> &array)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;
        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        array_list* list = json_object_get_array(o);
        if (!list) return PB_INVAL;
        obj_stack_.push(cur_obj_);
        for (int i = 0; i < list->length; ++i)
        {
            cur_obj_ = static_cast<json_object*>(list->array[i]);
            std::string result = json_object_get_string(cur_obj_);
            if (!result.empty())
                array.push_back(result);
        }
        cur_obj_ = obj_stack_.top();
        obj_stack_.pop();
        return 0;
    }

    int Parse(const std::string& key, double* value)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;
        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        *value = json_object_get_double(o);
        return 0;
    }

    int Parse(const std::string& key, bool* value)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;
        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        *value = json_object_get_boolean(o);
        return 0;
    }

    template<typename T, typename F> int Parse(std::vector<T>& value, F element_parser)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_array != json_object_get_type(cur_obj_)) return PB_INVAL;
        array_list* list = json_object_get_array(cur_obj_);
        if (!list) return PB_INVAL;
        obj_stack_.push(cur_obj_);
        for (int i = 0; i < list->length; ++i)
        {
            T new_el;
            cur_obj_ = static_cast<json_object*>(list->array[i]);
            element_parser(this, new_el);
            value.push_back(new_el);
        }
        obj_stack_.pop();
        return 0;
    }

    template<typename F> int ParseArray(F &element_parser)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_array != json_object_get_type(cur_obj_)) return PB_INVAL;

        array_list* list = json_object_get_array(cur_obj_);
        if (!list) return PB_INVAL;
        obj_stack_.push(cur_obj_);
            for (int i = 0; i < list->length; ++i)
            {
                cur_obj_ = static_cast<json_object*>(list->array[i]);
                element_parser(*this);
            }
            obj_stack_.pop();
            return 0;
    }

    template<typename F> int ParseArrayConstFunctor(const F &element_parser)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_array != json_object_get_type(cur_obj_)) return PB_INVAL;

        array_list* list = json_object_get_array(cur_obj_);
        if (!list) return PB_INVAL;
        obj_stack_.push(cur_obj_);
            for (int i = 0; i < list->length; ++i)
            {
                cur_obj_ = static_cast<json_object*>(list->array[i]);
                element_parser(*this);
            }
            obj_stack_.pop();
            return 0;
    }
    template<typename F> int ParseArray(const std::string &key, const F &element_parser)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;
        json_object * o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        if (json_type_array != json_object_get_type(o)) return PB_INVAL;
        array_list* list = json_object_get_array(o);
        if (!list) return PB_INVAL;
        obj_stack_.push(cur_obj_);
        for (int i = 0; i < list->length; ++i)
        {
            cur_obj_ = static_cast<json_object*>(list->array[i]);
            element_parser(*this);
        }
        cur_obj_ = obj_stack_.top();
        obj_stack_.pop();
        return 0;
    }

    bool IsPresent(const std::string &key)
    {
        if (!cur_obj_)
            return false;
        if (json_type_object != json_object_get_type(cur_obj_))
            return false;

        json_object * o = get_subobject(key.c_str());
        if (!o)
            return false;
        return true;
    }

    int GetListCount(const std::string &key = "")
    {
        if (!cur_obj_)
            return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_))
            return PB_INVAL;
        json_object *o;
        if (key.size()) {
            o = get_subobject(key.c_str());
        } else {
            o = cur_obj_;
        }
        if (!o)
            return PB_NOT_FOUND;
        if (json_type_array != json_object_get_type(o))
            return PB_INVAL;
        array_list* list = json_object_get_array(o);
        if (!list)
            return PB_INVAL;
        return list->length;
    }

    int PushKey(const std::string& key)
    {
        if (!cur_obj_) return PB_INVAL;
        if (json_type_object != json_object_get_type(cur_obj_)) return PB_INVAL;
        json_object* o = get_subobject(key.c_str());
        if (!o) return PB_NOT_FOUND;
        obj_stack_.push(cur_obj_);
        cur_obj_ = o;
        return PB_OK;
    }

    int PopKey()
    {
        if (obj_stack_.empty()) return PB_WRONG_STATE;
        cur_obj_ = obj_stack_.top();
        obj_stack_.pop();
        return PB_OK;
    }

	int GetString(std::string *value)
	{
		if (!cur_obj_) return PB_INVAL;
		*value = json_object_get_string(cur_obj_);
		return 0;
	}

    ~JsonParser()
    {
        if (root_obj_ && !is_error(root_obj_)) json_object_put(root_obj_);
    }

    JsonParser* Clone()
    {
        JsonParser* result = new JsonParser();
        result->root_obj_ = json_object_get(root_obj_);
        result->cur_obj_ = cur_obj_;
        result->obj_stack_ = obj_stack_;
        return result;
    }

    std::vector<std::string> GetChildrenObjectsNamesByType(json_type type)
    {
        std::vector<std::string> result;
        json_object_object_foreach(cur_obj_, key, val) {
            if (json_object_get_type(val) == type) {
                result.push_back(key);
            }
        }
        return result;
    }

    bool IsObject(const std::string &key)
    {
        if (!cur_obj_)
            return false;
        if (json_type_object != json_object_get_type(cur_obj_))
            return false;
        json_object * o = get_subobject(key.c_str());
        if (!o)
            return false;
        return (json_type_object == json_object_get_type(o));
    }
};

} // namespace utilities
} // namespace pocketbook

#endif //_JSON_PARSER_UTILS_H_
