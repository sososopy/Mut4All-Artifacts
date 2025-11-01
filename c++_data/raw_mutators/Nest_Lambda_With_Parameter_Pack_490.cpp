//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nest_Lambda_With_Parameter_Pack_490
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
    };
};

//source file
#include "../include/Nest_Lambda_With_Parameter_Pack_490.h"

// ========================================================================================================
#define MUT490_OUTPUT 1

void MutatorFrontendAction_490::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithPack")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        size_t lambdaStart = declaration.find("[&](");
        if (lambdaStart != std::string::npos) {
          size_t lambdaBodyStart = declaration.find("{", lambdaStart);
          if (lambdaBodyStart != std::string::npos) {
            std::string nestedLambda = "\n[&](auto... nestedIndexes) {";
            nestedLambda += "\n/*mut490*/";
            nestedLambda += declaration.substr(lambdaBodyStart + 1, declaration.find("}", lambdaBodyStart) - lambdaBodyStart - 1);
            nestedLambda += "\n};";
            declaration.insert(lambdaBodyStart + 1, nestedLambda);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_490::MutatorASTConsumer_490::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(hasDescendant(lambdaExpr())).bind("FunctionWithPack");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}