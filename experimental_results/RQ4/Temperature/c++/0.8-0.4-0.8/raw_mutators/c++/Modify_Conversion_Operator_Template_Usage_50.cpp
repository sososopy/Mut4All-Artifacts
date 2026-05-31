//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_conversion_operator_template_usage_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)

private:
    class MutatorASTConsumer_50 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_conversion_operator_template_usage_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("ConversionOperator")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getExprLoc()))
            return;

        auto sourceRange = MT->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        // Perform mutation by replacing the conversion type with an undefined type
        std::string mutatedText = sourceText;
        size_t pos = mutatedText.find("operator");
        if (pos != std::string::npos) {
            size_t endPos = mutatedText.find("(", pos);
            if (endPos != std::string::npos) {
                mutatedText.replace(pos, endPos - pos, "operator UndefinedType");
            }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}

void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMemberCallExpr(callee(functionDecl(hasName("operator")))).bind("ConversionOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}