//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mismatched_Type_Comparison_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> typeReplacementMap;
        void initializeTypeReplacementMap();
    };
};

//source file
#include "../include/mismatched_type_comparison_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::initializeTypeReplacementMap() {
    typeReplacementMap["int"] = "float";
    typeReplacementMap["unsigned int"] = "double";
    typeReplacementMap["long"] = "double";
    // Add more type replacements as needed
}

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getExprLoc()))
            return;

        auto lhsType = BO->getLHS()->getType().getAsString();
        auto rhsType = BO->getRHS()->getType().getAsString();

        if (typeReplacementMap.find(lhsType) != typeReplacementMap.end()) {
            std::string newType = typeReplacementMap[lhsType];
            std::string lhsText = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();

            std::string newLhsText = "(" + newType + ")" + lhsText;
            Rewrite.ReplaceText(BO->getLHS()->getSourceRange(), newLhsText);
        }
    }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = binaryOperator(hasOperatorName("<")).bind("BinaryOp");
    Callback callback(TheRewriter);
    callback.initializeTypeReplacementMap();
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}