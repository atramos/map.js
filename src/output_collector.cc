#include "output_collector.h"

using namespace v8;
using namespace node;
using namespace mapjs;

Persistent<FunctionTemplate> OutputCollector::constructor_template;

void OutputCollector::Initialize(Handle<Object> target) {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    OutputCollector::constructor_template = Persistent<FunctionTemplate>::New(t);
    OutputCollector::constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
    OutputCollector::constructor_template->SetClassName(String::NewSymbol("OutputCollector"));
    
    NODE_SET_PROTOTYPE_METHOD(OutputCollector::constructor_template, "collect", OutputCollector::Collect);
    NODE_SET_PROTOTYPE_METHOD(OutputCollector::constructor_template, "values", Values);
    NODE_SET_PROTOTYPE_METHOD(OutputCollector::constructor_template, "join", Join);
    NODE_SET_PROTOTYPE_METHOD(OutputCollector::constructor_template, "keys", Keys);
    NODE_SET_PROTOTYPE_METHOD(OutputCollector::constructor_template, "size", Size);
    NODE_SET_PROTOTYPE_METHOD(OutputCollector::constructor_template, "toArray", ToArray);
    target->Set(String::NewSymbol("OutputCollector"), OutputCollector::constructor_template->GetFunction());
    target->Set(String::New("collect"), FunctionTemplate::New(Collect)->GetFunction());
    target->Set(String::New("values"), FunctionTemplate::New(Values)->GetFunction());
    target->Set(String::New("join"), FunctionTemplate::New(Join)->GetFunction());
    target->Set(String::New("keys"), FunctionTemplate::New(Keys)->GetFunction());
    target->Set(String::New("size"), FunctionTemplate::New(Size)->GetFunction());
    target->Set(String::New("toArray"), FunctionTemplate::New(ToArray)->GetFunction());
}

Handle<Value> OutputCollector::New(const Arguments& args) {
    HandleScope scope;

    OutputCollector* outputCollector  = new OutputCollector();
    outputCollector->Wrap(args.This());
    return args.This();
}

Handle<Value> OutputCollector::Collect(const Arguments& args) {
    HandleScope scope;

    std::string key = V8StringToStdString(args[0]);
    std::string value = V8StringToStdString(args[1]);

    OutputCollector* outputCollector = ObjectWrap::Unwrap<OutputCollector>(args.This());
    outputCollector->map()->insert(std::pair<std::string, std::string>(key, value));

    return Undefined();
}

Handle<Value> OutputCollector::Values(const Arguments& args) {
    HandleScope scope;

    OutputCollector* outputCollector = ObjectWrap::Unwrap<OutputCollector>(args.This());

    std::string key = V8StringToStdString(args[0]);
    OutputCollectorMap* map = outputCollector->map();
    std::pair<OutputCollectorMap::iterator, OutputCollectorMap::iterator> ret; 
    OutputCollectorMap::iterator itr;
    ret = map->equal_range(key);

    Local<Array> array = Array::New();
    int i = 0;
    for(itr = ret.first, i; itr != ret.second; ++itr, ++i) {
        array->Set(Integer::New(i), String::New((*itr).second.c_str()));
    }

    return scope.Close(array);
}

Handle<Value> OutputCollector::Join(const Arguments& args) {
    HandleScope scope;

    OutputCollector* outputCollector = ObjectWrap::Unwrap<OutputCollector>(args.This());
    OutputCollector* outputCollector2 = ObjectWrap::Unwrap<OutputCollector>(args[0]->ToObject());

    outputCollector->map()->insert(outputCollector2->map()->begin(),
        outputCollector2->map()->end());

    return Undefined();
}

Handle<Value> OutputCollector::Keys(const Arguments& args) {
    HandleScope scope;

    OutputCollector* outputCollector = ObjectWrap::Unwrap<OutputCollector>(args.This());
    OutputCollectorMap* map = outputCollector->map();

    Local<Array> array = Array::New();
    int i = 0;
    std::string lastKey= "";
    for(OutputCollectorMap::iterator itr = map->begin(); itr != map->end(); ++itr) {
        std::string key = (*itr).first;
        if(lastKey != key) {
            array->Set(Integer::New(i), String::New(key.c_str()));
            lastKey = key;
            ++i;
        }
    }

    return scope.Close(array);
}

Handle<Value> OutputCollector::Size(const Arguments& args) {
    HandleScope scope;

    OutputCollector* outputCollector = ObjectWrap::Unwrap<OutputCollector>(args.This());
    OutputCollectorMap* map = outputCollector->map();
    Local<Value> size = Integer::New(map->size());

    return scope.Close(size);
}

Handle<Value> OutputCollector::ToArray(const Arguments& args) {
    HandleScope scope;

    OutputCollector* outputCollector = ObjectWrap::Unwrap<OutputCollector>(args.This());
    OutputCollectorMap* map = outputCollector->map();
    Local<Array> array = Array::New();
    int i = 0;

    for(OutputCollectorMap::iterator itr = map->begin(); itr != map->end(); ++itr) {
        std::string key = (*itr).first;
        std::string value = (*itr).second;
        array->Set(Integer::New(i), String::New((key + " : " + value).c_str()));
        i++;
    }

    return scope.Close(array);
}
