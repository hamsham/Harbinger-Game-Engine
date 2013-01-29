/* 
 * File:   script_functions.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:22 PM
 */

#ifndef __HGE_SCRIPT_FUNCTIONS_H__
#define	__HGE_SCRIPT_FUNCTIONS_H__

#include <fstream>
#include "script.h"
#include "script_base.h"
#include "script_variables.h"

//-----------------------------------------------------------------------------
//		Evaluation Function Base Class
//		Abstract
//-----------------------------------------------------------------------------
class HGE_API c_scriptEvaluation : virtual public c_scriptFunc< c_scriptBool > {
	
	protected:
		int evalType;
		const c_scriptVarBase *evalVar, *compVar;
		//The object to evaluate, the object to compare it to
	
	public:
		c_scriptEvaluation();
		c_scriptEvaluation( const c_scriptEvaluation& evalCopy );
		virtual ~c_scriptEvaluation() = 0;
			
		virtual int getScriptSubType() const {
			return SCRIPT_FUNC_EVAL; // invalid
		}
		
		void read ( std::ifstream&, scriptMap_t& );
		void write ( std::ofstream& ) const;
		
		int getEvalType() const;
		virtual void setEvalType( int eval = 0 );
		
		virtual const c_scriptVarBase* getVarToEvaluate() const;
		virtual void setVarToEvaluate( const c_scriptVarBase* inVar );
		
		virtual const c_scriptVarBase* getVarToCompare() const;
		virtual void setVarToCompare( const c_scriptVarBase* inVar );
};

//-----------------------------------------------------------------------------
//		Math Function Base Class
//		Abstract
//-----------------------------------------------------------------------------
class HGE_API c_scriptNumeric : virtual public c_scriptFunc< c_scriptFloat > {
	
	protected:
		int evalType;
		const c_scriptNum *evalVar, *compVar;
		//The object to evaluate, the object to compare to
	
	public:
		c_scriptNumeric();
		c_scriptNumeric( const c_scriptNumeric& numFunc );
		virtual ~c_scriptNumeric() = 0;
			
		virtual int getScriptSubType() const {
			return SCRIPT_FUNC_NUMERICAL; // invalid
		}
		
		void read ( std::ifstream&, scriptMap_t& );
		void write ( std::ofstream& ) const;
		
		int getEvalType() const;
		virtual void setEvalType( int eval = 0 );
		
		virtual const c_scriptNum* getVarToEvaluate() const;
		virtual void setVarToEvaluate( const c_scriptNum* inVar );
		
		virtual const c_scriptNum* getVarToCompare() const;
		virtual void setVarToCompare( const c_scriptNum* inVar );
};

//-----------------------------------------------------------------------------
//		Numerical Evaluations
//-----------------------------------------------------------------------------
class HGE_API c_scriptNumEval : public c_scriptEvaluation {
		
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
		
		void setEvalType( int eval );
		
		const c_scriptNum* getVarToEvaluate() const;
		void setVarToEvaluate( const c_scriptNum* inVar );
		
		const c_scriptNum* getVarToCompare() const;
		void setVarToCompare( const c_scriptNum* inVar );
		
		void run();
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_EVAL;
		}
};

//-----------------------------------------------------------------------------
//		Misc. Mathematical Functions
//-----------------------------------------------------------------------------
class HGE_API c_scriptMiscMath : public c_scriptNumeric {
		
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
		
		void setEvalType( int eval );
		
		void run();
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_MISC;
		}
};

//-----------------------------------------------------------------------------
//		Numerical Arithmetic
//-----------------------------------------------------------------------------
class HGE_API c_scriptArithmetic : public c_scriptNumeric {
		
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
		
		void setEvalType( int eval );
		
		void run();
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_ARITH;
		}
};

//-----------------------------------------------------------------------------
//		Numerical Trigonometry
//-----------------------------------------------------------------------------
class HGE_API c_scriptTrigonometry : public c_scriptNumeric {
	
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
		
		void setEvalType( int eval );
		
		void run();
		
		int getScriptSubType() const {
			return SCRIPT_FUNC_NUM_TRIG;
		}
};

#endif	/* __HGE_SCRIPT_FUNCTIONS_H__ */
