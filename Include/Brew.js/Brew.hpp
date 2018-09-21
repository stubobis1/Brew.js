
/**

    @file Brew.hpp
    @brief Main header of Brew.js API
    @author XorTroll
    @copyright Brew.js project

*/

/*

	///////////////////////////////////////////////////////////////////////////////////////////

	         -----  Brew.js - C++ userland JavaScript API for Nintendo homebrew  -----         
	
	///////////////////////////////////////////////////////////////////////////////////////////

	Using duktape engine (https://duktape.org), base of the JS API.

	FAQ:
	
	- Which ECMAScript version if supported?
	  According to duktape's documentation, duktape's JavaScript runs like ES5 (although some API of ES5 is not supported).
	  It also has some ES6 features. read more about this on duktape's GitHub: https://github.com/svaarala/duktape/blob/master/README.md

	- Which Nintendo OSs are supported?
	  Currently Brew.js runs under Nintendo Switch, Nintendo 3DS and Nintendo DS. More platforms are planned, like Wii or GBA,
	  but we plan to fully support these 3 current OSs before starting with a new one.

	- Do I have to add something extra to embed Brew.js on my project?
	  Theoretically no. Brew.js automatically should detect the current OS we're compiling for, and it'll error if it fails detecting it.
	  Everything should be automatically done.

	- Where can I get more info or docs?
	  You have the documentations and the repository here: https://XorTroll.github.io/Brew.js
	
*/

#pragma once
#include "API.hpp"

#include "Modules/Node/FS.hpp"
#include "Modules/Node/OS.hpp"
#include "Modules/Node/Path.hpp"

#if __curOS == 0
	#include "Modules/NX/NX.hpp"
	#include "Modules/NX/Gfx.hpp"
	#include "Modules/NX/Input.hpp"
	#include "Modules/NX/PegaSwitch.hpp"
#elif __curOS == 1
	#include "Modules/CTR/CTR.hpp"
	#include "Modules/CTR/Input.hpp"
	#include "Modules/CTR/SF2D.hpp"
#elif __curOS == 2
	#include "Modules/NTR/Gfx.hpp"
	#include "Modules/NTR/Input.hpp"
	#include "Modules/NTR/NTR.hpp"
#endif

namespace Brew
{
	/// The possible operating systems on which Brew.js can run.
	enum OS
	{
		None = -1,  ///< No module found (Brew.js should error anyway)
		NX = 0,     ///< Nintendo Switch
		CTR = 1,    ///< Nintendo 3DS
		NTR = 2,    ///< Nintendo DS
	};

	/// Represents a JavaScript project which can be executed via an \ref Environment.
	struct Project
	{
		string Name;         ///< The name of the project.
		string Description;  ///< The description of the project.
		string Version;      ///< The version of the project.
		string EntrySource;  ///< The entrypoint source file of the project.
		string Directory;    ///< The directory where the project is located.
	};

	/// Represents the result for evaluating a JavaScript source, file or project.
	class ExecutionResult
	{
		public:

			/**
				@brief Creates a \ref ExecutionResult from a type, a result string and an error code.
				@param Type The returning type of the execution.
				@param Result The result as a string.
				@param Code The result code of the execution.
				@note This results should only be obtained from evaluation functions from \ref Environment.
			*/
			ExecutionResult(API::Type Type, string Result, int Code);
			
			/**
				@brief Gets the result of the execution as a string.
				@return The result string.
			*/
			string resultString();
			
			/**
				@brief Returns whether the execution went ok or had any errors.
				@return Whether the result was successful or not.
			*/
			bool isOK();

			/**
				@brief Gets the duktape error code from the execution.
				@return The error code.
			*/
			int nativeCode();

			/**
				@brief Gets the original type of the execution result.
				@return The type of the result.
			*/
			API::Type getType();

			/**
				@brief Gets whether the result was undefined. (result is usually undefined if everything went OK)
				@return Whether the result was undefined.
			*/
			bool isUndefined();
		
		private:
			API::Type type;
			string sres;
			int res;
	};
	
	/// Possible Brew.js errors related to evaluating files or projects.
	enum Error
	{
		FileNotFound,          ///< The selected file was not found.
		ProjectsPathNotFound,  ///< The folder containing projects was not found.
		ProjectFolderNotFound, ///< The folder containing the project files was not found.
		EntrySourceNotFound,   ///< The entrypoint source file was not found.
		PackageJsonNotFound,   ///< Project file "package.json" file was not found, so it's not a valid project.
	};

	/// Class holding a Brew.js environment, which is a wrapper for a context with some evaluation functions.
	class Environment
	{
		public:

			/**
				@brief Creates a \ref Environment with a context to use.
				@param Context The context to use.
			*/
			Environment(API::NativeContext Context);
			~Environment();

			void reloadContext(API::NativeContext NewContext);
			
			/**
				@brief Gets a vector list of projects from the given directory.
				@param ProjectsDirectory The directory to search projects on.
			*/
			vector<Project> getProjectList(string ProjectsDirectory);
			
			/**
				@brief Evaluates source code and returns its execution result.
				@param Source JavaScript source code to evaluate.
				@return The result of the evaluation.
			*/
			ExecutionResult evaluateSourceCode(string Source);
			
			/**
				@brief Evaluates source code from a file and returns its execution result.
				@param Path File to evaluate its source code.
				@return The result of the evaluation.
			*/
			ExecutionResult evaluateFile(string Path);
			
			/**
				@brief Evaluates source code from a project and returns its execution result.
				@param Path Project to evaluate its source files.
				@return The result of the evaluation.
			*/
			ExecutionResult evaluateProject(Project Project);
			
			/**
				@brief Gets the error string from the current error \ref Result.
				@return The error as a string.
			*/
			string getError();

			/**
				@brief Gets the \ref latest Result of the \ref Environment.
				@return The result of the \ref Environment.
			*/
			Result getResult();
		private:
			API::NativeContext ctx;
			Result res;
	};

	/**
		@brief Gets the current operating system (Nintendo platform).
		@return The current \ref OS.
	*/
	OS currentOS();
	
	/**
		@brief Creates a libnx Result from a Brew.js error type.
		@return The result from that error type.
	*/
	Result createError(Error ErrorType);
}