//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_default_arg_694
 */ 
class MutatorFrontendAction_694 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(694)

private:
    class MutatorASTConsumer_694 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_694(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_default_arg_694.h"

// ========================================================================================================
#define MUT694_OUTPUT 1

void MutatorFrontendAction_694::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CtorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CtorDecl->getLocation()))
        return;

      if (CtorDecl->getNumParams() > 0) {
        auto *Param = CtorDecl->getParamDecl(0);
        if (Param->hasDefaultArg()) {
          //Get the source code text of target node
          auto defaultArg = stringutils::rangetoStr(*(Result.SourceManager),
                                                    Param->getDefaultArgRange());
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedArg = "sizeof(T) + Example<T>()";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(Param->getDefaultArgRange(), mutatedArg);
        }
      }
    }
}
  
void MutatorFrontendAction_694::MutatorASTConsumer_694::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(
        ofClass(hasTemplateArgument(0, anything()))).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}