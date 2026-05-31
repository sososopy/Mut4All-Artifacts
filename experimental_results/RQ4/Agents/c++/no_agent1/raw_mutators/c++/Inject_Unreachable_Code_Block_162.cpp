//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Unreachable_Code_Block_162
 */ 

class MutatorFrontendAction_162 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(162)
private:
    class MutatorASTConsumer_162 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_162(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inject_unreachable_code_block_162.h"

// ========================================================================================================
#define MUT162_OUTPUT 1

void MutatorFrontendAction_162::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
            return;

        if (FD->hasBody()) {
            // Get the source code text of target node
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FD->getBody()->getSourceRange());
            // Perform mutation on the source code text by applying string replacement
            std::string unreachableCode = "\nif (false) { /*mut162*/ int unreachable = 0; }\n";
            functionBody.insert(functionBody.find("{") + 1, unreachableCode);
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
        }
    }
}
  
void MutatorFrontendAction_162::MutatorASTConsumer_162::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}