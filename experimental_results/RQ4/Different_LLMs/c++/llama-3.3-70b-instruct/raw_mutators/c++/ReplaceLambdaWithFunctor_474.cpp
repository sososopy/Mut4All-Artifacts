//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceLambdaWithFunctor_474
 */ 
class MutatorFrontendAction_474 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(474)

private:
    class MutatorASTConsumer_474 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_474(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceLambdaWithFunctor_474.h"

// ========================================================================================================
#define MUT474_OUTPUT 1

void MutatorFrontendAction_474::Callback::run(const MatchFinder::MatchResult &Result) {
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
      // Handle lambda capture
      for (const auto &capture : LT->captures()) {
        if (capture.getKind() == clang::LambdaCaptureKind::LK_ByCopy) {
          functorText += "  " + capture.getVariable().getType().getAsString() + " " + capture.getVariable().getNameAsString() + ";\n";
        }
      }
      functorText += "  template <typename Self>\n";
      functorText += "  auto operator()(this Self&& self) {\n";
      // Handle lambda body
      functorText += "    " + stringutils::rangetoStr(*(Result.SourceManager), LT->getBody()->getSourceRange()) + "\n";
      functorText += "  }\n";
      functorText += "};\n";
      functorText = "/*mut474*/" + functorText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), functorText);
    }
}

void MutatorFrontendAction_474::MutatorASTConsumer_474::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}