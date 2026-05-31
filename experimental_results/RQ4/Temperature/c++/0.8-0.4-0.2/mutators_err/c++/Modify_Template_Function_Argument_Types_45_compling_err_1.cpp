//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Function_Argument_Types_45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
    class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl*, std::string> templateFuncTypes;
    };
};

//source file
#include "../include/Modify_Template_Function_Argument_Types_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        if (FD->getDescribedFunctionTemplate()) {
            templateFuncTypes[FD] = FD->getTemplateSpecializationArgsAsWritten()->getSourceRange().getBegin().printToString(Result.Context->getSourceManager());
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FuncCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                CE->getBeginLoc()))
            return;

        const FunctionDecl *Callee = CE->getDirectCallee();
        if (Callee && templateFuncTypes.find(Callee) != templateFuncTypes.end()) {
            std::string newType = "CustomType";
            std::string originalType = templateFuncTypes[Callee];
            std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());

            size_t pos = callText.find(originalType);
            if (pos != std::string::npos) {
                callText.replace(pos, originalType.length(), newType);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), callText);
            }
        }
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto templateFuncMatcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunc");
    auto funcCallMatcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("FuncCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateFuncMatcher, &callback);
    matchFinder.addMatcher(funcCallMatcher, &callback);
    matchFinder.matchAST(Context);
}