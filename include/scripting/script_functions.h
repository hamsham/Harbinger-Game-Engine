/* 
 * File:   script_functions.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:22 PM
 */

#ifndef SCRIPT_FUNCTIONS_H
#define	SCRIPT_FUNCTIONS_H

namespace harbinger {

//-----------------------------------------------------------------------------
//		Numerical Evaluations
//-----------------------------------------------------------------------------
class c_scriptNumEval : public c_scriptEvaluation {
	private:
		int evalType;
		c_scriptNum *evalNum, *refNum;
		//NOTE: a boolean "outputVal" was inherited from c_scriptFunction
	public:
		enum e_evalType {
			IS_EQUAL,
			IS_NOT_EQUAL,
			IS_GREATER,
			IS_LESS,
			IS_GREATER_OR_EQUAL,
			IS_LESS_OR_EQUAL
		};
		
		c_scriptNumEval();
		c_scriptNumEval( const c_scriptNumEval& evalCopy );
		~c_scriptNumEval();

		const c_scriptNum* getNumToEvaluate();
		const c_scriptNum* getNumToReference();
		void setNumToEvaluate( c_scriptNum* arg );
		void setNumToReference( c_scriptNum* arg );
		
		void setEvalType( e_evalType eval );
		void setEvalType( int eval );
		int getEvalType() const;
		
		void run();
		void tick( float timeElapsed = 0 );
		
		int getScriptType() const {
			return SCRIPT_FUNC_NUM_EVAL | c_scriptEvaluation::getScriptType();
		}
		const char* getScriptTypeStr() const {
			return "SCRIPT_FUNC_NUM_EVAL";
		}
		std::string toString() const;
		bool fromString( const std::string& inStr );
};

//-----------------------------------------------------------------------------
//		Misc. Mathematical Functions
//-----------------------------------------------------------------------------
class c_scriptMiscMath : public c_scriptFunc<float> {
	private:
		int evalType;
		c_scriptNum *evalNum;
		// inherited member "returnVal"
		
	public:
		enum e_math {
			SQRT,
			LOG,	//log base 10
			ABS,
			RND,
			CEIL,
			FLOOR
		};
		
		c_scriptMiscMath();
		c_scriptMiscMath( const c_scriptMiscMath& evalCopy );
		~c_scriptMiscMath();

		const c_scriptNum* getNumToEvaluate();
		void setNumToEvaluate( c_scriptNum* arg );
		
		void setEvalType( e_math eval );
		void setEvalType( int eval );
		int getEvalType() const;
		
		void run();
		void tick( float timeElapsed = 0 );
		
		int getScriptType() const {
			return SCRIPT_FUNC_NUM_MATH | c_scriptFunc::getScriptType();
		}
		const char* getScriptTypeStr() const {
			return "SCRIPT_FUNC_NUM_MATH";
		}
		std::string toString() const;
		bool fromString( const std::string& inStr );
};

//-----------------------------------------------------------------------------
//		Numerical Arithmetic
//-----------------------------------------------------------------------------
class c_scriptArithmetic : public c_scriptFunc<c_scriptNum> {
	private:
		int evalType;
		c_scriptNum *evalNum, *refNum;
		
	public:
		enum e_arithmetic {
			ADD,
			SUB,
			MUL,
			DIV,
			MOD,
			POW,
			EQL
		};
		
		c_scriptArithmetic();
		c_scriptArithmetic( const c_scriptArithmetic& evalCopy );
		~c_scriptArithmetic();

		const c_scriptNum* getNumToEvaluate();
		const c_scriptNum* getNumToReference();
		void setNumToEvaluate( c_scriptNum* arg );
		void setNumToReference( c_scriptNum* arg );
		
		void setEvalType( e_arithmetic eval );
		void setEvalType( int eval );
		int getEvalType() const;
		
		void run();
		void tick( float timeElapsed = 0 );
		
		int getScriptType() const {
			return SCRIPT_FUNC_NUM_ARITHMETIC | c_scriptFunc::getScriptType();
		}
		const char* getScriptTypeStr() const {
			return "SCRIPT_FUNC_NUM_ARITHMETIC";
		}
		std::string toString() const;
		bool fromString( const std::string& inStr );
};

//-----------------------------------------------------------------------------
//		Numerical Trigonometry
//-----------------------------------------------------------------------------
class c_scriptTrigonometry : public c_scriptFunc<float> {
	private:
		int evalType;
		c_scriptNum *evalNum;
		// The inherited member "returnVal" will always be in radians
	public:
		enum e_trigonometry {
			SIN,
			COS,
			TAN,
			CSC,
			SEC,
			COT,
			ARC_SIN,
			ARC_COS,
			ARC_TAN,
			HYP_SIN,
			HYP_COS,
			HYP_TAN
		};
		
		c_scriptTrigonometry();
		c_scriptTrigonometry( const c_scriptTrigonometry& evalCopy );
		~c_scriptTrigonometry();

		const c_scriptNum* getNumToEvaluate();
		void setNumToEvaluate( c_scriptNum* arg );
		
		void setEvalType( e_trigonometry eval );
		void setEvalType( int eval );
		int getEvalType() const;
		
		void run();
		void tick( float timeElapsed = 0 );
		
		int getScriptType() const {
			return SCRIPT_FUNC_NUM_TRIG | c_scriptFunc::getScriptType();
		}
		const char* getScriptTypeStr() const {
			return "SCRIPT_FUNC_NUM_TRIG";
		}
		std::string toString() const;
		bool fromString( const std::string& inStr );
};

} // end harbinger namespace
#endif	/* SCRIPT_FUNCTIONS_H */

