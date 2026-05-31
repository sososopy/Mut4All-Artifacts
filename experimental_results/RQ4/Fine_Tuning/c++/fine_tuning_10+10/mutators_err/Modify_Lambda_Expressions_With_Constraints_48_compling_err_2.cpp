//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Expressions_With_Constraints_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_expressions_with_constraints_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        //Filter nodes in header files
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        size_t requiresPos = lambdaSource.find("requires");
        if (requiresPos != std::string::npos) {
            size_t insertPos = lambdaSource.find('{', requiresPos);
            if (insertPos != std::string::npos) {
                std::string newTemplateParam = ", typename U";
                std::string newRequiresClause = " sizeof(U); ";
                lambdaSource.insert(requiresPos, newTemplateParam);
                lambdaSource.insert(insertPos + 1, newRequiresClause);
            }
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
    }
}

void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasAnyTemplateArgument(
        templateTypeParmDecl().bind("TemplateTypeParmDecl")
    )).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}