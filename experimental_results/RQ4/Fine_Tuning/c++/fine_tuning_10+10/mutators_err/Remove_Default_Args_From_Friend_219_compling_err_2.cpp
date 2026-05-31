//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Default_Args_From_Friend_219
 */ 
class MutatorFrontendAction_219 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(219)

private:
    class MutatorASTConsumer_219 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_219(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl *, std::vector<Expr *>> defaultArgsMap;
    };
};

//source file
#include "../include/Mutator_219.h"

// ========================================================================================================
#define MUT219_OUTPUT 1

void MutatorFrontendAction_219::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("friendFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        std::vector<Expr *> defaultArgs;
        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (FD->getParamDecl(i)->hasDefaultArg()) {
                defaultArgs.push_back(const_cast<Expr *>(FD->getParamDecl(i)->getDefaultArg()));
                FD->getParamDecl(i)->setHasDefaultArg(false);
            }
        }

        if (!defaultArgs.empty()) {
            defaultArgsMap[FD] = defaultArgs;
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        const FunctionDecl *callee = CE->getDirectCallee();
        if (callee && defaultArgsMap.find(callee) != defaultArgsMap.end()) {
            std::string newCall = Lexer::getSourceText(CharSourceRange::getTokenRange(CE->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
            for (auto &arg : defaultArgsMap[callee]) {
                newCall += ", " + Lexer::getSourceText(CharSourceRange::getTokenRange(arg->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), newCall);
        }
    }
}

void MutatorFrontendAction_219::MutatorASTConsumer_219::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher friendFuncMatcher = functionDecl(hasAttr<clang::FriendAttr>(), isInTemplateInstantiation()).bind("friendFunc");
    StatementMatcher callExprMatcher = callExpr().bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(friendFuncMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}