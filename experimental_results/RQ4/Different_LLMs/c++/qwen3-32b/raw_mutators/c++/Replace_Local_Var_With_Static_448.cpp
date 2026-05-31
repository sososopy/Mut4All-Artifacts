//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Local_Var_With_Static_448
 */ 
class MutatorFrontendAction_448 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(448)

private:
    class MutatorASTConsumer_448 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_448(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Local_Var_With_Static_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        // Check if the variable is used in a template argument
        bool usedInTemplate = false;
        for (const auto *Ref : VD->redecls()) {
            for (const auto *Use : Ref->getUsedInTemplateArgs()) {
                usedInTemplate = true;
                break;
            }
            if (usedInTemplate) break;
        }
        if (!usedInTemplate) return;

        // Get the source range of the variable declaration
        SourceRange SR = VD->getSourceRange();
        if (SR.isInvalid()) return;

        // Get the source text of the declaration
        SourceManager &SM = Rewrite.getSourceMgr();
        const char *Start = SM.getCharacterData(SR.getBegin());
        const char *End = SM.getCharacterData(SR.getEnd());
        std::string DeclText(Start, End - Start);

        // Insert 'static' at the beginning of the declaration
        std::string NewDecl = "static " + DeclText;

        // Replace the original declaration with the new one
        Rewrite.ReplaceText(SR, NewDecl);
    }
}
  
void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(
        unless(isStatic()),
        hasAncestor(functionTemplateDecl())
    ).bind("var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}