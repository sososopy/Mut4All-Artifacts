//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Return_Type_With_Auto_123
 */ 
class MutatorFrontendAction_123 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(123)

private:
    class MutatorASTConsumer_123 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_123(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Lambda_Return_Type_With_Auto_123.h"

// ========================================================================================================
#define MUT123_OUTPUT 1

void MutatorFrontendAction_123::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        //Filter nodes in header files
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto *LambdaTypeInfo = LambdaExpr->getLambdaClass()->getLambdaTypeInfo();
        if (LambdaTypeInfo && LambdaTypeInfo->getReturnType().isSpecified()) {
            //Get the source code text of target node
            auto lambdaRange = LambdaExpr->getSourceRange();
            std::string lambdaText = Lexer::getSourceText(CharSourceRange::getTokenRange(lambdaRange),
                                                          *Result.SourceManager, Result.Context->getLangOpts());

            //Perform mutation on the source code text by applying string replacement
            size_t pos = lambdaText.find("->");
            if (pos != std::string::npos) {
                pos = lambdaText.find(" ", pos);
                if (pos != std::string::npos) {
                    lambdaText.replace(pos + 1, 4, "auto");
                }
            }

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
        }
    }
}

void MutatorFrontendAction_123::MutatorASTConsumer_123::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasType(cxxRecordDecl())).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}