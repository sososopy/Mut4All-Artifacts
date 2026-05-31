//header file
#pragma once
#include "Mutator_base.h"

/**
 * Negate_Noexcept_Condition_475
 */ 
class MutatorFrontendAction_475 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(475)

private:
    class MutatorASTConsumer_475 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_475(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Negate_Noexcept_Condition_475.h"

// ========================================================================================================
#define MUT475_OUTPUT 1

void MutatorFrontendAction_475::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if the function has a conditional noexcept specifier
      if (FD->hasAttr<clang::NoThrowAttr>() || FD->getExceptionSpecType() != clang::ExceptionSpecificationType::EST_Dynamic) {
        return;
      }
      const clang::Expr *NoexceptExpr = FD->getExceptionSpecSourceRange().isValid() ? FD->getExceptionSpecExpr() : nullptr;
      if (!NoexceptExpr) {
        return;
      }
      //Check if the expression is a simple boolean literal
      if (clang::isa<clang::CXXBoolLiteralExpr>(NoexceptExpr)) {
        return;
      }
      //Get the source code text of target node
      std::string OriginalExpr = stringutils::rangetoStr(*(Result.SourceManager), NoexceptExpr->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedExpr = "!(" + OriginalExpr + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NoexceptExpr->getSourceRange()), MutatedExpr);
    }
    else if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getExprLoc()))
        return;
      //Check if the lambda has a conditional noexcept specifier
      clang::FunctionDecl *CallOperator = LD->getCallOperator();
      if (!CallOperator) return;
      clang::FunctionProtoType *FPT = CallOperator->getType()->getAs<clang::FunctionProtoType>();
      if (!FPT || FPT->getExceptionSpecType() != clang::ExceptionSpecificationType::EST_Dynamic) {
        return;
      }
      const clang::Expr *NoexceptExpr = CallOperator->getExceptionSpecSourceRange().isValid() ? CallOperator->getExceptionSpecExpr() : nullptr;
      if (!NoexceptExpr) {
        return;
      }
      //Check if the expression is a simple boolean literal
      if (clang::isa<clang::CXXBoolLiteralExpr>(NoexceptExpr)) {
        return;
      }
      //Get the source code text of target node
      std::string OriginalExpr = stringutils::rangetoStr(*(Result.SourceManager), NoexceptExpr->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedExpr = "!(" + OriginalExpr + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NoexceptExpr->getSourceRange()), MutatedExpr);
    }
}
  
void MutatorFrontendAction_475::MutatorASTConsumer_475::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher FunctionMatcher = functionDecl(hasDynamicExceptionSpec()).bind("FunctionDecl");
    StatementMatcher LambdaMatcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(FunctionMatcher, &callback);
    matchFinder.addMatcher(LambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}