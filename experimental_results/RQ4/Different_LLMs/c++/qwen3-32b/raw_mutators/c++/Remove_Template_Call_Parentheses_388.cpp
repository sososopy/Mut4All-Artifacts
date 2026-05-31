//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Call_Parentheses_388
 */ 
class MutatorFrontendAction_388 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(388)

private:
    class MutatorASTConsumer_388 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_388(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Remove_Template_Call_Parentheses_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("callExpr")) {
        // Filter nodes in header files
        if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(Call->getBeginLoc()))
            return;

        // Get the source code text of the target node
        std::string callText = stringutils::rangetoStr(*Result.SourceManager, Call->getSourceRange());
        size_t openParenPos = callText.find('(');
        if (openParenPos == std::string::npos) {
            return; // Shouldn't happen for a valid call
        }

        // Perform mutation by removing the function call part
        std::string mutatedText = callText.substr(0, openParenPos);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(Call->getSourceRange(), mutatedText);
    }
}

void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxMemberCallExpr(hasExplicitTemplateArgs()).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}