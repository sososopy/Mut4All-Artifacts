//source file
#include "../include/template_argument_misalignment_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplates")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->isTemplateInstantiation()) {
            templateFunctions.push_back(FD);
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateInstantiations")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        if (!templateFunctions.empty()) {
            auto targetFunc = templateFunctions.back();
            templateFunctions.pop_back();

            auto argCount = CE->getNumArgs();
            if (argCount > 0) {
                auto firstArg = CE->getArg(0);
                auto argType = firstArg->getType().getAsString();

                std::string newType = "NewType";
                std::string newVarDecl = "NewType newVar; ";
                
                std::string callExprText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
                std::string modifiedCallExpr = newVarDecl + callExprText;

                if (argType != newType) {
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), modifiedCallExpr);
                }
            }
        }
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionTemplateMatcher = functionDecl(isTemplateInstantiation()).bind("FunctionTemplates");
    StatementMatcher templateInstantiationMatcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("TemplateInstantiations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.addMatcher(templateInstantiationMatcher, &callback);
    matchFinder.matchAST(Context);
}