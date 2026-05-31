//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reverse_ParameterPack_RequiresClause_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Reverse_ParameterPack_RequiresClause_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("targetFunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        auto *RE = FT->getRequiresClause();
        if (!RE) return;

        auto *Body = RE->getCommonExpr();
        if (!Body) return;

        if (auto *PE = dyn_cast<clang::PackExpansionExpr>(Body)) {
            auto *Pattern = PE->getPattern();
            if (auto *DRE = dyn_cast<clang::DeclRefExpr>(Pattern)) {
                auto *Decl = DRE->getDecl();
                if (auto *TP = dyn_cast<clang::TemplateTypeParmDecl>(Decl)) {
                    SourceLocation Start = DRE->getBeginLoc();
                    SourceLocation End = DRE->getEndLoc();
                    if (Start.isInvalid() || End.isInvalid()) return;

                    std::string Replacement = "reverse<" + TP->getNameAsString() + ">";
                    Rewrite.ReplaceText(SourceRange(Start, End), Replacement);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        has(cxxMethodDecl(
            ofClass(cxxRecordDecl(isLambda())),
            hasRequiresClause(requiresExpr(hasBody(packExpansionExpr())))
        )
    )).bind("targetFunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}