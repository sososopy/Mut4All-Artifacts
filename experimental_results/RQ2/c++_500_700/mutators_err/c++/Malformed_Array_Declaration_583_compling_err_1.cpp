//header file
#pragma once
#include "Mutator_base.h"

/**
 * malformed_array_declaration_583
 */ 
class MutatorFrontendAction_583 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(583)

private:
    class MutatorASTConsumer_583 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_583(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_583.h"

// ========================================================================================================
#define MUT583_OUTPUT 1

void MutatorFrontendAction_583::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VD->getLocation()))
            return;

        if (VD->getType()->isArrayType()) {
            if (auto *AT = Result.Context->getAsArrayType(VD->getType())) {
                if (auto *CAT = dyn_cast<ConstantArrayType>(AT)) {
                    auto sizeExpr = CAT->getSize().toString(10, true);
                    std::string mutatedSizeExpr = sizeExpr + " / 0";
                    std::string originalDecl = VD->getNameAsString() + "[" + sizeExpr + "]";
                    std::string mutatedDecl = VD->getNameAsString() + "[" + mutatedSizeExpr + "]";
                    
                    SourceRange declRange = VD->getSourceRange();
                    auto declText = stringutils::rangetoStr(*(Result.SourceManager), declRange);
                    size_t pos = declText.find(originalDecl);
                    if (pos != std::string::npos) {
                        declText.replace(pos, originalDecl.length(), mutatedDecl);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(declRange), declText);
                    }
                }
            }
        }
    } else if (auto *AE = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("arrayUsage")) {
        if (!AE || !Result.Context->getSourceManager().isWrittenInMainFile(
                   AE->getBeginLoc()))
            return;

        auto indexExpr = stringutils::rangetoStr(*(Result.SourceManager), AE->getIdx()->getSourceRange());
        std::string mutatedIndexExpr = "(" + indexExpr + ") / 0";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(AE->getIdx()->getSourceRange()), mutatedIndexExpr);
    }
}
  
void MutatorFrontendAction_583::MutatorASTConsumer_583::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher arrayDeclMatcher = varDecl(hasType(arrayType())).bind("arrayDecl");
    StatementMatcher arrayUsageMatcher = arraySubscriptExpr().bind("arrayUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayDeclMatcher, &callback);
    matchFinder.addMatcher(arrayUsageMatcher, &callback);
    matchFinder.matchAST(Context);
}