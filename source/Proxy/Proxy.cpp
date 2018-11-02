#include "Proxy/Proxy.hpp"
#include "Proxy/ProxyImpl.hpp"

namespace simplelogger::simpleproxy {

    Proxy::Proxy(simplelogger::simplemessage::Message message) 
        :
		m_impl(std::make_unique<ProxyImpl>(std::move(message)))
    {

    }


    Proxy::~Proxy() {
		m_impl->flush();  
    }	


	Proxy&& operator<<(Proxy&& proxy, std::string_view view) {
		if(view.size() <= 0 )
			/*TODO: Possible error, log inside*/;
		proxy.m_impl->write(view.data());
		return std::move(proxy);
	}


	simpleproxy::Proxy log_impl(
		simplemessage::MessageType message_type,
		std::string_view file,
		std::string_view func,
		const int line
	)
	{
		return { { {}, message_type, file.data(), func.data(), line , {}, std::chrono::steady_clock::now() } };
	}
}