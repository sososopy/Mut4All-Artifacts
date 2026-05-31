//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ExprCXX.h"
#include "clang/Basic/SourceManager.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_pseudo_destructor_with_variadic_16
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
#include "../include/modify_pseudo_destructor_with_variadic_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PD = Result.Nodes.getNodeAs<clang::CXXPseudoDestructorExpr>("PseudoDtor")) {
      //Filter nodes in header files
      if (!PD || !Result.SourceManager->isWrittenInMainFile(PD->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto pseudoDtorExpr = stringutils::rangetoStr(*(Result.SourceManager),
                                                    PD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedExpr = pseudoDtorExpr;
      size_t pos = mutatedExpr.find(".~");
      if (pos != std::string::npos) {
          mutatedExpr.replace(pos, 2, ".~T..."); // Replace with variadic template
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PD->getSourceRange()), mutatedExpr);
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxPseudoDestructorExpr().bind("PseudoDtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}