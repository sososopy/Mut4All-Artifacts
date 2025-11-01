//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Type_Casting_In_Switch_Statements_427
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
        const EnumDecl *enumDecl = nullptr; // Record the enum declaration
    };
};

//source file
#include "../include/Enum_Type_Casting_In_Switch_Statements_427.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SwitchStmt = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
        if (!SwitchStmt || !Result.Context->getSourceManager().isWrittenInMainFile(SwitchStmt->getBeginLoc()))
            return;

        const Expr *CondExpr = SwitchStmt->getCond();
        const auto *EnumType = CondExpr->getType()->getAs<EnumType>();

        if (EnumType) {
            // Check if the condition is already a static_cast
            if (const auto *CastExpr = dyn_cast<ExplicitCastExpr>(CondExpr)) {
                if (CastExpr->getCastKind() == CK_IntegralCast) {
                    // Remove the cast
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(CastExpr->getSourceRange()), 
                                        Lexer::getSourceText(CharSourceRange::getTokenRange(CastExpr->getSubExpr()->getSourceRange()), 
                                        Rewrite.getSourceMgr(), Rewrite.getLangOpts()));
                }
            } else {
                // Add static_cast to the enum type
                std::string CastedExpr = "static_cast<" + EnumType->getDecl()->getNameAsString() + ">(" +
                                         Lexer::getSourceText(CharSourceRange::getTokenRange(CondExpr->getSourceRange()), 
                                         Rewrite.getSourceMgr(), Rewrite.getLangOpts()) + ")";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CondExpr->getSourceRange()), CastedExpr);
            }
        } else if (!enumDecl) {
            // If no enum is found, introduce a basic enum declaration
            std::string EnumDeclStr = "enum ExampleEnum { Value1 };\n";
            Rewrite.InsertTextBefore(SwitchStmt->getBeginLoc(), EnumDeclStr);
        }
    }
}

void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    StatementMatcher switchMatcher = switchStmt(hasCondition(expr())).bind("SwitchStmt");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(switchMatcher, &callback);
    matchFinder.matchAST(Context);
}