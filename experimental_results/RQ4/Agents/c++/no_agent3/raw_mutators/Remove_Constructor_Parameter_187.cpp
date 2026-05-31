//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Constructor_Parameter_187
 */ 
class MutatorFrontendAction_187 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(187)

private:
    class MutatorASTConsumer_187 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_187(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Remove_Constructor_Parameter_187.h"

// ========================================================================================================
#define MUT187_OUTPUT 1

void MutatorFrontendAction_187::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CtorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor")) {
        if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(CtorDecl->getLocation()))
            return;

        if (CtorDecl->getNumParams() < 2)
            return;

        // Get the source code text of target node
        auto constructorSource = stringutils::rangetoStr(*(Result.SourceManager), CtorDecl->getSourceRange());

        // Perform mutation: remove the second parameter and its initialization logic
        size_t paramStart = constructorSource.find(',');
        size_t paramEnd = constructorSource.find(')', paramStart);
        if (paramStart != std::string::npos && paramEnd != std::string::npos) {
            constructorSource.erase(paramStart, paramEnd - paramStart);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CtorDecl->getSourceRange()), constructorSource);
    }
}

void MutatorFrontendAction_187::MutatorASTConsumer_187::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(ofClass(cxxRecordDecl())).bind("constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}