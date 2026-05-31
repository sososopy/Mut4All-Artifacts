//source file
#include "../include/template_function_argument_misuse_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        templateFunctionDecl = FD->getTemplatedDecl();
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        //Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        if (templateFunctionDecl && CE->getDirectCallee() == templateFunctionDecl) {
            //Get the source code text of target node
            auto callExprStr = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string incorrectArg = "\"string_literal\"";
            auto pos = callExprStr.find('(');
            if (pos != std::string::npos) {
                callExprStr.insert(pos + 1, incorrectArg + ", ");
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), callExprStr);
        }
    }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher functionTemplateMatcher = functionTemplateDecl().bind("FunctionTemplate");
    StatementMatcher functionCallMatcher = callExpr().bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.addMatcher(functionCallMatcher, &callback);
    matchFinder.matchAST(Context);
}