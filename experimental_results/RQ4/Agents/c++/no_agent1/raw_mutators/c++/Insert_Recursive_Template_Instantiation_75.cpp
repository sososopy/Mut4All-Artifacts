//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_75
 */ 

class MutatorFrontendAction_75 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(75)
private:
    class MutatorASTConsumer_75 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_75(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> recursiveFunctions;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_75.h"

// ========================================================================================================
#define MUT75_OUTPUT 1

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        
        if (FD->hasBody() && FD->isTemplateInstantiation()) {
            recursiveFunctions.push_back(FD);
        }
    } else if (auto *RT = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
        if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RT->getBeginLoc()))
            return;

        if (!recursiveFunctions.empty()) {
            size_t index = getrandom::getRandomIndex(recursiveFunctions.size() - 1);
            auto targetFunc = recursiveFunctions[index];

            std::string funcName = targetFunc->getNameAsString();
            std::string templateCall = "/*mut75*/" + funcName + "<int>();";

            Rewrite.InsertTextAfterToken(RT->getEndLoc(), templateCall);
        }
    }
}

void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto functionMatcher = functionDecl(isTemplateInstantiation()).bind("Function");
    auto returnStmtMatcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(returnStmtMatcher, &callback);
    matchFinder.matchAST(Context);
}