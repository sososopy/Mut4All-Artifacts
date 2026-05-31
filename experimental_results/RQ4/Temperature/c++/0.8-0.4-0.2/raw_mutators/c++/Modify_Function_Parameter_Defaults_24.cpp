//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Parameter_Defaults_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl*, std::vector<Expr*>> defaultArgs;
    };
};

//source file
#include "../include/Modify_Function_Parameter_Defaults_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaults")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        bool hasDefaultArgs = false;
        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (FD->getParamDecl(i)->hasDefaultArg()) {
                hasDefaultArgs = true;
                break;
            }
        }

        if (hasDefaultArgs) {
            std::string funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            for (unsigned i = 0; i < FD->getNumParams(); ++i) {
                if (FD->getParamDecl(i)->hasDefaultArg()) {
                    defaultArgs[FD].push_back(FD->getParamDecl(i)->getDefaultArg());
                    FD->getParamDecl(i)->setHasDefaultArg(false);
                }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDecl);
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        const FunctionDecl *FD = CE->getDirectCallee();
        if (FD && defaultArgs.find(FD) != defaultArgs.end()) {
            std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
            std::string newArgs;
            for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
                if (i > 0) newArgs += ", ";
                newArgs += stringutils::rangetoStr(*(Result.SourceManager), CE->getArg(i)->getSourceRange());
            }
            for (auto *defaultArg : defaultArgs[FD]) {
                if (!newArgs.empty()) newArgs += ", ";
                newArgs += stringutils::rangetoStr(*(Result.SourceManager), defaultArg->getSourceRange());
            }
            std::string newCall = CE->getDirectCallee()->getNameAsString() + "(" + newArgs + ")";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), newCall);
        }
    }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(hasAnyParameter(hasDefaultArg())).bind("FunctionWithDefaults");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(hasAnyParameter(hasDefaultArg())))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}