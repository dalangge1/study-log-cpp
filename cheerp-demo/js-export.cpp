#include <cheerp/clientlib.h>

class [[cheerp::jsexport]] [[cheerp::genericjs]] JsStruct
{
private:
        float a;
        int b;
public:
        JsStruct(float _a, int _b):a(_a),b(_b)
        {
        }
        void test()
        {
                client::console.log("Invoked test");
        }
};

