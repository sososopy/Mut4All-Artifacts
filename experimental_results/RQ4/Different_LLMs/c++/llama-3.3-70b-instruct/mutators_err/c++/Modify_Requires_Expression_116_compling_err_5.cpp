//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Stmt.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Expr.h"

/**
 * Modify_Requires_Expression_116
 */ 
class MutatorFrontendAction_116 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(116)

private:
    class MutatorASTConsumer_116 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_116(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Requires_Expression_116.h"
#include "clang/AST/RequiresExpr.h"

// ========================================================================================================
#define MUT116_OUTPUT 1

void MutatorFrontendAction_116::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangeToString(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      // Remove or add a closing parenthesis in the requires expression
      if (mutatedDeclaration.find(")") != std::string::npos) {
        mutatedDeclaration.erase(mutatedDeclaration.find(")"), 1);
      } else {
        mutatedDeclaration += ")";
      }
      // Change the keyword requires to a similar keyword
      mutatedDeclaration.replace(mutatedDeclaration.find("requires"), 8, "require");
      // Insert or remove a semicolon within the requires expression
      if (mutatedDeclaration.find(";") != std::string::npos) {
        mutatedDeclaration.erase(mutatedDeclaration.find(";"), 1);
      } else {
        mutatedDeclaration += ";";
      }
      // Replace the requires keyword with a different keyword
      mutatedDeclaration.replace(mutatedDeclaration.find("require"), 7, "if");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_116::MutatorASTConsumer_116::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::expr(clang::ast_matchers::hasDescendant(clang::ast_matchers::expr(clang::ast_matchers::requiresExpr()))).bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}