//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Switch_Condition_With_Var_427
 */ 
class MutatorFrontendAction_427 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(427)

private:
    class MutatorASTConsumer_427 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_427(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Switch_Condition_With_Var_427.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *switchStmt = Result.Nodes.getNodeAs<clang::SwitchStmt>("switchStmt")) {
        if (!switchStmt || !Result.Context->getSourceManager().isWrittenInMainFile(switchStmt->getBeginLoc())) {
            return;
        }
        auto *condition = switchStmt->getCond();
        if (auto *castExpr = dyn_cast<clang::CXXStaticCastExpr>(condition)) {
            if (castExpr->getType()->isEnumType()) {
                auto *sourceExpr = castExpr->getSubExpr();
                SourceLocation start = castExpr->getBeginLoc();
                SourceLocation end = castExpr->getEndLoc();
                SourceLocation sourceStart = sourceExpr->getBeginLoc();
                SourceLocation sourceEnd = sourceExpr->getEndLoc();
                std::string sourceText = Rewrite.getRewrittenText(SourceRange(sourceStart, sourceEnd));
                Rewrite.ReplaceText(SourceRange(start, end), sourceText);
            }
        }
    }
}
  
void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = switchStmt(hasCondition(cxxStaticCastExpr(hasDestinationType(enumType())))).bind("switchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}