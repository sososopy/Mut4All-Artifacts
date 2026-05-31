//header file
#pragma once
#include "Mutator_base.h"

/**
 * omit_template_parameter_in_instantiation_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *targetFunctionDecl = nullptr;
    };
};

//source file
#include "../include/omit_template_parameter_in_instantiation_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;

        if (FTD->getTemplateParameters()->size() == 0)
            return;

        targetFunctionDecl = FTD->getTemplatedDecl();
        if (!targetFunctionDecl)
            return;
    }

    if (auto *CI = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
        if (!CI || !Result.Context->getSourceManager().isWrittenInMainFile(
                      CI->getExprLoc()))
            return;

        if (targetFunctionDecl && CI->getDirectCallee() == targetFunctionDecl) {
            std::string callText = Lexer::getSourceText(
                CharSourceRange::getTokenRange(CI->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();
            
            size_t pos = callText.find_last_of(')');
            if (pos != std::string::npos) {
                callText = callText.substr(0, pos);
                callText += " /*mut20*/"; // Omit one parameter intentionally
                callText += ")";
                Rewrite.ReplaceText(CI->getSourceRange(), callText);
            }
        }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionTemplateMatcher = functionTemplateDecl(hasTemplateParameterList()).bind("FunctionTemplate");
    StatementMatcher callExprMatcher = callExpr().bind("CallExpr");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}