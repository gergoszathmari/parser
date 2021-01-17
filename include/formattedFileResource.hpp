#ifndef FORMATTEDFILERESOURCE_HPP
#define FORMATTEDFILERESOURCE_HPP

#include <Wt/WFileResource.h>
#include <Wt/WAnchor.h>
#include <Wt/WContainerWidget.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include <Wt/WObject.h>

class FormattedResource : public Wt::WResource
{
public:
    FormattedResource()
        : WResource()
    {
	    suggestFileName("formatted.c");
    }

    ~FormattedResource() {
	    beingDeleted();
    }

    void handleRequest(const Wt::Http::Request &request,
                       Wt::Http::Response &response) {
	    response.setMimeType("plain/text");
	    response.out() << m_Content;
    }

    void setContent(std::string&& content){
        m_Content = content;
    }

private:
    std::string m_Content;
};

#endif //FORMATTEDFILERESOURCE_HPP