//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Templated_Function_260
 */ 
class MutatorFrontendAction_260 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(260)

private:
    class MutatorASTConsumer_260 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_260(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionTemplateDecl *TargetTemplate = nullptr;
    };
};

//source file
#include "../include/Misuse_Templated_Function_260.h"

// ========================================================================================================
#define MUT260_OUTPUT 1

void MutatorFrontendAction_260::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateFunc")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;
        
        TargetTemplate = FTD;
    } else if (const auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("funcCall")) {
        if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(FC->getBeginLoc()))
            return;
        
        if (TargetTemplate && FC->getDirectCallee() && FC->getDirectCallee()->getPrimaryTemplate() == TargetTemplate) {
            auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), FC->getSourceRange());
            auto pos = sourceText.find('(');
            if (pos != std::string::npos) {
                sourceText.insert(pos + 1, "10.5, \"extra\", ");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()), sourceText);
            }
        }
    }
}

void MutatorFrontendAction_260::MutatorASTConsumer_260::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = functionTemplateDecl(hasDescendant(functionDecl())).bind("templateFunc");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(hasParent(functionTemplateDecl())))).bind("funcCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}