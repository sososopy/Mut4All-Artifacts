//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"

/**
 * Modify_Include_Directive_372
 */ 
class MutatorFrontendAction_372 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(372)

private:
    class MutatorASTConsumer_372 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_372(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_include_directive_372.h"

// ========================================================================================================
#define MUT372_OUTPUT 1

void MutatorFrontendAction_372::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *ID = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("IncludeDirective")) {
      if (!ID || !Result.SourceManager->isWrittenInMainFile(ID->getBeginLoc()))
        return;
      auto includeText = ID->getSourceRange().getBegin();
      std::string mutatedInclude = "#include <nonexistent_header>";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ID->getSourceRange()), mutatedInclude);
    }
}
  
void MutatorFrontendAction_372::MutatorASTConsumer_372::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::includeDirective().bind("IncludeDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}