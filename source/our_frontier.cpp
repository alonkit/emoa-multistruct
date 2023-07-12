//https://www.codeproject.com/articles/993067/calling-java-from-cplusplus-with-jni

#include "emoa.hpp"

JavaVM* rzq::search::Frontier3d::jvm = nullptr;
JNIEnv* rzq::search::Frontier3d::env = nullptr;

rzq::search::Frontier3d::~Frontier3d() {
    return;
};


rzq::search::Frontier3d::Frontier3d()
{
    if (jvm == nullptr) {
        JavaVMInitArgs vm_args;                        // Initialization arguments
        JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
        options[0].optionString = (char*)"-Djava.class.path=C:\\Users\\alonk\\technion\\a-project\\multiobjective_data_structures";   // where to find java .class
        vm_args.version = JNI_VERSION_1_6;             // minimum Java version
        vm_args.nOptions = 1;                          // number of options
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
        //=============== load and initialize Java VM and JNI interface =============
        jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);  // YES !!
        delete options;    // we then no longer need the initialisation options. 
        if (rc != JNI_OK) {
            // TO DO: error processing... 
            exit(EXIT_FAILURE);
        }

        //=============== Display JVM version =======================================
        std::cout << "JVM load succeeded: Version ";
        jint ver = env->GetVersion();
        std::cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << std::endl;  
    }


    Solution = env->FindClass("multiobjective_data_structures/implementations/tests/ProxySolution");
    SolutionInit = env->GetMethodID(Solution, "<init>", "([D)V");
    jfitness = env->GetMethodID(Solution, "getFitness", "(I)D");

    auto mgrClass = env->FindClass("multiobjective_data_structures/implementations/MTQuadTree1");
    auto mgrInit = env->GetMethodID(mgrClass, "<init>", "(I)V");
    jmgr = env->NewObject(mgrClass, mgrInit, (jint)3);
    jcheck = env->GetMethodID(mgrClass, "weaklyDominates", "(Lmultiobjective_data_structures/Solution;)Z");
    jupdate = env->GetMethodID(mgrClass, "add", "(Lmultiobjective_data_structures/Solution;)Z");
    jsize = env->GetMethodID(mgrClass, "size", "()I");
    

    int x = 5;

}

jobject rzq::search::Frontier3d::costVecToSolution(basic::CostVector costs)
{
    std::vector<double> d_arr(costs.begin(), costs.end());
    jdoubleArray jarr = env->NewDoubleArray(costs.size());
    env->SetDoubleArrayRegion(jarr, 0, costs.size(), (jdouble*)&(d_arr[0]));
    jobject res = env->NewObject(Solution, SolutionInit, jarr);
    /*std::cout 
        << (double)env->CallDoubleMethod(res, jfitness, (jint)0)
        << (double)env->CallDoubleMethod(res, jfitness, (jint)1) 
        << (double)env->CallDoubleMethod(res, jfitness, (jint)2) 
        << std::endl;*/
    return res;
}

std::vector<float> rzq::search::Frontier3d::solutionToCostVec(jobject solution)
{
    return std::vector<float>();
}

bool rzq::search::Frontier3d::Check(basic::CostVector g) {
    std::cout << "--check " << (int)env->CallIntMethod(jmgr,jsize)<< g;
    jobject solution = costVecToSolution(g);

    
    jboolean res = env->CallBooleanMethod(jmgr, jcheck, solution);
    std::cout << " res: " << (bool)res << std::endl;
    return (bool)res;
}

void rzq::search::Frontier3d::Update(Label l) {
    std::cout << "--update " << l.g<< std::endl;
    label_ids.insert(l.id);
    auto pg = l.g;
    jobject solution = costVecToSolution(pg);
    jboolean res = env->CallBooleanMethod(jmgr, jupdate, solution);
    //std::cout << (bool)res << std::endl;
}

rzq::basic::CostVector rzq::search::Frontier3d::_p(basic::CostVector v) {
    // TODO, remove this, use _proj().
    basic::CostVector out;
    for (size_t i = 1; i < v.size(); i++) {
        out.push_back(v[i]);
    }
    return out;
};
