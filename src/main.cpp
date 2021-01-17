#include <iostream>
#include <typeinfo>
#include <string>
#include "webapplication.hpp"
extern "C"{
    #include "interface.h"
}

int main(int argc, char** argv){

    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
        
        auto app = std::make_unique<WebApplication>(env);
        auto theme = std::make_shared<Wt::WBootstrapTheme>();
        theme->setVersion(Wt::BootstrapVersion::v3);
        app->setTheme(theme);

        app->messageResourceBundle().use(app->appRoot() + "index");

        app->root()->addNew<WebTemplate>();
        
        return app;
    });
}