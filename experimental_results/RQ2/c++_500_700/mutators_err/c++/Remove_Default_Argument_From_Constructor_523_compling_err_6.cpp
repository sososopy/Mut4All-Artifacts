//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

/**
 * remove_default_argument_from_constructor_523
 */ 
class MutatorFrontendAction_523 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(523)

private:
    class MutatorASTConsumer_523 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_523(Rewriter &R) : TheRewriter(R) {}
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
#include "MutatorFrontendAction_523.h"

using namespace clang;
using namespace clang::ast_matchers;

#define MUT523_OUTPUT 1

void MutatorFrontendAction_523::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CtorDecl = Result.Nodes.getNodeAs<CXXConstructorDecl>("ConstructorWithDefaultArg")) {
        if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(CtorDecl->getLocation()))
            return;

        for (auto *Param : CtorDecl->parameters()) {
            if (Param->hasDefaultArg()) {
                auto paramRange = Param->getSourceRange();
                auto paramText = Lexer::getSourceText(CharSourceRange::getTokenRange(paramRange), *Result.SourceManager, Result.Context->getLangOpts());

                size_t equalPos = paramText.find('=');
                if (equalPos != std::string::npos) {
                    paramText = paramText.substr(0, equalPos);
                }

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), paramText);
            }
        }
    }
}
  
void MutatorFrontendAction_523::MutatorASTConsumer_523::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArg())).bind("ConstructorWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}