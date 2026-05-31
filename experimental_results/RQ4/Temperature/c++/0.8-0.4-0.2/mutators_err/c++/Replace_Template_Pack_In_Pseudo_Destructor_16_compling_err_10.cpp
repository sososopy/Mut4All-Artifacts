//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

/**
 * Replace_Template_Pack_In_Pseudo_Destructor_16
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
#include "../include/replace_template_pack_in_pseudo_destructor_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PD = Result.Nodes.getNodeAs<clang::CXXPseudoDestructorExpr>("PseudoDestructor")) {
      if (!PD || !Result.SourceManager->isWrittenInMainFile(
                     PD->getBeginLoc()))
        return;

      auto sourceRange = PD->getSourceRange();
      auto sourceText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(sourceRange), 
                                             *Result.SourceManager, 
                                             Result.Context->getLangOpts());

      std::string mutatedText = sourceText.str();
      size_t pos = mutatedText.find("~T...");
      if (pos != std::string::npos) {
          mutatedText.replace(pos, 5, "~int[incomplete]");
      }

      Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::cxxPseudoDestructorExpr().bind("PseudoDestructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}