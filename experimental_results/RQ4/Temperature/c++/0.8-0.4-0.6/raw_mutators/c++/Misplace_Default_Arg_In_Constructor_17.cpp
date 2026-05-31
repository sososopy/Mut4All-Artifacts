//header file
#pragma once
#include "Mutator_base.h"

/**
 * misplace_default_arg_in_constructor_17
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(17)

private:
    class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplace_default_arg_in_constructor_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        for (auto *param : CD->parameters()) {
            if (param->hasDefaultArg()) {
                auto defaultArgRange = param->getDefaultArgRange();
                auto defaultArgText = Lexer::getSourceText(CharSourceRange::getTokenRange(defaultArgRange), 
                                                           *Result.SourceManager, 
                                                           Result.Context->getLangOpts());
                if (defaultArgText.empty())
                    continue;

                std::string modifiedText = defaultArgText.substr(0, defaultArgText.find('=') + 1);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(defaultArgRange), modifiedText);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArg())).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}