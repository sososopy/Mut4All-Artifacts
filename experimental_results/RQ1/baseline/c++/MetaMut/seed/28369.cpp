

module;

#include <iostream>
#include <string_view>
#include <string>
export module hello;
export import :sub_mod;

namespace X {

        inline unsigned gv = 52;

        void greeter2 ( std::string_view const &name ){
                std::string h = "non exported greeter";


                std::cout << "Bye" << name << "!" << h << "\n";

               // Y::g();
        }

}

export {

        namespace X {

                void greeter ( std::string_view const &name ){
                        std::string h = "exported greeter";

                        std::cout << "Hello " << name << "!" << h << "\n";

                        greeter2("greet2");
                }

        }
}

