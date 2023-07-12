//#ifndef _DB
//#define _DB
//
//
////https://www.codeproject.com/articles/993067/calling-java-from-cplusplus-with-jni
//
//#include <jni.h>
//#include <iostream>
//#include <vector>
//#include "emoa.hpp"
//
//
//namespace rzq {
//	namespace search {
//		class Frontier3d : public Frontier {
//		private:
//			static JavaVM* jvm;                      // Pointer to the JVM (Java Virtual Machine)
//			static JNIEnv* env;                      // Pointer to native interface
//			jobject jmgr;
//			jclass Solution;
//			jmethodID SolutionInit;
//			jmethodID jcheck;
//			jmethodID jupdate;
//
//		public:
//			Frontier3d();
//			virtual ~Frontier3d();
//			virtual bool Check(basic::CostVector g) override;
//			virtual void Update(Label l) override;
//			virtual basic::CostVector _p(basic::CostVector v);
//			TOATree _tree;
//
//			jobject costVecToSolution(basic::CostVector costs);
//			std::vector<float> solutionToCostVec(jobject solution);
//		};
//
//		class FrontierKd : public Frontier3d
//		{
//		public:
//			FrontierKd();
//			virtual ~FrontierKd();
//			virtual bool Check(basic::CostVector g) override;
//			virtual void Update(Label l) override;
//			// protected:
//			  // project the 3d vector to 2d vector by removing the first component.
//			  // virtual basic::CostVector _p(basic::CostVector v);
//			KOATree _tree; // TODO use polymorphism, change to pointer...
//			int _mode = 0;
//		};
//	}
//}
//
//
//#endif // _DB
