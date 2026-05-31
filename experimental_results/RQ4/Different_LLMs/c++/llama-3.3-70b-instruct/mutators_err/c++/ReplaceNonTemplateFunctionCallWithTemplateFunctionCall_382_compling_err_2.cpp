//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * ReplaceNonTemplateFunctionCallWithTemplateFunctionCall_382
 */ 
class MutatorFrontendAction_382 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(382)

private:
    class MutatorASTConsumer_382 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_382(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceNonTemplateFunctionCallWithTemplateFunctionCall_382.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"

// ========================================================================================================
#define MUT382_OUTPUT 1

void MutatorFrontendAction_382::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isInMainFile(Call->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(Call->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Find the function name
      auto funcName = Call->getDirectCallee()->getNameAsString();
      // Find the template function with the same name
      auto templateFunc = Result.Context->getTranslationUnitDecl()->lookupSingleName(
          Result.Context->getDeclarationNameTable().getDeclarationName(funcName));
      if (templateFunc) {
        // Replace the original function call with the template function call
        auto mutatedDeclaration = declaration + "<int>(";
        mutatedDeclaration.replace(mutatedDeclaration.find(funcName), funcName.length(), funcName + "<int>");
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(Call->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_382::MutatorASTConsumer_382::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}