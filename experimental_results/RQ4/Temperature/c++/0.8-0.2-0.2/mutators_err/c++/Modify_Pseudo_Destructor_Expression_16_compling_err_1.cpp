//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_pseudo_destructor_expression_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_pseudo_destructor_expression_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXPseudoDestructorExpr>("PseudoDestructor")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto startLoc = MT->getBeginLoc();
      auto endLoc = MT->getEndLoc();
      auto range = CharSourceRange::getTokenRange(startLoc, endLoc);

      std::string originalExpr = Lexer::getSourceText(range, *Result.SourceManager, LangOptions(), 0);
      
      // Modify the pseudo destructor expression to use an unrelated type 'int'
      std::string mutatedExpr = originalExpr;
      size_t pos = mutatedExpr.find("~");
      if (pos != std::string::npos) {
        mutatedExpr.replace(pos + 1, mutatedExpr.find("(", pos) - pos - 1, "int");
      }

      Rewrite.ReplaceText(range, mutatedExpr);
    }
}

void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxPseudoDestructorExpr().bind("PseudoDestructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}