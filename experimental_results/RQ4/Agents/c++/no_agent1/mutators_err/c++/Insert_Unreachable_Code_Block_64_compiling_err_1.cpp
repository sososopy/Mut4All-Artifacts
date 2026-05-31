//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_64
 */ 
class MutatorFrontendAction_64 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(64)

private:
    class MutatorASTConsumer_64 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_64(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_64.h"

// ========================================================================================================
#define MUT64_OUTPUT 1

void MutatorFrontendAction_64::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      FD->getLocation()))
            return;

        if (FD->hasBody()) {
            //Get the source code text of target node
            auto functionBody = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    functionBody->getSourceRange());

            //Perform mutation on the source code text by applying string replacement
            std::string unreachableBlock = "{\n/*mut64*/ if (false) { int unreachable = 0; unreachable++; }\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableBlock);

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionBody->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_64::MutatorASTConsumer_64::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}