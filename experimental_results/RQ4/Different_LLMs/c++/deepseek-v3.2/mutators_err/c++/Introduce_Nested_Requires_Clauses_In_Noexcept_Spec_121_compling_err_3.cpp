//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Nested_Requires_Clauses_In_Noexcept_Spec_121
 */ 
class MutatorFrontendAction_121 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(121)

private:
    class MutatorASTConsumer_121 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_121(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Introduce_Nested_Requires_Clauses_In_Noexcept_Spec_121.h"

// ========================================================================================================
#define MUT121_OUTPUT 1

void MutatorFrontendAction_121::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      // Get the lambda's call operator
      const CXXMethodDecl *callOp = MT->getCallOperator();
      if (!callOp) return;
      
      // Check if lambda has explicit template parameters
      const FunctionTemplateDecl *tempDecl = callOp->getDescribedFunctionTemplate();
      if (!tempDecl) return;
      
      // Get template parameters
      TemplateParameterList *tpl = tempDecl->getTemplateParameters();
      if (!tpl || tpl->size() == 0)1) return;
      
      // Use the first template parameter
      NamedDecl *firstParam = tpl->getParam(0);
      if (!firstParam) return;
      
      std::string paramName = firstParam->getNameAsString();
      if (paramName.empty()) paramName = "T";
      
      // Get lambda source range
      SourceRange lambdaRange = MT->getSourceRange();
      if (!lambdaRange.isValid()) return;
      
      // Get source manager
      SourceManager &SM = Rewrite.getSourceMgr();
      
      // Get the lambda body location
      CompoundStmt *body = dyn_cast_or_null<CompoundStmt>(callOp->getBody());
      if (!body) return;
      
      SourceLocation bodyStart = body->getBeginLoc();
      if (bodyStart.isInvalid()) return;
      
      // Get the text before the body
      std::string lambdaText = stringutils::rangetoStr(SM, 
          SourceRange(lambdaRange.getBegin(), bodyStart.getLocWithOffset(-1)));
      
      // Remove trailing whitespace/newline before body
      while (!lambdaText.empty() && 
             (lambdaText.back() == ' ' || lambdaText.back() == '\n' || lambdaText.back() == '\t')) {
        lambdaText.pop_back();
      }
      
      // Construct the new noexcept specification
      std::string newNoexcept = " noexcept(requires { requires requires { typename " + 
                                paramName + "::x; } }) ";
      
      // Check if lambda already has a noexcept specification
      const FunctionProtoType *protoType = callOp->getType()->getAs<FunctionProtoType>();
      if (protoType && protoType->getExceptionSpecType() != EST_None) {
        // Find and replace existing noexcept
        // This is simplified - in practice would need to locate the exact noexcept text
        // For now, we'll append before the body
        lambdaText += newNoexcept;
      } else {
        // Add noexcept before the body
        lambdaText += newNoexcept;
      }
      
      // Add the body back
      std::string bodyText = stringutils::rangetoStr(SM, body->getSourceRange());
      lambdaText += bodyText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_121::MutatorASTConsumer_121::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasDescendant(functionDecl(hasDescendant(compoundStmt())))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}