//source file
#include "../include/omit_template_parameter_in_instantiation_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;

        if (FTD->getTemplateParameters()->size() == 0)
            return;

        targetFunctionDecl = FTD->getTemplatedDecl();
        if (!targetFunctionDecl)
            return;
    }

    if (auto *CI = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
        if (!CI || !Result.Context->getSourceManager().isWrittenInMainFile(
                      CI->getExprLoc()))
            return;

        if (targetFunctionDecl && CI->getDirectCallee() == targetFunctionDecl) {
            std::string callText = Lexer::getSourceText(
                CharSourceRange::getTokenRange(CI->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();
            
            size_t pos = callText.find_last_of(')');
            if (pos != std::string::npos) {
                callText = callText.substr(0, pos);
                callText += " /*mut20*/"; // Omit one parameter intentionally
                callText += ")";
                Rewrite.ReplaceText(CI->getSourceRange(), callText);
            }
        }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionTemplateMatcher = functionTemplateDecl(has(templateTypeParmDecl())).bind("FunctionTemplate");
    StatementMatcher callExprMatcher = callExpr().bind("CallExpr");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}