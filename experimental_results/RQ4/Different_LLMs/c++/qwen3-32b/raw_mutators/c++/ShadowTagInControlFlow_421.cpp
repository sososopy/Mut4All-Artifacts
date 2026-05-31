//header file
#pragma once
#include "Mutator_base.h"

/**
 * ShadowTagInControlFlow_421
 */ 
class MutatorFrontendAction_421 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(421)

private:
    class MutatorASTConsumer_421 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_421(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ShadowTagInControlFlow_421.h"

// ========================================================================================================
#define MUT421_OUTPUT 1

void MutatorFrontendAction_421::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forStmt")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(FS->getBeginLoc()))
            return;

        if (auto *Init = dyn_cast<DeclStmt>(FS->getInit())) {
            for (const auto *D : Init->decls()) {
                if (const auto *VD = dyn_cast<VarDecl>(D)) {
                    QualType T = VD->getType();
                    if (auto *TT = T->getAs<TypedefType>()) {
                        const TypedefDecl *TD = TT->getDecl();
                        std::string TDName = TD->getNameAsString();
                        std::string VarName = VD->getNameAsString();

                        std::string NewDecl = "struct " + TDName + " { float f; } " + VarName + ";";

                        SourceLocation StartLoc = Init->getBeginLoc();
                        SourceLocation EndLoc = Init->getEndLoc();
                        CharSourceRange Range = CharSourceRange::getTokenRange(SourceRange(StartLoc, EndLoc));
                        Rewrite.ReplaceText(Range, NewDecl);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_421::MutatorASTConsumer_421::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = forStmt(hasLoopInit(declStmt(hasDescendant(varDecl(hasType(typedefType())))))).bind("forStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}