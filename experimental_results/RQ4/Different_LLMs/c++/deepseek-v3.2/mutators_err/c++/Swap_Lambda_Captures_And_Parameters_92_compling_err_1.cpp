//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Lambda_Captures_And_Parameters_92
 */ 
class MutatorFrontendAction_92 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(92)

private:
    class MutatorASTConsumer_92 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_92(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Swap_Lambda_Captures_And_Parameters_92.h"

// ========================================================================================================
#define MUT92_OUTPUT 1

void MutatorFrontendAction_92::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getLocation()))
        return;
      
      // Get the lambda's call operator
      auto *callOp = LT->getCallOperator();
      if (!callOp) return;
      
      // Check if lambda is inside a template function/context
      bool insideTemplate = false;
      auto *parent = dyn_cast<FunctionDecl>(callOp->getParent());
      if (parent) {
        // Check if parent function is templated
        if (parent->getTemplatedKind() != FunctionDecl::TK_NonTemplate) {
          insideTemplate = true;
        } else {
          // Check enclosing contexts
          auto *context = parent->getParent();
          while (context) {
            if (auto *record = dyn_cast<CXXRecordDecl>(context)) {
              if (record->getDescribedClassTemplate() || record->getTemplateSpecializationKind() != TSK_Undeclared) {
                insideTemplate = true;
                break;
              }
            } else if (auto *func = dyn_cast<FunctionDecl>(context)) {
              if (func->getTemplatedKind() != FunctionDecl::TK_NonTemplate) {
                insideTemplate = true;
                break;
              }
            }
            context = context->getParent();
          }
        }
      }
      
      if (!insideTemplate) return;
      
      // Check if lambda has generic parameters (auto or auto&&)
      bool hasGenericParam = false;
      for (auto *param : callOp->parameters()) {
        if (param->getType()->isAutoType() || 
            (param->getType()->isRValueReferenceType() && 
             param->getType()->getPointeeType()->isAutoType())) {
          hasGenericParam = true;
          break;
        }
      }
      
      if (!hasGenericParam) return;
      
      // Get capture list
      auto captures = LT->captures();
      if (captures.empty()) return;  // Need non-empty capture list
      
      // Get parameter list - must not be empty
      if (callOp->getNumParams() == 0) return;
      
      // Get source ranges
      SourceManager &SM = *Result.SourceManager;
      LangOptions LangOpts = Result.Context->getLangOpts();
      
      // Get capture range
      SourceLocation captureBegin = LT->getIntroducerRange().getBegin();
      SourceLocation captureEnd = LT->getIntroducerRange().getEnd();
      if (!captureBegin.isValid() || !captureEnd.isValid()) return;
      
      // Get parameter range
      SourceLocation paramBegin = callOp->getParamDecl(0)->getBeginLoc();
      SourceLocation paramEnd = callOp->getParamDecl(callOp->getNumParams() - 1)->getEndLoc();
      if (!paramBegin.isValid() || !paramEnd.isValid()) return;
      
      // Get body begin (after parameters)
      SourceLocation bodyBegin = LT->getBody()->getBeginLoc();
      if (!bodyBegin.isValid()) return;
      
      // Extract capture text
      std::string captureText = stringutils::rangetoStr(SM, SourceRange(captureBegin, captureEnd));
      
      // Extract parameter text
      // Need to handle cases where parameters might be on multiple lines
      std::string paramText;
      for (unsigned i = 0; i < callOp->getNumParams(); ++i) {
        auto *param = callOp->getParamDecl(i);
        if (i > 0) {
          // Add commas between parameters
          SourceLocation prevEnd = callOp->getParamDecl(i - 1)->getEndLoc();
          SourceLocation commaLoc = Lexer::findLocationAfterToken(prevEnd, 
                                                                  tok::comma, 
                                                                  SM, 
                                                                  LangOpts, 
                                                                  true);
          if (commaLoc.isValid()) {
            paramText += stringutils::rangetoStr(SM, SourceRange(prevEnd, commaLoc));
          }
        }
        paramText += stringutils::rangetoStr(SM, param->getSourceRange());
      }
      
      // Clean up parameter text - remove leading/trailing whitespace
      paramText = stringutils::trim(paramText);
      
      // Build new lambda with swapped captures and parameters
      std::string newLambda = "[" + paramText + "]" + captureText + " ";
      
      // Get everything from after the original parameters to before the body
      SourceLocation afterParams = Lexer::getLocForEndOfToken(paramEnd, 0, SM, LangOpts);
      if (!afterParams.isValid()) return;
      
      // Extract text between parameters and body (includes any trailing specifiers)
      std::string middleText = stringutils::rangetoStr(SM, SourceRange(afterParams, bodyBegin));
      
      // Add the middle part (mutable, constexpr, etc.)
      newLambda += middleText;
      
      // Get the body text
      std::string bodyText = stringutils::rangetoStr(SM, LT->getBody()->getSourceRange());
      newLambda += bodyText;
      
      // Add mutation comment
      newLambda = "/*mut92*/" + newLambda;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), newLambda);
    }
}
  
void MutatorFrontendAction_92::MutatorASTConsumer_92::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}