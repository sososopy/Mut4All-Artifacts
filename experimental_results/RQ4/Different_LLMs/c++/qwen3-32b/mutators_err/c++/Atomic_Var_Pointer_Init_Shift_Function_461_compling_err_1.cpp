//header file
#pragma once
#include "Mutator_base.h"

/**
 * Atomic_Var_Pointer_Init_Shift_Function_461
 */ 
class MutatorFrontendAction_461 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(461)

private:
    class MutatorASTConsumer_461 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_461(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> atomicVars; // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Atomic_Var_Pointer_Init_Shift_Function_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("atomicVar")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        // Check if it's a global variable
        if (VD->isGlobal()) {
            atomicVars.push_back(VD);
        }
    }
    // Check whether the matched AST node is the function with bit shift
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("shiftFunc")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (atomicVars.empty()) return;
        // Get the first atomic variable to mutate
        const clang::VarDecl *selectedVar = atomicVars[0];
        // Get the source code text of target node
        std::string declStr = stringutils::rangetoStr(*Result.SourceManager, selectedVar->getSourceRange());
        // Perform mutation on the source code text by applying string replacement
        size_t closeParenPos = declStr.find(")");
        if (closeParenPos != std::string::npos) {
            declStr.insert(closeParenPos, "*");
            declStr += " = 0;";
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(selectedVar->getSourceRange(), declStr);
        }
    }
}

void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST nodes for mutation
    DeclarationMatcher atomicVarMatcher = varDecl(hasType(atomicType())).bind("atomicVar");
    DeclarationMatcher shiftFuncMatcher = functionDecl(hasBody(compoundStmt(hasDescendant(binaryOperator(hasOperatorName("<<") || hasOperatorName(">>"), hasLHS(declRefExpr(to(atomicVarDecl())))))).bind("shiftFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(atomicVarMatcher, &callback);
    matchFinder.addMatcher(shiftFuncMatcher, &callback);
    matchFinder.matchAST(Context);
}