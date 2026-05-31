//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Templated_Lambda_With_Fold_489
 */ 
class MutatorFrontendAction_489 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(489)

private:
    class MutatorASTConsumer_489 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_489(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Templated_Lambda_With_Fold_489.h"

// ========================================================================================================
#define MUT489_OUTPUT 1

void MutatorFrontendAction_489::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        SourceManager &SM = *Result.SourceManager;
        std::string originalText = stringutils::rangetoStr(SM, lambda->getSourceRange());

        size_t openParenPos = originalText.find('(');
        if (openParenPos == std::string::npos) {
            openParenPos = originalText.find('{');
        }

        if (openParenPos != std::string::npos) {
            std::string introcer = originalText.substr(0, openParenPos);
            std::string paramsAndBody = originalText.substr(openParenPos);

            std::string newIntrocer = introcer + "<T... Args>";
            std::string newLambdaText = newIntrocer + paramsAndBody;

            size_t bodyStart = newLambdaText.find('{');
            if (bodyStart != std::string::npos) {
                size_t bodyEnd = newLambdaText.find('}');
                if (bodyEnd != std::string::npos) {
                    std::string beforeBody = newLambdaText.substr(0, bodyStart);
                    std::string afterBody = newLambdaText.substr(bodyEnd);

                    std::string newBody = " ([], Args{} && ...,); ";
                    newLambdaText = beforeBody + newBody + afterBody;
                }
            }

            newLambdaText += " .template operator()<int, float, char>();";
            Rewrite.ReplaceText(lambda->getSourceRange(), newLambdaText);
        }
    }
}

void MutatorFrontendAction_489::MutatorASTConsumer_489::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(lambdaExpr())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}