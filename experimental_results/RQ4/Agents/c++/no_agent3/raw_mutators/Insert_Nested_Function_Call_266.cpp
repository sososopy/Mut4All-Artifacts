//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Nested_Function_Call_266
 */ 
class MutatorFrontendAction_266 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(266)

private:
    class MutatorASTConsumer_266 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_266(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const FunctionDecl *> existingFunctions;
        bool helperFunctionInserted = false;
    };
};

//source file
#include "../include/insert_nested_function_call_266.h"

// ========================================================================================================
#define MUT266_OUTPUT 1

void MutatorFrontendAction_266::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::Stmt>("stmtWithFuncCall")) {
        if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(ST->getBeginLoc()))
            return;

        const FunctionDecl *selectedFunction = nullptr;
        for (const auto *func : existingFunctions) {
            if (func->getNumParams() > 0) {
                selectedFunction = func;
                break;
            }
        }

        std::string newFunctionCall;
        if (selectedFunction) {
            std::string funcName = selectedFunction->getNameInfo().getName().getAsString();
            std::string args;
            for (unsigned i = 0; i < selectedFunction->getNumParams(); ++i) {
                if (i > 0) args += ", ";
                args += selectedFunction->getParamDecl(i)->getNameAsString();
            }
            newFunctionCall = funcName + "(" + args + ");";
        } else if (!helperFunctionInserted) {
            newFunctionCall = "int helperFunction() { return 42; }\n";
            helperFunctionInserted = true;
        }

        if (!newFunctionCall.empty()) {
            Rewrite.InsertText(ST->getBeginLoc(), newFunctionCall + "\n/*mut266*/", true, true);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        existingFunctions.push_back(FD);
    }
}
  
void MutatorFrontendAction_266::MutatorASTConsumer_266::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher stmtMatcher = stmt(anyOf(forStmt(), whileStmt(), ifStmt(), switchStmt()), hasDescendant(callExpr())).bind("stmtWithFuncCall");
    DeclarationMatcher funcMatcher = functionDecl(unless(isMain())).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(stmtMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}