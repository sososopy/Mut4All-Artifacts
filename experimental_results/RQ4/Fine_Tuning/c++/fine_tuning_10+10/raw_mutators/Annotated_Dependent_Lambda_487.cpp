//header file
#pragma once
#include "Mutator_base.h"

/**
 * Annotated_Dependent_Lambda_487
 */ 
class MutatorFrontendAction_487 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(487)

private:
    class MutatorASTConsumer_487 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_487(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Annotated_Dependent_Lambda_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithLambda")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Get the lambda expression and its source range
        if (const auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("AnnotatedLambda")) {
            auto lambdaRange = LE->getSourceRange();
            auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

            // Perform mutation on the source code text by applying string replacement
            std::string newLambdaText = "[u](T, U) [[clang::annotate_type(\"example\")]] { }";
            std::string modifiedFunctionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            size_t lambdaPos = modifiedFunctionText.find(lambdaText);
            if (lambdaPos != std::string::npos) {
                modifiedFunctionText.replace(lambdaPos, lambdaText.length(), newLambdaText);
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), modifiedFunctionText);
        }
    }
}
  
void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto lambdaMatcher = lambdaExpr(hasAttr(clang::attr::AnnotateType)).bind("AnnotatedLambda");
    auto functionMatcher = functionDecl(hasDescendant(lambdaMatcher)).bind("FunctionWithLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}