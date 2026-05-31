//source file
#include "../include/modify_template_function_instantiation_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        for (const auto *param : FD->parameters()) {
            std::string paramType = param->getType().getAsString();
            if (paramType == "int") {
                complexTypes[paramType] = "std::pair<int*, double>";
            } else if (paramType == "double") {
                complexTypes[paramType] = "std::pair<double, int*>";
            }
        }
    }

    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                CE->getBeginLoc()))
            return;

        const auto *callee = CE->getDirectCallee();
        if (!callee || !callee->isTemplateInstantiation())
            return;

        std::string originalType = callee->getTemplateSpecializationArgs()->get(0).getAsType().getAsString();
        if (complexTypes.find(originalType) != complexTypes.end()) {
            std::string newType = complexTypes[originalType];
            std::string replacement = "process<" + newType + ">(";
            Rewrite.ReplaceText(CE->getBeginLoc(), 8, replacement);
        }
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateFuncMatcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunc");
    StatementMatcher templateCallMatcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("TemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateFuncMatcher, &callback);
    matchFinder.addMatcher(templateCallMatcher, &callback);
    matchFinder.matchAST(Context);
}