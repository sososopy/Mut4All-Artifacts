//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Modify_Concept_Requirements_339
 */ 
class MutatorFrontendAction_339 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(339)

private:
    class MutatorASTConsumer_339 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_339(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Concept_Requirements_339.h"

// ========================================================================================================
#define MUT339_OUTPUT 1

void MutatorFrontendAction_339::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *RT = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RT->getRequiresKWLoc()))
        return;

      //Get the source code text of target node
      auto requiresText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  RT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      // Example mutation: Change Addable<int> to Addable<float>
      size_t pos = requiresText.find("Addable<int>");
      if (pos != std::string::npos) {
        requiresText.replace(pos, std::string("Addable<int>").length(), "Addable<float>");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RT->getSourceRange()), requiresText);
    }
}
  
void MutatorFrontendAction_339::MutatorASTConsumer_339::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}