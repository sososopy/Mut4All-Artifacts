//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enum_Initializer_With_C23_BitInt_352
 */ 
class MutatorFrontendAction_352 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(352)

private:
    class MutatorASTConsumer_352 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_352(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Enum_Initializer_With_C23_BitInt_352.h"

// ========================================================================================================
#define MUT352_OUTPUT 1

void MutatorFrontendAction_352::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumConstantDecl>("enumConst")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->hasInitExpr()) {
            const Expr *init = MT->getInitExpr();
            SourceLocation start = init->getBeginLoc();
            SourceLocation end = init->getEndLoc();
            if (start.isValid() && end.isValid()) {
                Rewrite.ReplaceText(SourceRange(start, end), "1152921504606846976wb");
            }
        }
    }
}
  
void MutatorFrontendAction_352::MutatorASTConsumer_352::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumConstantDecl(hasInitializer(integerLiteral())).bind("enumConst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}