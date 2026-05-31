//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Module_Import_Statement_81
 */ 
class MutatorFrontendAction_81 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(81)

private:
    class MutatorASTConsumer_81 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_81(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Alter_Module_Import_Statement_81.h"

// ========================================================================================================
#define MUT81_OUTPUT 1

void MutatorFrontendAction_81::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MI = Result.Nodes.getNodeAs<clang::ModuleDecl>("ModuleImport")) {
        if (!MI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MI->getLocation()))
            return;

        // Get the source code text of target node
        auto moduleName = MI->getNameAsString();
        // Perform mutation on the source code text by applying string replacement
        std::string mutatedModuleName = "nonexistentmodule";
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(MI->getLocation(), moduleName.length(), mutatedModuleName);
    }
}
  
void MutatorFrontendAction_81::MutatorASTConsumer_81::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = moduleDecl().bind("ModuleImport");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}