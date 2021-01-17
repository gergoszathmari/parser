#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <errno.h>

#include "webapplication.hpp"

#include <Wt/DomElement.h>

extern "C"{
    #include "y.tab.h"
    #include "interface.h"

    typedef struct yy_buffer_state * YY_BUFFER_STATE;
    extern YY_BUFFER_STATE yy_scan_string(const char * str);
    extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
    extern void yy_switch_to_buffer(YY_BUFFER_STATE buffer);
}

extern int errorAt[2];

WebApplication::WebApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
{
    setTitle("Szakdolgozat");
    addMetaHeader("viewport", "width=device-width, initial-scale=1.0");
    addMetaHeader("charset", "UTF-8");

    require("resources/js/prism.js");
    
    Wt::WLink prismLink("resources/style/prism.css");
    useStyleSheet(prismLink);
}

WebTemplate::WebTemplate() 
    : WTemplate{tr("page.index")}
{
     
    fileUpload = bindNew<Wt::WFileUpload>("fileUploadPlace");

    submitButton = bindNew<Wt::WPushButton>("submit", "Submit");
    downloadButton = bindNew<Wt::WPushButton>("download", "Download");

    downloadButton->disable();
    submitButton->disable();


    submitButton->clicked().connect([=]{
        fileUpload->upload();
    });

    fileResource = std::make_shared<FormattedResource>();

    Wt::WLink link = Wt::WLink(fileResource);
    link.setTarget(Wt::LinkTarget::Download);
    downloadButton->setLink(link);

    auto container = bindWidget<Wt::WContainerWidget>("radioButtonGroup", Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto group = std::make_shared<Wt::WButtonGroup>();
    Wt::WRadioButton* button;

    button = container->addNew<Wt::WRadioButton>("Unchanged");
    button->setInline(false);
    group->addButton(button);

    button = container->addNew<Wt::WRadioButton>("Same Line");
    button->setInline(false);
    group->addButton(button);

    button = container->addNew<Wt::WRadioButton>("New Line");
    button->setInline(false);
    group->addButton(button);

    fileUpload->changed().connect([=] {
        submitButton->enable();
    });

    fileUpload->uploaded().connect([=] {
        fileUpload->stealSpooledFile();
        fileName = fileUpload->spoolFileName();

        std::fstream file(fileName, file.in);
        panelContent = "";
        for(std::string line; std::getline(file, line);){
            panelContent.append(line + '\n');
        }
        file.close();
        auto buf = yy_scan_string(panelContent.c_str());
        yy_switch_to_buffer(buf);
        int ret = yyparse();
        yy_delete_buffer(buf);

        if(ret){
            updatePanel(ret);
        }

        int level = 0;
        for(size_t i = 0; i < panelContent.length(); ++i){
            if(panelContent[i] == '/' && panelContent[i+1] == '*')
            {
                for(;!(panelContent[i-2] == '*' && panelContent[i-1] == '/'); ++i);
            }else if(panelContent[i] == '/' && panelContent[i+1] == '/')
            {
                for(;panelContent[i] != '\n'; ++i);
            }
            if(panelContent[i] == '{'){
                if(group->checkedId() == 1){
                    int j = i-1;
                    while(isspace(panelContent[j])){
                        --j;
                    }
                    panelContent[i] = ' ';
                    panelContent[j+1] = '{';
                }else if(group->checkedId() == 2){
                    int j = i+1;
                    while(isspace(panelContent[j])){
                        ++j;
                    }
                    panelContent.insert(i,level, '\t');
                    panelContent.insert(i,1, '\n');
                    i = j+1;
                    ++level;
                }
            }else if(panelContent[i] == '}'){
                --level;
            }
        }

        file = std::fstream(fileName, file.out | file.trunc);
        //std::cout << panelContent;
        file << panelContent;
        file.flush();
        file.close();
        fileResource->setContent(std::move(panelContent));
        updatePanel(ret);

        errorAt[0] = 0;
    });

    fileUpload->fileTooLarge().connect([=] {
        bindString("content", "File is too large.", Wt::TextFormat::Plain);
    });
}

void WebTemplate::updatePanel(int result){
    if(result){
        doJavaScript("document.getElementById('codePanel').className = 'panel panel-danger';");
        doJavaScript(std::string("var content = document.getElementById('testContent'); content.innerHTML ='Error at line: ").append(std::to_string(errorAt[0])).append(":").append(std::to_string(errorAt[1])).append("';"));
    }else{
        doJavaScript("var content = document.getElementById('testContent'); content.setAttribute('data-src-status', ''); content.innerHTML=''");
        doJavaScript("document.getElementById('codePanel').className = 'panel panel-success';");
        fileName.erase(0,7);
        doJavaScript("var content = document.getElementById('testContent'); content.setAttribute('data-src', '" + fileName + "'); Prism.highlightElement(content);");

        //submitButton->disable();
        downloadButton->enable();
    }
    panelContent.clear();
    clearTypes();
}