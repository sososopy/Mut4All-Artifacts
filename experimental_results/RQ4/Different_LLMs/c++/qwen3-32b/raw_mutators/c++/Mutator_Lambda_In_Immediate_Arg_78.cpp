//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Lambda_In_Immediate_Arg_78
 */ 
class MutatorFrontendAction_78 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(78)

private:
    class MutatorASTConsumer_78 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_78(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Lambda_In_Immediate_Arg_78.h"

// ========================================================================================================
#define MUT78_OUTPUT 1

void MutatorFrontendAction_78::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        const FunctionDecl *Callee = CE->getDirectCallee();
        if (!Callee || (!Callee->isConstexpr() && !Callee->isConsteval()))
            return;

        DeclContext *DC = CE->getLexicalParent();
        const FunctionDecl *FuncDecl = dyn_cast<FunctionDecl>(DC);
        if (!FuncDecl)
            return;

        std::vector<const VarDecl *> vars;
        for (const auto *D : FuncDecl->decls()) {
            if (const auto *VD = dyn_cast<VarDecl>(D)) {
                if (VD->getParentFunctionOrMethod() == FuncDecl) {
                    vars.push_back(VD);
                }
            }
        }

        if (vars.empty())
            return;

        int varIndex = getrandom::getRandomIndex(vars.size());
        const VarDecl *SelectedVar = vars[varIndex];
        std::string VarName = SelectedVar->getNameAsString();

        std::vector<unsigned> scalarArgs;
        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            Expr *Arg = CE->getArg(i);
            if (Arg->getType()->isScalarType()) {
                scalarArgs.push_back(i);
            }
        }

        if (scalarArgs.empty())
            return;

        int argIndex = getrandom::getRandomIndex(scalarArgs.size());
        unsigned selectedArgIndex = scalarArgs[argIndex];
        Expr *SelectedArg = CE->getArg(selectedArgIndex);

        std::string LambdaStr = "[&" + VarName + "]{ return " + VarName + "; }";
        LambdaStr = "/*mut78*/" + LambdaStr;

        SourceLocation ArgBegin = SelectedArg->getBeginLoc();
        SourceLocation ArgEnd = SelectedArg->getEndLoc();
        Rewrite.ReplaceText(SourceRange(ArgBegin, ArgEnd), LambdaStr);
    }
}

void MutatorFrontendAction_78::MutatorASTConsumer_78::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr(callee(constEvalFunction())).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}