#pragma once

#ifndef SIMPLE_LOGGER_DLL_HEADER
#define SIMPLE_LOGGER_DLL_HEADER

#ifdef simplelogger_EXPORTS	
#define LOGGER_API __declspec(dllexport)
#define EXPIMP_TEMPLATE     
#else
#define LOGGER_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#endif