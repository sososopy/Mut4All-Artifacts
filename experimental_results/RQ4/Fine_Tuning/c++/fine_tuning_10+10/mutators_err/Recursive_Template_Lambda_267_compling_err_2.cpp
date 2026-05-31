//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Recursive_Template_Lambda_267
 */ 
class MutatorFrontendAction_267 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(267)

private:
    class MutatorASTConsumer_267 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_267(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/recursive_template_lambda_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (const auto *FuncDecl = FD->getTemplatedDecl()) {
        if (const auto *Body = FuncDecl->getBody()) {
          for (const auto &Stmt : Body->children()) {
            if (const auto *LambdaExpr = dyn_cast<clang::LambdaExpr>(Stmt)) {
              //Get the source code text of target node
              auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                          LambdaExpr->getSourceRange());
              //Perform mutation on the source code text by applying string replacement
              std::string newLambda = "myFunction([=]{});";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), newLambda);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(lambdaExpr(captureless(), hasBody(compoundStmt(hasNoStatements()))))
    ).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}