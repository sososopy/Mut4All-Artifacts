//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Function_Call_4
 */ 

class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)
private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl *> visitedFunctions;
    };
};

//source file
#include "../include/insert_recursive_function_call_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody() || FD->isMain())
            return;
        
        if (visitedFunctions.find(FD) == visitedFunctions.end()) {
            visitedFunctions.insert(FD);
            auto body = FD->getBody();
            std::string funcName = FD->getNameAsString();
            std::string recursiveCall = "\n/*mut4*/" + funcName + "();\n";
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            if (bodyText.find("return") == std::string::npos) {
                bodyText.insert(bodyText.rfind('}'), recursiveCall);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
            }
        }
    }
}

void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}