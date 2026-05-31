//header file
#pragma once
#include "Mutator_base.h"

/**
 * remove_template_parameter_from_function_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)

private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_template_parameter_from_function_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (FTD->getTemplateParameters()->size() > 0) {
        auto *param = FTD->getTemplateParameters()->getParam(0); 
        auto paramRange = param->getSourceRange();

        // Get the source code text of target node
        auto text = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
        
        // If parameter is used, replace with a concrete type (using `int` as a simple example)
        if (FTD->getTemplatedDecl()->getType().getAsString().find(param->getName()) != std::string::npos) {
            Rewrite.ReplaceText(FTD->getTemplatedDecl()->getReturnTypeSourceRange(), "int");
        }
        
        // Remove the parameter from the template parameter list
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), "");
      }
    }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}