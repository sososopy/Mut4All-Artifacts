//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Argument_In_Function_Template_161
 */ 
class MutatorFrontendAction_161 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(161)

private:
    class MutatorASTConsumer_161 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_161(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Default_Argument_In_Function_Template_161.h"

// ========================================================================================================
#define MUT161_OUTPUT 1

void MutatorFrontendAction_161::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        // Filter nodes in header files
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
            return;

        for (auto *FD : FTD->specializations()) {
            if (FD->getNumParams() > 0) {
                auto *param = FD->getParamDecl(0);
                if (param->hasDefaultArg()) {
                    auto defaultArg = param->getDefaultArg();
                    if (auto *callExpr = llvm::dyn_cast<clang::CallExpr>(defaultArg)) {
                        auto srcText = stringutils::rangetoStr(*(Result.SourceManager),
                                                               callExpr->getSourceRange());
                        // Perform mutation on the source code text by applying string replacement
                        std::string mutatedText = "(" + srcText + ") + evaluate(" + param->getNameAsString() + ")";
                        // Replace the original AST node with the mutated one
                        Rewrite.ReplaceText(callExpr->getSourceRange(), mutatedText);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_161::MutatorASTConsumer_161::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(parmVarDecl(hasDefaultArgument(callExpr())))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}