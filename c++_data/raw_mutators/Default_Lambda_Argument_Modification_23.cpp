//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Lambda_Argument_Modification_23
 */ 
class MutatorFrontendAction_23 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(23)

private:
    class MutatorASTConsumer_23 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_23(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Default_Lambda_Argument_Modification_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;

      auto &SM = *Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      auto Range = CharSourceRange::getTokenRange(LE->getSourceRange());
      std::string LambdaText = Lexer::getSourceText(Range, SM, LangOpts).str();

      // Check if the lambda has parameters and if any have default arguments
      if (LE->getLambdaClass()->getLambdaCallOperator()->getNumParams() > 0) {
        bool hasDefaultArg = false;
        for (const auto *Param : LE->getLambdaClass()->getLambdaCallOperator()->parameters()) {
          if (Param->hasDefaultArg()) {
            hasDefaultArg = true;
            break;
          }
        }

        //Perform mutation on the source code text by applying string replacement
        if (!hasDefaultArg) {
          // Add default lambda argument
          size_t pos = LambdaText.find('(');
          if (pos != std::string::npos) {
            LambdaText.insert(pos + 1, "auto x = []() { return 42; }");
          }
        } else {
          // Modify existing default argument
          size_t pos = LambdaText.find('=');
          if (pos != std::string::npos) {
            size_t endPos = LambdaText.find(',', pos);
            if (endPos == std::string::npos) {
              endPos = LambdaText.find(')', pos);
            }
            LambdaText.replace(pos + 1, endPos - pos - 1, " []() { return 42; }");
          }
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(Range, LambdaText);
      }
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasAnyParameter(hasType(autoType()))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}