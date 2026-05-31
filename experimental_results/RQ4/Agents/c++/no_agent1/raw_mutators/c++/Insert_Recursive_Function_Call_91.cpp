//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Function_Call_91
 */ 
class MutatorFrontendAction_91 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(91)

private:
    class MutatorASTConsumer_91 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_91(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl*> visitedFunctions;
    };
};

//source file
#include "../include/insert_recursive_function_call_91.h"

// ========================================================================================================
#define MUT91_OUTPUT 1

void MutatorFrontendAction_91::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (!FD->hasBody() || visitedFunctions.count(FD))
            return;

        visitedFunctions.insert(FD);

        auto body = FD->getBody();
        std::string funcName = FD->getNameAsString();
        std::string recursiveCall = "\n/*mut91*/" + funcName + "();\n";

        if (body) {
            std::string bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            size_t pos = bodyStr.find_last_of('}');
            if (pos != std::string::npos) {
                bodyStr.insert(pos, recursiveCall);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
            }
        }
    }
}

void MutatorFrontendAction_91::MutatorASTConsumer_91::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}