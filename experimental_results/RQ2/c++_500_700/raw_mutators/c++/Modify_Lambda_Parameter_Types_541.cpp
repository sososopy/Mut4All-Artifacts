//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_parameter_types_541
 */ 
class MutatorFrontendAction_541 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(541)

private:
    class MutatorASTConsumer_541 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_541(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_parameter_types_541.h"

// ========================================================================================================
#define MUT541_OUTPUT 1

void MutatorFrontendAction_541::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
            return;

        const auto &CaptureDefault = LE->getCaptureDefault();
        if (CaptureDefault != clang::LCD_None) {
            return;
        }

        for (auto *Child : LE->getBody()->children()) {
            if (auto *NestedLE = dyn_cast<clang::LambdaExpr>(Child)) {
                auto SourceRange = NestedLE->getSourceRange();
                auto StartLoc = SourceRange.getBegin();
                auto EndLoc = SourceRange.getEnd();

                std::string OriginalText = clang::Lexer::getSourceText(
                    clang::CharSourceRange::getTokenRange(SourceRange),
                    Result.Context->getSourceManager(),
                    Result.Context->getLangOpts());

                std::string MutatedText = OriginalText;
                size_t ParamStart = MutatedText.find('(');
                size_t ParamEnd = MutatedText.find(')', ParamStart);

                if (ParamStart != std::string::npos && ParamEnd != std::string::npos) {
                    std::string Params = MutatedText.substr(ParamStart + 1, ParamEnd - ParamStart - 1);
                    std::string NewParams = "decltype(x)... y";
                    MutatedText.replace(ParamStart + 1, ParamEnd - ParamStart - 1, NewParams);
                    size_t BodyStart = MutatedText.find('{', ParamEnd);
                    if (BodyStart != std::string::npos) {
                        MutatedText.insert(BodyStart, " requires true");
                    }
                }

                Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(SourceRange), MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_541::MutatorASTConsumer_541::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}