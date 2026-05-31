//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_builtin_function_call_argument_type_11
 */ 
class MutatorFrontendAction_11 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(11)

private:
    class MutatorASTConsumer_11 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_11(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_builtin_function_call_argument_type_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      //Get the source code text of target node
      std::string originalCode = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      std::string mutatedCode = originalCode;
      
      //Perform mutation on the source code text by applying string replacement
      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
          if (CE->getArg(i)->getType()->isFloatingType()) {
              // Replace floating-point argument with a string literal
              mutatedCode.replace(mutatedCode.find(CE->getArg(i)->getSourceRange().getBegin().printToString(*Result.SourceManager)), 
                                  CE->getArg(i)->getSourceRange().getEnd().getRawEncoding() - CE->getArg(i)->getSourceRange().getBegin().getRawEncoding(), 
                                  "\"invalid\"");
              break;
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_fpclassify")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}