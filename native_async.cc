#include <nan.h>
#include <node.h>
#include <unistd.h>

using namespace v8;
using std::string;

class Work {
public:
  Work() = default;
  ~Work() = default;

  uv_work_t request;
  Persistent<Function> callback;
  string data = "";
};

static void WorkAsync (uv_work_t *request) {
  Work* work = static_cast<Work*>(request->data);

  work->data = "done";
  sleep(3);
}

static void WorkAsyncComplete (uv_work_t *request, int status) {
  Nan::HandleScope scope;
  Work* work = static_cast<Work*>(request->data);

  Local<Value> argv[] = { Nan::New<String>(work->data).ToLocalChecked() };
  Local<Function> callback = Nan::New<Function>(work->callback);

  Nan::Call(callback, Nan::New<Object>(), 1, argv);

  delete work;
}

void MethodAsync (const FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  Work* work = new Work();
  work->request.data = work;

  Local<Function> callback = Local<Function>::Cast(info[0]);
  work->callback.Reset(isolate, callback);

  uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_MODULE_INIT (init) {
  Nan::Set(target, Nan::New("methodAsync").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(MethodAsync)).ToLocalChecked());
}

NODE_MODULE(async, init);
