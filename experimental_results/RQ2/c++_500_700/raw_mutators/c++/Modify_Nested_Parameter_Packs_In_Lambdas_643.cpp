//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_nested_parameter_packs_in_lambdas_643
 */ 
class MutatorFrontendAction_643 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(643)

private:
    class MutatorASTConsumer_643 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_643(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_nested_parameter_packs_in_lambdas_643.h"

// ========================================================================================================
#define MUT643_OUTPUT 1

void MutatorFrontendAction_643::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        // Locate the lambda expression and introduce a nested lambda
        size_t lambdaPos = declaration.find("[&](");
        if (lambdaPos != std::string::npos) {
          size_t lambdaEnd = declaration.find(";", lambdaPos);
          if (lambdaEnd != std::string::npos) {
            std::string nestedLambda = "[&]{ (void)[&]{ /*mut643*/ }; };";
            declaration.insert(lambdaEnd, nestedLambda);
          }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_643::MutatorASTConsumer_643::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(lambdaExpr())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}