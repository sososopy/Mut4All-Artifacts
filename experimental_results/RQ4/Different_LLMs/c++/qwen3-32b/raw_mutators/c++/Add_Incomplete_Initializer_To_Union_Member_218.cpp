//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Incomplete_Initializer_To_Union_Member_218
 */ 
class MutatorFrontendAction_218 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(218)

private:
    class MutatorASTConsumer_218 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_218(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Incomplete_Initializer_To_Union_Member_218.h"

// ========================================================================================================
#define MUT218_OUTPUT 1

void MutatorFrontendAction_218::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UnionDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("unionDecl")) {
        if (!UnionDecl || !Result.Context->getSourceManager().isWrittenInMainFile(UnionDecl->getLocation()))
            return;

        for (const auto *Field : UnionDecl->fields()) {
            if (const auto *FD = dyn_cast<FieldDecl>(Field)) {
                if (!FD->hasInClassInitializer()) {
                    SourceLocation EndLoc = FD->getSourceRange().getEnd();
                    Rewrite.ReplaceText(EndLoc, 1, " = ;");
                    break;
                }
            }
        }
    }
}
  
void MutatorFrontendAction_218::MutatorASTConsumer_218::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isUnion(), has(fieldDecl())).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}