//source file
#include "../include/Lambda_Inheritance_Explicit_Object_Parameter_Reference_Capture_Derived_Constructor_492.h"

// ========================================================================================================
#define MUT492_OUTPUT 1

void MutatorFrontendAction_492::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        //Record candidate functions that could be modified to return lambda
        if (FD->hasBody() && FD->getReturnType()->isVoidType()) {
            candidateFunctions.push_back(FD);
        }
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
            return;
        if ((!RD->isStruct() && !RD->isClass()) || RD->isLambda())
            return;
        if (!RD->isCompleteDefinition())
            return;
        //Record candidate classes that could be used as base class
        candidateClasses.push_back(RD);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;
        //Record global variable names for capture
        if (VD->hasGlobalStorage() && VD->getType()->isIntegerType()) {
            globalVarNames.push_back(VD->getNameAsString());
        }
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
            return;
        //Get the source code text of target node
        auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        if (candidateFunctions.empty() || candidateClasses.empty() || globalVarNames.empty())
            return;
        
        size_t funcIdx = getrandom::getRandomIndex(candidateFunctions.size() - 1);
        size_t classIdx = getrandom::getRandomIndex(candidateClasses.size() - 1);
        size_t varIdx = getrandom::getRandomIndex(globalVarNames.size() - 1);
        
        const clang::FunctionDecl* targetFunc = candidateFunctions[funcIdx];
        const clang::CXXRecordDecl* targetClass = candidateClasses[classIdx];
        std::string captureVar = globalVarNames[varIdx];
        
        //Modify function to return lambda with explicit object parameter and reference capture
        std::string newFuncDecl = "auto " + targetFunc->getNameAsString() + "(int& arg) {\n    return [&](this auto self) {\n        arg;\n    };\n}";
        
        //Create derived class with constructor initializing base with lambda
        std::string derivedClass = "using BaseLambda = decltype(" + targetFunc->getNameAsString() + "(" + captureVar + "));\nstruct DerivedLambda_492 : BaseLambda {\n    DerivedLambda_492() : BaseLambda(" + targetFunc->getNameAsString() + "(" + captureVar + ")) {}\n};";
        
        //Insert instantiation and invocation in main function scope
        std::string instantiation = "DerivedLambda_492 obj_492;\nobj_492();";
        
        //Combine mutations
        std::string mutation = "\n/*mut492*/" + newFuncDecl + "\n" + derivedClass + "\n" + instantiation;
        
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(MT->getEndLoc(), mutation);
    }
}
  
void MutatorFrontendAction_492::MutatorASTConsumer_492::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("CXXRecordDecl");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    StatementMatcher lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}