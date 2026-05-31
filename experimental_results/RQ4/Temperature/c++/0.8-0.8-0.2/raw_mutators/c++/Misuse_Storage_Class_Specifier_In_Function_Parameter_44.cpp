//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_storage_class_specifier_in_function_parameter_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_storage_class_specifier_in_function_parameter_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionWithParams")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->hasBody()) {
        for (auto *param : FD->parameters()) {
          // Perform mutation by adding an inappropriate storage class specifier
          if (!param->hasDefaultArg()) {
            auto paramName = param->getNameAsString();
            if (!paramName.empty()) {
              std::string mutatedParamDecl = "extern " + param->getType().getAsString() + " " + paramName + ";";
              SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
              Rewrite.InsertText(insertLoc, "\n/*mut44*/" + mutatedParamDecl);
              break; // Apply mutation only to the first parameter for simplicity
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(to(parmVarDecl()))).bind("functionWithParams");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}