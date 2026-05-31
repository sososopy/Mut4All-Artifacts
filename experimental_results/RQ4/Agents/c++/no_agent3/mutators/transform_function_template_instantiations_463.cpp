//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Function_Template_Instantiations_463
 */ 
class MutatorFrontendAction_463 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(463)

private:
    class MutatorASTConsumer_463 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_463(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionTemplateDecl *OriginalTemplate = nullptr;
    };
};

//source file
#include "../include/Transform_Function_Template_Instantiations_463.h"

// ========================================================================================================
#define MUT463_OUTPUT 1

void MutatorFrontendAction_463::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;
        
        OriginalTemplate = FTD;
        
        std::string newFunctionDecl = "template<typename T>\nvoid " + FTD->getNameAsString() + "(T value, int extra) {\n    // Modified implementation\n}\n";
        Rewrite.InsertTextAfterToken(FTD->getEndLoc(), "\n/*mut463*/" + newFunctionDecl);
    }
    else if (auto *FCE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        if (!FCE || !Result.Context->getSourceManager().isWrittenInMainFile(FCE->getExprLoc()))
            return;

        std::string originalCall = stringutils::rangetoStr(*(Result.SourceManager), FCE->getSourceRange());
        std::string mutatedCall = "#ifdef USE_EXTRA_VERSION\n" + originalCall.substr(0, originalCall.find(')')) + ", 42);\n#else\n" + originalCall + "\n#endif\n";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FCE->getSourceRange()), "/*mut463*/" + mutatedCall);
    }
}
  
void MutatorFrontendAction_463::MutatorASTConsumer_463::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("FunctionTemplate");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(hasParent(functionTemplateDecl())))).bind("FunctionCall");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}