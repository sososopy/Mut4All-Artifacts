```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Consteval_Constructor_Usage_73
 */ 
class MutatorFrontendAction_73 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(73)

private:
    class MutatorASTConsumer_73 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_73(Rewriter &R) : TheRewriter(R) {}
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
        std::string functionName;
        std::string lambdaReturnType;
    };
};

//source file
#include "../include/modify_consteval_constructor_usage_73.h"

// ========================================================================================================
#define MUT73_OUTPUT 1

void MutatorFrontendAction_73::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVar")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        if (auto *InitExpr = VD->getInit()) {
            if (auto *LambdaExpr = llvm::dyn_cast<clang::LambdaExpr>(InitExpr)) {
                //Get the source code text of target node
                auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                            LambdaExpr->getSourceRange());
                //Extract return type from lambda
                lambdaReturnType = LambdaExpr->getCallOperator()->getReturnType().getAsString();
                //Create a unique function name
                functionName = "mutatedFunction_" + std::to_string(VD->getID());

                //Perform mutation on the source code text by applying string replacement
                std::string functionDef = "constexpr " + lambdaReturnType + " " + functionName + "() noexcept {}";
                std::string newVarInit = functionName + "()";

                //Replace the original AST node with the mutated one
                Rewrite.InsertTextBefore(VD->getBeginLoc(), functionDef + "\n/*mut73*/");
                Rewrite.ReplaceText(LambdaExpr->getSourceRange(), newVarInit);
            }
        }
    }
}

void MutatorFrontendAction_73::MutatorASTConsumer_73::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(asString("Sample"))).bind("constexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```