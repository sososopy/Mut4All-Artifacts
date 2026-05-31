//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Virtual_Function_Override_119
 */ 
class MutatorFrontendAction_119 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(119)

private:
    class MutatorASTConsumer_119 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_119(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_virtual_function_override_119.h"

// ========================================================================================================
#define MUT119_OUTPUT 1

void MutatorFrontendAction_119::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("VirtualMethod")) {
        // Filter nodes in header files
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MD->getLocation()))
            return;
        
        if (MD->isVirtual() && MD->hasBody()) {
            // Get the source code text of target node
            auto methodText = stringutils::rangetoStr(*(Result.SourceManager),
                                                      MD->getSourceRange());
            // Perform mutation on the source code text by applying string replacement
            methodText.insert(methodText.find("{") + 1, "\n/*mut119*/if(false) return;");
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), methodText);
        }
    }
}
  
void MutatorFrontendAction_119::MutatorASTConsumer_119::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual(), unless(isPure())).bind("VirtualMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}