//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_builtin_va_arg_665
 */ 
class MutatorFrontendAction_665 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(665)

private:
    class MutatorASTConsumer_665 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_665(Rewriter &R) : TheRewriter(R) {}
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
        const clang::VarDecl *misusedVar = nullptr;
    };
};

//source file
#include "../include/misuse_builtin_va_arg_665.h"

// ========================================================================================================
#define MUT665_OUTPUT 1

void MutatorFrontendAction_665::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithVaList")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        // Identify a non-va_list variable in the function to misuse
        for (auto *D : FD->decls()) {
            if (auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
                if (!VD->getType()->isVaListType()) {
                    misusedVar = VD;
                    break;
                }
            }
        }

        if (!misusedVar)
            return;

        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string misuse = "int value = __builtin_va_arg(" + misusedVar->getNameAsString() + ", int);";
        functionBody.insert(functionBody.find("{") + 1, "\n/*mut665*/" + misuse + "\n");

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
    }
}
  
void MutatorFrontendAction_665::MutatorASTConsumer_665::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(callExpr(callee(functionDecl(hasName("__builtin_va_start")))))).bind("FunctionWithVaList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}