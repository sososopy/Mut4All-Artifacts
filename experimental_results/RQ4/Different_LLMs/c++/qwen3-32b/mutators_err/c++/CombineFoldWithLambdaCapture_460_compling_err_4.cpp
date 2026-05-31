//header file
#pragma once
#include "Mutator_base.h"

/**
 * CombineFoldWithLambdaCapture_460
 */ 
class MutatorFrontendAction_460 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(460)

private:
    class MutatorASTConsumer_460 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_460(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/CombineFoldWithLambdaCapture_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
      if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     lambda->getBeginLoc()))
        return;
      const CXXRecordDecl *closure = lambda->getLambdaClass();
      if (!closure)
        return;
      const CXXMethodDecl *callOperator = closure->getLambdaCallOperator();
      if (!callOperator)
        return;
      for (const LambdaCapture &capture : closure->captures()) {
        const VarDecl *capturedVar = capture.getCapturedVar();
        if (!capturedVar || capturedVar->getNameAsString().empty())
          continue;
        string varName = capturedVar->getNameAsString();
        string params;
        for (const ParmVarDecl *param : callOperator->parameters()) {
          if (!params.empty())
            params += ", ";
          params += param->getNameAsString();
        }
        string nestedLambda = "[](auto... b) { return ((" + varName + " + b) + ...); }";
        string application = params.empty() ? "" : params;
        string newBody = "return " + nestedLambda + "(" + application + ");";
        SourceLocation bodyLoc = lambda->getBody()->getBeginLoc();
        SourceLocation endLoc = lambda->getBody()->getEndLoc();
        Rewrite.ReplaceText(SourceRange(bodyLoc, endLoc), newBody);
        break;
      }
    }
}
  
void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(clang::ast_matchers::hasAnyCapture(), hasBody(compoundStmt())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}