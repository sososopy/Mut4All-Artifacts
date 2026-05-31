//source file
#include "../include/template_function_instantiation_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation())
            return;

        templateFuncName = FD->getNameAsString();

        std::string nonTemplateFuncDecl = "void " + templateFuncName + "(int a, int b) {\n    // some processing on a and b\n}\n";
        Rewrite.InsertTextAfterToken(FD->getEndLoc(), "\n/*mut7*/" + nonTemplateFuncDecl);

        std::string erroneousCall = templateFuncName + "(3, 4, 5); // Incorrect instantiation\n";
        Rewrite.InsertTextAfterToken(FD->getEndLoc(), "\n/*mut7*/" + erroneousCall);
    }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), unless(isImplicit())).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}