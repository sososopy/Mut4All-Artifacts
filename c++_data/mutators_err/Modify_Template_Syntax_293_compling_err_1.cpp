//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Syntax_293
 */ 
class MutatorFrontendAction_293 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(293)

private:
    class MutatorASTConsumer_293 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_293(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Syntax_293.h"

// ========================================================================================================
#define MUT293_OUTPUT 1

void MutatorFrontendAction_293::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateInst")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getTemplateNameLoc()))
        return;

      //Get the source code text of target node
      auto args = TI->template_arguments();
      if (args.size() > 0 && args[0].getKind() == TemplateArgument::Integral) {
        //Perform mutation on the source code text by applying string replacement
        auto argLoc = args[0].getSourceRange();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(argLoc), "'a'");
      }
    }
}
  
void MutatorFrontendAction_293::MutatorASTConsumer_293::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}