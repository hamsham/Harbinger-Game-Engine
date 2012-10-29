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
	friend class c_scriptManager;
	friend std::ostream& operator << ( std::ostream&, const c_scriptNumEval& );
	friend std::istream& operator >> ( std::istream&, c_scriptNumEval& );
	
	protected:
		const c_scriptNum *varToEval, *varToRef;
		//NOTE: a boolean "returnVal" was inherited from c_scriptFunction
		
	public:
		enum e_evalType {
			IS_EQUAL,
			IS_NOT_EQUAL,
			IS_GREATER,
			IS_LESS,
			IS_GREATER_OR_EQUAL,
			IS_LESS_OR_EQUAL,
			FUNC_NUM_INVALID
		};
		
		c_scriptNumEval();
		c_scriptNumEval( const c_scriptNumEval& evalCopy );
		~c_scriptNumEval();
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_EVAL;
		}
		
		const c_scriptNum* getVarToEvaluate() const;
		void attachVarToEvaluate( const c_scriptNum* inVar );
		void detachVarToEvaluate();
		
		const c_scriptNum* getVarToReference() const;
		void attachVarToReference( const c_scriptNum* inVar );
		void detachVarToReference();
		
		void setEvalType( int eval );
		
		void run();
		void tick( float timeElapsed = 0 );
};

//-----------------------------------------------------------------------------
//		Misc. Mathematical Functions
//-----------------------------------------------------------------------------
class c_scriptMiscMath : public c_scriptFunc<float> {
	friend class c_scriptManager;
	friend std::ostream& operator << ( std::ostream&, const c_scriptMiscMath& );
	friend std::istream& operator >> ( std::istream&, c_scriptMiscMath& );
	
	private:
		int evalType;
		c_scriptNum *evalNum;
		// inherited member "returnVal"
		
	public:
		enum e_math {
			SQRT,
			LOG,	//log base 10
			ABS,
			RND, //basically, floor() + 0.5f
			CEIL,
			FLOOR,
			FUNC_MATH_INVALID
		};
		
		c_scriptMiscMath();
		c_scriptMiscMath( const c_scriptMiscMath& evalCopy );
		~c_scriptMiscMath();

		const c_scriptNum* getNumToEvaluate();
		void setNumToEvaluate( c_scriptNum* arg );
		
		void setEvalType( int eval );
		
		void run();
		void tick( float timeElapsed = 0 );
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_MISC;
		}
};

//-----------------------------------------------------------------------------
//		Numerical Arithmetic
//-----------------------------------------------------------------------------
class c_scriptArithmetic : public c_scriptFunc<c_scriptNum> {
	friend class c_scriptManager;
	friend std::ostream& operator << ( std::ostream&, const c_scriptArithmetic& );
	friend std::istream& operator >> ( std::istream&, c_scriptArithmetic& );
	
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
			EQL,
			FUNC_ARITH_INVALID
		};
		
		c_scriptArithmetic();
		c_scriptArithmetic( const c_scriptArithmetic& evalCopy );
		~c_scriptArithmetic();

		const c_scriptNum* getNumToEvaluate();
		const c_scriptNum* getNumToReference();
		void setNumToEvaluate( c_scriptNum* arg );
		void setNumToReference( c_scriptNum* arg );
		
		void setEvalType( int eval );
		
		void run();
		void tick( float timeElapsed = 0 );
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_ARITH;
		}
};

//-----------------------------------------------------------------------------
//		Numerical Trigonometry
//-----------------------------------------------------------------------------
class c_scriptTrigonometry : public c_scriptFunc<float> {
	friend class c_scriptManager;
	friend std::ostream& operator << ( std::ostream&, const c_scriptTrigonometry& );
	friend std::istream& operator >> ( std::istream&, c_scriptTrigonometry& );
	
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
			HYP_TAN,
			FUNC_TRIG_INVALID
		};
		
		c_scriptTrigonometry();
		c_scriptTrigonometry( const c_scriptTrigonometry& evalCopy );
		~c_scriptTrigonometry();

		const c_scriptNum* getNumToEvaluate();
		void setNumToEvaluate( c_scriptNum* arg );
		
		void setEvalType( int eval );
		
		void run();
		void tick( float timeElapsed = 0 );
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_TRIG;
		}
};

} // end harbinger namespace
#endif	/* SCRIPT_FUNCTIONS_H */

