#include <nan.h>
#include <node.h>
#include <unistd.h>

using namespace v8;
using std::string;

class Work : public Nan::AsyncWorker {
public:
  Work (Nan::Callback* callback) : AsyncWorker(callback) {};
  ~Work () {};
  void Execute ();
  void Destory () {};
private:
  string data = "";
protected:
  void HandleOKCallback ();
  void HandleErrorCallback () {};
};

void Work::Execute () {
  this->data = "done";
  sleep(3);
}

void Work::HandleOKCallback () {
  Nan::HandleScope scope;
  Local<Value> argv[] = { Nan::New<String>(this->data).ToLocalChecked() };
  this->callback->Call(1, argv);
}

NAN_METHOD (MethodAsync) {
  Local<Function> cb = Local<Function>::Cast(info[0]);
  Nan::Callback* callback = new Nan::Callback(cb);
  Nan::AsyncQueueWorker(new Work(callback));

  info.GetReturnValue().Set(Nan::Null());
}

NAN_MODULE_INIT (init) {
  Nan::Set(target, Nan::New("methodAsync").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(MethodAsync)).ToLocalChecked());
}

NODE_MODULE(nan_async, init)
