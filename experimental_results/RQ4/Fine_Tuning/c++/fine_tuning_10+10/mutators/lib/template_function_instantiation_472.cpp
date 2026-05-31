//source file
#include "../include/Template_Function_Instantiation_472.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation()) {
            templateFunctions.push_back(FD);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionCall")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!templateFunctions.empty()) {
            auto *templateFunction = templateFunctions.back();
            std::string instantiationText = "/*mut472*/" + templateFunction->getNameAsString() + "<LargeStruct>(LargeStructInstance);\n";
            instantiationText += templateFunction->getNameAsString() + "<NestedVector>(NestedVectorInstance);\n";
            Rewrite.InsertTextAfterToken(FD->getEndLoc(), instantiationText);
        }
    }
}

void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto templateMatcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunction");
    auto functionCallMatcher = functionDecl(hasDescendant(callExpr())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionCallMatcher, &callback);
    matchFinder.matchAST(Context);
}