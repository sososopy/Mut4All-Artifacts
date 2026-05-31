//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::set<std::string> visitedClasses;
    };
};

//source file
#include "../include/insert_virtual_base_class_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        std::string className = CL->getNameAsString();
        if (visitedClasses.find(className) != visitedClasses.end())
            return;

        visitedClasses.insert(className);
        std::string virtualBaseClass = "class VirtualBase {};";

        // Perform mutation on the source code text by applying string replacement
        std::string insertion = "class Derived_" + className + " : virtual public " + className + ", virtual public VirtualBase {};\n";
        insertion = "/*mut398*/" + insertion;

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfter(CL->getEndLoc(), insertion);
    }
}

void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}