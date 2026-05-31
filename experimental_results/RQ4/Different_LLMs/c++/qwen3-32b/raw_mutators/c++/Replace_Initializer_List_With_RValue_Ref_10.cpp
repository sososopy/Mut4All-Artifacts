//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Initializer_List_With_RValue_Ref_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), tempCounter(0) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        int tempCounter;
    };
};

//source file
#include "../include/Replace_Initializer_List_With_RValue_Ref_10.h"
#include "clang/Lex/Lexer.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *callExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        auto callee = callExpr->getCallee()->IgnoreImplicit();
        if (auto *FD = dyn_cast<FunctionDecl>(callee)) {
            if (FD->getNameAsString() != "begin") {
                return;
            }
        } else {
            return;
        }

        if (callExpr->getNumArgs() < 1) {
            return;
        }
        auto arg = callExpr->getArg(0);
        if (!isa<InitListExpr>(arg)) {
            return;
        }
        auto ile = cast<InitListExpr>(arg);

        if (!Result.Context->getSourceManager().isWrittenInMainFile(ile->getBeginLoc())) {
            return;
        }

        std::string varName = "temp_" + std::to_string(tempCounter++);
        SourceManager &SM = Rewrite.getSourceMgr();
        LangOptions &LO = Rewrite.getLangOpts();
        std::string ileText = Lexer::getSourceText(CharSourceRange::getTokenRange(ile->getSourceRange()), SM, LO);

        std::string decl = "auto&& " + varName + " = " + ileText + ";";
        SourceLocation insertLoc = callExpr->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, decl + "\n");
        Rewrite.ReplaceText(ile->getSourceRange(), varName);
    } else if (auto *forStmt = Result.Nodes.getNodeAs<clang::ForStmt>("forStmt")) {
        auto rangeInit = forStmt->getRangeInit();
        if (!rangeInit || !isa<InitListExpr>(rangeInit)) {
            return;
        }
        auto ile = cast<InitListExpr>(rangeInit);

        if (!Result.Context->getSourceManager().isWrittenInMainFile(ile->getBeginLoc())) {
            return;
        }

        std::string varName = "temp_" + std::to_string(tempCounter++);
        SourceManager &SM = Rewrite.getSourceMgr();
        LangOptions &LO = Rewrite.getLangOpts();
        std::string ileText = Lexer::getSourceText(CharSourceRange::getTokenRange(ile->getSourceRange()), SM, LO);

        std::string decl = "auto&& " + varName + " = " + ileText + ";";
        SourceLocation insertLoc = forStmt->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, decl + "\n");
        Rewrite.ReplaceText(ile->getSourceRange(), varName);
    }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    StatementMatcher callMatcher = callExpr(
        callee(functionDecl(hasName("begin"))),
        hasArgument(0, initListExpr())
    ).bind("callExpr");

    StatementMatcher forMatcher = forStmt(
        hasRange(InitListExpr())
    ).bind("forStmt");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.addMatcher(forMatcher, &callback);
    matchFinder.matchAST(Context);
}