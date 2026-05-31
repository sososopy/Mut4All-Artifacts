//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Module_Interface_442
 */ 
class MutatorFrontendAction_442 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(442)

private:
    class MutatorASTConsumer_442 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_442(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Module_Interface_442.h"

// ========================================================================================================
#define MUT442_OUTPUT 1

void MutatorFrontendAction_442::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TUD = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("tu")) {
        bool hasModuleDecl = false;
        for (clang::Decl *D : TUD->decls()) {
            if (isa<clang::AST::ModuleDecl>(D)) {
                hasModuleDecl = true;
                break;
            }
        }
        if (!hasModuleDecl) {
            SourceLocation startLoc = TUD->getLocation();
            Rewrite.InsertText(startLoc, "module;\n", true, true);
        }
    }
}
  
void MutatorFrontendAction_442::MutatorASTConsumer_442::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = translationUnitDecl().bind("tu");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}