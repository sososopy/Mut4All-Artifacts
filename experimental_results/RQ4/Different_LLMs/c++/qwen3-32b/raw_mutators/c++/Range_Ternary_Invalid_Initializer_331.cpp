//header file
#pragma once
#include "Mutator_base.h"

/**
 * Range_Ternary_Invalid_Initializer_331
 */ 
class MutatorFrontendAction_331 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(331)

private:
    class MutatorASTConsumer_331 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_331(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Range_Ternary_Invalid_Initializer_331.h"

// ========================================================================================================
#define MUT331_OUTPUT 1

void MutatorFrontendAction_331::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *forStmt = Result.Nodes.getNodeAs<clang::ForRangeStmt>("forLoop")) {
        if (!forStmt || !Result.Context->getSourceManager().isWrittenInMainFile(forStmt->getBeginLoc()))
            return;

        clang::VarDecl *loopVar = forStmt->getLoopVariable();
        if (!loopVar)
            return;

        clang::DeclStmt *loopVarDecl = forStmt->getLoopVariableDeclStmt();
        if (!loopVarDecl)
            return;

        clang::Expr *rangeExpr = forStmt->getRangeInit();
        if (!rangeExpr)
            return;

        clang::FunctionDecl *func = dyn_cast<clang::FunctionDecl>(forStmt->getDeclContext());
        if (!func)
            return;

        clang::CompoundStmt *body = func->getBody();
        if (!body)
            return;

        SmallVector<clang::VarDecl*, 4> bool_vars;
        SmallVector<clang::VarDecl*, 4> sameTypeVars;

        for (clang::Stmt *stmt : body->body()) {
            if (auto *declStmt = dyn_cast<clang::DeclStmt>(stmt)) {
                for (clang::Decl *decl : declStmt->decls()) {
                    if (auto *varDecl = dyn_cast<clang::VarDecl>(decl)) {
                        if (varDecl->getType()->isBooleanType()) {
                            bool_vars.push_back(varDecl);
                        } else if (varDecl->getType() == loopVar->getType()) {
                            sameTypeVars.push_back(varDecl);
                        }
                    }
                }
            }
        }

        if (bool_vars.empty() || sameTypeVars.size() < 2)
            return;

        int condIndex = getrandom::getRandomIndex(bool_vars.size());
        int val1Index = getrandom::getRandomIndex(sameTypeVars.size());
        int val2Index = getrandom::getRandomIndex(sameTypeVars.size());
        while (val2Index == val1Index) {
            val2Index = getrandom::getRandomIndex(sameTypeVars.size());
        }

        clang::VarDecl *condVar = bool_vars[condIndex];
        clang::VarDecl *val1Var = sameTypeVars[val1Index];
        clang::VarDecl *val2Var = sameTypeVars[val2Index];

        auto loopVarDeclStr = stringutils::rangetoStr(*Result.SourceManager, loopVarDecl->getSourceRange());
        auto rangeExprStr = stringutils::rangetoStr(*Result.SourceManager, rangeExpr->getSourceRange());

        std::string newInitializer = loopVarDeclStr + " = " + condVar->getNameAsString() + " ? " + val1Var->getNameAsString() + " : " + val2Var->getNameAsString();

        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(loopVarDecl->getSourceRange()), newInitializer);
    }
}
  
void MutatorFrontendAction_331::MutatorASTConsumer_331::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = forRangeStmt().bind("forLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}