//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_array_initializations_605
 */ 
class MutatorFrontendAction_605 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(605)

private:
    class MutatorASTConsumer_605 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_605(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::Expr*, std::string> exprToVarMap;
    };
};

//source file
#include "../include/modify_array_initializations_605.h"

// ========================================================================================================
#define MUT605_OUTPUT 1

void MutatorFrontendAction_605::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *InitList = Result.Nodes.getNodeAs<clang::InitListExpr>("InitList")) {
        //Filter nodes in header files
        if (!InitList || !Result.Context->getSourceManager().isWrittenInMainFile(
                InitList->getBeginLoc()))
            return;

        // Collect existing variables for use in assignments
        auto &Context = *Result.Context;
        auto &SM = Context.getSourceManager();
        auto &LangOpts = Context.getLangOpts();
        auto &ASTContext = Context.getASTContext();

        std::string newInitList;
        for (unsigned i = 0; i < InitList->getNumInits(); ++i) {
            const Expr *InitExpr = InitList->getInit(i);
            std::string exprText = Lexer::getSourceText(CharSourceRange::getTokenRange(InitExpr->getSourceRange()), SM, LangOpts).str();

            std::string varName;
            if (exprToVarMap.find(InitExpr) != exprToVarMap.end()) {
                varName = exprToVarMap[InitExpr];
            } else {
                varName = "var" + std::to_string(exprToVarMap.size() + 1);
                exprToVarMap[InitExpr] = varName;

                // Determine the type of the expression
                QualType exprType = InitExpr->getType();
                std::string typeName = exprType.getAsString();
                std::string declText = typeName + " " + varName + ";\n";

                // Insert declaration before the array initialization
                TheRewriter.InsertTextBefore(InitList->getBeginLoc(), declText);
            }

            newInitList += varName + " = " + exprText;
            if (i != InitList->getNumInits() - 1) {
                newInitList += ", ";
            }
        }

        // Perform mutation on the source code text by applying string replacement
        TheRewriter.ReplaceText(InitList->getSourceRange(), "{" + newInitList + "}");
    }
}
  
void MutatorFrontendAction_605::MutatorASTConsumer_605::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = initListExpr().bind("InitList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}