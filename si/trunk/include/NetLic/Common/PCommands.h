#pragma once

class CPCommand{
public:
	virtual ~CPCommand(){}
	virtual void execute() = 0;
};