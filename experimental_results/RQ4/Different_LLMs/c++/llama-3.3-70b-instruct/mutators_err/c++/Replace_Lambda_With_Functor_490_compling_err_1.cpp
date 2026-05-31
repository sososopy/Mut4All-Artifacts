//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_With_Functor_490
 */ 
class MutatorFrontendAction_490 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(490)

private:
    class MutatorASTConsumer_490 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_490(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Lambda_With_Functor_490.h"

// ========================================================================================================
#define MUT490_OUTPUT 1

void MutatorFrontendAction_490::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string functorText = "struct Functor {\n";
      if (LT->captureDefault() == LambdaExpr::CaptureDefault::CD_ByValue) {
        functorText += "  template <typename... Args>\n";
      } else if (LT->captureDefault() == LambdaExpr::CaptureDefault::CD_ByReference) {
        functorText += "  template <typename... Args>\n";
      }
      functorText += "  void operator()(Args... args) {\n";
      functorText += "    /* functor body */\n";
      functorText += "  }\n";
      functorText += "};\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), functorText);
    }
}

void MutatorFrontendAction_490::MutatorASTConsumer_490::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}