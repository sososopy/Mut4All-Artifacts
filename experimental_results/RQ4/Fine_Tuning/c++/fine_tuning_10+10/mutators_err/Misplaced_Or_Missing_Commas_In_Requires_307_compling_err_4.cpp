//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

/**
 * Misplaced_Or_Missing_Commas_In_Requires_307
 */ 
class MutatorFrontendAction_307 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(307)

private:
    class MutatorASTConsumer_307 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_307(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Misplaced_Or_Missing_Commas_In_Requires_307.h"

// ========================================================================================================
#define MUT307_OUTPUT 1

void MutatorFrontendAction_307::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *RC = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresClause")) {
      //Filter nodes in header files
      if (!Result.Context->getSourceManager().isWrittenInMainFile(RC->getRequiresKWLoc()))
        return;
      
      //Get the source code text of target node
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), RC->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      // Introduce a missing comma or add an unnecessary comma in the requires clause
      size_t pos = sourceText.find(",");
      if (pos != std::string::npos) {
          // Remove a comma to introduce a missing comma error
          sourceText.erase(pos, 1);
      } else {
          // Add a comma incorrectly
          pos = sourceText.find(";");
          if (pos != std::string::npos) {
              sourceText.insert(pos, ",");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), sourceText);
    }
}
  
void MutatorFrontendAction_307::MutatorASTConsumer_307::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::expr(clang::ast_matchers::hasType(clang::ast_matchers::requiresExpr())).bind("RequiresClause");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}