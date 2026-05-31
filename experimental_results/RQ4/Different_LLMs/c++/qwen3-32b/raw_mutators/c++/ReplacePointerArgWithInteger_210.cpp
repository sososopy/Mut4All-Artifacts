//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplacePointerArgWithInteger_210
 */ 
class MutatorFrontendAction_210 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(210)

private:
    class MutatorASTConsumer_210 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_210(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplacePointerArgWithInteger_210.h"

// ========================================================================================================
#define MUT210_OUTPUT 1

void MutatorFrontendAction_210::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *call = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        //Filter nodes in header files
        if (!call || !Result.Context->getSourceManager().isWrittenInMainFile(
                           call->getBeginLoc()))
            return;
        
        //Check if the called function is __builtin_assume_aligned
        auto *callee = call->getDirectCallee();
        if (!callee || !callee->isBuiltinFunction() || callee->getName() != "__builtin_assume_aligned")
            return;
        
        if (call->getNumArgs() < 1)
            return;
        Expr *firstArg = call->getArg(0);
        
        //Find the function declaration containing this call
        FunctionDecl *funcDecl = call->getCalleeDecl()->getParentFunctionOrMethod();
        if (!funcDecl)
            return;
        
        //Collect integer variables in the same scope
        Stmt *body = funcDecl->getBody();
        if (!body)
            return;
        
        std::vector<VarDecl*> intVars;
        for (const auto *stmt : body->children()) {
            if (auto *var = dyn_cast<VarDecl>(stmt)) {
                if (var->getType()->isIntegerType()) {
                    intVars.push_back(const_cast<VarDecl*>(var));
                }
            }
        }
        
        VarDecl *selectedVar = nullptr;
        if (!intVars.empty()) {
            //Select the first available integer variable
            selectedVar = intVars[0];
        } else {
            //Insert a new integer variable declaration before the call
            std::string varDecl = "int temp = 0;";
            SourceLocation insertLoc = call->getBeginLoc();
            Rewrite.InsertText(insertLoc, varDecl, true, true);
            selectedVar = nullptr; // Cannot resolve AST node, use name directly
        }
        
        if (selectedVar) {
            std::string varName = selectedVar->getNameAsString();
            Rewrite.ReplaceText(firstArg->getSourceRange(), varName);
        } else {
            //Use the inserted variable name "temp"
            Rewrite.ReplaceText(firstArg->getSourceRange(), "temp");
        }
    }
}
  
void MutatorFrontendAction_210::MutatorASTConsumer_210::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatcher to identify built-in function calls expecting a pointer argument
    DeclarationMatcher matcher = callExpr(
        callee(builtinFunctionDecl(hasName("__builtin_assume_aligned")))
    ).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}