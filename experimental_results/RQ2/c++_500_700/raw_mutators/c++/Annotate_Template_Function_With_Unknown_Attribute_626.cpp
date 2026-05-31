//header file
#pragma once
#include "Mutator_base.h"

/**
 * annotate_template_function_with_unknown_attribute_626
 */ 
class MutatorFrontendAction_626 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(626)

private:
    class MutatorASTConsumer_626 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_626(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/annotate_template_function_with_unknown_attribute_626.h"

// ========================================================================================================
#define MUT626_OUTPUT 1

void MutatorFrontendAction_626::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      auto functionDecl = FD->getTemplatedDecl();
      if (!functionDecl)
        return;

      auto functionSourceRange = functionDecl->getSourceRange();
      std::string modifiedFunction = stringutils::rangetoStr(*(Result.SourceManager), functionSourceRange);
      
      std::string annotation = "[[unknown_attribute]]\n";
      modifiedFunction = annotation + modifiedFunction;

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionSourceRange), modifiedFunction);
    }
}
  
void MutatorFrontendAction_626::MutatorASTConsumer_626::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}