//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_461
 */ 
class MutatorFrontendAction_461 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(461)

private:
    class MutatorASTConsumer_461 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_461(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_virtual_base_class_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;

        std::string baseClassName = BaseClass->getNameAsString();
        if (baseClassName.empty() || visitedClasses.count(baseClassName))
            return;

        visitedClasses.insert(baseClassName);
        std::string virtualBaseClassDecl = "class VirtualBase_" + baseClassName + " : virtual public " + baseClassName + " {};";
        Rewrite.InsertTextAfter(BaseClass->getEndLoc(), "\n/*mut461*/" + virtualBaseClassDecl + "\n");
    }
}

void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseClassMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseClassMatcher, &callback);
    matchFinder.matchAST(Context);
}