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