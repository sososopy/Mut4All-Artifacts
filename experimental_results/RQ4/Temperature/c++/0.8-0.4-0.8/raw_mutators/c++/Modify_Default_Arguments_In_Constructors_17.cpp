//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_arguments_in_constructors_17
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(17)

private:
    class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_arguments_in_constructors_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstructorWithDefault")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;

      for (auto param : Ctor->parameters()) {
        if (param->hasDefaultArg()) {
          //Get the source code text of target node
          auto defaultArgRange = param->getDefaultArgRange();
          auto defaultArgText = stringutils::rangetoStr(*(Result.SourceManager), defaultArgRange);
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedDefaultArg = "/*mut17*/";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(defaultArgRange, mutatedDefaultArg);
        }
      }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArgument())).bind("ConstructorWithDefault");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}