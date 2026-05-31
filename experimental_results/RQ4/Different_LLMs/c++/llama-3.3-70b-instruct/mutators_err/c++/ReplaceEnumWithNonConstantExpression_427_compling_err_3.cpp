//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * ReplaceEnumWithNonConstantExpression_427
 */ 
class MutatorFrontendAction_427 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(427)

private:
    class MutatorASTConsumer_427 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_427(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceEnumWithNonConstantExpression_427.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CS = Result.Nodes.getNodeAs<clang::CaseStmt>("CaseStmt")) {
      //Filter nodes in header files
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto caseLabel = CS->getLHS();
      if (caseLabel && caseLabel->getStmtClass() == clang::Stmt::EnumConstantDeclClass) {
        //Perform mutation on the source code text by applying string replacement
        auto enumDecl = clang::dyn_cast<clang::EnumConstantDecl>(caseLabel->IgnoreImplicit());
        auto enumName = enumDecl->getNameAsString();
        auto variableName = "variable_" + enumName;
        auto mutatedText = variableName;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(caseLabel->getSourceRange(), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::caseStmt().bind("CaseStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}