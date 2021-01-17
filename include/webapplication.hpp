#ifndef WEBAPPLICATION_HPP
#define WEBAPPLICATION_HPP

#include <Wt/WApplication.h>
#include <Wt/WTemplate.h>
#include <Wt/WEnvironment.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WPushButton.h>
#include <Wt/WFileUpload.h>
#include <Wt/WButtonGroup.h>
#include <Wt/WRadioButton.h>

#include "formattedFileResource.hpp"

class WebApplication : public Wt::WApplication
{
public:
    WebApplication(const Wt::WEnvironment& env);
};

class WebTemplate : public Wt::WTemplate
{
public:
    WebTemplate();
private:
    Wt::WPushButton* submitButton;
    Wt::WPushButton* downloadButton;
    Wt::WFileUpload* fileUpload;
    std::shared_ptr<FormattedResource> fileResource;

    std::string fileName;
    std::string panelContent;

    void updatePanel(int result);
};

#endif //WEBAPPLICATION_HPP