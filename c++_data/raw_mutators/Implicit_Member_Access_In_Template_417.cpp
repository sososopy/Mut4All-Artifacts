//header file
#pragma once
#include "Mutator_base.h"

/**
 * Implicit_Member_Access_In_Template_417
 */ 
class MutatorFrontendAction_417 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(417)

private:
    class MutatorASTConsumer_417 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_417(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Implicit_Member_Access_In_Template_417.h"

// ========================================================================================================
#define MUT417_OUTPUT 1

void MutatorFrontendAction_417::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("memberVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        
        if (VD->hasInit()) {
            auto initExpr = VD->getInit();
            if (auto *DRE = dyn_cast<DeclRefExpr>(initExpr)) {
                if (DRE->getDecl() == VD) {
                    return; // Already mutated
                }
            }
            std::string varName = VD->getNameAsString();
            std::string replacement = varName + " = " + varName + ";";
            Rewrite.ReplaceText(VD->getSourceRange(), replacement);
        }
    }
}
  
void MutatorFrontendAction_417::MutatorASTConsumer_417::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasAncestor(cxxRecordDecl(isTemplateInstantiation()))).bind("memberVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}