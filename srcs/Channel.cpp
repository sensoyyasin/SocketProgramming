#include "../headers/Channel.hpp"

Channel::Channel(std::string channelName)
{
	this->channelName = channelName;
}

Channel::~Channel() {}

std::string Channel::getchannelName()
{
	return (this->channelName);
}
