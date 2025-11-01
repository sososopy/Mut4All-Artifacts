//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalidate_Template_Parameter_28
 */ 
class MutatorFrontendAction_28 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(28)

private:
    class MutatorASTConsumer_28 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_28(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalidate_Template_Parameter_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateFunc")) {
        // Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string invalidType = "unknownType";
        size_t pos = templateDecl.find("typename");
        if (pos != std::string::npos) {
            templateDecl.replace(pos, 8, invalidType);
            templateDecl += " = delete;";
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDecl);
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionTemplateDecl().bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}