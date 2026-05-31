//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Conflicting_Overloads_454
 */ 

class MutatorFrontendAction_454 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(454)
private:
    class MutatorASTConsumer_454 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_454(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> visitedClasses;
    };
};

//source file
#include "../include/insert_conflicting_overloads_454.h"

// ========================================================================================================
#define MUT454_OUTPUT 1

void MutatorFrontendAction_454::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if (!CL->isCompleteDefinition() || CL->isLambda() || visitedClasses.count(CL))
            return;

        visitedClasses.insert(CL);

        std::string className = CL->getNameAsString();
        if (className.empty())
            return;

        std::string conflictingOverload = "int " + className + "(int x) { return x; }\n";
        conflictingOverload += "double " + className + "(int x) { return x * 2.0; }\n";

        std::string insertionText = "\n/*mut454*/" + conflictingOverload;
        Rewrite.InsertTextAfterToken(CL->getEndLoc(), insertionText);
    }
}

void MutatorFrontendAction_454::MutatorASTConsumer_454::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}