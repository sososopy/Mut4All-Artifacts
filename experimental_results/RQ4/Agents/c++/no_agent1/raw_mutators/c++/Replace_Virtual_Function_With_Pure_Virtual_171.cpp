//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Virtual_Function_With_Pure_Virtual_171
 */ 
class MutatorFrontendAction_171 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(171)

private:
    class MutatorASTConsumer_171 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_171(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_virtual_function_with_pure_virtual_171.h"

// ========================================================================================================
#define MUT171_OUTPUT 1

void MutatorFrontendAction_171::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("VirtualMethods")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;
        if (MD->isVirtual() && !MD->isPure()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), MD->getSourceRange());
            size_t pos = declaration.find('{');
            if (pos != std::string::npos) {
                declaration.erase(pos);
                declaration.append(" = 0;");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_171::MutatorASTConsumer_171::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual(), unless(isPure())).bind("VirtualMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}