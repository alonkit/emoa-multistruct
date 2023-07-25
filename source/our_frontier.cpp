//https://www.codeproject.com/articles/993067/calling-java-from-cplusplus-with-jni
//https://www.codeproject.com/Tips/1129615/JNI-Signature-for-Java-Method

#include "emoa.hpp"

JavaVM* rzq::search::Frontier3d::jvm = nullptr;
JNIEnv* rzq::search::Frontier3d::env = nullptr;

jclass rzq::search::Frontier3d::Solution = nullptr;
jclass rzq::search::Frontier3d::Mgr = nullptr;

jmethodID rzq::search::Frontier3d::MgrInit = nullptr;
jmethodID rzq::search::Frontier3d::SolutionInit = nullptr;
jmethodID rzq::search::Frontier3d::jcheck = nullptr;
jmethodID rzq::search::Frontier3d::jupdate = nullptr;
jmethodID rzq::search::Frontier3d::jsize = nullptr;
jmethodID rzq::search::Frontier3d::jfitness = nullptr;


rzq::search::Frontier3d::~Frontier3d() {
    return;
};

enum Types {MTQuadTree1, LinearListManager, BSPTreeArchiveManager, NDTree};

rzq::search::Frontier3d::Frontier3d()
{

    // change this :
    Types type = NDTree;


    if (jvm == nullptr) {
        JavaVMInitArgs vm_args;                        // Initialization arguments
        JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
        options[0].optionString = (char*)"-Djava.class.path=C:\\Users\\alonk\\technion\\a-project\\emoa-multistruct";   // where to find java .class
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

        Solution = env->FindClass("multiobjective_data_structures/implementations/tests/ProxySolution");
        SolutionInit = env->GetMethodID(Solution, "<init>", "([D)V");
        jfitness = env->GetMethodID(Solution, "getFitness", "(I)D");


        switch (type)
        {
        case MTQuadTree1:{
                Mgr = env->FindClass("multiobjective_data_structures/implementations/MTQuadTree1");
                MgrInit = env->GetMethodID(Mgr, "<init>", "(I)V");
                break;
            }
        case LinearListManager:{
            Mgr = env->FindClass("multiobjective_data_structures/implementations/LinearListManager");
            MgrInit = env->GetMethodID(Mgr, "<init>", "(JIZ)V");
            break;
        }
        case BSPTreeArchiveManager: {
            Mgr = env->FindClass("multiobjective_data_structures/implementations/BSPTreeArchiveManager");
            MgrInit = env->GetMethodID(Mgr, "<init>", "(I)V");
            break;
        }
        case NDTree: {
            Mgr = env->FindClass("multiobjective_data_structures/implementations/NDTree");
            MgrInit = env->GetMethodID(Mgr, "<init>", "(I)V");
            break;
        }
        default:
            break;
        }
        
        
        jcheck = env->GetMethodID(Mgr, "weaklyDominates", "(Lmultiobjective_data_structures/Solution;)Z");
        jupdate = env->GetMethodID(Mgr, "add", "(Lmultiobjective_data_structures/Solution;)Z");
        jsize = env->GetMethodID(Mgr, "size", "()I");
    

        if (jsize == nullptr) exit(1);
        if (jcheck == nullptr) exit(2);
        if (jupdate == nullptr) exit(3);
        if (jfitness == nullptr) exit(4);
        if (SolutionInit == nullptr) exit(5);
        if (MgrInit== nullptr) exit(6);
    }


    
    
    switch (type)
    {
    case MTQuadTree1: {
        // change 3rd value to amount of weights per edge
        jmgr = env->NewObject(Mgr, MgrInit, (jint)3);
        break;
    }
    case LinearListManager: {
        // change 4rd value to amount of weights per edge
        jmgr = env->NewObject(Mgr, MgrInit, (jlong)42, (jint)2, (jboolean)true);
        break;
    }
    case BSPTreeArchiveManager: {
        // change 3rd value to amount of weights per edge
        jmgr = env->NewObject(Mgr, MgrInit, (jint)2);
        break;
    }
    case NDTree: {
        // change 3rd value to amount of weights per edge
        jmgr = env->NewObject(Mgr, MgrInit, (jint)2);
        break;
    }

    default:
        break;
    }


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
    //std::cout << "--check " << (int)env->CallIntMethod(jmgr,jsize)<< g;
    jobject solution = costVecToSolution(g);

    
    jboolean res = env->CallBooleanMethod(jmgr, jcheck, solution);
    //std::cout << " res: " << (bool)res << std::endl;
    return (bool)res;
}

void rzq::search::Frontier3d::Update(Label l) {
    //std::cout << "--update " << l.g<< std::endl;
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
