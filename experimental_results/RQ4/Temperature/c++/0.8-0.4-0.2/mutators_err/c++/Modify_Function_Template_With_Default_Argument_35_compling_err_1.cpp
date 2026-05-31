//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Template_With_Default_Argument_35
 */ 
class MutatorFrontendAction_35 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(35)

private:
    class MutatorASTConsumer_35 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_35(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_function_template_with_default_argument_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (FTD->getTemplateParameters()->size() > 0) {
        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Result.Context->getLangOpts();
        auto TemplateParams = FTD->getTemplateParameters();
        std::string NewParam = ", typename U = double";

        SourceLocation InsertLoc = TemplateParams->getRAngleLoc();
        InsertLoc = InsertLoc.getLocWithOffset(-1); // Move before '>'
        
        Rewrite.InsertText(InsertLoc, NewParam, true, true);
        
        // Modify function to use the new template parameter
        if (auto *FD = FTD->getTemplatedDecl()) {
          if (FD->getNumParams() > 0) {
            auto ParamRange = FD->getParamDecl(0)->getSourceRange();
            std::string NewParamUsage = ", U additionalValue = U()";
            Rewrite.InsertTextAfterToken(ParamRange.getEnd(), NewParamUsage);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateParameterList(
        has(templateTypeParmDecl(hasDefaultArgument()))))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}