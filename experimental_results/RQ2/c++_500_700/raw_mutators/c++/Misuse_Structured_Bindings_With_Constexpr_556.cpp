//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_structured_bindings_with_constexpr_556
 */ 
class MutatorFrontendAction_556 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(556)

private:
    class MutatorASTConsumer_556 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_556(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_structured_bindings_with_constexpr_556.h"

// ========================================================================================================
#define MUT556_OUTPUT 1

void MutatorFrontendAction_556::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("structuredBinding")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (auto *DRE = dyn_cast<DeclRefExpr>(VD->getInit()->IgnoreImplicit())) {
            if (auto *RD = dyn_cast<CXXRecordDecl>(DRE->getDecl()->getType()->getAsCXXRecordDecl())) {
                if (RD->isCompleteDefinition()) {
                    std::string structName = RD->getNameAsString();
                    std::string newDecl = "constexpr " + structName + " myInstance{};\n";
                    newDecl += "constexpr auto [" + VD->getNameAsString() + ", ...rest] = myInstance; /*mut556*/\n";
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newDecl);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_556::MutatorASTConsumer_556::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(anything()), hasType(autoType())).bind("structuredBinding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}